#pragma once
#ifndef My_List_H_Fantasil
#define My_List_H_Fantasil
#include<iterator>
#include"my_node.h"
#include<exception>
namespace sli
{
	

	template<class Node>
	class list_iter {
	public:
		//type_traits
		using value_type = typename node_traits<Node>::value_type;
		using pointer = typename node_traits<Node>::pointer;
		using reference = typename node_traits<Node>::reference;
		using node_type = typename node_traits<Node>::node_type;
		using node_type_pointer = typename node_traits<Node>::node_type_pointer;
		using node_type_reference = typename node_traits<Node>::node_type_reference;
		using iterator_category = std::bidirectional_iterator_tag;
		using size_type = size_t;
		using difference_type = std::ptrdiff_t;
		using self = list_iter<Node>;
	public:
		inline bool operator==(const self& other) { return cur == other.cur; }
		inline bool operator!=(const self& other) { return !operator==(other); }
		inline reference operator*() { return (*cur).val; }
		inline node_type_pointer operator->() { return cur; }
		inline self& operator++() { cur = cur->next; return *this; }
		inline self operator++(int) { self temp = *this; cur = cur->next; return temp; }
		inline self& operator--() { cur = cur->prev; return *this; }
		inline self operator--(int) { self temp = *this; cur = cur->prev; return temp; }
		inline self& operator+(int n) = delete;
		inline self& operator-(int n) = delete;
		inline self& operator*(int n) = delete;
		inline self& operator/(int n) = delete;
		inline self& operator%(int n) = delete;
		inline self& operator[](int n) = delete;
		node_type_pointer cur;
		
	};

	template<class T,class Node=bi_node<T>,class Iter=list_iter<Node>,class Alloc=std::allocator<Node>>
	class my_list {
	public:
		//type_traits
		using iterator = Iter;
		using iterator_category = typename std::iterator_traits<iterator>::iterator_category;
		using value_type = typename Iter::value_type;
		using reference = typename Iter::reference;
		using const_reference = const reference;
		using pointer = typename Iter::pointer;
		using const_pointer = const pointer;
		using node_type = typename Iter::node_type;
		using node_type_pointer = typename Iter::node_type_pointer;
		using node_type_reference = typename Iter::node_type_reference;
		using node_category = typename Iter::node_type;
		using difference_type = typename Iter::difference_type;
		using allocator_type = typename std::allocator_traits<Alloc>::allocator_type;
		using size_type = size_t;
	public:
		my_list();
		~my_list();
	public:
		inline iterator begin() { iterator temp{}; temp.cur = finish->next; return temp; }
		inline iterator end() const { return finish; }
	public:
		inline bool empty() const { return _size==0; }
		inline size_type size() const { return _size; }
	public:
		inline value_type front() const { return finish->next; }
		inline value_type back() const { return finish->prev; }
	public:
		void push_back(const value_type& val);
		void push_front(const value_type& val);
		void pop_back() throw(std::exception);
		void pop_front() throw(std::exception);
		void insert(iterator position, const value_type& val) throw(std::exception);
		void erase(iterator position) throw(std::exception);

		template<class istream,class U>
		friend istream& operator>>(istream& is, my_list<U>& ml);

		template<class ostream,class U>
		friend ostream& operator<<(ostream& os, my_list<U>& ml);
	private:
		//
		bool is_valid_iter(iterator position);
		iterator finish;
		size_type _size;
		Alloc alloc;
	};
	

	

	template<class T, class Node, class Iter, class Alloc>
	my_list<T, Node, Iter, Alloc>::my_list()
	{
		node_type_pointer new_node=alloc.allocate(1);
		alloc.construct(new_node);
		//node_type_pointer new_node = new node_type{ 0 };
		finish.cur = new_node;
		finish->prev = finish.cur;
		finish->next = finish.cur;
	}

	template<class T, class Node, class Iter, class Alloc>
	my_list<T, Node, Iter, Alloc>::~my_list()
	{
		auto p = finish.cur;
		auto q = finish->next;
		while (_size!=0)
		{
			p = q;
			q = q->next;
			alloc.deallocate(p,1);
			--_size;
		}
	}

	
	template<class T, class Node, class Iter,class Alloc>
	void my_list<T, Node, Iter,Alloc>::push_back(const value_type & val)
	{
		insert(end(), std::ref(val));
	}

	template<class T, class Node, class Iter, class Alloc>
	void my_list<T, Node, Iter, Alloc>::push_front(const value_type & val)
	{
		insert(begin(), std::ref(val));
	}

	/*
		@std::exception:可能会产生异常,如链表为空,迭代器无效等等.
		内部调用erase.
	*/
	template<class T, class Node, class Iter,class Alloc>
	void my_list<T, Node, Iter,Alloc>::pop_back() throw(std::exception)
	{
		iterator temp;
		temp.cur = finish->prev;
		erase(temp);
	}

	template<class T, class Node, class Iter, class Alloc>
	void my_list<T, Node, Iter, Alloc>::pop_front() throw(std::exception)
	{
		erase(begin());
	}

	template<class T, class Node, class Iter, class Alloc>
	void my_list<T, Node, Iter, Alloc>::insert(iterator position, const value_type & val) throw(std::exception)
	{
		/*node_type_pointer new_node = alloc.allocate(1);
		alloc.construct(new_node, val);*/
		node_type_pointer new_node = new node_type{ val };
		/*
		if(!is_valid_iter(position))
			throw std::exception{"invalid iter param pos"};
		*/
		//当链表为空时,position只能为finish
		//将new_node插入到postion和_prev之间,若position是finish,则finish和new_node构成了一个环路
		node_type_pointer _prev = position->prev;
		_prev->next = new_node;
		new_node->prev = _prev;
		new_node->next = position.cur;
		position->prev = new_node;
		++_size;
		
	}

	template<class T, class Node, class Iter, class Alloc>
	void my_list<T, Node, Iter, Alloc>::erase(iterator position) throw(std::exception)
	{
		/*
			当从finish开始遍历节点,其中有存在和position相等的节点时,position才是有效的
			简单起见,只确认positon的next和prev域都不为空,且position不为finish节点(finish节点不可达)
			if(!is_valid_iter(position))
			{
				throw std::exception{"invalid iter param"};
			}
		*/
		if (empty())
 			throw std::exception{ " cont is empty." };
		if ((position->next)==nullptr || (position->prev)==nullptr||position==finish)
		{
			throw std::exception{ "invalid iter param." };
		}

		//连接_prev和_next,并删除positon
		node_type_pointer _prev = position->prev;
		node_type_pointer _next = position->next;

		_prev->next = _next;
		_next->prev = _prev;
		alloc.deallocate(position.cur,1);
		--_size;
	}

	template<class T, class Node, class Iter, class Alloc>
	bool my_list<T, Node, Iter, Alloc>::is_valid_iter(iterator position)
	{
		//该iterator必须是链表中的某一个节点
		for (auto p = begin(); p != end(); ++p)
		{
			if (p == position)
				return true;
		}
		return false;
	}

	template<class istream, class U>
	istream & operator>>(istream & is, my_list<U>& ml)
	{
		U temp{};
		while (is >> temp)
		{
			ml.push_back(temp);
		}
		return is;
	}

	template<class ostream, class U>
	ostream & operator<<(ostream & os, my_list<U>& ml)
	{
		os << "{";
		auto last = --(ml.end());
		for (auto p = ml.begin(); p != last; ++p)
		{
			os << p->val << ",";
		}
		os << last->val << "}";
		return os;
	}

}

#endif // ! My_List_H_Fantasil
