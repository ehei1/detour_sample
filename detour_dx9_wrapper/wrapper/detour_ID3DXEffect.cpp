#include "stdafx.h"

#include "utility.h"

#include "detour_ID3DXEffect.h"


/*
find other signature in ID3DXEffect interface
*/
using set_int_t = HRESULT(STDMETHODCALLTYPE*)(ID3DXEffect*, D3DXHANDLE, INT);
using set_float_t = HRESULT(STDMETHODCALLTYPE*)(ID3DXEffect*, D3DXHANDLE, FLOAT);
using set_float_array_t = HRESULT(STDMETHODCALLTYPE*)(ID3DXEffect*, D3DXHANDLE, CONST FLOAT*, UINT);
using set_matrix_t = HRESULT(STDMETHODCALLTYPE*)(ID3DXEffect*, D3DXHANDLE, CONST D3DXMATRIX*);


/*
target function addresses. when it detoured, address is not null
*/
set_int_t target_set_int;
set_float_t target_set_float;
set_float_array_t target_set_float_array;
set_matrix_t target_set_matrix;


/*
hooked functions
*/
HRESULT STDMETHODCALLTYPE detour_set_int(ID3DXEffect* table, D3DXHANDLE handle, INT n);
HRESULT STDMETHODCALLTYPE detour_set_float(ID3DXEffect*, D3DXHANDLE, FLOAT);
HRESULT STDMETHODCALLTYPE detour_set_float_array(ID3DXEffect* table, D3DXHANDLE handle, CONST FLOAT*, UINT);
HRESULT STDMETHODCALLTYPE detour_set_matrix(ID3DXEffect*, D3DXHANDLE, CONST D3DXMATRIX*);


detour::functions detour_ID3DXEffect::detour(ID3DXEffect* table)
{
	void** virtual_table = *(reinterpret_cast<void***>(table));

	set_float_t set_int = reinterpret_cast<set_float_t>(virtual_table[27]);
	set_float_t set_float = reinterpret_cast<set_float_t>(virtual_table[31]);
	set_float_t set_float_array = reinterpret_cast<set_float_t>(virtual_table[33]);
	set_float_t set_matrix = reinterpret_cast<set_float_t>(virtual_table[39]);

	return {
		{set_int, detour_set_int, reinterpret_cast<void**>(&target_set_int)},
		{set_float, detour_set_float, reinterpret_cast<void**>(&target_set_float)},
		{set_float_array, detour_set_float_array, reinterpret_cast<void**>(&target_set_float_array)},
		{set_matrix, detour_set_matrix, reinterpret_cast<void**>(&target_set_matrix)},
	};
}

HRESULT STDMETHODCALLTYPE detour_set_int(ID3DXEffect* table, D3DXHANDLE handle, INT n)
{
	return put_message(target_set_int(table, handle, n), TEXT("ID3DXEffect::SetInt"), handle);
}

HRESULT STDMETHODCALLTYPE detour_set_float(ID3DXEffect* effect, D3DXHANDLE handle, FLOAT f)
{
	return put_message(target_set_float(effect, handle, f), TEXT("ID3DXEffect::SetFloat"), handle);
}

HRESULT STDMETHODCALLTYPE detour_set_float_array(ID3DXEffect* table, D3DXHANDLE handle, CONST FLOAT* f, UINT count)
{
	return put_message(target_set_float_array(table, handle, f, count), TEXT("ID3DXEffect::SetFloatArray"), handle);
}

HRESULT STDMETHODCALLTYPE detour_set_matrix(ID3DXEffect* table, D3DXHANDLE handle, CONST D3DXMATRIX* m)
{
	return put_message(target_set_matrix(table, handle, m), TEXT("ID3DXEffect::SetMatrix"), handle);
}