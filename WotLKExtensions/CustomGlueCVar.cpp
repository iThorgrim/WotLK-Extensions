#include "CustomGlueCVar.h"
#include <vector>

// Storage for custom CVars to be registered
static std::vector<CustomCVarEntry> g_customCVars;

// CVar registration function at 0x767FC0
// Correct signature based on disassembly analysis:
// void* CVar_Register(name, description, size, defaultValue, callback, flags, unk7, unk8, unk9)
namespace CVarInternal
{
	CLIENT_FUNCTION(Register, 0x767FC0, __cdecl, void*, (
		const char* name,
		const char* description,
		uint32_t size,            // Max string size (0 for unlimited, 0x40 for 64 chars, etc.)
		const char* defaultValue,
		void* callback,
		uint32_t flags,
		char unk7,                // Usually '\0'
		uint32_t unk8,            // Usually 0
		char unk9                 // Usually '\0'
	))

	// Original function that registers all Glue CVars (accountName, accountList, movie, etc.)
	// Located at 0x401B60, called from 0x404146
	CLIENT_FUNCTION(RegisterGlueCVars, 0x401B60, __cdecl, void, ())
}

// Our wrapper that calls original + registers custom CVars
static void __cdecl RegisterGlueCVarsWrapper()
{
	// Call the original function to register all built-in Glue CVars
	CVarInternal::RegisterGlueCVars();

	// Register our custom CVars
	CustomGlueCVar::RegisterCustomCVars();
}

void CustomGlueCVar::Apply()
{
	AddCVar({
		"characterOrder",
		"Order of characters on character select screen",
		"1,2,3,4,5,6,7,8,9,10",
		nullptr,
		CVAR_FLAG_GLUE | CVAR_FLAG_ARCHIVE
	});

	// Hook the CALL instruction at 0x404146 that calls RegisterGlueCVars (0x401B60)
	// This is exactly like CustomGlueLua hooks LoadFunctions
	uint32_t callSiteAddress = 0x404146;
	uint32_t newOffset = (uint32_t)&RegisterGlueCVarsWrapper - (callSiteAddress + 5);

	DWORD oldProtect;
	VirtualProtect((void*)callSiteAddress, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
	*(uint32_t*)(callSiteAddress + 1) = newOffset;
	VirtualProtect((void*)callSiteAddress, 5, oldProtect, &oldProtect);
}

void CustomGlueCVar::RegisterCustomCVars()
{
	// Register user-added custom CVars
	for (const auto& entry : g_customCVars)
	{
		CVarInternal::Register(
			entry.name,
			entry.description,
			0,            // size: 0 for unlimited
			entry.defaultValue,
			entry.callback,
			entry.flags,
			'\0',         // unk7
			0,            // unk8
			'\0'          // unk9
		);
	}
}

void* CustomGlueCVar::RegisterGlueCVar(
	const char* name,
	const char* description,
	const char* defaultValue,
	void* callback,
	uint32_t flags)
{
	return CVarInternal::Register(
		name,
		description,
		0,            // size: 0 for unlimited
		defaultValue,
		callback,
		flags,
		'\0',         // unk7
		0,            // unk8
		'\0'          // unk9
	);
}

void CustomGlueCVar::AddCVar(const CustomCVarEntry& entry)
{
	g_customCVars.push_back(entry);
}

void* CustomGlueCVar::GetCVar(const char* name)
{
	return CVar::LookupCvar(name);
}
