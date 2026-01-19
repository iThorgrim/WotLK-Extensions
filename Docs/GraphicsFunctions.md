# Graphics Enhanced Functions


## Overview
The Graphics Enhanced system patches several internal client limitations:
- **View Distance (Farclip)**: Extended from 1277 to 3831
- **Camera Distance**: Extended from 50 to 500
- **Environment Detail**: Extended from 0.5-1.5 range to 0.5-6.0
- **Fog**: Patched to reduce the "blue fog wall" effect

---

## Patched Limits

|---------|---------------|-----------|
| Farclip (View Distance) | 1277.0 | 3831.0 |
| Camera Distance Max | 50.0 | 500.0 |
| Horizon Farclip Scale | 3.0 - 6.0 | 1.0 - 12.0 |

---

## Lua Functions

All graphics Lua functions are registered when `graphicsEnhancedFunctions` is enabled.

### SetExtendedFarclip

Sets the farclip (view distance) beyond the normal 1277 limit.

**Syntax:**
```lua
success = SetExtendedFarclip(value)
```

**Arguments:**
- `value` (float): Range 100-3831 (default client max was 1277)

**Returns:**
- `success` (boolean): true if value was set successfully

**Example:**
```lua
SetExtendedFarclip(1500)  -- Set view distance to 1500
SetExtendedFarclip(3831)  -- Maximum view distance
```

---

### SetExtendedCameraDistance

Sets the camera distance beyond the normal 50 limit.

**Syntax:**
```lua
success = SetExtendedCameraDistance(value)
```

**Arguments:**
- `value` (float): Range 1-500 (default client max was 50)

**Returns:**
- `success` (boolean): true if value was set successfully

**Example:**
```lua
SetExtendedCameraDistance(100)  -- Double the normal max distance
SetExtendedCameraDistance(500)  -- Maximum camera distance
```

---

### SetHorizonScale

Sets the horizon far clip scale (controls how far terrain is visible).

**Syntax:**
```lua
success = SetHorizonScale(value)
```

**Arguments:**
- `value` (float): Range 1.0-12.0 (default client limit was 3-6)

**Returns:**
- `success` (boolean): true if value was set successfully

**Example:**
```lua
SetHorizonScale(12)  -- Maximum horizon distance
SetHorizonScale(6)   -- Default maximum
```

**Note:** Higher values allow you to see terrain further in the distance.

---

### SetEnvironmentDetail

Sets environment detail level (controls doodad/model density and detail).

**Syntax:**
```lua
success = SetEnvironmentDetail(value)
```

**Arguments:**
- `value` (float): Range 0.5-6.0 (default client limit was 0.5-1.5)

**Returns:**
- `success` (boolean): true if value was set successfully

**Example:**
```lua
SetEnvironmentDetail(6.0)  -- Maximum environment detail
SetEnvironmentDetail(1.5)  -- Default maximum
```

---

### SetTextureQuality

Controls multiple texture quality settings at once.

**Syntax:**
```lua
success = SetTextureQuality(baseMip, lodBias, terrainAlpha, cacheSize)
```

**Arguments:**
- `baseMip` (integer): 0-3 (0=best quality, 3=worst)
- `lodBias` (float): -1.0 to 1.0 (0=normal, negative=sharper textures)
- `terrainAlpha` (integer): 1-8 (8=best terrain blending)
- `cacheSize` (integer): 0-512 MB (0=auto)

**Returns:**
- `success` (boolean): true if values were set successfully

**Example:**
```lua
SetTextureQuality(0, 0.0, 8, 256)   -- High quality settings
SetTextureQuality(0, -0.5, 8, 512)  -- Maximum quality with sharper textures
```

**Note:** All parameters are optional. Only provided values will be changed.

---

### SetRenderFlags

Controls various rendering flags for visual quality.

**Syntax:**
```lua
success = SetRenderFlags(specular, mapShadows, footprints, objectFade, lod)
```

**Arguments:**
- `specular` (boolean): Enable specular highlights
- `mapShadows` (boolean): Enable map shadows
- `footprints` (boolean): Show footprints
- `objectFade` (boolean): Fade objects at distance
- `lod` (integer): 0-1 (0=best quality)

**Returns:**
- `success` (boolean): true if values were set successfully

**Example:**
```lua
SetRenderFlags(true, true, true, false, 0)  -- High quality, no object fading
SetRenderFlags(true, true, true, true, 1)   -- Balanced settings
```

---

### SetFogDistance

Controls the fog/haze distance to reduce the "blue fog wall" effect.

**Syntax:**
```lua
success = SetFogDistance(fogNear, fogFar)
```

**Arguments:**
- `fogNear` (float): Distance where fog starts
- `fogFar` (float): Distance where fog is fully opaque

**Returns:**
- `success` (boolean): true if values were set successfully

**Example:**
```lua
SetFogDistance(2000, 5000)      -- Push fog way back
SetFogDistance(-10000, 10000)   -- Basically disable fog
```

**Note:** These are runtime values and may be overwritten by zone changes. You may need to reapply them when changing zones.

---

## Ingame Tested Settings (not totally stable, not recommended in Northrend)

### Console / Macro
```lua
/console farclip 2500
/run SetFogDistance(-10000, 10000)
/run SetExtendedFarclip(3831)
/run SetHorizonScale(12)
/run SetRenderFlags(true, true, true, false, 1)
/run SetTextureQuality(0, -1.0, 8, 512)
/run SetEnvironmentDetail(6.0)

/run SetExtendedCameraDistance(500)
/console characterAmbient 0.0
/console overridefarclip 1
/console horizonfarclip 6226
/console skycloudlod 3
```

### Config.wtf
```txt
SET gxWindow "1"
SET hwDetect "0"
SET gxRefresh "100"
SET gxMultisampleQuality "0.000000"
SET gxFixLag "0"
SET videoOptionsVersion "3"
SET textureFilteringMode "5"
SET readTOS "1"
SET readEULA "1"
SET readTerminationWithoutNotice "1"
SET showToolsUI "1"
SET componentTextureLevel "9"
SET shadowLevel "0"
SET farclip "3831.000000"
SET specular "1"
SET groundEffectDensity "64"
SET groundEffectDist "140"
SET environmentDetail "6.000000"
SET projectedTextures "1"
SET weatherDensity "3"
SET gxResolution "1920x1080"
SET gxTripleBuffer "1"
SET gxMultisample "8"
SET lod "1"
SET texLodBias "-1.000000"
SET horizonFarclipScale "12.000000"
SET objectFade "1"
SET gxTextureCacheSize "512"
SET SkyCloudLOD "3"
SET extShadowQuality "5"
```

Run the game as Administrator. For reasons that are still unknown, this reduces crashes.