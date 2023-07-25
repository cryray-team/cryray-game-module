#pragma once
#include "../xrEngine/x_ray.h"
#include "LevelInfo.h"

namespace XRay
{
	public ref class Engine abstract sealed
	{
	public:

		static property array<LevelInfo>^ Levels
		{
			array<LevelInfo>^ get()
			{
				size_t NumLevels = pApp->Levels.size();
				array<LevelInfo>^ Result = gcnew array<LevelInfo>((u32)NumLevels);

				for (u32 i = 0; i < NumLevels; ++i)
				{
					Result[i] = GetLevelInfoFromSource(pApp->Levels[i]);
				}

				return Result;
			}
		}

		static property LevelInfo CurrentLevel
		{
			LevelInfo get()
			{
				if (pApp->Level_Current < pApp->Levels.size())
				{
					return GetLevelInfoFromSource(pApp->Levels[pApp->Level_Current]);
				}

				LevelInfo Null;
				return Null;
			}
		}

		static void RescanLevels();
		static void LoadAllArchives();

	private:
		static LevelInfo GetLevelInfoFromSource(CApplication::sLevelInfo& LevelInfo);
	};
}