// leave this as first line for PCH reasons...
//
#include "../server/exe_headers.h"

#include "client.h"
#include "client_ui.h"

#include "vmachine.h"

int PC_ReadTokenHandle(int handle, struct pc_token_s *pc_token);

int CL_UISystemCalls(int *args);

//prototypes
//extern qboolean SG_GetSaveImage( const char *psPathlessBaseName, void *pvAddress );
extern int SG_GetSaveGameComment(const char *psPathlessBaseName, char *sComment, char *sMapName);
extern qboolean SG_GameAllowedToSaveHere(qboolean inCamera);
extern void SG_StoreSaveGameComment(const char *sComment);
//extern byte *SCR_GetScreenshot(qboolean *qValid);

/*
====================
Helper functions for User Interface
====================
*/

/*
====================
GetClientState
====================
*/
static connstate_t GetClientState(void)
{
	return cls.state;
}

/*
====================
CL_GetGlConfig
====================
*/
static void UI_GetGlconfig(glconfig_t *config)
{
	*config = cls.glconfig;
}

/*
====================
GetClipboardData
====================
*/
static void GetClipboardData(char *buf, int buflen)
{
	char *cbd;

	cbd = Sys_GetClipboardData();

	if (!cbd)
	{
		*buf = 0;
		return;
	}

	Q_strncpyz(buf, cbd, buflen);

	Z_Free(cbd);
}

/*
====================
Key_KeynumToStringBuf
====================
*/
// only ever called by binding-display code, therefore returns non-technical "friendly" names
//	in any language that don't necessarily match those in the config file...
//
void Key_KeynumToStringBuf(int keynum, char *buf, int buflen)
{
	const char *psKeyName = Key_KeynumToString(keynum /*, qtrue */);

	// see if there's a more friendly (or localised) name...
	//
	const char *psKeyNameFriendly = SE_GetString(va("KEYNAMES_KEYNAME_%s", psKeyName));

	Q_strncpyz(buf, (psKeyNameFriendly && psKeyNameFriendly[0]) ? psKeyNameFriendly : psKeyName, buflen);
}

/*
====================
Key_GetBindingBuf
====================
*/
void Key_GetBindingBuf(int keynum, char *buf, int buflen)
{
	char *value;

	value = Key_GetBinding(keynum);
	if (value)
	{
		Q_strncpyz(buf, value, buflen);
	}
	else
	{
		*buf = 0;
	}
}

/*
====================
Key_GetCatcher
====================
*/
int Key_GetCatcher(void)
{
	return cls.keyCatchers;
}

/*
====================
Key_GetCatcher
====================
*/
void Key_SetCatcher(int catcher)
{
	cls.keyCatchers = catcher;
}

/*
====================
FloatAsInt
====================
*/
int FloatAsInt(float f)
{
	int temp;

	*(float *)&temp = f;

	return temp;
}

static void UI_Cvar_Create(const char *var_name, const char *var_value, int flags)
{
	Cvar_Get(var_name, var_value, flags);
}

static int GetConfigString(int index, char *buf, int size)
{
	int offset;

	if (index < 0 || index >= MAX_CONFIGSTRINGS)
		return qfalse;

	offset = cl.gameState.stringOffsets[index];
	if (!offset)
		return qfalse;

	Q_strncpyz(buf, cl.gameState.stringData + offset, size);

	return qtrue;
}

/*
====================
CL_ShutdownUI
====================
*/
void CL_ShutdownUI(void)
{
	cls.keyCatchers &= ~KEYCATCH_UI;
	cls.uiStarted = qfalse;
}

void CL_DrawDatapad(int HUDType)
{
	switch (HUDType)
	{
	case DP_OBJECTIVES:
		VM_Call(CG_DRAW_DATAPAD_OBJECTIVES);
		break;
	case DP_WEAPONS:
		VM_Call(CG_DRAW_DATAPAD_WEAPONS);
		break;
	case DP_INVENTORY:
		VM_Call(CG_DRAW_DATAPAD_INVENTORY);
		break;
	case DP_FORCEPOWERS:
		VM_Call(CG_DRAW_DATAPAD_FORCEPOWERS);
		break;
	default:
		break;
	}
}

void UI_Init(int apiVersion, uiimport_t *uiimport, qboolean inGameLoad);

/*
====================
CL_InitUI
====================
*/
void CL_InitUI(void)
{
	uiimport_t uii;

	memset(&uii, 0, sizeof(uii));

	uii.Printf = Com_Printf;
	uii.Error = Com_Error;

	uii.Cvar_Set = Cvar_Set;
	uii.Cvar_VariableValue = Cvar_VariableValue;
	uii.Cvar_VariableStringBuffer = Cvar_VariableStringBuffer;
	uii.Cvar_SetValue = Cvar_SetValue;
	uii.Cvar_Reset = Cvar_Reset;
	uii.Cvar_Create = UI_Cvar_Create;
	uii.Cvar_InfoStringBuffer = Cvar_InfoStringBuffer;

	uii.Draw_DataPad = CL_DrawDatapad;

	uii.Argc = Cmd_Argc;
	uii.Argv = Cmd_ArgvBuffer;
	uii.Cmd_TokenizeString = Cmd_TokenizeString;

	uii.Cmd_ExecuteText = Cbuf_ExecuteText;

	uii.FS_FOpenFile = FS_FOpenFileByMode;
	uii.FS_Read = FS_Read;
	uii.FS_Write = FS_Write;
	uii.FS_FCloseFile = FS_FCloseFile;
	uii.FS_GetFileList = FS_GetFileList;
	uii.FS_ReadFile = FS_ReadFile;
	uii.FS_FreeFile = FS_FreeFile;

	uii.R_RegisterModel = re.RegisterModel;
	uii.R_RegisterSkin = re.RegisterSkin;
	uii.R_RegisterShader = re.RegisterShader;
	uii.R_RegisterShaderNoMip = re.RegisterShaderNoMip;
	uii.R_RegisterFont = re.RegisterFont;
#ifndef _XBOX
	uii.R_Font_StrLenPixels = re.Font_StrLenPixels;
	uii.R_Font_HeightPixels = re.Font_HeightPixels;
	uii.R_Font_DrawString = re.Font_DrawString;
#endif
	uii.R_Font_StrLenChars = re.Font_StrLenChars;
	uii.Language_IsAsian = re.Language_IsAsian;
	uii.Language_UsesSpaces = re.Language_UsesSpaces;
	uii.AnyLanguage_ReadCharFromString = re.AnyLanguage_ReadCharFromString;

	//uii.SG_GetSaveImage			= SG_GetSaveImage;
	uii.SG_GetSaveGameComment = SG_GetSaveGameComment;
	uii.SG_StoreSaveGameComment = SG_StoreSaveGameComment;
	uii.SG_GameAllowedToSaveHere = SG_GameAllowedToSaveHere;

	//uii.SCR_GetScreenshot		= SCR_GetScreenshot;

	//uii.DrawStretchRaw			= re.DrawStretchRaw;
	uii.R_ClearScene = re.ClearScene;
	uii.R_AddRefEntityToScene = re.AddRefEntityToScene;
	uii.R_AddPolyToScene = re.AddPolyToScene;
	uii.R_AddLightToScene = re.AddLightToScene;
	uii.R_RenderScene = re.RenderScene;

	uii.R_ModelBounds = re.ModelBounds;

	uii.R_SetColor = re.SetColor;
	uii.R_DrawStretchPic = re.DrawStretchPic;
	uii.UpdateScreen = SCR_UpdateScreen;

#ifdef _XBOX
	uii.PrecacheScreenshot = SCR_PrecacheScreenshot;
#endif

	uii.R_LerpTag = re.LerpTag;

	uii.S_StartLocalLoopingSound = S_StartLocalLoopingSound;
	uii.S_StartLocalSound = S_StartLocalSound;
	uii.S_RegisterSound = S_RegisterSound;

	uii.Key_KeynumToStringBuf = Key_KeynumToStringBuf;
	uii.Key_GetBindingBuf = Key_GetBindingBuf;
	uii.Key_SetBinding = Key_SetBinding;
	uii.Key_IsDown = Key_IsDown;
	uii.Key_GetOverstrikeMode = Key_GetOverstrikeMode;
	uii.Key_SetOverstrikeMode = Key_SetOverstrikeMode;
	uii.Key_ClearStates = Key_ClearStates;
	uii.Key_GetCatcher = Key_GetCatcher;
	uii.Key_SetCatcher = Key_SetCatcher;

	uii.GetClipboardData = GetClipboardData;

	uii.GetClientState = GetClientState;

	uii.GetGlconfig = UI_GetGlconfig;

	uii.GetConfigString = (void (*)(int, char *, int))GetConfigString;

	uii.Milliseconds = Sys_Milliseconds;

	UI_Init(UI_API_VERSION, &uii, (cls.state > CA_DISCONNECTED && cls.state <= CA_ACTIVE));

//JLF MPSKIPPED
#ifdef _XBOX
	extern void UpdateDemoTimer();
	UpdateDemoTimer();

#endif

	//	uie->UI_Init( UI_API_VERSION, &uii );
}

qboolean UI_GameCommand(void)
{
	if (!cls.uiStarted)
	{
		return qfalse;
	}
	return UI_ConsoleCommand();
}

void CL_GenericMenu_f(void)
{
	char *arg = Cmd_Argv(1);

	if (cls.uiStarted)
	{
		UI_SetActiveMenu("ingame", arg);
	}
}

void CL_EndScreenDissolve_f(void)
{
	re.InitDissolve(qtrue); // dissolve from cinematic to underlying ingame
}

void CL_DataPad_f(void)
{
	if (cls.uiStarted && cls.cgameStarted && (cls.state == CA_ACTIVE))
	{
		UI_SetActiveMenu("datapad", NULL);
	}
}

/*
====================
CL_GetGlConfig
====================
*/
static void CL_GetGlconfig(glconfig_t *config)
{
	*config = cls.glconfig;
}
/*
int PC_ReadTokenHandle(int handle, pc_token_t *pc_token);
int PC_SourceFileAndLine(int handle, char *filename, int *line);
*/
/*
====================
CL_UISystemCalls

The ui module is making a system call
====================
*/
vm_t uivm;

#define VMA(x) ((void *)args[x])
#define VMF(x) ((float *)args)[x]
int CL_UISystemCalls(int *args)
{

	switch (args[0])
	{
	case UI_ERROR:
		Com_Error(ERR_DROP, "%s", VMA(1));
		return 0;

	case UI_CVAR_REGISTER:
		Cvar_Register((vmCvar_t *)VMA(1), (const char *)VMA(2), (const char *)VMA(3), args[4]);
		return 0;

	case UI_CVAR_SET:
		Cvar_Set((const char *)VMA(1), (const char *)VMA(2));
		return 0;

	case UI_CVAR_SETVALUE:
		Cvar_SetValue((const char *)VMA(1), VMF(2));
		return 0;

	case UI_CVAR_UPDATE:
		Cvar_Update((vmCvar_t *)VMA(1));
		return 0;

	case UI_R_REGISTERMODEL:
		return re.RegisterModel((const char *)VMA(1));

	case UI_R_REGISTERSHADERNOMIP:
		return re.RegisterShaderNoMip((const char *)VMA(1));

	case UI_GETGLCONFIG:
		CL_GetGlconfig((glconfig_t *)VMA(1));
		return 0;

	case UI_CMD_EXECUTETEXT:
		Cbuf_ExecuteText(args[1], (const char *)VMA(2));
		return 0;

	case UI_CVAR_VARIABLEVALUE:
		return FloatAsInt(Cvar_VariableValue((const char *)VMA(1)));

	case UI_FS_GETFILELIST:
		return FS_GetFileList((const char *)VMA(1), (const char *)VMA(2), (char *)VMA(3), args[4]);

	case UI_KEY_SETCATCHER:
		Key_SetCatcher(args[1]);
		return 0;

	case UI_KEY_CLEARSTATES:
		Key_ClearStates();
		return 0;

	case UI_R_SETCOLOR:
		re.SetColor((const float *)VMA(1));
		return 0;

	case UI_R_DRAWSTRETCHPIC:
		re.DrawStretchPic(VMF(1), VMF(2), VMF(3), VMF(4), VMF(5), VMF(6), VMF(7), VMF(8), args[9]);
		return 0;

	case UI_CVAR_VARIABLESTRINGBUFFER:
		Cvar_VariableStringBuffer((const char *)VMA(1), (char *)VMA(2), args[3]);
		return 0;

	case UI_R_MODELBOUNDS:
		re.ModelBounds(args[1], (float *)VMA(2), (float *)VMA(3));
		return 0;

	case UI_R_CLEARSCENE:
		re.ClearScene();
		return 0;

		//	case UI_KEY_GETOVERSTRIKEMODE:
		//		return Key_GetOverstrikeMode();
		//		return 0;

		//	case UI_PC_READ_TOKEN:
		//		return PC_ReadTokenHandle( args[1], VMA(2) );

		//	case UI_PC_SOURCE_FILE_AND_LINE:
		//		return PC_SourceFileAndLine( args[1], VMA(2), VMA(3) );

	case UI_KEY_GETCATCHER:
		return Key_GetCatcher();

	case UI_MILLISECONDS:
		return Sys_Milliseconds();

	case UI_S_REGISTERSOUND:
		return S_RegisterSound((const char *)VMA(1));

	case UI_S_STARTLOCALSOUND:
		S_StartLocalSound(args[1], args[2]);
		return 0;

		//	case UI_R_REGISTERFONT:
		//		re.RegisterFont( VMA(1), args[2], VMA(3));
		//		return 0;

	case UI_CIN_PLAYCINEMATIC:
		Com_DPrintf("UI_CIN_PlayCinematic\n");
		return CIN_PlayCinematic((const char *)VMA(1), args[2], args[3], args[4], args[5], args[6], (const char *)VMA(7));

	case UI_CIN_STOPCINEMATIC:
		return CIN_StopCinematic(args[1]);

	case UI_CIN_RUNCINEMATIC:
		return CIN_RunCinematic(args[1]);

#ifndef _XBOX
	case UI_CIN_DRAWCINEMATIC:
		CIN_DrawCinematic(args[1]);
		return 0;
#endif

	case UI_KEY_SETBINDING:
		Key_SetBinding(args[1], (const char *)VMA(2));
		return 0;

	case UI_KEY_KEYNUMTOSTRINGBUF:
		Key_KeynumToStringBuf(args[1], (char *)VMA(2), args[3]);
		return 0;

	case UI_CIN_SETEXTENTS:
		CIN_SetExtents(args[1], args[2], args[3], args[4], args[5]);
		return 0;

	case UI_KEY_GETBINDINGBUF:
		Key_GetBindingBuf(args[1], (char *)VMA(2), args[3]);
		return 0;

	default:
		Com_Error(ERR_DROP, "Bad UI system trap: %i", args[0]);
	}

	return 0;
}
