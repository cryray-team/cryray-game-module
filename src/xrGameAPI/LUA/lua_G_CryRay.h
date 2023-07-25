#pragma once

namespace lua_G_CryRay
{
	void KillActorScript();

	void GameOverScript();
    void UseDoorScript();
    void ControllerPsyHitScript();
    void SnorkHitInJumpScript();
    void PseudoGigantThreatenExecuteScript();
    void CDetectorCheckerScript();
    void CFlashCardScript();
    void UpdateReactionArtefactScript();
    void ActorShowPDAONScript();
    void ActorShowPDAOFFScript();
    void WeaponJammedScript();
    void ActorShowMenuOnScript();
    void ActorShowMenuOffScript();
    void DistToExplodeHit();

    float GetHitExplode();

    float set_blender_mode_main(float blender_num);
    float get_blender_mode_main();

    float set_blender_mode_second(float blender_num);
    float get_blender_mode_second();

    void set_shader_params(const Fmatrix&);
    void get_gist_to_explode_hit(float v);

    Fmatrix get_shader_params();
    }