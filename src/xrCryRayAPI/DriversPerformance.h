#pragma once
#include "wtypes.h"

//-' Always request high performance GPU
extern "C" 
{
	//-' https://docs.nvidia.com/gameworks/content/technologies/desktop/optimus.htm
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001; //-' NVIDIA Optimus

	//-' https://gpuopen.com/amdpowerxpressrequesthighperformance/
	_declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001; //-' PowerXpress or Hybrid Graphics
}