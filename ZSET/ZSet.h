#pragma once
#include"SkipList.h"
#include<unordered_map>
#include<string>

class ZSet
{
private:
	SkipList<std::pair<int, std::string>, std::string> sl_;
	std::unordered_map<std::string, int> dict_;
public:
	void add(const std::string& member, int score) {
		auto it = dict_.find(member);
		if (it != dict_.end()) {
			int old = it->second;
			if (old == score) return;

			sl_.erase({ old, member });
		}
		dict_[member] = score;

		sl_.insert({ score , member }, member);
	}

	bool score(const std::string& member, int& out) const 
	{
		const auto it = dict_.find(member);
		if (it == dict_.end()) {
			return false;
		}

		out = it->second;
		return true;
	}

	bool erase(const std::string& member)
	{
		const auto it = dict_.find(member);
		if (it == dict_.end()) {
			return false;
		}
		sl_.erase({ it->second, it->first });
		dict_.erase(member);

		return true;
	}

	int rank(const std::string& member)
	{
		const auto it = dict_.find(member);
		if (it == dict_.end()) return -1;
		int score = it->second;
		return sl_.size() - sl_.getRank({ score , member }) + 1;
	}

	bool getByRank(const int rank, std::string& out) const {
		std::pair<int, std::string> key;
		std::string value;
		if (!sl_.getByRank(rank, key, value)) return false;
		out = value;
		return true;
	}

	std::vector<std::pair<std::string, int>> range(int start, int stop) const
	{
		
	}

	int count() const
	{
		
	}

	bool incrBy(const std::string& member, int delta, int& out)
	{
		
	}
};