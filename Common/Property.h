#pragma once

#ifndef __COMMON_PROPERTY
#define __COMMON_PROPERTY

/// Namespace that contains common classes for generic property handling
namespace CommonProperty
{
	/// Class that holds the value of a generic property, this class expects a fundamental type as template type, but any type that can be copied is valid
	template<class T>
	class Property
	{
	public:
		Property(const T& value) : mValue(value) {}
		T GetPropertyValue() { return mValue; }
	protected:
		T mValue;
	};
}

#endif