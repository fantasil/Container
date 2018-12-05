#pragma once
#ifndef Complete_Binary_Tree_H_Fantasil
#define Complete_Binary_Tree_H_Fantasil
#include"my_node.h"
#include<allocators>
#include<exception>
#include<type_traits>
/*
	complete_binary_tree(完全二叉树),这里实现为大根堆,底层为数组,会自动重新分配空间,为原来的两倍大小,
	@push:
		将数据置入容器中,并保持大根堆结构.
		1:总是置于末尾;
		2:从该节点开始上溯进行一次冒泡,使之保持堆结构
	@pop:
		将最大数据弹出容器,并宝石大根堆结构
		1:交换首尾元素
		2:析构末尾元素,并堆根节点进行下溯冒泡,使之保持堆结构
	@top:
		简单的返回最大元素,如果为空,会抛出std::exception异常
	@make_heap:
		使目标数组堆化,
		1:从最后一个含有子节点的节点开始进行下溯保持堆结构
		2:向前步进,直到根节点下溯保持堆结构.
	@parent/left/right:
		简单返回传递节点的双亲节点/左子节点/右子节点,不做越位保证.
	@percolate_up||down:
		上溯(下溯)使之向上(向下)保持堆结构的特征,即双亲节点的值>子节点的值;
	@resize_space:
		重新分配空间,大小为原来的两倍

	//好像把名字换成max_heap更好?

*/
namespace sli
{
	template<class T,class Alloc=std::allocator<T>>
	class complete_binary_tree {
	public:
		using value_type = T;
		using pointer = T * ;
		using reference = T & ;
		using const_pointer = const pointer;
		using const_reference = const reference;
		using iterator = pointer;
		using difference_type = std::ptrdiff_t;
		using size_type = size_t;
	public:
		inline bool empty() const { return finish == start; }
		inline size_type size() const { return finish - start; }
		void push(const_reference val);	
		void pop();	//弹出堆顶元素 (大根堆的最大元素)
		reference top();	//返回堆顶元素 
		void make_heap(pointer addr, size_type sz);	//将数组addr堆化
		template<class ostream,class U,class Alloca=std::allocator<U>>
		friend ostream& operator<<(ostream& os, complete_binary_tree<U, Alloca>& cbr);
	private:
		inline iterator parent(iterator index);	//父节点位置
		inline iterator left(iterator index);	//左孩子位置
		inline iterator right(iterator index);	//右孩子位置

		void percolate_up(iterator index);		//上溯保持堆结构
		void percolate_down(iterator index);	//下溯保持堆结构

		void resize_space();
		iterator start;
		iterator finish;
		iterator end_of_storage;
		Alloc alloc;
	public:
		complete_binary_tree(size_type ca=64);
		~complete_binary_tree();
	};
	template<class T, class Alloc>
	void complete_binary_tree<T, Alloc>::push(const_reference val)
	{
		if (finish == end_of_storage)
		{
			resize_space();
		}
		*finish = val;
		percolate_up(finish);
		++finish;
		
	}
	template<class T, class Alloc>
	void complete_binary_tree<T, Alloc>::pop()
	{
		if (empty())
			return;
		iterator last = finish - 1;
		std::swap(*start, *last);
		alloc.destroy(last);
		--finish;
		percolate_down(start);
	}
	template<class T, class Alloc>
	inline T& complete_binary_tree<T, Alloc>::top()
	{
		if (empty())
			throw std::exception{ "cont is empty" };
		return *start;
	}
	template<class T, class Alloc>
	void complete_binary_tree<T, Alloc>::make_heap(pointer addr, size_type sz)
	{
		auto start_temp = start;
		auto finish_temp = finish;
		auto end_of_stroage_temp = end_of_storage;	//信息保存;

		start = addr;
		finish = start + sz;
		end_of_storage = finish;

		iterator last = finish - 1;
		iterator _parent = parent(last);	//从最后一个拥有孩子节点的节点开始结构化堆
		for (; _parent != start; --_parent)
		{
			percolate_down(_parent);
		}

		percolate_down(start);

		//信息恢复
		start = start_temp;
		finish = finish_temp;
		end_of_storage = end_of_stroage_temp;
	}
	template<class T, class Alloc>
	inline T* complete_binary_tree<T, Alloc>::parent(iterator index)
	{
		return start+static_cast<difference_type>((index-1-start)/2);	
	}
	template<class T, class Alloc>
	inline T* complete_binary_tree<T, Alloc>::left(iterator index)
	{
		return start + (index - start) * 2 + 1;	
	}
	template<class T, class Alloc>
	inline T* complete_binary_tree<T, Alloc>::right(iterator index)
	{
		return start + (index - start) * 2 + 2;
	}

	template<class T, class Alloc>
	void complete_binary_tree<T, Alloc>::percolate_up(iterator index)
	{

		iterator _parent = parent(index);
		while (index != start && *_parent < *index)
		{
			std::swap(*_parent, *index);
			index = _parent;
			_parent = parent(index);
		}

	}
	template<class T, class Alloc>
	void complete_binary_tree<T, Alloc>::percolate_down(iterator index)
	{
		iterator _left = left(index);
		iterator _right = right(index);
		iterator temp = nullptr;
		while (index < finish)
		{
			if (_left >= finish)
			{
				return;	//无孩子节点,不必继续下溯
			}
			else if (_right >= finish)
			{
				if (*_left > *index)
					std::swap(*_left, *index);
				return; //仅有一个左孩子,亦不必继续下溯
			}
			else {
				temp = *_left > *_right ? _left : _right;
				temp = *temp > *index ? temp : index;
				if (temp != index) 
				{
					std::swap(*temp, *index);
					if (temp == _left)
						index = _left;
					else
						index = _right;

					_left = left(index);
					_right = right(index);
				}
				else {
					return;	//该结构是大根堆,不必继续下溯
				}
			}
		}
	}
	template<class T, class Alloc>
	void complete_binary_tree<T, Alloc>::resize_space()
	{
		difference_type cur_sz = finish - start;
		difference_type space_size = 2 * (end_of_storage - start);
		pointer new_space=alloc.allocate(space_size);
		std::copy(start, finish, new_space);	//因为new_space的空间>现有的space,可以保证copy不会越界
		start = new_space;
		end_of_storage = start + space_size;
		finish = start + cur_sz;
	}
	template<class T, class Alloc>
	inline complete_binary_tree<T, Alloc>::complete_binary_tree(size_type ca)
	{
		pointer space=alloc.allocate(ca);
		start = space;
		end_of_storage = space + ca;
		finish = start;
	}
	template<class T, class Alloc>
	inline complete_binary_tree<T, Alloc>::~complete_binary_tree()
	{
		//如果不是简单数据类型,需要调用析构函数
		if (!std::is_pod<T>::value)
		{
			for (auto p = start; p != finish; ++p)
				alloc.destroy(p);
		}

		alloc.deallocate(start, end_of_storage - start);
	}


	

	template<class ostream, class U, class Alloca>
	ostream & operator<<(ostream & os, complete_binary_tree<U, Alloca>& cbr)
	{
		os << "{";
		for (auto p = cbr.start; p != cbr.finish; ++p)
		{
			os << *p << " ";
		}
		os << "}";
		return os;
	}

}

#endif // !Complete_Binary_Tree_H_Fantasil
