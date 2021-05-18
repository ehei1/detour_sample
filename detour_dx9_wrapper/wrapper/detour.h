#pragma once

#include <vector>
#include <unordered_map>


class detour
{
public:
	using detoured_ptr = void*;
	using source_ptr = void*;
	using target_ptr = void*;

	struct function {
		source_ptr source_ptr;
		detoured_ptr detoured_ptr;
		target_ptr* target_ptr;
	};

	using functions = std::vector<function>;

public:
	detour() = default;
	~detour();

	detour(detour const&) = delete;
	detour& operator=(detour const&) = delete;

	void hook(ID3DXConstantTable*);
	void hook(ID3DXEffect*);

private:
	void _unhook(detoured_ptr function);
	void _hook(functions&&);

private:
	std::vector<void*> _detoured_functions;

	std::unordered_map<target_ptr, detoured_ptr> _target_ptrs;
	std::unordered_map<detoured_ptr, target_ptr> _detoured_ptrs;
};