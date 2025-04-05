#pragma once

#include <cassert>
#include <vector>
#include <functional>
#include <algorithm>

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
		void remove(T value);
		void removeAt(int index);
		T& get(int index);
		void set(int index, T value);
		int indexOf(T value);
		void clear();
		T* ptr();
		std::vector<T>::iterator begin() { return list.begin(); }
		std::vector<T>::iterator end() { return list.end(); }
		bool tryFind(T& out, std::function<bool(T&)> func);
		bool tryFind(T& search);
		void sort(std::function<bool(T&, T&)> func);

		List& operator = (std::vector<T> other);
	};

	template<typename T>
	inline void List<T>::add(T value)
	{
		list.push_back(value);
	}

	template<typename T>
	inline void List<T>::remove(T value)
	{
		auto it = std::find(list.begin(), list.end(), value);
		if (it != list.end())
			list.erase(it);
	}

	template<typename T>
	inline void Core::List<T>::removeAt(int index)
	{
		assert(list.size() > index && "List index out of bounds");

		list.erase(list.begin() + index);
	}

	template<typename T>
	inline T& List<T>::get(int index)
	{
		assert(list.size() > index && "List index out of bounds");

		return list[index];
	}

	template<typename T>
	inline void List<T>::set(int index, T value)
	{
		assert(list.size() > index && "List index out of bounds");

		list[index] = value;
	}

	template<typename T>
	inline int List<T>::indexOf(T value)
	{
		auto it = std::find(list.begin(), list.end(), value);
		if (it != list.end())
			return std::distance(list.begin(), it);

		return -1;
	}

	template<typename T>
	inline void List<T>::clear()
	{
		list.clear();
	}

	template<typename T>
	inline T* List<T>::ptr()
	{
		return &list[0];
	}

	template<typename T>
	inline bool List<T>::tryFind(T& out, std::function<bool(T&)> func)
	{
		auto it = std::find_if(list.begin(), list.end(), func);
		if (it == list.end()) return false;

		out = *it;
		return true;
	}

	template<typename T>
	inline bool List<T>::tryFind(T& search)
	{
		auto it = std::find(list.begin(), list.end(), search);
		if (it == list.end()) return false;

		return true;
	}

	template<typename T>
	inline void List<T>::sort(std::function<bool(T&, T&)> func)
	{
		std::sort(list.begin(), list.end(), func);
	}

	template<typename T>
	inline List<T>& List<T>::operator=(std::vector<T> other)
	{
		list = other;
		return *this;
	}
}