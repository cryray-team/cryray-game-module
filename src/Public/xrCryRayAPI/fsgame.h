#pragma once
#include "../xrCryRayAPI/xrCryRayAPI.h"

namespace fsgame
{
    enum class EGame
    {
        type1,
        type2,
    };

    enum class EGamePath
    {
        fsgame = -1,
        call_of_chernobyl_1_4_22,
        call_of_chernobyl_anomaly_1_5_2,
    };

    class XRCRYRAY_API xrGameManager
    {
    public:
        static EGame GetGame();
        static EGamePath GetPath();
    };

    extern XRCRYRAY_API bool IsDebugFsGame();
    extern XRCRYRAY_API EGamePath GCurrentGame;
} // namespace fsgame