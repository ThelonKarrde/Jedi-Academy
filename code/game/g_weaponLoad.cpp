// g_weaponLoad.cpp
// fills in memory struct with ext_dat\weapons.dat

// this is excluded from PCH usage 'cos it looks kinda scary to me, being game and ui.... -Ste

#ifdef _USRDLL //UI dll

#include "../ui/gameinfo.h"
#include "weapons.h"
extern gameinfo_import_t gi;
extern weaponData_t weaponData[];
extern ammoData_t ammoData[];

#else //we are in the game

// ONLY DO THIS ON THE GAME SIDE
#include "g_local.h"

typedef struct
{
	char *name;
	void (*func)(centity_t *cent, const struct weaponInfo_s *weapon);
} func_t;

// Bryar
void FX_BryarProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);
void FX_BryarAltProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);

// Blaster
void FX_BlasterProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);
void FX_BlasterAltFireThink(centity_t *cent, const struct weaponInfo_s *weapon);

// Bowcaster
void FX_BowcasterProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);

// Heavy Repeater
void FX_RepeaterProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);
void FX_RepeaterAltProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);

// DEMP2
void FX_DEMP2_ProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);
void FX_DEMP2_AltProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);

// Golan Arms Flechette
void FX_FlechetteProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);
void FX_FlechetteAltProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);

// Personal Rocket Launcher
void FX_RocketProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);
void FX_RocketAltProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);

// Concussion Rifle
void FX_ConcProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);

// Emplaced weapon
void FX_EmplacedProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);

// Turret weapon
void FX_TurretProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);

// ATST Main weapon
void FX_ATSTMainProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);

// ATST Side weapons
void FX_ATSTSideMainProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);
void FX_ATSTSideAltProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);

//Tusken projectile
void FX_TuskenShotProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);

//Noghri projectile
void FX_NoghriShotProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon);

// Table used to attach an extern missile function string to the actual cgame function
func_t funcs[] = {
	{"bryar_func", FX_BryarProjectileThink},
	{"bryar_alt_func", FX_BryarAltProjectileThink},
	{"blaster_func", FX_BlasterProjectileThink},
	{"blaster_alt_func", FX_BlasterAltFireThink},
	{"bowcaster_func", FX_BowcasterProjectileThink},
	{"repeater_func", FX_RepeaterProjectileThink},
	{"repeater_alt_func", FX_RepeaterAltProjectileThink},
	{"demp2_func", FX_DEMP2_ProjectileThink},
	{"demp2_alt_func", FX_DEMP2_AltProjectileThink},
	{"flechette_func", FX_FlechetteProjectileThink},
	{"flechette_alt_func", FX_FlechetteAltProjectileThink},
	{"rocket_func", FX_RocketProjectileThink},
	{"rocket_alt_func", FX_RocketAltProjectileThink},
	{"conc_func", FX_ConcProjectileThink},
	{"emplaced_func", FX_EmplacedProjectileThink},
	{"turret_func", FX_TurretProjectileThink},
	{"atstmain_func", FX_ATSTMainProjectileThink},
	{"atst_side_alt_func", FX_ATSTSideAltProjectileThink},
	{"atst_side_main_func", FX_ATSTSideMainProjectileThink},
	{"tusk_shot_func", FX_TuskenShotProjectileThink},
	{"noghri_shot_func", FX_NoghriShotProjectileThink},
	{NULL, NULL}};
#endif

//qboolean COM_ParseInt( char **data, int *i );
//qboolean COM_ParseString( char **data, char **s );
//qboolean COM_ParseFloat( char **data, float *f );

struct
{
	int weaponNum; // Current weapon number
	int ammoNum;
} wpnParms;

void WPN_Ammo(const char **holdBuf);
void WPN_AmmoIcon(const char **holdBuf);
void WPN_AmmoMax(const char **holdBuf);
void WPN_AmmoLowCnt(const char **holdBuf);
void WPN_AmmoType(const char **holdBuf);
void WPN_EnergyPerShot(const char **holdBuf);
void WPN_FireTime(const char **holdBuf);
void WPN_FiringSnd(const char **holdBuf);
void WPN_AltFiringSnd(const char **holdBuf);
void WPN_StopSnd(const char **holdBuf);
//void WPN_FlashSnd (char **holdBuf);
//void WPN_AltFlashSnd (char **holdBuf);
void WPN_ChargeSnd(const char **holdBuf);
void WPN_AltChargeSnd(const char **holdBuf);
void WPN_SelectSnd(const char **holdBuf);
void WPN_Range(const char **holdBuf);
void WPN_WeaponClass(const char **holdBuf);
void WPN_WeaponIcon(const char **holdBuf);
void WPN_WeaponModel(const char **holdBuf);
void WPN_WeaponType(const char **holdBuf);
void WPN_AltEnergyPerShot(const char **holdBuf);
void WPN_AltFireTime(const char **holdBuf);
void WPN_AltRange(const char **holdBuf);
void WPN_BarrelCount(const char **holdBuf);
void WPN_MissileName(const char **holdBuf);
void WPN_AltMissileName(const char **holdBuf);
void WPN_MissileSound(const char **holdBuf);
void WPN_AltMissileSound(const char **holdBuf);
void WPN_MissileLight(const char **holdBuf);
void WPN_AltMissileLight(const char **holdBuf);
void WPN_MissileLightColor(const char **holdBuf);
void WPN_AltMissileLightColor(const char **holdBuf);
void WPN_FuncName(const char **holdBuf);
void WPN_AltFuncName(const char **holdBuf);
void WPN_MissileHitSound(const char **holdBuf);
void WPN_AltMissileHitSound(const char **holdBuf);
void WPN_MuzzleEffect(const char **holdBuf);
void WPN_AltMuzzleEffect(const char **holdBuf);
//#ifdef _IMMERSION
void WPN_FiringFrc(const char **holdBuf);
void WPN_AltFiringFrc(const char **holdBuf);
void WPN_ChargeFrc(const char **holdBuf);
void WPN_AltChargeFrc(const char **holdBuf);
void WPN_StopFrc(const char **holdBuf);
void WPN_SelectFrc(const char **holdBuf);
//#endif // _IMMERSION

typedef struct
{
	char *parmName;
	void (*func)(const char **holdBuf);
} wpnParms_t;

wpnParms_t WpnParms[] =
	{
		"ammo", WPN_Ammo, //ammo
		"ammoicon", WPN_AmmoIcon,
		"ammomax", WPN_AmmoMax,
		"ammolowcount", WPN_AmmoLowCnt, //weapons
		"ammotype", WPN_AmmoType,
		"energypershot", WPN_EnergyPerShot,
		"fireTime", WPN_FireTime,
		"firingsound", WPN_FiringSnd,
		"altfiringsound", WPN_AltFiringSnd,
		//	"flashsound",		WPN_FlashSnd,
		//	"altflashsound",	WPN_AltFlashSnd,
		"stopsound", WPN_StopSnd,
		"chargesound", WPN_ChargeSnd,
		"altchargesound", WPN_AltChargeSnd,
		"selectsound", WPN_SelectSnd,
		"range", WPN_Range,
		"weaponclass", WPN_WeaponClass,
		"weaponicon", WPN_WeaponIcon,
		"weaponmodel", WPN_WeaponModel,
		"weapontype", WPN_WeaponType,
		"altenergypershot", WPN_AltEnergyPerShot,
		"altfireTime", WPN_AltFireTime,
		"altrange", WPN_AltRange,
		"barrelcount", WPN_BarrelCount,
		"missileModel", WPN_MissileName,
		"altmissileModel", WPN_AltMissileName,
		"missileSound", WPN_MissileSound,
		"altmissileSound", WPN_AltMissileSound,
		"missileLight", WPN_MissileLight,
		"altmissileLight", WPN_AltMissileLight,
		"missileLightColor", WPN_MissileLightColor,
		"altmissileLightColor", WPN_AltMissileLightColor,
		"missileFuncName", WPN_FuncName,
		"altmissileFuncName", WPN_AltFuncName,
		"missileHitSound", WPN_MissileHitSound,
		"altmissileHitSound", WPN_AltMissileHitSound,
		"muzzleEffect", WPN_MuzzleEffect,
		"altmuzzleEffect", WPN_AltMuzzleEffect
		//#ifdef _IMMERSION
		,
		"firingForce", WPN_FiringFrc, "altFiringForce", WPN_AltFiringFrc, "chargeForce", WPN_ChargeFrc, "altChargeForce", WPN_AltChargeFrc, "stopForce", WPN_StopFrc, "selectForce", WPN_SelectFrc
		//#endif // _IMMERSION
};

const int WPN_PARM_MAX = sizeof(WpnParms) / sizeof(WpnParms[0]);

void WPN_WeaponType(const char **holdBuf)
{
	int weaponNum;
	const char *tokenStr;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	// FIXME : put this in an array (maybe a weaponDataInternal array???)
	if (!Q_stricmp(tokenStr, "WP_NONE"))
		weaponNum = WP_NONE;
	else if (!Q_stricmp(tokenStr, "WP_SABER"))
		weaponNum = WP_SABER;
	else if (!Q_stricmp(tokenStr, "WP_BLASTER_PISTOL"))
		weaponNum = WP_BLASTER_PISTOL;
	else if (!Q_stricmp(tokenStr, "WP_BRYAR_PISTOL"))
		weaponNum = WP_BRYAR_PISTOL;
	else if (!Q_stricmp(tokenStr, "WP_BLASTER"))
		weaponNum = WP_BLASTER;
	else if (!Q_stricmp(tokenStr, "WP_DISRUPTOR"))
		weaponNum = WP_DISRUPTOR;
	else if (!Q_stricmp(tokenStr, "WP_BOWCASTER"))
		weaponNum = WP_BOWCASTER;
	else if (!Q_stricmp(tokenStr, "WP_REPEATER"))
		weaponNum = WP_REPEATER;
	else if (!Q_stricmp(tokenStr, "WP_DEMP2"))
		weaponNum = WP_DEMP2;
	else if (!Q_stricmp(tokenStr, "WP_FLECHETTE"))
		weaponNum = WP_FLECHETTE;
	else if (!Q_stricmp(tokenStr, "WP_ROCKET_LAUNCHER"))
		weaponNum = WP_ROCKET_LAUNCHER;
	else if (!Q_stricmp(tokenStr, "WP_CONCUSSION"))
		weaponNum = WP_CONCUSSION;
	else if (!Q_stricmp(tokenStr, "WP_THERMAL"))
		weaponNum = WP_THERMAL;
	else if (!Q_stricmp(tokenStr, "WP_TRIP_MINE"))
		weaponNum = WP_TRIP_MINE;
	else if (!Q_stricmp(tokenStr, "WP_DET_PACK"))
		weaponNum = WP_DET_PACK;
	else if (!Q_stricmp(tokenStr, "WP_STUN_BATON"))
		weaponNum = WP_STUN_BATON;
	else if (!Q_stricmp(tokenStr, "WP_BOT_LASER"))
		weaponNum = WP_BOT_LASER;
	else if (!Q_stricmp(tokenStr, "WP_EMPLACED_GUN"))
		weaponNum = WP_EMPLACED_GUN;
	else if (!Q_stricmp(tokenStr, "WP_MELEE"))
		weaponNum = WP_MELEE;
	else if (!Q_stricmp(tokenStr, "WP_TURRET"))
		weaponNum = WP_TURRET;
	else if (!Q_stricmp(tokenStr, "WP_ATST_MAIN"))
		weaponNum = WP_ATST_MAIN;
	else if (!Q_stricmp(tokenStr, "WP_ATST_SIDE"))
		weaponNum = WP_ATST_SIDE;
	else if (!Q_stricmp(tokenStr, "WP_TIE_FIGHTER"))
		weaponNum = WP_TIE_FIGHTER;
	else if (!Q_stricmp(tokenStr, "WP_RAPID_FIRE_CONC"))
		weaponNum = WP_RAPID_FIRE_CONC;
	else if (!Q_stricmp(tokenStr, "WP_JAWA"))
		weaponNum = WP_JAWA;
	else if (!Q_stricmp(tokenStr, "WP_TUSKEN_RIFLE"))
		weaponNum = WP_TUSKEN_RIFLE;
	else if (!Q_stricmp(tokenStr, "WP_TUSKEN_STAFF"))
		weaponNum = WP_TUSKEN_STAFF;
	else if (!Q_stricmp(tokenStr, "WP_SCEPTER"))
		weaponNum = WP_SCEPTER;
	else if (!Q_stricmp(tokenStr, "WP_NOGHRI_STICK"))
		weaponNum = WP_NOGHRI_STICK;
	else
	{
		weaponNum = 0;
		gi.Printf(S_COLOR_YELLOW "WARNING: bad weapontype in external weapon data '%s'\n", tokenStr);
	}

	wpnParms.weaponNum = weaponNum;
}

//--------------------------------------------
void WPN_WeaponClass(const char **holdBuf)
{
	int len;
	const char *tokenStr;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 32)
	{
		len = 32;
		gi.Printf(S_COLOR_YELLOW "WARNING: weaponclass too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].classname, tokenStr, len);
}

//--------------------------------------------
void WPN_WeaponModel(const char **holdBuf)
{
	int len;
	const char *tokenStr;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: weaponMdl too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].weaponMdl, tokenStr, len);
}

//--------------------------------------------
void WPN_WeaponIcon(const char **holdBuf)
{
	int len;
	const char *tokenStr;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: weaponIcon too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].weaponIcon, tokenStr, len);
}

//--------------------------------------------
void WPN_AmmoType(const char **holdBuf)
{
	int tokenInt;

	if (COM_ParseInt(holdBuf, &tokenInt))
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < AMMO_NONE) || (tokenInt >= AMMO_MAX))
	{
		gi.Printf(S_COLOR_YELLOW "WARNING: bad Ammotype in external weapon data '%d'\n", tokenInt);
		return;
	}

	weaponData[wpnParms.weaponNum].ammoIndex = tokenInt;
}

//--------------------------------------------
void WPN_AmmoLowCnt(const char **holdBuf)
{
	int tokenInt;

	if (COM_ParseInt(holdBuf, &tokenInt))
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < 0) || (tokenInt > 200)) // FIXME :What are the right values?
	{
		gi.Printf(S_COLOR_YELLOW "WARNING: bad Ammolowcount in external weapon data '%d'\n", tokenInt);
		return;
	}

	weaponData[wpnParms.weaponNum].ammoLow = tokenInt;
}

//--------------------------------------------
void WPN_FiringSnd(const char **holdBuf)
{
	const char *tokenStr;
	int len;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: firingSnd too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].firingSnd, tokenStr, len);
}

//--------------------------------------------
void WPN_AltFiringSnd(const char **holdBuf)
{
	const char *tokenStr;
	int len;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: altFiringSnd too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].altFiringSnd, tokenStr, len);
}

//--------------------------------------------
void WPN_StopSnd(const char **holdBuf)
{
	const char *tokenStr;
	int len;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: stopSnd too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].stopSnd, tokenStr, len);
}
/*
//--------------------------------------------
void WPN_FlashSnd(const char **holdBuf)
{
	const char	*tokenStr;
	int		len;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: flashSnd too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].flashSnd,tokenStr,len);
}

//--------------------------------------------
void WPN_AltFlashSnd(const char **holdBuf)
{
	const char	*tokenStr;
	int		len;

	if ( COM_ParseString(holdBuf,&tokenStr)) 
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW"WARNING: altFlashSnd too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].altFlashSnd,tokenStr,len);
}
*/
//--------------------------------------------
void WPN_ChargeSnd(const char **holdBuf)
{
	const char *tokenStr;
	int len;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: chargeSnd too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].chargeSnd, tokenStr, len);
}

//--------------------------------------------
void WPN_AltChargeSnd(const char **holdBuf)
{
	const char *tokenStr;
	int len;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: altChargeSnd too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].altChargeSnd, tokenStr, len);
}

//--------------------------------------------
void WPN_SelectSnd(const char **holdBuf)
{
	const char *tokenStr;
	int len;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;

	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: selectSnd too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].selectSnd, tokenStr, len);
}

//#ifdef _IMMERSION

//--------------------------------------------
void WPN_FiringFrc(const char **holdBuf)
{
	const char *tokenStr;
	int len;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;

	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: firingFrc too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

#ifdef _IMMERSION
	Q_strncpyz(weaponData[wpnParms.weaponNum].firingFrc, tokenStr, len);
#endif
}

//--------------------------------------------
void WPN_AltFiringFrc(const char **holdBuf)
{
	const char *tokenStr;
	int len;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;

	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: altFiringFrc too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

#ifdef _IMMERSION
	Q_strncpyz(weaponData[wpnParms.weaponNum].altFiringFrc, tokenStr, len);
#endif
}

//--------------------------------------------
void WPN_ChargeFrc(const char **holdBuf)
{
	const char *tokenStr;
	int len;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;

	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: chargeFrc too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

#ifdef _IMMERSION
	Q_strncpyz(weaponData[wpnParms.weaponNum].chargeFrc, tokenStr, len);
#endif
}

//--------------------------------------------
void WPN_AltChargeFrc(const char **holdBuf)
{
	const char *tokenStr;
	int len;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;

	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: altChargeFrc too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

#ifdef _IMMERSION
	Q_strncpyz(weaponData[wpnParms.weaponNum].altChargeFrc, tokenStr, len);
#endif
}

//--------------------------------------------
void WPN_StopFrc(const char **holdBuf)
{
	const char *tokenStr;
	int len;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;

	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: stopFrc too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

#ifdef _IMMERSION
	Q_strncpyz(weaponData[wpnParms.weaponNum].stopFrc, tokenStr, len);
#endif
}

//--------------------------------------------
void WPN_SelectFrc(const char **holdBuf)
{
	const char *tokenStr;
	int len;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;

	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: selectFrc too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

#ifdef _IMMERSION
	Q_strncpyz(weaponData[wpnParms.weaponNum].selectFrc, tokenStr, len);
#endif
}

//#endif // _IMMERSION

//--------------------------------------------
void WPN_FireTime(const char **holdBuf)
{
	int tokenInt;

	if (COM_ParseInt(holdBuf, &tokenInt))
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < 0) || (tokenInt > 10000)) // FIXME :What are the right values?
	{
		gi.Printf(S_COLOR_YELLOW "WARNING: bad Firetime in external weapon data '%d'\n", tokenInt);
		return;
	}
	weaponData[wpnParms.weaponNum].fireTime = tokenInt;
}

//--------------------------------------------
void WPN_Range(const char **holdBuf)
{
	int tokenInt;

	if (COM_ParseInt(holdBuf, &tokenInt))
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < 0) || (tokenInt > 10000)) // FIXME :What are the right values?
	{
		gi.Printf(S_COLOR_YELLOW "WARNING: bad Range in external weapon data '%d'\n", tokenInt);
		return;
	}

	weaponData[wpnParms.weaponNum].range = tokenInt;
}

//--------------------------------------------
void WPN_EnergyPerShot(const char **holdBuf)
{
	int tokenInt;

	if (COM_ParseInt(holdBuf, &tokenInt))
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < 0) || (tokenInt > 1000)) // FIXME :What are the right values?
	{
		gi.Printf(S_COLOR_YELLOW "WARNING: bad EnergyPerShot in external weapon data '%d'\n", tokenInt);
		return;
	}
	weaponData[wpnParms.weaponNum].energyPerShot = tokenInt;
}

//--------------------------------------------
void WPN_AltFireTime(const char **holdBuf)
{
	int tokenInt;

	if (COM_ParseInt(holdBuf, &tokenInt))
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < 0) || (tokenInt > 10000)) // FIXME :What are the right values?
	{
		gi.Printf(S_COLOR_YELLOW "WARNING: bad altFireTime in external weapon data '%d'\n", tokenInt);
		return;
	}
	weaponData[wpnParms.weaponNum].altFireTime = tokenInt;
}

//--------------------------------------------
void WPN_AltRange(const char **holdBuf)
{
	int tokenInt;

	if (COM_ParseInt(holdBuf, &tokenInt))
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < 0) || (tokenInt > 10000)) // FIXME :What are the right values?
	{
		gi.Printf(S_COLOR_YELLOW "WARNING: bad AltRange in external weapon data '%d'\n", tokenInt);
		return;
	}

	weaponData[wpnParms.weaponNum].altRange = tokenInt;
}

//--------------------------------------------
void WPN_AltEnergyPerShot(const char **holdBuf)
{
	int tokenInt;

	if (COM_ParseInt(holdBuf, &tokenInt))
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < 0) || (tokenInt > 1000)) // FIXME :What are the right values?
	{
		gi.Printf(S_COLOR_YELLOW "WARNING: bad AltEnergyPerShot in external weapon data '%d'\n", tokenInt);
		return;
	}
	weaponData[wpnParms.weaponNum].altEnergyPerShot = tokenInt;
}

//--------------------------------------------
void WPN_Ammo(const char **holdBuf)
{
	const char *tokenStr;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	if (!Q_stricmp(tokenStr, "AMMO_NONE"))
		wpnParms.ammoNum = AMMO_NONE;
	else if (!Q_stricmp(tokenStr, "AMMO_FORCE"))
		wpnParms.ammoNum = AMMO_FORCE;
	else if (!Q_stricmp(tokenStr, "AMMO_BLASTER"))
		wpnParms.ammoNum = AMMO_BLASTER;
	else if (!Q_stricmp(tokenStr, "AMMO_POWERCELL"))
		wpnParms.ammoNum = AMMO_POWERCELL;
	else if (!Q_stricmp(tokenStr, "AMMO_METAL_BOLTS"))
		wpnParms.ammoNum = AMMO_METAL_BOLTS;
	else if (!Q_stricmp(tokenStr, "AMMO_ROCKETS"))
		wpnParms.ammoNum = AMMO_ROCKETS;
	else if (!Q_stricmp(tokenStr, "AMMO_EMPLACED"))
		wpnParms.ammoNum = AMMO_EMPLACED;
	else if (!Q_stricmp(tokenStr, "AMMO_THERMAL"))
		wpnParms.ammoNum = AMMO_THERMAL;
	else if (!Q_stricmp(tokenStr, "AMMO_TRIPMINE"))
		wpnParms.ammoNum = AMMO_TRIPMINE;
	else if (!Q_stricmp(tokenStr, "AMMO_DETPACK"))
		wpnParms.ammoNum = AMMO_DETPACK;
	else
	{
		gi.Printf(S_COLOR_YELLOW "WARNING: bad ammotype in external weapon data '%s'\n", tokenStr);
		wpnParms.ammoNum = 0;
	}
}

//--------------------------------------------
void WPN_AmmoIcon(const char **holdBuf)
{
	const char *tokenStr;
	int len;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 32)
	{
		len = 32;
		gi.Printf(S_COLOR_YELLOW "WARNING: ammoicon too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(ammoData[wpnParms.ammoNum].icon, tokenStr, len);
}

//--------------------------------------------
void WPN_AmmoMax(const char **holdBuf)
{
	int tokenInt;

	if (COM_ParseInt(holdBuf, &tokenInt))
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < 0) || (tokenInt > 1000))
	{
		gi.Printf(S_COLOR_YELLOW "WARNING: bad Ammo Max in external weapon data '%d'\n", tokenInt);
		return;
	}
	ammoData[wpnParms.ammoNum].max = tokenInt;
}

//--------------------------------------------
void WPN_BarrelCount(const char **holdBuf)
{
	int tokenInt;

	if (COM_ParseInt(holdBuf, &tokenInt))
	{
		SkipRestOfLine(holdBuf);
		return;
	}

	if ((tokenInt < 0) || (tokenInt > 4))
	{
		gi.Printf(S_COLOR_YELLOW "WARNING: bad Range in external weapon data '%d'\n", tokenInt);
		return;
	}

	weaponData[wpnParms.weaponNum].numBarrels = tokenInt;
}

//--------------------------------------------
static void WP_ParseWeaponParms(const char **holdBuf)
{
	const char *token;
	int i;

	while (holdBuf)
	{
		token = COM_ParseExt(holdBuf, qtrue);

		if (!Q_stricmp(token, "}")) // End of data for this weapon
			break;

		// Loop through possible parameters
		for (i = 0; i < WPN_PARM_MAX; ++i)
		{
			if (!Q_stricmp(token, WpnParms[i].parmName))
			{
				WpnParms[i].func(holdBuf);
				break;
			}
		}

		if (i < WPN_PARM_MAX) // Find parameter???
		{
			continue;
		}
		Com_Error(ERR_FATAL, "bad parameter in external weapon data '%s'\n", token);
	}
}

//--------------------------------------------
void WPN_MissileName(const char **holdBuf)
{
	int len;
	const char *tokenStr;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: MissileName too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].missileMdl, tokenStr, len);
}

//--------------------------------------------
void WPN_AltMissileName(const char **holdBuf)
{
	int len;
	const char *tokenStr;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: AltMissileName too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].alt_missileMdl, tokenStr, len);
}

//--------------------------------------------
void WPN_MissileHitSound(const char **holdBuf)
{
	int len;
	const char *tokenStr;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: MissileHitSound too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].missileHitSound, tokenStr, len);
}

//--------------------------------------------
void WPN_AltMissileHitSound(const char **holdBuf)
{
	int len;
	const char *tokenStr;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: AltMissileHitSound too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].altmissileHitSound, tokenStr, len);
}

//--------------------------------------------
void WPN_MissileSound(const char **holdBuf)
{
	int len;
	const char *tokenStr;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: MissileSound too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].missileSound, tokenStr, len);
}

//--------------------------------------------
void WPN_AltMissileSound(const char **holdBuf)
{
	int len;
	const char *tokenStr;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: AltMissileSound too long in external WEAPONS.DAT '%s'\n", tokenStr);
	}

	Q_strncpyz(weaponData[wpnParms.weaponNum].alt_missileSound, tokenStr, len);
}

//--------------------------------------------
void WPN_MissileLightColor(const char **holdBuf)
{
	int i;
	float tokenFlt;

	for (i = 0; i < 3; ++i)
	{
		if (COM_ParseFloat(holdBuf, &tokenFlt))
		{
			SkipRestOfLine(holdBuf);
			continue;
		}

		if ((tokenFlt < 0) || (tokenFlt > 1))
		{
			gi.Printf(S_COLOR_YELLOW "WARNING: bad missilelightcolor in external weapon data '%f'\n", tokenFlt);
			continue;
		}
		weaponData[wpnParms.weaponNum].missileDlightColor[i] = tokenFlt;
	}
}

//--------------------------------------------
void WPN_AltMissileLightColor(const char **holdBuf)
{
	int i;
	float tokenFlt;

	for (i = 0; i < 3; ++i)
	{
		if (COM_ParseFloat(holdBuf, &tokenFlt))
		{
			SkipRestOfLine(holdBuf);
			continue;
		}

		if ((tokenFlt < 0) || (tokenFlt > 1))
		{
			gi.Printf(S_COLOR_YELLOW "WARNING: bad altmissilelightcolor in external weapon data '%f'\n", tokenFlt);
			continue;
		}
		weaponData[wpnParms.weaponNum].alt_missileDlightColor[i] = tokenFlt;
	}
}

//--------------------------------------------
void WPN_MissileLight(const char **holdBuf)
{
	float tokenFlt;

	if (COM_ParseFloat(holdBuf, &tokenFlt))
	{
		SkipRestOfLine(holdBuf);
	}

	if ((tokenFlt < 0) || (tokenFlt > 255)) // FIXME :What are the right values?
	{
		gi.Printf(S_COLOR_YELLOW "WARNING: bad missilelight in external weapon data '%f'\n", tokenFlt);
	}
	weaponData[wpnParms.weaponNum].missileDlight = tokenFlt;
}

//--------------------------------------------
void WPN_AltMissileLight(const char **holdBuf)
{
	float tokenFlt;

	if (COM_ParseFloat(holdBuf, &tokenFlt))
	{
		SkipRestOfLine(holdBuf);
	}

	if ((tokenFlt < 0) || (tokenFlt > 255)) // FIXME :What are the right values?
	{
		gi.Printf(S_COLOR_YELLOW "WARNING: bad altmissilelight in external weapon data '%f'\n", tokenFlt);
	}
	weaponData[wpnParms.weaponNum].alt_missileDlight = tokenFlt;
}

//--------------------------------------------
void WPN_FuncName(const char **holdBuf)
{
	const char *tokenStr;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}
	// ONLY DO THIS ON THE GAME SIDE
#ifndef _USRDLL
	int len = strlen(tokenStr);

	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: FuncName '%s' too long in external WEAPONS.DAT\n", tokenStr);
	}

	for (func_t *s = funcs; s->name; s++)
	{
		if (!Q_stricmp(s->name, tokenStr))
		{
			// found it
			weaponData[wpnParms.weaponNum].func = (void *)s->func;
			return;
		}
	}
	gi.Printf(S_COLOR_YELLOW "WARNING: FuncName '%s' in external WEAPONS.DAT does not exist\n", tokenStr);
#endif
}

//--------------------------------------------
void WPN_AltFuncName(const char **holdBuf)
{
	const char *tokenStr;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}

	// ONLY DO THIS ON THE GAME SIDE
#ifndef _USRDLL
	int len = strlen(tokenStr);
	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: AltFuncName '%s' too long in external WEAPONS.DAT\n", tokenStr);
	}

	for (func_t *s = funcs; s->name; s++)
	{
		if (!Q_stricmp(s->name, tokenStr))
		{
			// found it
			weaponData[wpnParms.weaponNum].altfunc = (void *)s->func;
			return;
		}
	}
	gi.Printf(S_COLOR_YELLOW "WARNING: AltFuncName %s in external WEAPONS.DAT does not exist\n", tokenStr);

#endif
}

//--------------------------------------------
void WPN_MuzzleEffect(const char **holdBuf)
{
	const char *tokenStr;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}
	// ONLY DO THIS ON THE GAME SIDE
#ifndef _USRDLL

	int len = strlen(tokenStr);

	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: MuzzleEffect '%s' too long in external WEAPONS.DAT\n", tokenStr);
	}

	G_EffectIndex(tokenStr);
	Q_strncpyz(weaponData[wpnParms.weaponNum].mMuzzleEffect, tokenStr, len);

#endif
}

//--------------------------------------------
void WPN_AltMuzzleEffect(const char **holdBuf)
{
	const char *tokenStr;

	if (COM_ParseString(holdBuf, &tokenStr))
	{
		return;
	}
	// ONLY DO THIS ON THE GAME SIDE
#ifndef _USRDLL

	int len = strlen(tokenStr);

	len++;
	if (len > 64)
	{
		len = 64;
		gi.Printf(S_COLOR_YELLOW "WARNING: AltMuzzleEffect '%s' too long in external WEAPONS.DAT\n", tokenStr);
	}

	G_EffectIndex(tokenStr);
	Q_strncpyz(weaponData[wpnParms.weaponNum].mAltMuzzleEffect, tokenStr, len);

#endif
}

//--------------------------------------------
static void WP_ParseParms(const char *buffer)
{
	const char *holdBuf;
	const char *token;

	holdBuf = buffer;
	COM_BeginParseSession();

	while (holdBuf)
	{
		token = COM_ParseExt(&holdBuf, qtrue);

		if (!Q_stricmp(token, "{"))
		{
			token = token;
			WP_ParseWeaponParms(&holdBuf);
		}
	}
}

//--------------------------------------------
void WP_LoadWeaponParms(void)
{
	char *buffer;
	int len;

	len = gi.FS_ReadFile("ext_data/weapons.dat", (void **)&buffer);

	if (len == -1)
	{
		Com_Error(ERR_FATAL, "Cannot find ext_data/weapons.dat!\n");
	}

	// initialise the data area
	memset(weaponData, 0, WP_NUM_WEAPONS * sizeof(weaponData_t));

	WP_ParseParms(buffer);

	gi.FS_FreeFile(buffer); //let go of the buffer
}