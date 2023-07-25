#pragma once
#include "../xrEngine/xr_object.h"
#include "NativeObject.h"

using namespace System;
using namespace System::Numerics;

namespace XRay
{
	// @ CObject class
	public ref class Object : public NativeObject
	{
	public:

		property String^ Name
		{
			String^ get()
			{
				shared_str CurrentName = pNativeObject->cName();
				return gcnew String(CurrentName.data(), 0, CurrentName.size());
			}

			void set(String^ value)
			{
				string512 ObjectName;
				ConvertDotNetStringToAscii(value, ObjectName);
				pNativeObject->cName_set(ObjectName);
			}
		}

		property String^ SectionName
		{
			String^ get()
			{
				shared_str CurrentSectionName = pNativeObject->cNameSect();
				return gcnew String(CurrentSectionName.data(), 0, CurrentSectionName.size());
			}

			void set(String^ value)
			{
				string512 SectionName;
				ConvertDotNetStringToAscii(value, SectionName);
				pNativeObject->cNameSect_set(SectionName);
			}
		}

		property String^ VisualName
		{
			String^ get()
			{
				shared_str CurrentVisualName = pNativeObject->cNameVisual();
				return gcnew String(CurrentVisualName.data(), 0, CurrentVisualName.size());
			}

			void set(String^ value)
			{
				string512 VisualName;
				ConvertDotNetStringToAscii(value, VisualName);
				pNativeObject->cNameVisual_set(VisualName);
			}
		}

		property Vector3 Position
		{
			Vector3 get()
			{
				return CONVERT_FVECTOR(pNativeObject->Position());
			}

			void set(Vector3 value)
			{
				pNativeObject->Position().set(value.X, value.Y, value.Z);
			}
		}

		property Vector3 Direction
		{
			Vector3 get()
			{
				return CONVERT_FVECTOR(pNativeObject->Direction());
			}

			void set(Vector3 value)
			{
				pNativeObject->Direction().set(value.X, value.Y, value.Z);
			}
		}

		property Numerics::Matrix4x4 XForm
		{
			Numerics::Matrix4x4 get()
			{
				return CONVERT_FMATRIX(pNativeObject->XFORM());
			}

			void set(Numerics::Matrix4x4 value)
			{
				Fmatrix& matrix = pNativeObject->XFORM();
				matrix.m[1][1] = value.M11; matrix.m[1][2] = value.M12; matrix.m[1][3] = value.M13; matrix.m[1][4] = value.M14;
				matrix.m[2][1] = value.M21; matrix.m[2][2] = value.M22; matrix.m[2][3] = value.M23; matrix.m[2][4] = value.M24;
				matrix.m[3][1] = value.M31; matrix.m[3][2] = value.M32; matrix.m[3][3] = value.M33; matrix.m[3][4] = value.M34;
				matrix.m[4][1] = value.M41; matrix.m[4][2] = value.M42; matrix.m[4][3] = value.M43; matrix.m[4][4] = value.M44;
			}
		}

		property Object^ Parent
		{
			Object^ get()
			{
				CObject* parent = pNativeObject->H_Parent();
				return (Object^)Object::Create(IntPtr(parent), Object::typeid);
			}

			void set(Object^ value)
			{
				IntPtr Ptr = value->GetNativeObject();
				pNativeObject->H_SetParent((CObject*)Ptr.ToPointer());
			}
		}

		property bool IsCrow
		{
			bool get()
			{
				return pNativeObject->AmICrow();
			}
		}

		property bool IsVisible
		{
			bool get()
			{
				return pNativeObject->getVisible();
			}

			void set(bool value)
			{
				pNativeObject->setVisible(value);
			}
		}

		property bool IsEnabled
		{
			bool get()
			{
				return pNativeObject->getEnabled();
			}

			void set(bool value)
			{
				pNativeObject->setEnabled(value);
			}
		}

		property bool IsDestroyed
		{
			bool get()
			{
				return pNativeObject->getDestroy();
			}
		}

		virtual void shedule_update(UInt32 deltaTime);

	protected:
		Object();
		Object(IntPtr InNativeObject);

	private:

		CObject* pNativeObject;

	};
}