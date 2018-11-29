#pragma once
#ifndef My_Vector_H_Fantasil
#define My_Vector_H_Fantasil

#include<allocators>
#include<exception>
#include<algorithm>
namespace sli {

	template<class T, class Alloc = std::allocator<T>>
	class my_vector {
	public:
		//type_traits
		using value_type = T;
		using reference = value_type & ;
		using const_reference = const reference;
		using pointer = value_type * ;
		using const_pointer = const pointer;
		using size_type = size_t;
		using iterator = typename pointer;
		using const_iterator = typename const iterator;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::random_access_iterator_tag;
		using allocator_type = typename std::allocator_traits<Alloc>::allocator_type;
	public:
		//constructor and destructor
		my_vector(size_type ca);
		~my_vector();

	public:
		//iterators
		inline iterator begin() { return start; }
		inline iterator end() { return finish; }

	public:
		//capacity;
		inline size_type size() const { return finish - start; }
		inline size_type capacity() const { return end_of_storage - start; }
		inline bool empty() const { return start == finish; }
		inline bool full() const {return  finish == end_of_storage; }

	public:
		//element access;
		inline reference operator[](size_type n) { return *(start + n); }
		inline reference at(size_type n) { return *(start + n); }
		reference front() { if (empty()) { throw std::exception{ "cont is empty" }; } return *start; }
		reference back() { if (empty()) { throw std::exception{ "cont is empty" }; } return *(finish - 1); }
		inline pointer data() { return start; }
	public:
		//modifiers
		void push_back(const value_type& val);
		void pop_back();
		void insert(iterator position, const value_type& val);
		void insert(iterator position, size_type n, const value_type& val);
		/*template<class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);*/
		void erase(iterator position);
		void erase(iterator first, iterator last);
	public:
		//stream
		template<class istream,class U>
		friend istream& operator>>(istream& is, my_vector<U>& mv);

		template<class ostream,class  U>
		friend ostream& operator<<(ostream& os, my_vector<U>& mv);
	protected:
		iterator start;
		iterator finish;
		iterator end_of_storage;
		Alloc alloc;
	private:
		//���ڼ���ṩ�ĵ������Ƿ���Ч
		bool is_valid_read_iter(iterator position);
		bool is_valid_write_iter(iterator position);
		bool is_valid_rw_iter(iterator position);
		bool is_valid_range_read_iter(iterator first, iterator last);
		bool is_valid_range_write_iter(iterator first, iterator last);
		//����ԭʼ����Ŀռ�,Ϊԭ��������
		void resize_capacity();
		

	};
	template<class T, class Alloc>
	my_vector<T, Alloc>::my_vector(size_type ca)
		:alloc{}
	{
		start = alloc.allocate(ca);
		finish = start;
		end_of_storage = start + ca;
	}

	template<class T, class Alloc>
	my_vector<T, Alloc>::~my_vector()
	{
		if (start) 
		{
			alloc.deallocate(start, end_of_storage - start);
			start = nullptr;
			finish = nullptr;
			end_of_storage = nullptr;
		}
	}

	

	template<class T, class Alloc>
	void my_vector<T, Alloc>::push_back(const value_type& val)
	{
		if (full())
		{
			resize_capacity();
		}

		alloc.construct(finish, val);
		++finish;
	}

	template<class T, class Alloc>
	void my_vector<T, Alloc>::pop_back()
	{
		if (empty())
			throw std::exception{ "cont is empty" };
		--finish;
		alloc.destroy(finish);
	}

	template<class T, class Alloc>
	void my_vector<T, Alloc>::insert(iterator position, const value_type & val)
	{
		insert(position, 1, std::ref(val));
	}

	template<class T, class Alloc>
	void my_vector<T, Alloc>::insert(iterator position, size_type n, const value_type & val)
	{
		//���������Ч,���׳�std::exception
		if (!is_valid_write_iter(position))
			throw std::exception{ "invalid iter param." };
		if (n < 0)
			throw std::exception{ "invalid size_type param" };
		//���ռ�ʣ����������,������
		if (finish + n >= end_of_storage)
		{
			resize_capacity();
		}
		//����[position,last)��[position+n,last+n)
		iterator src = position;
		iterator last = finish;
		iterator des = finish + n;
		std::copy_backward(src, last, des);
		//���[position,position+n)
		std::fill_n(position, n, val);
		finish += n;
	}

	template<class T, class Alloc>
	void my_vector<T, Alloc>::erase(iterator position)
	{
		//��ɾ������[position,position+1)
		iterator last = position + 1;
		erase(position, last);

	}

	template<class T, class Alloc>
	void my_vector<T, Alloc>::erase(iterator first, iterator last)
	{
		if (!is_valid_range_read_iter(first, last))
			throw std::exception{ "invalid range iter" };

		//����[first,last)�еĶ���
		iterator cur = first;
		while (cur != last)
		{
			alloc.destroy(cur);  //�����cur->~T();
			++cur;
		}
		//[last,finish)��Ԫ���ƶ���[first,finish-last)��
		std::move(last, finish, first);
		finish -= (last - first);
	}


	template<class T, class Alloc>
	inline bool my_vector<T, Alloc>::is_valid_read_iter(iterator position)
	{
		//�ɶ�ȡ��ζ�ſ��ԶԵ�����������,��my_vector��,����[start,finish)֮��
		bool condition_1 = position - start >= 0;
		bool condition_2 = finish - position > 0;
		return condition_1 && condition_2;
	}

	template<class T, class Alloc>
	bool my_vector<T, Alloc>::is_valid_write_iter(iterator position)
	{
		//��������λ��Ӧ�ô���[start,finish]֮��,��position=finish��д��,��Ч��push_back;
		bool condition_1 = position - start >= 0;
		bool condition_2 = finish - position >= 0;
		return condition_1 && condition_2;
	}

	template<class T, class Alloc>
	bool my_vector<T, Alloc>::is_valid_rw_iter(iterator position)
	{
		//[start,finish)��[start,finish]��һ���Ӽ�,�ɶ���д��[start,finish)&&[start,finish],����[start,finish)
		return is_valid_read_iter(position);
	}

	template<class T, class Alloc>
	bool my_vector<T, Alloc>::is_valid_range_read_iter(iterator first, iterator last)
	{
		//[first,last)Ӧ�ô���[start,finish)֮��,��last-first>=0;
		bool condition_1 = is_valid_read_iter(first) && is_valid_read_iter(last);
		bool condition_2 = last - first >= 0;
		return condition_1 && condition_2;
	}

	template<class T, class Alloc>
	bool my_vector<T, Alloc>::is_valid_range_write_iter(iterator first, iterator last)
	{
		//first����[start,finish]֮��,last����[start,end_of_storage)֮��,��last-first>=0;
		bool condition_1 = is_valid_write_iter(first) && (last - start >= 0) && (end_of_storage - last > 0);
		bool condition_2 = last - first >= 0;
		return condition_1 && condition_2;
	}

	template<class T, class Alloc>
	void my_vector<T, Alloc>::resize_capacity()
	{
		//�����¿ռ������,����Ĭ��Ϊԭ��������
		size_type old_ca = end_of_storage - start;
		size_type new_ca = old_ca * 2;
		//�����¿ռ�
		iterator new_start = alloc.allocate(new_ca);
		//��ԭ���Ŀռ俽�����ݵ��¿ռ�
		std::copy(start, end_of_storage, new_start);
		//����ԭ���Ŀռ�
		alloc.deallocate(start, old_ca);
		//���ó�Ա����start,finish,end_of_storage
		start = new_start;
		finish = start + old_ca;
		end_of_storage = start + new_ca;
	}



	template<class istream, class U>
	istream & operator>>(istream & is, my_vector<U>& mv)
	{
		U temp{};
		while (is >> temp)
		{
			mv.push_back(temp);
		}
		return is;
	}

	template<class ostream, class U>
	ostream & operator<<(ostream & os, my_vector<U>& mv)
	{
		os << "{";
		auto temp = mv.finish - 1;
		for (auto p = mv.start; p != temp; ++p)
		{
			os << *p << ",";
		}
		os << *temp << "}";
		return os;

	}

}
	


#endif // !My_Vector_H_Fantasil
