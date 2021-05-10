#include "stdafx.h"

#include <cassert>
#include <iomanip>
#include <sstream>
#include <string>

#include "dxerr.h"
#include "detour_ID3DXConstantTable.h"


SetInt target_set_int;
SetFloat target_set_float;
SetFloatArray target_set_float_array;
SetMatrix target_set_matrix;


HRESULT put_message(HRESULT hr, LPCTSTR function, LPCSTR handle)
{
	if (FAILED(hr)) {
#ifdef _UNICODE
		std::wstring handle_name(handle, handle + strlen(handle));
		std::wstringstream ss;
#else
		std::string handle_name = handle;
		std::stringstream ss;
#endif
		WCHAR buffer[MAX_PATH]{};
		DXGetErrorDescription(hr, buffer, _countof(buffer));

		ss << function << TEXT("(") << std::quoted(handle_name) << TEXT(") ") << DXGetErrorString(hr) << TEXT(": ") << buffer << std::endl;

		OutputDebugString(ss.str().c_str());
	}

	return hr;
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