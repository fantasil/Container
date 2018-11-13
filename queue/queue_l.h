#pragma once
#include"tools\node.h"
#include"basic_queue.h"
#include<iostream>

#ifndef Queue_List_Array_Fantasil_H
#define Queue_List_Array_Fatnasil_H


namespace sli {
	namespace cont {

		template<class T>
		class queue_l :basic_queue<T> {
		public:
			queue_l();
			~queue_l();
		public:
			inline bool empty() const;
			int enqueue(const T& val);
			int dequeue(T& val);
			template<class U>
			friend std::istream& operator>>(std::istream& is, queue_l<U>& ql);
			template<class U>
			friend std::ostream& operator<<(std::ostream& os, queue_l<U>& ql);
		private:
			node::bi_node<T>* _front;
			node::bi_node<T>* _tail;
		};
		template<class T>
		queue_l<T>::queue_l()
			:_front{nullptr},
			_tail{nullptr}
		{
		}
		template<class T>
		queue_l<T>::~queue_l()
		{
			node::bi_node<T>* cur = _front;
			auto next = cur->_next;
			while (cur)
			{
				delete cur;
				cur = next;
				if(cur)
					next = cur->_next;
			}
			_front = nullptr;
			_tail = nullptr;
		}
		template<class T>
		inline bool queue_l<T>::empty() const
		{
			return _front==nullptr;
		}
		template<class T>
		int queue_l<T>::enqueue(const T & val)
		{
			node::bi_node<T>* node = new node::bi_node<T>{ val,nullptr,nullptr };
			if (empty())
			{
				_front = _tail = node;
			}
			else {
				_tail->_next = node;
				node->_prev = _tail;
				_tail = node;
			}
			return 0;
		}
		template<class T>
		int queue_l<T>::dequeue(T & val)
		{
			val = std::move(_front->val);
			auto temp = _front;
			_front = _front->_next;
			delete temp;
			if(_front)
				_front->_prev = nullptr;
			return 0;
		}
		template<class U>
		std::istream & operator>>(std::istream & is, queue_l<U>& ql)
		{
			U temp{};
			while (is >> temp)
			{
				ql.enqueue(temp);
			}
			return is;
		}
		template<class U>
		std::ostream & operator<<(std::ostream & os, queue_l<U>& ql)
		{
			os << "{";
			for (auto p = ql._front; p != ql._tail; p = p->_next)
			{
				os << p->val << ",";
			}
			os << ql._tail->val << "}";
			return os;
		}
	}
}


#endif // !Queue_List_Array_Fantasil_H
