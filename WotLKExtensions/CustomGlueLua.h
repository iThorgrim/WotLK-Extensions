#pragma once

#include "SharedDefines.h"

class Main;

class CustomGlueLua
{
public:
	static void LoadScriptFunctionsWrapper();

private:
	static void AddToFunctionMap(char* name, void* ptr);
	static void Apply();
	static void RegisterFunctions();

	// Test functions
	static int TestGlueFunction(lua_State* L);
	static int ShowTestMessage(lua_State* L);

	friend class Main;
};
