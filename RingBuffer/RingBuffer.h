#pragma once
#include <array>
#include <atomic>

template<typename T, size_t N>
class RingBuffer
{
private:
	std::array<T, N> buffer_ = {};
	std::atomic<size_t> head_{ 0 };
	std::atomic<size_t> tail_{ 0 };
public:
	bool empty() const { return count_ == 0; }
	bool full() const { return count_ == N; }
	size_t size() const { return count_; }
	size_t capacity() const { return N; }

	bool push(const T& value)
	{
		if (full()) return false;
		buffer_[tail_] = value;
		tail_ = (tail_ + 1) % N;
		++count_;
		return true;
	}

	bool pop(T& out)
	{
		if (empty()) return false;
		out = buffer_[head_];
		head_ = (head_ + 1) % N;
		--count_;
		return true;
	}
};
