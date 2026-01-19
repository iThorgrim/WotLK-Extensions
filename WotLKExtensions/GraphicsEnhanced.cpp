#include "GraphicsEnhanced.h"
#include "Logger.h"
#include "Util.h"
#include <Windows.h>
#include <cstring>
#include <cstdio>

namespace GraphicsEnhanced
{
    static float* g_CustomHorizonMin = nullptr;
    static float* g_CustomHorizonMax = nullptr;
    static float* g_CustomEnvDetailMin = nullptr;
    static float* g_CustomEnvDetailMax = nullptr;

    namespace FloatConstants
    {
        constexpr uintptr_t Farclip_Max_NewWorld = 0xA3E710;
        constexpr uintptr_t Farclip_Max_OldWorld = 0xA3E70C;
        constexpr uintptr_t Farclip_Min = 0xA3E708;

        constexpr uintptr_t HorizonScale_Min = 0x9EBBC4;
        constexpr uintptr_t HorizonScale_Max = 0x9E8CF8;
        constexpr uintptr_t CameraDistanceMax = 0xA1E2FC;
        constexpr uintptr_t EnvDetail_Max = 0xA41B18;
    }

    static bool WriteProtectedMemory(uintptr_t address, const void* data, size_t size)
    {
        DWORD oldProtect;
        if (!VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &oldProtect))
        {
            return false;
        }

        memcpy((void*)address, data, size);

        VirtualProtect((LPVOID)address, size, oldProtect, &oldProtect);
        return true;
    }

    void PatchFloatConstant(uintptr_t address, float newValue)
    {
        WriteProtectedMemory(address, &newValue, sizeof(float));
    }

    void PatchJumpAlways(uintptr_t address)
    {
        uint8_t jmpAlways = 0xEB;
        WriteProtectedMemory(address, &jmpAlways, 1);
    }

    void AllocateCustomFloatConstants()
    {
        g_CustomHorizonMin = (float*)VirtualAlloc(nullptr, sizeof(float) * 4, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (g_CustomHorizonMin)
        {
            g_CustomHorizonMax = g_CustomHorizonMin + 1;
            g_CustomEnvDetailMin = g_CustomHorizonMin + 2;
            g_CustomEnvDetailMax = g_CustomHorizonMin + 3;

            *g_CustomHorizonMin = 1.0f;
            *g_CustomHorizonMax = HORIZON_FARCLIP_SCALE_MAX;
            *g_CustomEnvDetailMin = 0.5f;
            *g_CustomEnvDetailMax = ENVIRONMENT_DETAIL_MAX;
        }
    }

    void PatchHorizonFarclipValidator()
    {
        if (!g_CustomHorizonMin) return;

        uintptr_t patchAddr1 = 0x78D7CC + 2;
        WriteProtectedMemory(patchAddr1, &g_CustomHorizonMin, 4);

        uintptr_t patchAddr2 = 0x78D7DD + 2;
        WriteProtectedMemory(patchAddr2, &g_CustomHorizonMax, 4);
    }

    void PatchEnvironmentDetailValidator()
    {
        if (!g_CustomEnvDetailMin) return;

        uintptr_t patchAddr1 = 0x78DC6C + 2;
        WriteProtectedMemory(patchAddr1, &g_CustomEnvDetailMin, 4);

        uintptr_t patchAddr2 = 0x78DC7D + 2;
        WriteProtectedMemory(patchAddr2, &g_CustomEnvDetailMax, 4);
    }

    void PatchViewDistance()
    {
        PatchFloatConstant(FloatConstants::Farclip_Max_NewWorld, FARCLIP_MAX);
        PatchFloatConstant(FloatConstants::Farclip_Max_OldWorld, FARCLIP_MAX);
        PatchFloatConstant(FloatConstants::Farclip_Min, 100.0f);

        uint32_t newFarclipMax = (uint32_t)FARCLIP_MAX;
        uintptr_t pushAddr1 = Addresses::FarclipPush1;

        if (*(uint8_t*)pushAddr1 == 0x68)
            WriteProtectedMemory(pushAddr1 + 1, &newFarclipMax, 4);

        uintptr_t pushAddr2 = Addresses::FarclipPush2;
        if (*(uint8_t*)pushAddr2 == 0x68)
            WriteProtectedMemory(pushAddr2 + 1, &newFarclipMax, 4);
    }

    void PatchCameraDistance()
    {
        PatchFloatConstant(FloatConstants::CameraDistanceMax, CAMERA_DISTANCE_ABSOLUTE);
    }

    void Initialize()
    {
        AllocateCustomFloatConstants();
        PatchViewDistance();
        PatchHorizonFarclipValidator();
        PatchEnvironmentDetailValidator();
        PatchCameraDistance();
    }
}
