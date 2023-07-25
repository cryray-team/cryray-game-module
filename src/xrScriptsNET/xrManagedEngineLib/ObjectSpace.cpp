#include "stdafx.h"
#include "ObjectSpace.h"
#include "../xrEngine/IGame_Level.h"

namespace XRay
{
	ObjectSpace::ObjectSpace()
	{
		// We use global object, not need to initialize pNativeObject
		pNativeObject = nullptr;
		current_object = nullptr;
	}

	ObjectSpace::ObjectSpace(IntPtr InNativeObject)
	{
		CAST_TO_NATIVE_OBJECT(CObjectSpace, InNativeObject);
	}

	String^ ObjectSpace::GetHittedObjectName()
	{
		String^ result;
		if (current_object)
		{
			result = gcnew String(current_object->cName().data());
		}
		else
		{
			result = gcnew String("");

		}
		return result;
	}

	XRay::Object^ ObjectSpace::GetHittedObject()
	{
		// @ How to cast (I guess init) XRay::Object to (with) CObject and use it??? 
		return nullptr;
	}

	bool ObjectSpace::RayPick(Vector3^ start, Vector3^ dir, float range, RQ_TraceType type, XRay::Object^ ignore_object)
	{
		Fvector s;
		s.x = start->X;
		s.y = start->Y;
		s.z = start->Z;

		Fvector d;
		d.x = dir->X;
		d.y = dir->Y;
		d.z = dir->Z;

		collide::rq_target b = (collide::rq_target)type;

		collide::rq_result object_status;
		CObject* obj = (CObject*)ignore_object->GetNativeObject().ToPointer();  
		bool status = g_pGameLevel->ObjectSpace.RayPick(s, d, range, b, object_status, obj);
		if (status)
		{
			if (object_status.O)
			{
				current_object = object_status.O;
			}
			else
			{
				current_object = nullptr;
			}
		}

		return status;
	}

	
	bool ObjectSpace::RayTest(Vector3^ start, Vector3^ dir, float range, RQ_TraceType type, XRay::Object^ ignore_object)
	{
		Fvector s; 
		s.x = start->X;
		s.y = start->Y;
		s.z = start->Z;

		Fvector d;
		d.x = start->X;
		d.y = start->Y;
		d.z = start->Z;

		collide::rq_target b = (collide::rq_target)type;
		CObject* obj = (CObject*)ignore_object->GetNativeObject().ToPointer();
		bool status = g_pGameLevel->ObjectSpace.RayTest(s, d, range, b, nullptr, obj);
		
		return status;
	}

	bool ObjectSpace::RayPickFromActor(RQ_TraceType type)
	{
 
		return false;
	}
}

