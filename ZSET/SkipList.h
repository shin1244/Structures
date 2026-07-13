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
		Node** forward;
		int level;

		Node(const Key& k, const Value& v, int lvl) {
			key = k;
			value = v;
			level = lvl;
			forward = new Node*[lvl + 1];
			for (int i = 0; i <= lvl; ++i) forward[i] = nullptr;
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
		Node* cur = _head;
		for (int i = _level; i >= 0; --i) {
			while (cur->forward[i] != nullptr && cur->forward[i]->key < key) {
				cur = cur->forward[i];
			}
		}
		cur = cur->forward[0];
		if (cur != nullptr && cur->key == key) {
			return cur;
		}
		return nullptr;
	}

	void insert(const Key& key, const Value& value) {
		Node* update[MAX_LEVEL + 1];
		for (int i = 0; i <= MAX_LEVEL; ++i) update[i] = nullptr;

		Node* cur = _head;
		for (int i = _level; i >= 0; --i) {
			while (cur->forward[i] != nullptr && cur->forward[i]->key < key)
				cur = cur->forward[i];
			update[i] = cur;
		}

		cur = cur->forward[0];
		if (cur != nullptr && cur->key == key) {
			cur->value = value;
			return;
		}

		int lvl = randomLevel();
		if (lvl > _level) {
			for (int i = _level + 1; i <= lvl; ++i)
				update[i] = _head;
			_level = lvl;
		}

		Node* n = new Node(key, value, lvl);
		for (int i = 0; i <= lvl; ++i) {
			n->forward[i] = update[i]->forward[i];
			update[i]->forward[i] = n;
		}

		_size++;
	}

	bool erase(const Key& key) {
		Node* update[MAX_LEVEL + 1];
		for (int i = 0; i <= MAX_LEVEL; ++i) update[i] = nullptr;
		Node* cur = _head;

		for (int i = _level; i >= 0; --i) {
			while (cur->forward[i] != nullptr && cur->forward[i]->key < key)
				cur = cur->forward[i];
			update[i] = cur;
		}

		cur = cur->forward[0];
		if (cur != nullptr && cur->key == key) {
			for (int i = 0; i <= _level; ++i) {
				if (update[i]->forward[i] != cur) break;
				update[i]->forward[i] = cur->forward[i];
			}
			delete cur;

			while (_level > 0 && _head->forward[_level] == nullptr)
				_level--;
			_size--;
			return true;
		}
		return false;
	}

	void rangeSearch(Key& k1, Key& k2, std::vector<Node*>& v) {
		if (k1 > k2) return;

		Node* cur = _head;
		for (int i = _level; i >= 0; --i) {
			while (cur->forward[i] != nullptr && cur->forward[i]->key < k1)
				cur = cur->forward[i];
		}
		cur = cur->forward[0];
		
		while (cur != nullptr && cur->key <= k2){
			v.push_back(cur);
			cur = cur->forward[0];
		}
	}

	int size() { return _size; }
	bool empty() { return _size == 0; }
};