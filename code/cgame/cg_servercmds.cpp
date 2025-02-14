// cg_servercmds.c -- text commands sent by the server

// this line must stay at top so the whole PCH thing works...
#include "cg_headers.h"

//#include "cg_local.h"
#include "cg_media.h"
#include "FxScheduler.h"
#include "cg_lights.h"

/*
================
CG_ParseServerinfo

This is called explicitly when the gamestate is first received,
and whenever the server updates any serverinfo flagged cvars
================
*/
void CG_ParseServerinfo(void)
{
	const char *info;
	char *mapname;

	info = CG_ConfigString(CS_SERVERINFO);
	cgs.dmflags = atoi(Info_ValueForKey(info, "dmflags"));
	cgs.teamflags = atoi(Info_ValueForKey(info, "teamflags"));
	cgs.timelimit = atoi(Info_ValueForKey(info, "timelimit"));
	cgs.maxclients = 1;
	mapname = Info_ValueForKey(info, "mapname");
	Com_sprintf(cgs.mapname, sizeof(cgs.mapname), "maps/%s.bsp", mapname);
	char *p = strrchr(mapname, '/');
	strcpy(cgs.stripLevelName[0], p ? p + 1 : mapname);
	strupr(cgs.stripLevelName[0]);
	for (int i = 1; i < STRIPED_LEVELNAME_VARIATIONS; i++) // clear retry-array
	{
		cgs.stripLevelName[i][0] = '\0';
	}
	// be careful with the []-numbers here. Currently I use 0,1,2 for replacements or substitution, and [3] for "INGAME"
	//	I know, if I'd known there was going to be this much messing about I'd have subroutinised it all and done it
	//	neater, but it kinda evolved...   Feel free to bug me if you want to add to it... ?  -Ste.
	//

	//FIXME: a better way to handle sound-matched strings from other levels (currently uses levelname+sound as key)

	// additional String files needed for some levels...
	//
	// JKA...
	if (!stricmp(cgs.stripLevelName[0], "YAVIN1B"))
	{
		strcpy(cgs.stripLevelName[1], "YAVIN1");
	}

	/*	// JK2...
	if (!stricmp(cgs.stripLevelName[0],"KEJIM_BASE") ||
		!stricmp(cgs.stripLevelName[0],"KEJIM_POST")
		)
	{
		strcpy( cgs.stripLevelName[1], "ARTUS_MINE" );
	}
	if (!stricmp(cgs.stripLevelName[0],"DOOM_DETENTION") ||
		!stricmp(cgs.stripLevelName[0],"DOOM_SHIELDS")
		)
	{
		strcpy( cgs.stripLevelName[1], "DOOM_COMM" );
	}
	if (!stricmp(cgs.stripLevelName[0],"DOOM_COMM"))
	{
		strcpy( cgs.stripLevelName[1], "CAIRN_BAY" );
	}
	if (!stricmp(cgs.stripLevelName[0],"NS_STARPAD"))
	{
		strcpy( cgs.stripLevelName[1], "ARTUS_TOPSIDE" );	// for dream sequence...

		strcpy( cgs.stripLevelName[2], "BESPIN_UNDERCITY" );	// for dream sequence...
	}
	if (!stricmp(cgs.stripLevelName[0],"BESPIN_PLATFORM"))
	{
		strcpy( cgs.stripLevelName[1], "BESPIN_UNDERCITY" );
	}
*/
}

/*
================
CG_ConfigStringModified

================
*/
void CG_RegisterClientModels(int entityNum);
extern void cgi_R_WorldEffectCommand(const char *command);
static void CG_ConfigStringModified(void)
{
	const char *str;
	int num;

	num = atoi(CG_Argv(1));

	// get the gamestate from the client system, which will have the
	// new configstring already integrated
	cgi_GetGameState(&cgs.gameState);

	// look up the individual string that was modified
	str = CG_ConfigString(num);

	// do something with it if necessary
	if (num == CS_ITEMS)
	{
		int i;
		for (i = 1; i < bg_numItems; i++)
		{
			if (str[i] == '1')
			{
				if (bg_itemlist[i].classname)
				{
					CG_RegisterItemSounds(i);
					CG_RegisterItemVisuals(i);
				}
			}
		}
	}
	else if (num == CS_MUSIC)
	{
		CG_StartMusic(qtrue);
	}
	else if (num == CS_SERVERINFO)
	{
		CG_ParseServerinfo();
	}
	else if (num >= CS_MODELS && num < CS_MODELS + MAX_MODELS)
	{
		cgs.model_draw[num - CS_MODELS] = cgi_R_RegisterModel(str);
		//		OutputDebugString(va("### CG_ConfigStringModified(): cgs.model_draw[%d] = \"%s\"\n",num-CS_MODELS,str));
		// GHOUL2 Insert start
	}
	else if (num >= CS_CHARSKINS && num < CS_CHARSKINS + MAX_CHARSKINS)
	{
		cgs.skins[num - CS_CHARSKINS] = cgi_R_RegisterSkin(str);
		// Ghoul2 Insert end
	}
	else if (num >= CS_SOUNDS && num < CS_SOUNDS + MAX_SOUNDS)
	{
		if (str[0] != '*')
		{
			cgs.sound_precache[num - CS_SOUNDS] = cgi_S_RegisterSound(str);
		}
	}
	else if (num >= CS_EFFECTS && num < CS_EFFECTS + MAX_FX)
	{
		theFxScheduler.RegisterEffect(str);
	}
	else if (num >= CS_PLAYERS && num < CS_PLAYERS + MAX_CLIENTS)
	{
		CG_NewClientinfo(num - CS_PLAYERS);
		CG_RegisterClientModels(num - CS_PLAYERS);
	}
	else if (num >= CS_LIGHT_STYLES && num < CS_LIGHT_STYLES + (MAX_LIGHT_STYLES * 3))
	{
		CG_SetLightstyle(num - CS_LIGHT_STYLES);
	}
	else if (num >= CS_WORLD_FX && num < CS_WORLD_FX + MAX_WORLD_FX)
	{
		cgi_R_WorldEffectCommand(str);
	}
}

/*
=================
CG_ServerCommand

The string has been tokenized and can be retrieved with
Cmd_Argc() / Cmd_Argv()
=================
*/
static void CG_ServerCommand(void)
{
	const char *cmd;

	cmd = CG_Argv(0);

	if (!strcmp(cmd, "cp"))
	{
		CG_CenterPrint(CG_Argv(1), SCREEN_HEIGHT * 0.25);
		return;
	}

	if (!strcmp(cmd, "cs"))
	{
		CG_ConfigStringModified();
		return;
	}

	if (!strcmp(cmd, "print"))
	{
		CG_Printf("%s", CG_Argv(1));
		return;
	}

	if (!strcmp(cmd, "chat"))
	{
		//		cgi_S_StartLocalSound ( cgs.media.talkSound, CHAN_LOCAL_SOUND );
		CG_Printf("%s\n", CG_Argv(1));
		return;
	}

	// Scroll text
	if (!strcmp(cmd, "st"))
	{
#ifdef _XBOX
		if (cg.widescreen)
			CG_ScrollText(CG_Argv(1), 720 - 16);
		else
#endif
			CG_ScrollText(CG_Argv(1), SCREEN_WIDTH - 16);
		return;
	}

	// Cinematic text
	if (!strcmp(cmd, "ct"))
	{
		CG_CaptionText(CG_Argv(1), cgs.sound_precache[atoi(CG_Argv(2))], SCREEN_HEIGHT * 0.25);
		return;
	}

	// Text stop
	if (!strcmp(cmd, "cts"))
	{
		CG_CaptionTextStop();
		return;
	}

	// Text to appear in center of screen with an LCARS frame around it.
	if (!strcmp(cmd, "lt"))
	{
		CG_Printf("CG_LCARSText() being called. Tell Ste\nString: \"%s\"\n", CG_Argv(1));
		return;
	}

	// clientLevelShot is sent before taking a special screenshot for
	// the menu system during development
	if (!strcmp(cmd, "clientLevelShot"))
	{
		cg.levelShot = qtrue;
		return;
	}

	if (!strcmp(cmd, "vmsg"))
	{
#if 0
		char snd[MAX_QPATH];

		Com_sprintf(snd, sizeof(snd), 
			"sound/teamplay/vmsg/%s.wav", CG_Argv(1) );
		cgi_S_StartSound (NULL, cg.snap->ps.clientNum, CHAN_AUTO, 
			cgi_S_RegisterSound (snd) );
#endif
		return;
	}

	CG_Printf("Unknown client game command: %s\n", cmd);
}

/*
====================
CG_ExecuteNewServerCommands

Execute all of the server commands that were received along
with this this snapshot.
====================
*/
void CG_ExecuteNewServerCommands(int latestSequence)
{
	while (cgs.serverCommandSequence < latestSequence)
	{
		if (cgi_GetServerCommand(++cgs.serverCommandSequence))
		{
			CG_ServerCommand();
		}
	}
}
