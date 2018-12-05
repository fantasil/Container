#pragma once
#ifndef Complete_Binary_Tree_H_Fantasil
#define Complete_Binary_Tree_H_Fantasil
#include"my_node.h"
#include<allocators>
#include<exception>
#include<type_traits>
/*
	complete_binary_tree(��ȫ������),����ʵ��Ϊ�����,�ײ�Ϊ����,���Զ����·���ռ�,Ϊԭ����������С,
	@push:
		����������������,�����ִ���ѽṹ.
		1:��������ĩβ;
		2:�Ӹýڵ㿪ʼ���ݽ���һ��ð��,ʹ֮���ֶѽṹ
	@pop:
		��������ݵ�������,����ʯ����ѽṹ
		1:������βԪ��
		2:����ĩβԪ��,���Ѹ��ڵ��������ð��,ʹ֮���ֶѽṹ
	@top:
		�򵥵ķ������Ԫ��,���Ϊ��,���׳�std::exception�쳣
	@make_heap:
		ʹĿ������ѻ�,
		1:�����һ�������ӽڵ�Ľڵ㿪ʼ�������ݱ��ֶѽṹ
		2:��ǰ����,ֱ�����ڵ����ݱ��ֶѽṹ.
	@parent/left/right:
		�򵥷��ش��ݽڵ��˫�׽ڵ�/���ӽڵ�/���ӽڵ�,����Խλ��֤.
	@percolate_up||down:
		����(����)ʹ֮����(����)���ֶѽṹ������,��˫�׽ڵ��ֵ>�ӽڵ��ֵ;
	@resize_space:
		���·���ռ�,��СΪԭ��������

	//��������ֻ���max_heap����?

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
		void pop();	//�����Ѷ�Ԫ�� (����ѵ����Ԫ��)
		reference top();	//���ضѶ�Ԫ�� 
		void make_heap(pointer addr, size_type sz);	//������addr�ѻ�
		template<class ostream,class U,class Alloca=std::allocator<U>>
		friend ostream& operator<<(ostream& os, complete_binary_tree<U, Alloca>& cbr);
	private:
		inline iterator parent(iterator index);	//���ڵ�λ��
		inline iterator left(iterator index);	//����λ��
		inline iterator right(iterator index);	//�Һ���λ��

		void percolate_up(iterator index);		//���ݱ��ֶѽṹ
		void percolate_down(iterator index);	//���ݱ��ֶѽṹ

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
		auto end_of_stroage_temp = end_of_storage;	//��Ϣ����;

		start = addr;
		finish = start + sz;
		end_of_storage = finish;

		iterator last = finish - 1;
		iterator _parent = parent(last);	//�����һ��ӵ�к��ӽڵ�Ľڵ㿪ʼ�ṹ����
		for (; _parent != start; --_parent)
		{
			percolate_down(_parent);
		}

		percolate_down(start);

		//��Ϣ�ָ�
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
				return;	//�޺��ӽڵ�,���ؼ�������
			}
			else if (_right >= finish)
			{
				if (*_left > *index)
					std::swap(*_left, *index);
				return; //����һ������,�಻�ؼ�������
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
					return;	//�ýṹ�Ǵ����,���ؼ�������
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
		std::copy(start, finish, new_space);	//��Ϊnew_space�Ŀռ�>���е�space,���Ա�֤copy����Խ��
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
		//������Ǽ���������,��Ҫ������������
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
