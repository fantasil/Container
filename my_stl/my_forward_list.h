#pragma once
#ifndef My_Forward_List_H_Fantasil
#define My_Forward_List_H_Fantasil

#include<allocators>
#include<exception>
#include<iostream>
#include"my_node.h"

namespace sli
{

	template<class T,class Node=forward_node<T>>
	class forward_iter {
	public:
		using value_type = typename node_traits<Node>::value_type;
		using pointer = typename node_traits<Node>::pointer;
		using const_pointer = const pointer;
		using reference = typename node_traits<Node>::reference;
		using const_reference = const reference;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;
		using node_type = typename node_traits<Node>::node_type;
		using node_type_pointer = typename node_traits<Node>::node_type_pointer;
		using node_type_reference = typename node_traits<Node>::node_type_reference;
		using node_category = typename node_traits<Node>::node_category;
		using self = forward_iter<T, Node>;
	public:
		//Ö§³Ö++,==,!=,*,->²Ù×÷
		node_type_pointer cur;
		inline reference operator*() { return cur->val; }
		inline node_type_pointer operator->() { return cur; }
		inline self& operator++() { cur = cur->next; return *this; }
		inline self& operator++(int) { auto iter = *this; cur = cur->next; return iter; }
		inline bool operator==(const forward_iter<T,Node>& other) { return cur == other.cur; }
		inline bool operator==(const node_type_pointer node) { return cur == node; }
		inline bool operator!=(const forward_iter<T,Node>& other) { return cur != other.cur; }
		inline bool operator!=(const node_type_pointer node) { return cur != node; }
		inline self& operator--() = delete;
		inline self& operator--(int) = delete;
		inline self& operator+(int) = delete;
		inline self& operator-(int) = delete;
	public:
		forward_iter() :cur{nullptr}{}
		forward_iter(const forward_iter<T,Node>& other) :cur{ other.cur } {}
		forward_iter(const node_type_pointer node) :cur{ node } {}
	};

	template<class T , class Node = forward_node<T>, class Iter=forward_iter<T>,class Alloc = std::allocator<Node>>
	class my_forward_list {
	public:
		using iterator = Iter;
		using value_type = typename Iter::value_type;
		using pointer = typename Iter::pointer;
		using const_pointer = const pointer;
		using reference = typename Iter::reference;
		using const_reference = const reference;
		using difference_type = typename Iter::difference_type;
		using node_type = typename node_traits<Node>::node_type;
		using node_type_pointer = typename node_traits<Node>::node_type_pointer;
		using node_type_reference = typename node_traits<Node>::node_type_reference;
		using node_category = typename node_traits<Node>::node_category;
		using allocator_type = typename std::allocator_traits<Alloc>::allocator_type;
		using size_type = size_t;
	public:
		my_forward_list();
		~my_forward_list();
	public:
		iterator begin() { iterator temp; temp.cur = finish->next; return temp; }
		iterator end() { return finish; }
		size_type size() const noexcept { return _size; }
		inline bool empty() const { return finish==finish->next; }
		void insert_after(iterator pos, const T& val) throw(std::exception);
		void erase_after(iterator pos) throw(std::exception);
		void push_front(const T& val);
		void pop_front() throw(std::exception);
	public:
		template<class istream,class U>
		friend istream& operator>>(istream& is, my_forward_list<U>& mfl);
		template<class ostream,class U>
		friend ostream& operator<<(ostream& os, my_forward_list<U>& mfl);
	protected:
		//check the iterator 
		bool is_valid_iter(iterator pos) { return true; }
	private:
		iterator finish;
		Alloc alloc;
		size_type _size;
	};
	
	template<class T, class Node, class Iter, class Alloc>
	my_forward_list<T, Node, Iter, Alloc>::my_forward_list()
	{
		node_type_pointer _finish = alloc.allocate(1);
		alloc.construct(_finish);
		_finish->next = _finish;
		finish.cur = _finish;
	}

	template<class T, class Node, class Iter, class Alloc>
	my_forward_list<T, Node, Iter, Alloc>::~my_forward_list()
	{
		iterator _next;
		for (auto p = begin(); p != end();)
		{
			_next = p->next;
			alloc.deallocate(p.cur,1);
			p = _next;
		}
		alloc.deallocate(finish.cur,1);


	}

	template<class T, class Node, class Iter, class Alloc>
	void my_forward_list<T, Node, Iter, Alloc>::insert_after(iterator pos, const T & val) throw(std::exception)
	{
		if (!is_valid_iter(pos))
			throw std::exception{ "bad iter param." };

		/*node_type_pointer new_node=alloc.allocate(1);
		alloc.construct(new_node, { val,nullptr });*/
		node_type_pointer new_node = new node_type{ val };
		iterator next = pos->next;
		pos->next = new_node;
		new_node->next = next.cur;
		++_size;	
	}

	template<class T, class Node, class Iter, class Alloc>
	void my_forward_list<T, Node, Iter, Alloc>::erase_after(iterator pos) throw(std::exception)
	{
		if (!is_valid_iter(pos))
			throw std::exception{ "bad iter param." };

		iterator to_del = pos->next;
		if (to_del==finish) {
			throw std::exception{ "bad iter param." };
		}
		iterator next = to_del->next;
		pos->next = next.cur;
		alloc.deallocate(to_del.cur,1);
		--_size;
	}

	template<class T, class Node, class Iter, class Alloc>
	void my_forward_list<T, Node, Iter, Alloc>::push_front(const T & val)
	{
		insert_after(finish, std::ref(val));
	}

	template<class T, class Node, class Iter, class Alloc>
	void my_forward_list<T, Node, Iter, Alloc>::pop_front() throw(std::exception)
	{
		erase_after(finish);
	}

	template<class istream, class U>
	istream& operator>>(istream & is, my_forward_list<U>& mfl)
	{
		U temp{};
		while (is >> temp)
		{
			mfl.push_front(temp);
		}
		return istream();
	}

	template<class ostream, class U>
	ostream& operator<<(ostream & os, my_forward_list<U>& mfl)
	{
		os << "{";
		for (auto p =mfl.begin(); p !=mfl.end(); ++p)
		{
			os << *p << " ";
		}
		os << "}";
		return os;
	}

}


#endif // !My_Forward_List_H_Fantasil
