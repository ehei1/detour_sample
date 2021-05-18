#pragma once

#include "detour.h"


class detour_ID3DXContantTable
{
public:
	static detour::functions detour(struct ID3DXConstantTable*);
};
