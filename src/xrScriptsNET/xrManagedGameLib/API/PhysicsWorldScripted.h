#pragma once
//#include "PhysicsGameScripted.h"

class cphysics_world_scripted;
namespace XRay
{
	
	public ref class PhysicsWorldScripted
	{

	internal:
		cphysics_world_scripted* pNativeLevel;

	public:

		~PhysicsWorldScripted();

		//PhysicsWorldScripted(IPHWorld* imp);
	
// 		PhysicsWorldScripted(IPHWorld* imp):PhysicsGameScripted<IPHWorld>(imp) {};
 		PhysicsWorldScripted(cphysics_world_scripted* imp) : pNativeLevel(imp) {};

		property float Gravity
		{
			float get();
			void  set(float Coef);
		};
//		void		AddCall(CPHCondition*c, CPHAction*a);

		::System::IntPtr GetNative() 
		{
			return ::System::IntPtr(pNativeLevel);
		}
	};

}


