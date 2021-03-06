#pragma once
#ifndef My_Node_H_Fantasil
#define My_Node_H_Fantasil
#include<utility>


namespace sli
{
	
	struct forward_node_tag {};
	struct bidirectional_node_tag {};
	struct normal_tree_node_tag {};
	struct binary_tree_node_tag :normal_tree_node_tag {};
	
	/*
		@单向节点
		@含有成员val和next域;
	*/
	template<class T>
	struct forward_node {
		//type_traits
		using value_type = T;
		using pointer = value_type * ;
		using reference = value_type & ;
		using node_category = forward_node_tag;
		using node_type = forward_node<T>;
		using node_type_pointer = node_type * ;
		using node_type_reference = node_type & ;

		//member;
		value_type val;
		node_type_pointer next;
	};

	/*
		@双向节点 
		@含有成员val,prev域和next域;
	*/
	template<class T>
	struct bi_node {
		//type_traits
		using value_type = T;
		using pointer = value_type * ;
		using reference = value_type & ;
		using node_category = bidirectional_node_tag;
		using node_type = bi_node<T>;
		using node_type_pointer = node_type * ;
		using node_type_reference = node_type & ;
		//member
		value_type val;
		node_type_pointer prev;
		node_type_pointer next;
	};

	/*
		@常用的树节点
		@含有成员val,指针域parent,left,right
	*/
	template<class T>
	struct normal_tree_node {
		//
		using value_type = T;
		using pointer = value_type * ;
		using reference = value_type & ;
		using node_category = normal_tree_node_tag;
		using node_type = normal_tree_node<T>;
		using node_type_pointer = node_type * ;
		using node_type_reference = node_type & ;
		//
		value_type val;
		node_type_pointer parent;
		node_type_pointer left;
		node_type_pointer right;
		
		normal_tree_node() :val{}, parent{ nullptr }, left{ nullptr }, right{ nullptr } {}
		normal_tree_node(value_type v):val{v},parent{nullptr},left{nullptr},right{nullptr}{}
	};

	/*
		@一般二叉树节点
		@含有值域key,val,指针域parent,left,right
	*/

	template<class K,class V>
	struct binary_tree_node {
		using value_type = V;
		using key_type = K;
		using reference = value_type & ;
		using pointer = value_type * ;
		using node_type = binary_tree_node<K, V>;
		using node_type_pointer = node_type * ;
		using node_category = binary_tree_node_tag;

		key_type key;
		value_type val;
		node_type_pointer parent;
		node_type_pointer left;
		node_type_pointer right;

		binary_tree_node() :key{}, val{}, parent{ nullptr }, left{ nullptr }, right{ nullptr } {}
		binary_tree_node(K _key, V _val) :key{ _key }, val{ _val }, parent{ nullptr }, left{ nullptr }, right{ nullptr } {}
		binary_tree_node(std::pair<K, V>& p) :key{ p.first }, val{ p.second }, parent{ nullptr }, left{ nullptr }, right{ nullptr } {}
	};


	template<class Node>
	struct node_traits {
		using value_type = typename Node::value_type;
		using pointer = typename Node::pointer;
		using reference = typename Node::reference;
		using node_category = typename Node::node_category;
		using node_type = typename Node::node_type;
		using node_type_pointer = typename Node::node_type_pointer;
		using node_type_reference = typename Node::node_type_reference;
	};
}



#endif
