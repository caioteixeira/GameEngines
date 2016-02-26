#pragma once
#include <memory>
// Some C++11 template magic to help implement delegates

// The base timer delegate struct is used simply
// to wrap a function call that takes no parameters
// and returns void
template <typename ...Params>
struct DelegateBase
{
	virtual ~DelegateBase() {}

	// Function call operator
	virtual void Execute(Params...) { }
};

// This templated class is where (some of) the magic happens.
// We store a pointer to the templated type as well
// as a pointer to the function we want to call
// (Don't worry if this code doesn't make sense...
// basically, it's just a fancy function object)
template <typename T, typename ...Params>
struct Delegate : public DelegateBase<Params...>
{
	// This typedefs "FuncPtr" as a pointer to a
	// member function of class T
	typedef void (T::*FuncPtr)(Params...);

	// Construct a timer delegate given an object
	// and a pointer to a function
	Delegate(T* obj, FuncPtr func)
		:mObj(obj)
		,mFunc(func)
	{ }

	// Call the function bound to this
	virtual void Execute(Params... p) override
	{
		(mObj->*mFunc)(p...);
	}
private:
	T* mObj;
	FuncPtr mFunc;
};

template <typename T>
using DelegateNoParams = Delegate<T>;

using DelegateNoParamsPtr = std::shared_ptr<DelegateBase<>>;
