#pragma once

#include <vector>
#include <unordered_map>


class detour
{
	using detoured_ptr = void*;
	using source_ptr = void*;
	using target_ptr = void*;

public:
	detour() = default;
	~detour();

	detour(detour const&) = delete;
	detour& operator=(detour const&) = delete;

	void hook(ID3DXConstantTable*);

private:
	void _unhook(detoured_ptr function);

private:
	std::vector<void*> _detoured_functions;

	std::unordered_map<target_ptr, detoured_ptr> _target_ptrs;
	std::unordered_map<detoured_ptr, target_ptr> _detoured_ptrs;
};