#pragma once

#include <cassert>
#include <vector>
#include <functional>
#include <algorithm>

namespace Core
{
    template <typename T> class List
    {
      private:
        std::vector<T> list;

      public:
        size_t count() const { return list.size(); }

        void add(const T& value);
        void remove(const T& value);
        void removeAt(int index);
        T& get(int index);
        const T& get(int index) const;
        void set(int index, const T& value);
        int indexOf(const T& value) const;
        void clear() noexcept;
        void resize(int size);
        void fill(const T& value);
        T* ptr();
        std::vector<T>& vec() { return list; }

        T& operator[](int index) { return get(index); }
        const T& operator[](int index) const { return get(index); }

        std::vector<T>::iterator begin() { return list.begin(); }
        std::vector<T>::iterator end() { return list.end(); }
        std::vector<T>::const_iterator begin() const { return list.begin(); }
        std::vector<T>::const_iterator end() const { return list.end(); }
        std::vector<T>::const_iterator cbegin() const { return list.cbegin(); }
        std::vector<T>::const_iterator cend() const { return list.cend(); }

        bool tryFind(T& out, std::function<bool(T&)> func);
        bool contains(const T& value) const;
        bool isEmpty() const noexcept { return list.empty(); }
        void sort(std::function<bool(T&, T&)> func);

        List& operator=(std::vector<T> other);
    };

    template <typename T> inline void List<T>::add(const T& value)
    {
        list.push_back(value);
    }

    template <typename T> inline void List<T>::remove(const T& value)
    {
        auto it = std::find(list.begin(), list.end(), value);
        if (it != list.end()) list.erase(it);
    }

    template <typename T> inline void List<T>::removeAt(int index)
    {
        assert(index >= 0 && index < static_cast<int>(list.size()) && "List index out of bounds");
        list.erase(list.begin() + index);
    }

    template <typename T> inline T& List<T>::get(int index)
    {
        assert(index >= 0 && index < static_cast<int>(list.size()) && "List index out of bounds");
        return list[index];
    }

    template <typename T> inline const T& List<T>::get(int index) const
    {
        assert(index >= 0 && index < static_cast<int>(list.size()) && "List index out of bounds");
        return list[index];
    }

    template <typename T> inline void List<T>::set(int index, const T& value)
    {
        assert(index >= 0 && index < static_cast<int>(list.size()) && "List index out of bounds");
        list[index] = value;
    }

    template <typename T> inline int List<T>::indexOf(const T& value) const
    {
        auto it = std::find(list.begin(), list.end(), value);
        return it != list.end() ? std::distance(list.begin(), it) : -1;
    }

    template <typename T> inline void List<T>::clear() noexcept
    {
        list.clear();
    }

    template <typename T> inline void List<T>::resize(int size)
    {
        list.resize(size);
    }

    template <typename T> inline void List<T>::fill(const T& value)
    {
        std::fill(list.begin(), list.end(), value);
    }

    template <typename T> inline T* List<T>::ptr()
    {
        return list.empty() ? nullptr : list.data();
    }

    template <typename T> inline bool List<T>::tryFind(T& out, std::function<bool(T&)> func)
    {
        auto it = std::find_if(list.begin(), list.end(), func);
        if (it == list.end()) return false;

        out = *it;
        return true;
    }

    template <typename T> inline bool List<T>::contains(const T& value) const
    {
        return std::find(list.begin(), list.end(), value) != list.end();
    }

    template <typename T> inline void List<T>::sort(std::function<bool(T&, T&)> func)
    {
        std::sort(list.begin(), list.end(), func);
    }

    template <typename T> inline List<T>& List<T>::operator=(std::vector<T> other)
    {
        list = other;
        return *this;
    }
} // namespace Core