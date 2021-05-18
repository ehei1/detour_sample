#include "stdafx.h"

#include <cassert>

#include "utility.h"

#include "detour_ID3DXConstantTable.h"


/*
find other signature in ID3DXConstantTable interface
*/
using set_int_t = HRESULT(STDMETHODCALLTYPE*)(ID3DXConstantTable*, IDirect3DDevice9*, D3DXHANDLE, INT);
using set_float_t = HRESULT(STDMETHODCALLTYPE*)(ID3DXConstantTable*, IDirect3DDevice9*, D3DXHANDLE, FLOAT);
using set_float_array_t = HRESULT(STDMETHODCALLTYPE*)(ID3DXConstantTable*, IDirect3DDevice9*, D3DXHANDLE, CONST FLOAT*, UINT);
using set_matrix_t = HRESULT(STDMETHODCALLTYPE*)(ID3DXConstantTable*, IDirect3DDevice9*, D3DXHANDLE, CONST D3DXMATRIX*);


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
HRESULT STDMETHODCALLTYPE detour_set_int(ID3DXConstantTable* table, IDirect3DDevice9* device, D3DXHANDLE handle, INT n);
HRESULT STDMETHODCALLTYPE detour_set_float(ID3DXConstantTable* table, IDirect3DDevice9* device, D3DXHANDLE handle, FLOAT f);
HRESULT STDMETHODCALLTYPE detour_set_float_array(ID3DXConstantTable* table, IDirect3DDevice9* device, D3DXHANDLE handle, CONST FLOAT*, UINT);
HRESULT STDMETHODCALLTYPE detour_set_matrix(ID3DXConstantTable*, IDirect3DDevice9*, D3DXHANDLE, CONST D3DXMATRIX*);


detour::functions detour_ID3DXContantTable::detour(ID3DXConstantTable* table)
{
#ifdef CINTERFACE
	ID3DXConstantTableVtbl* virtual_table = table->lpVtbl;

	set_int_t set_int = virtual_table->SetInt;
	set_float_t set_float = virtual_table->SetFloat;
	set_float_array_t set_float_array = virtual_table->SetFloatArray;
	set_matrix_t set_matrix = virtual_table->SetMatrix;
#else
	void** virtual_table = *(reinterpret_cast<void***>(table));
	
	// order of the ID3DXConstantTable
	set_int_t set_int = reinterpret_cast<set_int_t>(virtual_table[15]);
	set_float_t set_float = reinterpret_cast<set_float_t>(virtual_table[17]);
	set_float_array_t set_float_array = reinterpret_cast<set_float_array_t>(virtual_table[18]);
	set_matrix_t set_matrix = reinterpret_cast<set_matrix_t>(virtual_table[21]);
#endif

	return {
		{set_int, detour_set_int, reinterpret_cast<void**>(&target_set_int)},
		{set_float, detour_set_float, reinterpret_cast<void**>(&target_set_float)},
		{set_float_array, detour_set_float_array, reinterpret_cast<void**>(&target_set_float_array)},
		{set_matrix, detour_set_matrix, reinterpret_cast<void**>(&target_set_matrix)},
	};
}


HRESULT STDMETHODCALLTYPE detour_set_int(ID3DXConstantTable* table, IDirect3DDevice9* device, D3DXHANDLE handle, INT n)
{
	return put_message(target_set_int(table, device, handle, n), TEXT("ID3DXConstantTable::SetInt"), handle);
}

HRESULT STDMETHODCALLTYPE detour_set_float(ID3DXConstantTable* table, IDirect3DDevice9* device, D3DXHANDLE handle, FLOAT f)
{
	return put_message(target_set_float(table, device, handle, f), TEXT("ID3DXConstantTable::SetFloat"), handle);
}

HRESULT STDMETHODCALLTYPE detour_set_float_array(ID3DXConstantTable* table, IDirect3DDevice9* device, D3DXHANDLE handle, CONST FLOAT* f, UINT count)
{
	return put_message(target_set_float_array(table, device, handle, f, count), TEXT("ID3DXConstantTable::SetFloatArray"), handle);
}

HRESULT STDMETHODCALLTYPE detour_set_matrix(ID3DXConstantTable* table, IDirect3DDevice9* device, D3DXHANDLE handle, CONST D3DXMATRIX* m)
{
	return put_message(target_set_matrix(table, device, handle, m), TEXT("ID3DXConstantTable::SetMatrix"), handle);
}