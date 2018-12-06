#pragma once
#ifndef Binary_Search_Tree_H_Fantasil
#define Binary_Search_Tree_H_Fantasil
#include"my_node.h"
#include<iterator>

namespace sli
{
	/*
		@二叉搜索树的迭代器,是一个双向迭代器
		支持++,--,*,->,==,!=操作
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
			{	//如果节点的右孩子不存在,那么上溯寻找较小节点
				if (!cur->parent)
				{
					//该节点是根节点且为最后一个节点
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
				//如果右孩子存在,那么就取代右孩子并一直向左走,直到左孩子不存在.
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
			{	//向上找左孩子
				if (!cur->parent)
					cur = nullptr; //该节点是根节点且为最小节点
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
						cur = nullptr; //该节点是最小节点.
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
		@二叉搜索树
		@min:返回指向最小key的迭代器
		@max:返回指向最大Key的迭代器
		//有了着两个迭代器就能进行遍历了
		@insert:插入一对数据<K,V>,根据key域排序,暂不支持自定义的排序方式
			1:插入到空树时,即成为root;
			2:从根节点开始下溯,如果key<节点key,向左步进,否则向右步进,直到找到自己的双亲节点,根据key域连接两个节点.
		@erase:给定一个key值,查找是否存在该节点,若存在,则根据子树情况进行删除:
			1:有两棵子树,选择次小节点(--node,左子树的最深右孩子)或者此大节点(++node,右子树的最深左孩子)取代自身
			2:仅有一棵子树,若左子树存在,选择次小节点(--node)取代自身,否在使用次大节点(++node)取代自身
			3:无子树,则删除自身.
		@operator<<
			输出值,测试用
		@find:
			给定key,返回迭代器,默认迭代器代表不存在为key的节点.
		@create_node:
			根据给定的k,v创建binary_tree_node节点
		@destroy_node:
			析构并回收节点内存
		@post_order_erase:
			使用后序遍历删除节点
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
			//仅仅调用析构函数并且回收空间
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
				{	//key域相同,修改value域即可
					cur->val = val;
					return;
				}
			}
			//此时par是new_node的双亲节点
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
		{	//拥有两棵子树
			//这里采取使用左子树的最深右节点来替代当前删除节点
			//--操作之后,q已经发生改变,node_to_del才是指向原来需要被删除的节点
			iterator prev = --q;
			//替代节点与双亲节点脱离关系
			iterator old_par = prev->parent;
			if (old_par->left == prev)
				old_par->left = nullptr;
			else
				old_par->right = nullptr;
			//替代节点与新双亲建立关系
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
		{	//只存在一棵子树
			//左子树存在就使用左子树右孩子的最深右节点,右子树存在则使用右子树左孩子的最深左节点
			if (q->left)
				--q;
			else
				++q;
			//剥离几点与双亲的关系
			iterator old_par = q->parent;
			if (old_par->left == q)
				old_par->left = nullptr;
			else
				old_par->right = nullptr;
			//与新双亲建立关系
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
		else {	//左右子树都不存在,删除叶节点
			if (par.cur)
			{
				if (par.cur->left == q)
					par.cur->left = nullptr;
				else
					p.cur->right == nullptr;
			}
			else {
				//此叶节点是根节点
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
