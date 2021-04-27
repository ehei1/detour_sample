# detour_sample
hooking DirectX 9 with [Microsoft Detours](https://github.com/microsoft/Detours)

## build
* run Visual Studio with administrator mode to build Detours

## detoure_dx9
* it needs to install [DirectX9 SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
* IDirect3DDevice9::EndScene() is hooked by Hooked_EndScene()