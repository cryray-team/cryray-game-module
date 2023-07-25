#pragma once
class CClientSpawnManager;

namespace XRay
{
	public ref class ClientSpawnManager
	{
	internal: 
		CClientSpawnManager* pNativeObject;
	public:
		delegate void Functor();

	public:
		ClientSpawnManager();
		ClientSpawnManager(CClientSpawnManager* pObj) : pNativeObject(pObj) {}

		using CALLBACK_TYPE = xrDelegate<void(CObject*)>;

		/*
		ref struct CSpawnCallback
		{
			CALLBACK_TYPE			m_object_callback;
			CScriptCallbackEx<void>	m_callback;
		};
		*/
	

		void Add(::System::UInt16 RequestingId, ::System::UInt16 RequestedId, Functor^ Funct);
		void Del(::System::UInt16 RequestingId, ::System::UInt16 RequestedId);
		void Clear();

		//void MergeWpawnCallbacks(CSpawnCallback &new_callback, CSpawnCallback &old_callback);
	};
}

