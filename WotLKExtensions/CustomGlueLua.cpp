#include "CustomGlueLua.h"

// Map for storing custom Glue Lua functions
static std::unordered_map<char*, void*> glueLuaFuncts;

// GlueScript function definitions
namespace GlueScript
{
	// LoadFunctions is at 0x4DD580 - iterates through a function table and registers them
	CLIENT_FUNCTION(LoadFunctions, 0x4DD580, __cdecl, int, ())

	CLIENT_FUNCTION(RegisterFunction, 0x817F90, __cdecl, int, (char*, void*))

	CLIENT_FUNCTION(PushBoolean, 0x84E4D0, __cdecl, int, (lua_State* L, bool))
	CLIENT_FUNCTION(PushString, 0x84E350, __cdecl, int, (lua_State*, char const*))
	CLIENT_FUNCTION(ToLString, 0x84E0E0, __cdecl, char*, (lua_State*, int, bool))
}

void CustomGlueLua::Apply()
{
	RegisterFunctions();

	// Hook the CALL instruction at 0x4DA726 that calls GlueScript::LoadFunctions (0x4DD580)
	uint32_t callSiteAddress = 0x4DA726;
	uint32_t newOffset = (uint32_t)&LoadScriptFunctionsWrapper - (callSiteAddress + 5);

	DWORD oldProtect;
	VirtualProtect((void*)callSiteAddress, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
	*(uint32_t*)(callSiteAddress + 1) = newOffset;
	VirtualProtect((void*)callSiteAddress, 5, oldProtect, &oldProtect);
}

void __cdecl CustomGlueLua::LoadScriptFunctionsWrapper()
{
	// Call the original function to register all built-in GlueScript functions
	GlueScript::LoadFunctions();

	// Register our custom functions
	for (auto it = glueLuaFuncts.begin(); it != glueLuaFuncts.end(); it++)
	{
		GlueScript::RegisterFunction(it->first, it->second);
	}
}

int CustomGlueLua::TestGlueFunction(lua_State* L)
{
	HWND activeWindow = *(HWND*)0x00D41620;

	if (activeWindow)
	{
		FLASHWINFO flashInfo;
		flashInfo.cbSize = sizeof(flashInfo);
		flashInfo.hwnd = activeWindow;
		flashInfo.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
		flashInfo.uCount = 3;
		flashInfo.dwTimeout = 200;

		FlashWindowEx(&flashInfo);

		GlueScript::PushBoolean(L, true);
		return 1;
	}

	GlueScript::PushBoolean(L, false);
	return 1;
}

int CustomGlueLua::ShowTestMessage(lua_State* L)
{
	const char* message = "CustomGlueLua is working!\n\nThe hook on GlueScript works correctly!";

	// Try to get string parameter if provided
	char* paramStr = GlueScript::ToLString(L, 1, false);
	if (paramStr != nullptr && paramStr[0] != '\0')
	{
		message = paramStr;
	}

	MessageBoxA(NULL, message, "GlueXML Custom Lua Test", MB_OK | MB_ICONINFORMATION);

	return 0;
}

void CustomGlueLua::AddToFunctionMap(char* name, void* ptr)
{
	glueLuaFuncts.insert(std::make_pair(name, ptr));
}

void CustomGlueLua::RegisterFunctions()
{
	// Register test functions
	AddToFunctionMap("TestGlueFunction", &TestGlueFunction);
	AddToFunctionMap("ShowTestMessage", &ShowTestMessage);
}
