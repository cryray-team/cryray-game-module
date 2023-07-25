#pragma once
#include "../xrPhysics/iphysics_scripted.h"

namespace XRay
{
	template<class T>
	public ref class PhysicsGameScripted
	{
	internal:

		cphysics_game_scripted<T>* pNativeLevel;

	private:
			T	&impl;
	public:

		PhysicsGameScripted(T* im) : impl(*im) {};
		~PhysicsGameScripted() {};

		typedef T type_impl;
	};

	
}
