#pragma once
#include <array>
#include <atomic>

template<typename T, size_t N>
class LockFreeQueue
{
private:
	struct Slot
	{
		T data;
		std::atomic<size_t> seq;
	};
	std::array<Slot, N> buffer_;
	std::atomic<size_t> head_{ 0 };
	std::atomic<size_t> tail_{ 0 };
public:
};
