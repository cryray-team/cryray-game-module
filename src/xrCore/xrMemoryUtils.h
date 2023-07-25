// Giperion June 2019
// [EUREKA] 3.10.2
// X-Ray Oxygen, Oxygen Team

//////////////////////////////////////////////////////////////
// Desc		: Smart pointers and other useful classes for managing memory
// Author	: Giperion
//////////////////////////////////////////////////////////////
// Oxygen Engine 2016-2019
//////////////////////////////////////////////////////////////
#pragma once

/// Helper class to allocate temporary memory in scope and release after scope ends
struct xrScopeMemPtr 
{
	xrScopeMemPtr();
	explicit xrScopeMemPtr(void* InMem);
	explicit xrScopeMemPtr(const size_t memSize);

	~xrScopeMemPtr();

	xrScopeMemPtr& operator = (void* InMem);

	operator void* () const;
	void reset(void* newMem);
private:
	void* memPtr;
};

// --- HEADER
/// Helper class to allocate one or several objects in heap and destroy them at the end of scope
template<class Type>
struct xrScopePtr
{
	xrScopePtr();
	ICF explicit xrScopePtr(Type* pElem);

	ICF ~xrScopePtr();

	ICF void reset(Type* newElem);
	ICF xrScopePtr<Type>& operator=(Type* pElem);

	ICF Type& operator*() const;
	ICF Type* operator->() const;
	ICF Type& operator[](size_t i) const;

	/// automatic access to internal memory. I hope no one will try to free memory by pointer
	ICF operator Type* () const;

	ICF Type* get() const;

private:
	Type* memory;
};

// --- END
// --- IMPLEMENTATION

template<class Type>
xrScopePtr<Type>& xrScopePtr<Type>::operator=(Type* pElem)
{
	delete memory; memory = nullptr;
	memory = pElem;
	return *this;
}

template<class Type>
xrScopePtr<Type>::operator Type* () const
{
	return memory;
}

template<class Type>
Type* xrScopePtr<Type>::get() const
{
	return memory;
}

template<class Type>
Type* xrScopePtr<Type>::operator->() const
{
	R_ASSERT2(memory != nullptr, "Attempt to access a NULL scoped pointer");
	return memory;
}

template<class Type>
Type& xrScopePtr<Type>::operator*() const
{
	R_ASSERT2(memory != nullptr, "Attempt to access a NULL scoped pointer");
	return *memory;
}

template<class Type>
Type& xrScopePtr<Type>::operator[](size_t i) const
{
	R_ASSERT2(memory != nullptr, "Attempt to access a NULL scoped pointer");
	return memory[i];
}

template<class Type>
void xrScopePtr<Type>::reset(Type* newElem)
{
	delete memory; memory = nullptr;
	memory = newElem;
}

template<class Type>
xrScopePtr<Type>::~xrScopePtr()
{
	delete memory;
	memory = nullptr;
}

template<class Type>
xrScopePtr<Type>::xrScopePtr(Type* pElem)
	: memory(pElem)
{}

template<class Type>
xrScopePtr<Type>::xrScopePtr()
	: memory(nullptr)
{}