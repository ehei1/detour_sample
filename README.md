# project
## detour_dx9
* minimal sample for hooking DirectX 9 with [Microsoft Detours](https://github.com/microsoft/Detours)
* IDirect3DDevice9::EndScene() is hooked by Hooked_EndScene()

## detour_dx9_wrapper
* easy detouring with wrapper
* see detour.h and detour_ID3DXConstantTable.h in wrapper folder
* it will warn to output window with OutputDebugString() about unused variable in hlsl
> ID3DXConstantTable::SetFloat("unused") D3DERR_INVALIDCALL: Invalid call

# build
* it needs to install [DirectX9 SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
* run Visual Studio with administrator mode to build Detours
