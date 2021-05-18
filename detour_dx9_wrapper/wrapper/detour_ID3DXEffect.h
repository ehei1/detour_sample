#pragma once

#include "detour.h"


class detour_ID3DXEffect
{
public:
	static detour::functions detour(struct ID3DXEffect*);
};

