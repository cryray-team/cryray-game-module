#pragma once

using namespace System;

namespace XRay
{
	[AttributeUsage(AttributeTargets::Class, AllowMultiple = true)]
	public ref class ClassRegistratorDecorator : Attribute
	{
	public:
		ClassRegistratorDecorator();
		static void RegisterTypes();
	};
}