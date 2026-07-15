#pragma once
#include <unordered_map>
#include "DoubleLinkedList.h"

template<typename Value>
class TimingWheel
{
private:
	static constexpr int WHEEL_SIZE = 3600;
	struct TimerEntry
	{
		Value value;
		int   rotation;
		uintptr_t   id;
	};
	using List = DoubleLinkedList<TimerEntry>;
	using Node = typename List::Node;

	List slots_[WHEEL_SIZE];
	int currentTick_ = 0;
public:
	uintptr_t add(int delayTicks, const Value& value)
	{	
		int insertRot = delayTicks / WHEEL_SIZE;
		int insertTick = (currentTick_ + delayTicks) % WHEEL_SIZE;

		TimerEntry entry{ value, insertRot, 0 };
		Node* n = slots_[insertTick].pushFront(entry);
		uintptr_t id = reinterpret_cast<uintptr_t>(n);
		n->value.id = id;

		return id;
	}
	void remove(uintptr_t id)
	{
		if (id == 0) return;
		Node* targetNode = reinterpret_cast<Node*>(id);

		if (targetNode->prev != nullptr) targetNode->prev->next = targetNode->next;
		if (targetNode->next != nullptr) targetNode->next->prev = targetNode->prev;

		delete targetNode;
	}
	void tick()
	{
		currentTick_ = (currentTick_ + 1) % WHEEL_SIZE;

		Node* cur = slots_[currentTick_];
		while (cur != nullptr)
		{
			Node* nextNode = cur->next;
			if (cur->rotation == 0)
			{
				if (cur->prev != nullptr) cur->prev->next = nextNode;
				if (nextNode != nullptr) nextNode->prev = cur->prev;
				if (slots_[currentTick_].head == cur) slots_[currentTick_].head = nextNode;

				delete cur;
			}
			else
			{
				--cur->rotation;
			}
			cur = nextNode;
		}
	}
};