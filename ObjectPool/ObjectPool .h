#pragma once
#include <array>
#include <vector>
using namespace std;

template<typename T, size_t N>
class ObjectPool
{
private:
	array<T, N> pool_;
	vector<size_t> free_;
public:
	ObjectPool()
	{
		free_.reserve(N);
		for (size_t i = 0; i < N; ++i)
			free_.emplace_back(i);
	}
	T* allocate()
	{
		
		return nullptr;
	}
	void deallocate(T* obj)
	{
		
	}
};
