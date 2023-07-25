#pragma once

#ifdef	MANAGED_GAME_BUILD
#define MANAGED_GAME_API		__declspec(dllexport)
#else
#define MANAGED_GAME_API		__declspec(dllimport)
#endif