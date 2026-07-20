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
	LockFreeQueue()
	{
		for (size_t i = 0; i < N; ++i)
		{
			buffer_[i].seq.store(i, std::memory_order_relaxed);
		}
	}
	static constexpr size_t capacity() { return N; }

	bool push(const T& value)
	{
		size_t t = tail_.load(std::memory_order_relaxed);

		while (true)
		{
			Slot& slot = buffer_[t % N];
			size_t seq = slot.seq.load(std::memory_order_acquire);
			intptr_t diff = static_cast<intptr_t>(seq) - static_cast<intptr_t>(t);
			if (diff == 0)
			{
				if (tail_.compare_exchange_weak(t, t + 1, std::memory_order_relaxed))
				{
					slot.data = value;
					slot.seq.store(t + 1, std::memory_order_release);
					return true;
				}
			}
			else if (diff < 0)
			{
				return false;
			}
			else
			{
				t = tail_.load(std::memory_order_relaxed);
			}
		}
	}

	bool pop(T& out)
	{
		size_t h = head_.load(std::memory_order_relaxed);

		while (true)
		{
			Slot& slot = buffer_[h % N];
			size_t seq = slot.seq.load(std::memory_order_acquire);
			intptr_t diff = static_cast<intptr_t>(seq) - static_cast<intptr_t>(h);
			if (diff == 1)
			{
				if (head_.compare_exchange_weak(h, h + 1, std::memory_order_relaxed))
				{
					out = buffer_[h % N].data;
					slot.seq.store(h + N, std::memory_order_release);
					return true;
				}
			}
			else if (diff < 0)
			{
				return false;
			} else
			{
				h = head_.load(std::memory_order_relaxed);
			}
		}
	}
};
