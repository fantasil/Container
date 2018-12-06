#pragma once
#ifndef Binary_Search_Tree_H_Fantasil
#define Binary_Search_Tree_H_Fantasil
#include"my_node.h"
#include<iterator>

namespace sli
{
	/*
		@�����������ĵ�����,��һ��˫�������
		֧��++,--,*,->,==,!=����
	*/
	template<class K,class V>
	class binary_search_tree_iter {
	public:
		using key_type = K;
		using value_type = V;
		using pointer = V * ;
		using reference = V & ;
		using const_pointer = const pointer;
		using const_reference = const reference;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;
		using node_type = binary_tree_node<K, V>;
		using node_type_pointer = node_type * ;
		using node_type_reference = node_type & ;
		using self = binary_search_tree_iter<K,V>;
	public:
		node_type_pointer cur;
	public:
		self & operator++()
		{
			if (!cur->right)
			{	//����ڵ���Һ��Ӳ�����,��ô����Ѱ�ҽ�С�ڵ�
				if (!cur->parent)
				{
					//�ýڵ��Ǹ��ڵ���Ϊ���һ���ڵ�
					cur = nullptr;
				}
				else {
					node_type_pointer parent = cur->parent;
					while (parent&&parent->right == cur)
					{
						cur = parent;
						parent = cur->parent;
					}
					if (parent)
						cur = parent;
					else
						cur = nullptr;
				}
			}
			else {
				//����Һ��Ӵ���,��ô��ȡ���Һ��Ӳ�һֱ������,ֱ�����Ӳ�����.
				cur = cur->right;
				while (cur->left)
				{
					cur = cur->left;
				}
			}
			return *this;
		}
		self& operator--() {
			if (!cur->left)
			{	//����������
				if (!cur->parent)
					cur = nullptr; //�ýڵ��Ǹ��ڵ���Ϊ��С�ڵ�
				else {
					node_type_pointer parent = cur->parent;
					while (parent&&parent->left == cur)
					{
						cur = parent;
						parent = cur->parent;
					}

					if (parent)
						cur = parent;
					else
						cur = nullptr; //�ýڵ�����С�ڵ�.
				}
			}
			else {
				cur = cur->left;
				while (cur->right)
				{
					cur = cur->right;
				}
			}
			return *this;
		}
		self& operator++(int) { auto temp = *this; ++*this; return temp; }
		self& operator--(int) { auto temp = *this; --temp; return temp; }
		inline reference operator*() { return cur->val; }
		inline node_type_pointer operator->() { return cur; }
		inline bool operator==(const self& other) { return cur == other.cur; }
		inline bool operator==(node_type_pointer node) { return cur == node; }
		inline bool operator!=(const self& other) { return !operator==(other); }
		inline bool operator!=(node_type_pointer node) { return cur != node; }
	public:
		binary_search_tree_iter() :cur{ nullptr } {}
		binary_search_tree_iter(const self& other) { cur = other.cur; }
		binary_search_tree_iter(node_type_pointer node) :cur{ node } {}
		self& operator=(const self& other) { cur = other.cur; return *this; }
		self& operator=(node_type_pointer node) { cur = node; return *this; }
	};

	/*
		@����������
		@min:����ָ����Сkey�ĵ�����
		@max:����ָ�����Key�ĵ�����
		//�������������������ܽ��б�����
		@insert:����һ������<K,V>,����key������,�ݲ�֧���Զ��������ʽ
			1:���뵽����ʱ,����Ϊroot;
			2:�Ӹ��ڵ㿪ʼ����,���key<�ڵ�key,���󲽽�,�������Ҳ���,ֱ���ҵ��Լ���˫�׽ڵ�,����key�����������ڵ�.
		@erase:����һ��keyֵ,�����Ƿ���ڸýڵ�,������,����������������ɾ��:
			1:����������,ѡ���С�ڵ�(--node,�������������Һ���)���ߴ˴�ڵ�(++node,����������������)ȡ������
			2:����һ������,������������,ѡ���С�ڵ�(--node)ȡ������,����ʹ�ôδ�ڵ�(++node)ȡ������
			3:������,��ɾ������.
		@operator<<
			���ֵ,������
		@find:
			����key,���ص�����,Ĭ�ϵ�������������Ϊkey�Ľڵ�.
		@create_node:
			���ݸ�����k,v����binary_tree_node�ڵ�
		@destroy_node:
			���������սڵ��ڴ�
		@post_order_erase:
			ʹ�ú������ɾ���ڵ�
		*/
	template<class K,class V,class Node=binary_tree_node<K,V>,class Alloc=std::allocator<Node>>
	class binary_search_tree {
	public:
		using key_type = K;
		using value_type = V;
		using pointer = V * ;
		using reference = V & ;
		using const_reference = const reference;
		using iterator = binary_search_tree_iter<K,V>;
		using node_type = binary_tree_node<K,V>;
		using node_type_pointer = node_type * ;
		using size_type = size_t;
	private:
		iterator root;
		size_type m_size;
		Alloc alloc;
	public:
		inline bool empty() { return m_size == 0; }
		inline size_type size() { return m_size; }
		iterator min() 
		{
			if (empty())
				throw std::exception{ "cont is empty." };
			iterator cur = root;
			iterator par{};
			while (cur.cur)
			{
				par = cur;
				cur = cur->left;
			}
			return par;
		}
		iterator max()
		{
			if (empty())
				throw std::exception{ "cont is empty." };

			iterator cur = root;
			iterator par{};
			while (cur.cur)
			{
				par = cur;
				cur = cur->right;
			}
			return par;
		}
		void insert(key_type key,value_type val);
		void erase(key_type key);
		template<class ostream,class T,class U>
		friend ostream& operator<<(ostream& os, binary_search_tree<T, U>& bst);
	private:
		iterator find(key_type key);
		node_type_pointer create_node(key_type key, value_type val)
		{
			std::pair<key_type, value_type> temp{ key,val };
			node_type_pointer new_node=alloc.allocate(1);
			alloc.construct(new_node, temp);
			return new_node;
		}
		void destroy_node(iterator node)
		{
			//�������������������һ��տռ�
			alloc.destroy(node.cur);
			alloc.deallocate(node.cur, 1);
		}
		void post_order_erase(iterator node) {
			if (node.cur)
			{
				post_order_erase(node->left);
				post_order_erase(node->right);
				destroy_node(node);
			}
		}
	public:
		binary_search_tree() :root{ nullptr }, m_size{ 0 }, alloc{} {}
		~binary_search_tree()
		{
			post_order_erase(root);
			m_size = 0;
		}
	};
	


	template<class K, class V, class Node, class Alloc>
	void binary_search_tree<K, V, Node, Alloc>::insert(key_type key, value_type val)
	{
		node_type_pointer new_node = create_node(key, val);

		if (empty())
			root.cur= new_node;
		else {

			iterator cur = root;
			iterator par{};
			while (cur.cur)
			{
				par = cur;
				if (key < cur->key)
					cur = cur->left;
				else if (key > cur->key)
					cur = cur->right;
				else
				{	//key����ͬ,�޸�value�򼴿�
					cur->val = val;
					return;
				}
			}
			//��ʱpar��new_node��˫�׽ڵ�
			if (key < par->key)
				par->left = new_node;
			else
				par->right = new_node;

			new_node->parent = par.cur;
		}
		++m_size;
	}

	template<class K, class V, class Node, class Alloc>
	void binary_search_tree<K, V, Node, Alloc>::erase(key_type key)
	{
		iterator q = find(key);

		if (!q.cur)
			return;

		iterator node_to_del = q;
		iterator par = q->parent;
		if (q->left&&q->right)
		{	//ӵ����������
			//�����ȡʹ���������������ҽڵ��������ǰɾ���ڵ�
			//--����֮��,q�Ѿ������ı�,node_to_del����ָ��ԭ����Ҫ��ɾ���Ľڵ�
			iterator prev = --q;
			//����ڵ���˫�׽ڵ������ϵ
			iterator old_par = prev->parent;
			if (old_par->left == prev)
				old_par->left = nullptr;
			else
				old_par->right = nullptr;
			//����ڵ�����˫�׽�����ϵ
			if (par.cur)
			{
				if (par->left == node_to_del)
					par->left = prev;
				else
					par->right = prev;
			}
			prev->parent = par;
			cp->right = node_to_del->right;
		}
		else if (q->left||q->right)
		{	//ֻ����һ������
			//���������ھ�ʹ���������Һ��ӵ������ҽڵ�,������������ʹ�����������ӵ�������ڵ�
			if (q->left)
				--q;
			else
				++q;
			//���뼸����˫�׵Ĺ�ϵ
			iterator old_par = q->parent;
			if (old_par->left == q)
				old_par->left = nullptr;
			else
				old_par->right = nullptr;
			//����˫�׽�����ϵ
			if (par.cur)
			{
				if (p->left == node_to_del)
					p->left = q;
				else
					p->right = q;

				q->parent = par;
			}
			else {
				root = q;
			}
		}
		else {	//����������������,ɾ��Ҷ�ڵ�
			if (par.cur)
			{
				if (par.cur->left == q)
					par.cur->left = nullptr;
				else
					p.cur->right == nullptr;
			}
			else {
				//��Ҷ�ڵ��Ǹ��ڵ�
				root.cur = nullptr;
			}
		}

		destroy_node(node_to_del);
		--m_size;
	}

	template<class K, class V, class Node, class Alloc>
	binary_search_tree_iter<K,V> binary_search_tree<K, V, Node, Alloc>::find(key_type key)
	{
		//
		iterator cur = root;
		while (cur.cur)
		{
			if (key < cur->key)
				cur = cur->left;
			else if (key > cur->key)
				cur = cur->right;
			else
				return cur;
		}
		return iterator{ nullptr };
	}

	template<class ostream, class T,class U >
	ostream & operator<<(ostream & os, binary_search_tree<T, U>& bst)
	{
		os << "{";
		auto min = bst.min();
		auto max = bst.max();
		for (auto p = min; p != max; ++p)
		{
			os << *p << " ";
		}
		os << *max << "}\n";
		return os;
	}

}


#endif // Binary_Search_Tree_H_Fantasil
