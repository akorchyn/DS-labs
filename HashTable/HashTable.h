#pragma once

#include <string>
#include "Tree.h"

template< class T>
class HashTable : public ICollection<T>
{
public:
	HashTable(std::function<size_t(T)> hash)
		: function(hash)
	{
		table = new Row[capacity];
	}
	~HashTable()
	{
		delete[] table;
	}

	void add(const T& val) override
	{
		if (elements > (capacity / 2 + capacity / 4))
		{
			rebuild();
		}

		size_t index = hash(val);
		table[index].add(val);
		elements++;
	}

	bool contains(const T& val) const override
	{
		unsigned long index = hash(val);
		return table[index].contains(val);
	}

	bool remove(const T& val) override
	{
		size_t index = hash(val);
		if (table[index].remove(val))
		{
			elements--;
			return true;
		}
		return false;
	}

	size_t size() const override
	{
		return elements;
	}

private:
	unsigned long hash(T val) const
	{
		return function(val) % capacity;
	}

	void rebuild()
	{
		auto old_table = table;
		auto old_capacity = capacity;
		auto new_table = new Row[capacity * 2];
		table = new_table;
		elements = 0;
		capacity = capacity * 2;

		auto add_elements = [this](const T& value) { add(value); };
		for (size_t i = 0; i < old_capacity; ++i)
		{
			old_table[i].traverse(add_elements);
		}
		delete[] old_table;
	}

	using Row = Tree<T>;
	Row* table = nullptr;
	size_t elements = 0;
	size_t capacity = 20;
	std::function<size_t(T)> function;
};
