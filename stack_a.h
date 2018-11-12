#pragma once
#include"basic_stack_a.h"
#include<utility>
#include<iostream>
#ifndef Stack_Array_Fantasil_H
#define Stack_Array_Fantasil_H

namespace sli {
	namespace cont {
		template<class T>
		class stack_a :basic_stack_a<T> {
		public:
			stack_a(int ca=16);
			~stack_a();
		public:
			bool empty() const;
			bool full() const;
			int size() const;

			int push(const T& val);
			int top(T& val);
			int pop(T& val);

			template<class U>
			friend std::istream& operator>>(std::istream& is, stack_a<U> sa);
			template<class U>
			friend std::ostream& operator<<(std::ostream& os, stack_a<U> sa);

		private:
			void resize(int factor = 2);
		private:
			T * _element;
			int _capacity, _size;
		};

		template<class U>
		std::istream & operator>>(std::istream & is, stack_a<U> sa)
		{
			U temp{};
			while (is >> temp)
			{
				sa.push(temp);
			}
			return is;
		}
		template<class U>
		std::ostream & operator<<(std::ostream & os, stack_a<U> sa)
		{
			int flag = sa._size - 1;
			os << "{";
			for (int i = 0; i < flag; ++i)
			{
				os << sa._element[i] << ",";
			}
			os << sa._element[flag] << "}\n";
			return os;
		}
	}
}







template<class T>
sli::cont::stack_a<T>::stack_a(int ca)
	:_capacity{ca},
	_size{0}
{
	_element = new T[_capacity];
}

template<class T>
sli::cont::stack_a<T>::~stack_a()
{
	if (_element)
	{
		delete[] _element;
	}
}

template<class T>
inline bool sli::cont::stack_a<T>::empty() const
{
	return _size==0;
}

template<class T>
inline bool sli::cont::stack_a<T>::full() const
{
	return _size==_capacity;
}

template<class T>
inline int sli::cont::stack_a<T>::size() const
{
	return _size;
}

template<class T>
int sli::cont::stack_a<T>::push(const T & val)
{
	int end = _size;
	if (full())
	{
		resize();
	}
	_element[end] = val;
	++_size;
	return 0;
}

template<class T>
int sli::cont::stack_a<T>::top(T & val)
{
	if (empty())
	{
		return -1;
	}
	int trail = _size - 1;
	val = _element[trail];
	return 0;
}

template<class T>
int sli::cont::stack_a<T>::pop(T & val)
{
	if (empty())
	{
		return -1;
	}
	int trail = _size - 1;
	val = std::move(_element[trail]);
	_element[trail].~T();
	--_size;
	return 0;
}

template<class T>
void sli::cont::stack_a<T>::resize(int factor)
{
	int new_capacity = _capacity * factor;
	T* array = new T[new_capacity];
	int capacity = new_capacity > _capacity ? _capacity : new_capacity;
	for (int i = 0; i < capacity; ++i)
	{
		array[i] = _element[i];
	}
	_capacity = new_capacity;
	delete _element;
	_element = array;
	array = nullptr;
}


#endif // !Stack_Array_Fantasil_H
