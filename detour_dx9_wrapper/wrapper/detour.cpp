#include "stdafx.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <unordered_map>

#include "detour_ID3DXConstantTable.h"
#include "detour_ID3DXEffect.h"
#include "detour.h"


template<size_t INDEX, typename TUPLE, typename TARGET_PTRS, typename DETOURED_PTRS>
std::vector<void*> __hook_functions([[maybe_unused]]TUPLE& tuple, [[maybe_unused]]TARGET_PTRS& target_ptrs, [[maybe_unused]]DETOURED_PTRS& detoured_ptrs)
{
	if constexpr (0 == INDEX) {
		return {};
	}
	else {
		auto& functions = std::get<INDEX - 1>(tuple);
		static_assert(3 == std::tuple_size_v<std::decay_t<decltype(functions)>>);

		auto source_function = std::get<0>(functions);
		auto detour_function = std::get<1>(functions);
		auto& target_function = std::get<2>(functions);

		std::vector<void*> result;

		if (auto function = _hook(source_function, detour_function, target_ptrs, detoured_ptrs)) {
			target_function = reinterpret_cast<decltype(target_function)>(function);
			result.emplace_back(function);
		}

		auto other_result = __hook_functions<INDEX - 1>(tuple, target_ptrs, detoured_ptrs);
		std::transform(std::cbegin(other_result), std::cend(other_result), std::back_inserter(result), [](auto v) { return v; });

		return result;
	}
}

template<typename TARGET_PTRS, typename DETOURED_PTRS>
void* __hook_function(void* source, void* target, TARGET_PTRS& target_ptrs, DETOURED_PTRS& detoured_ptrs)
{
	if (std::cend(target_ptrs) == target_ptrs.find(target)) {
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&source, target);
		DetourTransactionCommit();

		target_ptrs.emplace(target, source);
		detoured_ptrs.emplace(source, target);

		return source;
	}

	return {};
}

void detour::_unhook(detoured_ptr function)
{
	auto detoured_iteartor = _detoured_ptrs.find(function);

	if (std::cend(_detoured_ptrs) == detoured_iteartor) {
		assert(false);
	}
	else {
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&function, detoured_iteartor->second);
		DetourTransactionCommit();

		_target_ptrs.erase(detoured_iteartor->second);
		_detoured_ptrs.erase(detoured_iteartor);
	}
}


detour::~detour()
{
	for (auto function : _detoured_functions) {
		_unhook(function);
	}
}

void detour::hook(ID3DXConstantTable* instance)
{
	_hook(detour_ID3DXContantTable::detour(instance));
}

void detour::hook(ID3DXEffect* instance)
{
	_hook(detour_ID3DXEffect::detour(instance));
}

void detour::_hook(functions&& functions)
{
	for (auto& function : functions) {
		if (auto result = __hook_function(function.source_ptr, function.detoured_ptr, _target_ptrs, _detoured_ptrs)) {
			*function.target_ptr = result;

			_detoured_functions.emplace_back(result);
		}
	}
}