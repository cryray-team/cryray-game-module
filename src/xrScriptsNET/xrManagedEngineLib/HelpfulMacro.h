#pragma once

// Cast IntPtr to destination native type, with several checks
// Works only with DLL_Pure objects
#define CAST_TO_NATIVE_OBJECT(Type, IntPtr) \
DLL_Pure* NativeObject = reinterpret_cast<DLL_Pure*> (IntPtr.ToPointer()); \
R_ASSERT2(NativeObject, "Can't construct proxy of " #Type " : Native object is null!"); \
pNativeObject = dynamic_cast<Type*>(NativeObject); \
R_ASSERT2(pNativeObject, "Can't construct proxy of " #Type " : Object is not a target type!")

// start declaring a new event. VA_ARGS - parameters WITH types
#define BEGIN_DEFINE_EVENT(Name, ...) \
	delegate void Name##Func(__VA_ARGS__); \
	Name##Func^ p##Name; \
	\
	event Name##Func^ Name \
	{ \
		void add(Name##Func^ Func) \
		{ \
			p##Name = static_cast<Name##Func^>(::System::Delegate::Combine(p##Name, Func)); \
		} \
\
		void remove(Name##Func^ Func) \
		{ \
			p##Name = static_cast<Name##Func^>(::System::Delegate::Remove(p##Name, Func)); \
		} \
\
		void raise(__VA_ARGS__) \
		{ \
			if (p##Name != nullptr) \
			{

// end declaring a new event. VA_ARGS - parameters WITHOUT types. Parameter names should match with names in BEGIN_DEFINE_EVENT
#define END_DEFINE_EVENT(Name, ...) \
					p##Name->Invoke(__VA_ARGS__); \
			} \
		} \
	}

#define FIRE_EVENT(Name, ...) \
	Name(__VA_ARGS__)