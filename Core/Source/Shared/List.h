#pragma once

#include <vector>

namespace Core
{
	template<typename T>
	class List
	{
	private:
		std::vector<T> list;

	public:
		int count() { return list.size(); }

		void add(T value);
		void removeAt(int index);
		T getAt(int index);
	};

	template<typename T>
	inline void List<T>::add(T value)
	{
		list.push_back(value);
	}

	template<typename T>
	inline void Core::List<T>::removeAt(int index)
	{
		assert(list.size() > index && "List index out of bounds");

		list.erase(list.begin() + index);
	}

	template<typename T>
	inline T List<T>::getAt(int index)
	{
		assert(list.size() > index && "List index out of bounds");

		return list[index];
	}
}