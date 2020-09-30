#pragma once
#include "pch.h"

#define EXEPTION(what) GxExeption(what, E_FAIL, __FILE__, __LINE__)
#define EXEPTION_HR(what, hr) GxExeption(what, hr, __FILE__, __LINE__)

struct GxExeption {
	/// <summary>
	/// Exeption
	/// </summary>
	/// <param name="sWhat">What was tried to archive</param>
	/// <param name="hErr">What was the error code</param>
	GxExeption(std::wstring sWhat, HRESULT hErr, std::string iFile, int iLine) :
		what(sWhat),
		hr(hErr),
		file(iFile),
		line(iLine)
	{}

	/// <summary>
	/// String of what failed
	/// </summary>
	std::wstring what;
	
	/// <summary>
	/// Inner hresult of the exeption
	/// </summary>
	HRESULT hr;

	/// <summary>
	/// File in witch the error occured
	/// </summary>
	std::string file;

	/// <summary>
	/// Line in witch the error occured
	/// </summary>
	int line;
};