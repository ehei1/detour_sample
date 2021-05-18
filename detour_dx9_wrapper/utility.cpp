#include "stdafx.h"

#include <iomanip>
#include <sstream>
#include <string>

#include "dxerr.h"


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