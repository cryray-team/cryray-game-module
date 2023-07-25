#include "stdafx.h"

// Ыўсыў C++17 
namespace XRay::System
{	
	template<class T>
	public ref class EasyDelegate
	{
	private:
		T* pFunctor;

	public:
		EasyDelegate(T* pFunction) : pFunctor(pFunction) {};
		void operator()()
		{
			pFunctor();
		}
	};
}