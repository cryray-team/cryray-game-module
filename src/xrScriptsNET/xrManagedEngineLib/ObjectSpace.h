#pragma once
class CLevel;
class CObjectSpace;
using namespace System;

#include "API/Object.h"

namespace XRay
{
	public enum class RQ_TraceType
	{
		None = (0),
		Object = (1 << 0),
		Static = (1 << 1),
		Shape = (1 << 2),
		Obstacle = (1 << 3),
		Both = (Object | Static),
		rqtDyn = (Object | Shape | Obstacle)
	};

	public ref class ObjectSpace
	{
	public:
		ObjectSpace();
		ObjectSpace(IntPtr InNativeObject);
		String^ GetHittedObjectName();
		XRay::Object^ GetHittedObject(); 
		bool RayPick(Vector3^, Vector3^, float, RQ_TraceType, XRay::Object^);
		bool RayTest(Vector3^, Vector3^, float, RQ_TraceType, XRay::Object^);
		bool RayPickFromActor(RQ_TraceType type);
	private:
		CObject* current_object;
		CObjectSpace* pNativeObject;
	};


}
