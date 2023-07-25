#pragma once
#include "actor.h"

namespace CryRayParams
{
    void CryRayCheckParams();

    class CryRayClass
    {
    public:
        CryRayClass();
        ~CryRayClass();

        void LoadParameters();

        int GetNoviceRankStart();
        int GetExperiencesRankStart();
        int GetVeteranRankStart();
        int GetMasterRankStart();

        float af_koef();
        const float artefact_reac_value();

        virtual void save(NET_Packet& output_packet);
        virtual void load(IReader& input_packet);
    };
}; // namespace CryRayParams
