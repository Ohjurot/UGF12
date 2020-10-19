#pragma once
#include "pch.h"

#if defined(SHIPPING)
	#define TSysInfo(type) TSysInfo_REL<type>
#else
	#define TSysInfo(type) TSysInfo_DBG<type>
#endif

/// <summary>
/// Struck for storing system info
/// </summary>
/// <typeparam name="T">Type of info to store</typeparam>
template<typename T>
struct TSysInfo_DBG {
	/// <summary>
	/// Data element of type T
	/// </summary>
	T data;

	/// <summary>
	/// String representation of name
	/// </summary>
	std::string strName;

	/// <summary>
	/// String representation of value
	/// </summary>
	std::string strValue;

	/// <summary>
	/// Constructor for base type
	/// </summary>
	/// <param name="input"></param>
	TSysInfo_DBG(T input) {
		data = input;
	}

	/// <summary>
	/// Equal operator for type
	/// </summary>
	/// <param name="input">Input type</param>
	void operator=(T input) {
		data = input;
	}

	/// <summary>
	/// Compare operator
	/// </summary>
	/// <param name="compare"></param>
	/// <returns></returns>
	bool operator==(T compare) {
		return (data == compare);
	}
};

/// <summary>
/// Struck for storing system info
/// </summary>
/// <typeparam name="T">Type of info to store</typeparam>
template<typename T>
struct TSysInfo_REL {
	/// <summary>
	/// Data element of type T
	/// </summary>
	T data;

	/// <summary>
	/// Constructor for base type
	/// </summary>
	/// <param name="input"></param>
	TSysInfo_REL(T input) {
		data = input;
	}

	/// <summary>
	/// Equal operator for type
	/// </summary>
	/// <param name="input">Input type</param>
	void operator=(T input) {
		data = input;
	}

	/// <summary>
	/// Compare operator
	/// </summary>
	/// <param name="compare"></param>
	/// <returns></returns>
	bool operator==(T compare) {
		return (data == compare);
	}
};

