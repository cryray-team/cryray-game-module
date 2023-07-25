// OldSerpskiStaler
// Внутренний движковый конфиг

#include "StdAfx.h"
#include "CryRayGameConstants.h"
#include "GamePersistent.h"

signed int noviceRankStart = 0;
signed int experiencedRankStart = 0;
signed int veteranRankStart = 0;
signed int masterRankStart = 0;

float artefact_koef_reaction = 0.f;
float artefact_reaction_value = 0.f;

namespace CryRayParams
{
    CryRayClass CRParams;

    // Деструкторы класса
    CryRayClass::CryRayClass(){};
    CryRayClass::~CryRayClass(){}

    void CryRayCheckParams()
    {
        Msg(
            "# [CryRayParams]: Start game (+ NPC): Novice: (%i), Experienced: (%i), Veteran: (%i), Master: (%i)",
            noviceRankStart, experiencedRankStart, veteranRankStart, masterRankStart);
        Msg(
            "# [CryRayParams]: Start game (+ Artefact): artefact_koef_reaction (%f), artefact_reaction_v (%f)",
            artefact_koef_reaction, artefact_reaction_value);
        Msg("# [CryRayParams]: Check default params!");
    }

    void CryRayClass::LoadParameters()
    {
        // Ранги НПС по дефолту:
        noviceRankStart = READ_IF_EXISTS(pCryRaySettings, r_s32, "game_relations", "novice_rank", 0);
        experiencedRankStart = READ_IF_EXISTS(pCryRaySettings, r_s32, "game_relations", "experienced_rank", 300);
        veteranRankStart = READ_IF_EXISTS(pCryRaySettings, r_s32, "game_relations", "veteran_rank", 600);
        masterRankStart = READ_IF_EXISTS(pCryRaySettings, r_s32, "game_relations", "master_rank", 900);
        Msg(
            "# [CryRayParams]: Loaded parameters: Novice: (%i), Experienced: (%i), Veteran: (%i), Master: (%i)",
            noviceRankStart, experiencedRankStart, veteranRankStart, masterRankStart);

        // Артефакты:
        artefact_koef_reaction = READ_IF_EXISTS(pCryRaySettings, r_float, "artefacts_cryray", "artefact_koef_reaction", 100.f);
        artefact_reaction_value = READ_IF_EXISTS(pCryRaySettings, r_float, "artefacts_cryray", "artefact_reaction_v", 0.00015f);

        Msg("# [CryRayParams]: Loaded parametrs: artefact_koef_reaction (%f), artefact_reaction_v (%f)",
            artefact_koef_reaction, artefact_reaction_value);

        Msg("# [CryRayParams]: Are Loaded!");
    }

    void CryRayClass::save(NET_Packet& output_packet)
    {
        save_data(pSGameSettings->fEStunZoomWpn, output_packet);
    }

    void CryRayClass::load(IReader& input_packet)
    {
        load_data(pSGameSettings->fEStunZoomWpn, input_packet);
    }

    int CryRayClass::GetNoviceRankStart() { return noviceRankStart; }

    int CryRayClass::GetExperiencesRankStart() { return experiencedRankStart; }

    int CryRayClass::GetVeteranRankStart() { return veteranRankStart; }

    int CryRayClass::GetMasterRankStart() { return masterRankStart; }

    float CryRayClass::af_koef() { return artefact_koef_reaction; }

    const float CryRayClass::artefact_reac_value() { return artefact_reaction_value; }
} // namespace CryRayParams
