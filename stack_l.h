#pragma once
#include"basic_stack.h"
#include"node.h"
#include<iostream>

#ifndef Stack_List_Fantasil_H
#define Stack_List_Fantasil_H

namespace sli {
	namespace cont {



		template<class T>
		class stack_l :basic_stack<T> {
		public:
			stack_l();
			~stack_l();
		public:
			bool empty() const;
			int push(const T& val);
			int pop(T& val);
			int top(T& val);

			template<class U>
			friend std::istream& operator>>(std::istream& is, stack_l<U>& sl);
			template<class U>
			friend std::ostream& operator<<(std::ostream& os, stack_l<U>& sl);
		private:
			node::bi_node<T>* _head;
			node::bi_node<T>* _trail;
		};

		template<class T>
		stack_l<T>::stack_l()
			:_head{nullptr},
			_trail{nullptr}
		{
		}

		template<class T>
		stack_l<T>::~stack_l()
		{
			node::bi_node<T>* cur = _head;
			node::bi_node<T>* next = cur;
			while (cur)
			{
				next = cur->_next;
				delete cur;
				cur = next;
			}
			_head = nullptr;
			_trail = nullptr;
		}

		template<class T>
		bool stack_l<T>::empty() const
		{
			return _head==nullptr;
		}

		template<class T>
		int stack_l<T>::push(const T & val)
		{
			node::bi_node<T>* node = new node::bi_node<T>{ val,nullptr,nullptr };
			if (empty())
			{
				_head = _trail = node;
			}
			else {
				node->_prev = _trail;
				_trail->_next = node;
				_trail = node;
			}
			return 0;
		}

		template<class T>
		int stack_l<T>::pop(T & val)
		{
			if (empty())
			{
				return -1;
			}
			val = _trail->val;
			auto temp = _trail->_prev;
			delete _trail;
			_trail = temp;
			_trail->_next = nullptr;
			return 0;
		}

		template<class T>
		int stack_l<T>::top(T & val)
		{
			if (empty())
			{
				return -1;
			}
			val = _trail->val;
			return 0;
		}
		template<class U>
		std::istream & operator>>(std::istream & is, stack_l<U>& sl)
		{
			U temp{};
			while (is >> temp)
			{
				sl.push(temp);
			}
			return is;
		}
		template<class U>
		std::ostream & operator<<(std::ostream & os, stack_l<U>& sl)
		{
			std::cout << "{";
			for (node::bi_node<U>* p = sl._head; p != sl._trail; p=p->_next)
			{
				std::cout << p->val << ",";
			}
			std::cout << sl._trail->val << "}" << std::endl;
			return os;
		}
	}
}

#endif // !Stack_List_Fantasil_H
