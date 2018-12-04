#pragma once
#ifndef My_Deque_H_Fantasil
#define My_Deque_H_Fantasil
#include<list>
#include<type_traits>
#include<exception>
namespace sli
{
	/*
		@[first,last]是存储区域的边界
		@cur:当前迭代器指向的位置
		@map:控制中心,存储每一个存储区的位置
		@node:指向一个确定的存储区
	*/
	template<class T,int Buff=16>
	class my_deque_iter {
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using pointer = T * ;
		using reference = T & ;
		using const_pointer = const pointer;
		using const_reference = const reference;
		using difference_type = std::ptrdiff_t;
		using ctl_map = std::list<pointer>;
		using ctl_map_iter = typename ctl_map::iterator;
		using self = my_deque_iter<T, Buff>;
	public:
		inline bool operator==(const self& other) { return other.cur == cur; }
		inline bool operator!=(const self& other) { return other.cur != cur; }
		inline reference operator*() { return *cur; }
		inline pointer operator->() { return cur; }
		self & operator++()
		{
			if (cur == last)
			{
				++node;
				if (node != map.end()) {
					first = *node;
					last = first + Buff - 1;
					cur = first;
				}
				else {
					first = last = cur = nullptr;	//如果后面没有存储区域了,手动置空
				}
			}
			else {
				++cur;
			}
			return *this;
		}
		self& operator++(int) { auto temp = *this; ++*this; return temp; }
		self& operator--()
		{
			if (cur == first)
			{
				if (node != map.begin()) {
					--node;
					first = *node;
					last = first + Buff - 1;
					cur = last;
				}
				else {
					node = map.end();
					first = last = cur = nullptr;	//手动置空,节点指向map.end();
				}
			}
			else {
				--cur;
			}
			return *this;
		}
		self& operator--(int) { auto temp = *this; --*this; return temp; }
		self& operator-(difference_type offset)
		{
			difference_type left = cur - first;
			if (offset > left)
			{	//此时目标不在同一存储区
				difference_type temp = offset - left;
				difference_type offset_1 = static_cast<difference_type>(temp / Buff);
				difference_type offset_2 = temp % Buff;
				while (offset_1)
				{
					--node;
					--offset_1;
				}
				--node;	//目标所在区
				first = *node;
				last = first + Buff - 1;
				cur = last - offset + 1;	//目标所在位置
			}
			else {
				cur -= offset;
			}
			return *this;
		}
		self& operator+(difference_type offset)
		{
			difference_type left = last - cur;
			if (offset > left)
			{	//此时目标不在该存储区域
				difference_type temp = offset - left;
				difference_type offset_1 = static_cast<difference_type>(temp / Buff);
				difference_type offset_2 = temp % Buff;
				while (offset_1)
				{
					++node;
					--offset_1;
				}
				++node;	//目标所在存储区
				first = *node;
				last = first + Buff - 1;
				cur = first + offset_2 - 1;	//目标所在位置
			}
			else {
				cur += offset;
			}
			return *this;
		}
		difference_type operator-(const self& other)
		{
			if (node != other.node)
			{	//不在同一存储区域
				difference_type offset_1 = other.last - other.cur;
				difference_type offset_2{};
				int num{};
				ctl_map_iter node_1 = node;
				ctl_map_iter node_2 = other.node;
				if (node == map.end())
				{	//当前节点指向尾端,其值无效,
					for (; node_2 != map.end(); ++node_2)
					{
						++num;
					}
					--num;
					offset_2 = num * Buff;
				}
				else {
					while (node_1 != node_2)
					{
						--node_1;
						++num;
					}
					--num;
					offset_2 = num * Buff;
				}
				difference_type offset_3 = cur - first + 1;
				return offset_1 + offset_2 + offset_3;
			}

			return cur - other.cur;
		}
	public:
		pointer first;
		pointer last;
		pointer cur;
		ctl_map& map;
		ctl_map_iter node;
	public:
		my_deque_iter(ctl_map& _map) :first{ nullptr }, last{ nullptr }, cur{ nullptr }, map{_map} { node = map.end(); }
		my_deque_iter& operator=(const self& other)
		{
			first = other.first;
			last = other.last;
			cur = other.cur;
			map = other.map;
			node = other.node;
			return *this;
		}
		my_deque_iter(const self& other)
			:first{other.first},
			last{other.last},
			cur{other.cur},
			map{other.map},
			node{other.node}
		{
		}
	};


	template<class T,class Alloc=std::allocator<T>,int Buff=16>
	class my_deque {
	public:
		using value_type = T;
		using pointer = T * ;
		using reference = T & ;
		using const_pointer = const pointer;
		using const_reference = const reference;
		using iterator = my_deque_iter<T, Buff>;
		using ctl_map = std::list<pointer>;
		using allocater_type = typename std::allocator_traits<Alloc>::allocator_type;
		using size_type = size_t;
		using difference_type = std::ptrdiff_t;
	private:
		iterator start;
		iterator finish;
		ctl_map map;
		Alloc alloc;
	public:
		inline bool empty()  { return start == finish; }
		inline iterator begin() const { return start; }
		inline iterator end() const { return finish; }
		inline int size() { return finish - start; }
		inline reference front() { return *start; }
		inline reference back()
		{
			if (!empty())
			{
				auto temp = finish;
				return *(--temp);
			}
			throw std::exception{ "cont is empty." };
		}
		reference operator[](size_type n);
		void push_back(const_reference val);
		void push_front(const_reference val);
		void pop_back();
		void pop_front();

		template<class ostream,class U,class Alloc_=std::allocator<U>,int Buff_=16>
		friend ostream& operator<<(ostream& os, my_deque<U,Alloc_,Buff_>& md);
	public:
		my_deque();
		~my_deque();
	};
	
	template<class T, class Alloc, int Buff>
	T& my_deque<T, Alloc, Buff>::operator[](size_type n)
	{
		
		size_type size = static_cast<size_type>(finish - start);
		if (n > size)
		{
			//throw std::exception{"out of range"};
		}

		iterator temp = start +static_cast<difference_type>(n);
		return *temp;
	}

	template<class T, class Alloc, int Buff>
	void my_deque<T, Alloc, Buff>::push_back(const_reference val)
	{
		if (empty()||finish.node==map.end())
		{	
			//开辟新空间
			pointer new_space = alloc.allocate(Buff);
			map.push_back(new_space);
			
			if (empty()) {
				start.node = map.begin();
				start.first = *start.node;
				start.last = start.first + Buff - 1;
				start.cur = start.first + static_cast<int>(Buff/ 2);
				finish = start;
				//start和finish指向存储区域的中点,防止极端情况,一次push_back,一次push_front造成分配两次新空间
			}

			if (finish.node == map.end())
			{
				--finish.node;	//回退到最后一个存储区域,即新开辟的区域
				finish.first = *finish.node;
				finish.last = finish.first + Buff - 1;
				finish.cur = finish.first;
			}
			
		}
		*finish = val;
		++finish;
	}

	template<class T, class Alloc, int Buff>
	void my_deque<T, Alloc, Buff>::push_front(const_reference val)
	{
		if (empty()||start.cur==start.first)
		{
			//开辟新空间
			pointer new_space = alloc.allocate(Buff);
			map.push_front(new_space);
			
			if (empty()) {
				start.node = map.begin();
				start.first = *start.node;
				start.last = start.first + Buff - 1;
				start.cur = start.first + static_cast<int>(Buff / 2);
				finish = start;
			}
		}
		--start;
		*start = val;
	}

	template<class T, class Alloc, int Buff>
	void my_deque<T, Alloc, Buff>::pop_back()
	{
 		if (empty())
			return;
		if (finish.node == map.end())
		{
			--finish.node;
			finish.first =*finish.node;
			finish.last = finish.first + Buff - 1;
			finish.cur = finish.last;
		}
		else {
			--finish;

		}
		alloc.destroy(finish.cur);
		
	}

	template<class T, class Alloc, int Buff>
	void my_deque<T, Alloc, Buff>::pop_front()
	{
		if (empty())
			return;
		alloc.destroy(start.cur);
		++start;
	}

	template<class T, class Alloc, int Buff>
	my_deque<T, Alloc, Buff>::my_deque()
		:map{},start{map},finish{map}
	{
	}

	template<class T, class Alloc, int Buff>
	my_deque<T, Alloc, Buff>::~my_deque()
	{
		if (!std::is_pod<T>::value)
		{
			for (auto p = start; p != finish; ++p)
			{
				alloc.destroy(&(*p));
			}
		}

		for (auto p = map.begin(); p != map.end(); ++p)
		{
			alloc.deallocate(*p, Buff);
		}
	}




	template<class ostream, class U, class Alloc_, int Buff_>
	ostream & operator<<(ostream & os, my_deque<U, Alloc_, Buff_>& md)
	{
		os << "{";
		for (auto p = md.start; p != md.finish; ++p)
		{
			os << *p << " ";
		}
		os << "}";
		return os;
	}

}


#endif // !My_Deque_H_Fantasil
