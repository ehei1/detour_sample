#pragma once


/*
find other signature in ID3DXConstantTable interface
*/
using SetInt = HRESULT(STDMETHODCALLTYPE*)(ID3DXConstantTable*, IDirect3DDevice9*, D3DXHANDLE, INT);
using SetFloat = HRESULT(STDMETHODCALLTYPE*)(ID3DXConstantTable*, IDirect3DDevice9*, D3DXHANDLE, FLOAT);
using SetFloatArray = HRESULT(STDMETHODCALLTYPE*)(ID3DXConstantTable*, IDirect3DDevice9*, D3DXHANDLE, CONST FLOAT*, UINT);
using SetMatrix = HRESULT(STDMETHODCALLTYPE*)(ID3DXConstantTable*, IDirect3DDevice9*, D3DXHANDLE, CONST D3DXMATRIX*);


/*
target function addresses. when it detoured, address is not null
*/
extern SetInt target_set_int;
extern SetFloat target_set_float;
extern SetFloatArray target_set_float_array;
extern SetMatrix target_set_matrix;

/*
hooked functions
*/
HRESULT STDMETHODCALLTYPE detour_set_int(ID3DXConstantTable* table, IDirect3DDevice9* device, D3DXHANDLE handle, INT n);
HRESULT STDMETHODCALLTYPE detour_set_float(ID3DXConstantTable* table, IDirect3DDevice9* device, D3DXHANDLE handle, FLOAT f);
HRESULT STDMETHODCALLTYPE detour_set_float_array(ID3DXConstantTable* table, IDirect3DDevice9* device, D3DXHANDLE handle, CONST FLOAT*, UINT);
HRESULT STDMETHODCALLTYPE detour_set_matrix(ID3DXConstantTable*, IDirect3DDevice9*, D3DXHANDLE, CONST D3DXMATRIX*);