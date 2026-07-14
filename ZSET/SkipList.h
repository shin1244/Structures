#pragma once
#include <random>
#include <vector>

template<typename Key, typename Value>
class SkipList {
private:
	static constexpr int MAX_LEVEL = 16;
	struct Node
	{
		Key key;
		Value value;
		struct Link {
			Node* next = nullptr;
			int span = 0;
		};
		Link* forward;
		int level;

		Node(const Key& k, const Value& v, int lvl) {
			key = k;
			value = v;
			level = lvl;
			forward = new Link[lvl + 1];
		}
		~Node() {
			delete[] forward;
		}
	};

	Node* _head;
	int _level;
	int _size;

	int randomLevel() {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<int> dis(0, 1);

		int lvl = 0;

		while (dis(gen) == 1 && lvl < MAX_LEVEL) lvl++;
		return lvl;
	}
public:
	SkipList() {
		_level = 0;
		_size = 0;
		_head = new Node(Key{}, Value{}, MAX_LEVEL);
	}
	~SkipList() {
		Node* cur = _head->forward[0];
		while (cur != nullptr)
		{
			Node* next = cur->forward[0];
			delete cur;
			cur = next;
		}
		delete _head;
	}

	Node* search(const Key& key) const {
	}

	void insert(const Key& key, const Value& value) {
	}

	bool erase(const Key& key) {
	}

	void rangeSearch(Key& k1, Key& k2, std::vector<Node*>& v) {
	}

	int size() { return _size; }
	bool empty() { return _size == 0; }
};