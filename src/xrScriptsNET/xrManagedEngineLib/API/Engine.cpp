#include "stdafx.h"
#include "Engine.h"

void XRay::Engine::LoadAllArchives()
{
	pApp->LoadAllArchives();
}

XRay::LevelInfo XRay::Engine::GetLevelInfoFromSource(CApplication::sLevelInfo& LevelInfo)
{
	XRay::LevelInfo Result;

	Result.Name = gcnew String(LevelInfo.name);
	Result.Folder = gcnew String(LevelInfo.folder);

	return Result;
}

void XRay::Engine::RescanLevels()
{
	pApp->Level_Scan();
}