// DEMP2 Weapon

// this line must stay at top so the whole PCH thing works...
#include "cg_headers.h"

//#include "cg_local.h"
#include "cg_media.h"
#include "FxScheduler.h"
#include "FxUtil.h"

/*
---------------------------
FX_DEMP2_ProjectileThink
---------------------------
*/

void FX_DEMP2_ProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon)
{
	vec3_t forward;

	if (VectorNormalize2(cent->currentState.pos.trDelta, forward) == 0.0f)
	{
		forward[2] = 1.0f;
	}

	//	theFxScheduler.PlayEffect( "demp2/shot", cent->lerpOrigin, forward );
	//	theFxScheduler.PlayEffect( "demp2/shot2", cent->lerpOrigin, forward );
	theFxScheduler.PlayEffect("demp2/projectile", cent->lerpOrigin, forward);
}

/*
---------------------------
FX_DEMP2_HitWall
---------------------------
*/

void FX_DEMP2_HitWall(vec3_t origin, vec3_t normal)
{
	theFxScheduler.PlayEffect("demp2/wall_impact", origin, normal);
}

/*
---------------------------
FX_DEMP2_HitPlayer
---------------------------
*/

void FX_DEMP2_HitPlayer(vec3_t origin, vec3_t normal, qboolean humanoid)
{
	theFxScheduler.PlayEffect("demp2/flesh_impact", origin, normal);
}

/*
---------------------------
FX_DEMP2_AltProjectileThink
---------------------------
*/

void FX_DEMP2_AltProjectileThink(centity_t *cent, const struct weaponInfo_s *weapon)
{
	vec3_t forward;

	if (VectorNormalize2(cent->currentState.pos.trDelta, forward) == 0.0f)
	{
		forward[2] = 1.0f;
	}

	theFxScheduler.PlayEffect("demp2/projectile", cent->lerpOrigin, forward);
}

//---------------------------------------------
void FX_DEMP2_AltDetonate(vec3_t org, float size)
{
	localEntity_t *ex;

	ex = CG_AllocLocalEntity();
	ex->leType = LE_FADE_SCALE_MODEL;
	memset(&ex->refEntity, 0, sizeof(refEntity_t));

	ex->refEntity.renderfx |= RF_VOLUMETRIC;

	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 1300;

	ex->radius = size;
	ex->refEntity.customShader = cgi_R_RegisterShader("gfx/effects/demp2shell");

	ex->refEntity.hModel = cgi_R_RegisterModel("models/items/sphere.md3");
	VectorCopy(org, ex->refEntity.origin);

	ex->color[0] = ex->color[1] = ex->color[2] = 255.0f;
}
