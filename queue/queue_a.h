#pragma once
#include"basic_queue_a.h"
#include"tools\tools.h"
#include<iostream>
#ifndef Queue_Array_Fantasil_H
#define Queue_Array_Fantasil_H


namespace sli {
	namespace cont {
		template<class T>
		class queue_a :basic_queue_a<T> {
		public:
			queue_a(int ca = 16);
			~queue_a();
		public:
			inline bool empty() const;
			inline bool full() const;
			inline int size() const;

			int enqueue(const T& val);
			int dequeue(T& val);

			template<class U>
			friend std::istream& operator>>(std::istream& is, queue_a<U>& qa);

			template<class U>
			friend std::ostream& operator<<(std::ostream& os, queue_a<U>& qa);
		private:
			void resize(double factor=2.0);
		private:
			T * _element;
			int _capacity, _size, _front;
		};

		template<class T>
		queue_a<T>::queue_a(int ca)
			:_capacity{ca},
			_size{0},_front{0}
		{
			_element = new T[_capacity];
		}

		template<class T>
		queue_a<T>::~queue_a()
		{
			if (_element)
			{
				delete[] _element;
			}
		}

		template<class T>
		inline bool queue_a<T>::empty() const
		{
			return _size==0;
		}
		template<class T>
		inline bool queue_a<T>::full() const
		{
			return _capacity==_size;
		}
		template<class T>
		inline int queue_a<T>::size() const
		{
			return _size;
		}
		template<class T>
		int queue_a<T>::enqueue(const T & val)
		{
			if (full())
			{
				resize();
			}
			int tail = (_front + _size)%_capacity;
			_element[tail] = val;
			++_size;
			return 0;
		}

		template<class T>
		 int queue_a<T>::dequeue(T & val)
		{
			if (empty())
			{
				return -1;
			}
			val = _element[_front];
			_element[_front].~T();
			_front = (++_front) % _capacity;
			--_size;
			return 0;
		}

		template<class T>
		void queue_a<T>::resize(double factor)
		{
			sli::tool::array_resize(_element, _capacity, factor, _front);
			int temp = _capacity;
			_capacity = static_cast<int>(_capacity * factor);
			_size = _capacity > temp ? temp : _capacity;
			_front = 0;
		}

		template<class U>
		std::istream & operator>>(std::istream & is, queue_a<U>& qa)
		{
			U temp{};
			while (cin >> temp)
			{
				qa.enqueue(temp);
			}
			return is;
		}

		template<class U>
		std::ostream & operator<<(std::ostream & os, queue_a<U>& qa)
		{
			os << "{";
			int flag = qa._size - 1;
			int pos = qa._front;
			for (int i = 0; i < flag; ++i)
			{
				pos = pos % qa._capacity;
				os << qa._element[pos] << ",";
				++pos;
			}
			pos = pos % qa._capacity;
			os << qa._element[pos] << "}";
			return os;

		}

	}
}



#endif