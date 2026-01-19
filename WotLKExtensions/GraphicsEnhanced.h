#pragma once
#include <cstdint>
#include "Macros.h"

namespace GraphicsEnhanced
{
    constexpr float FARCLIP_MAX = 3831.0f;
    constexpr float HORIZON_FARCLIP_SCALE_MAX = 12.0f;
    constexpr float ENVIRONMENT_DETAIL_MAX = 6.0f;
    constexpr float CAMERA_DISTANCE_ABSOLUTE = 500.0f;
    namespace Addresses
    {
        constexpr uintptr_t FarclipPush1 = 0x6840FF;
        constexpr uintptr_t FarclipPush2 = 0x705758;
    }

    void Initialize();

    void AllocateCustomFloatConstants();
    void PatchHorizonFarclipValidator();
    void PatchEnvironmentDetailValidator();
    void PatchViewDistance();
    void PatchCameraDistance();

    void PatchFloatConstant(uintptr_t address, float newValue);
    void PatchJumpAlways(uintptr_t address);
}
