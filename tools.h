#pragma once
#include<utility>
#include<iostream>
#ifndef Tools_Fantasil_H
#define Tools_Fantasil_H



namespace sli {
	namespace tool {



		/*
			根据facotr伸缩数组a,且从指定位置开始拷贝,直到容量满足
			拷贝的顺序是[pos,a._capacity)[0,pos);
		*/
		template<class T>
		void array_resize(T*& a, int ca_a, double factor, int pos)
		{
			int new_ca = static_cast<int>(factor*ca_a);
			T* b = new T[new_ca];
			int mv_len = new_ca > ca_a ? ca_a : new_ca;
			int seq_1 = ca_a - pos;
			if (mv_len < seq_1)
			{
				array_move(a, ca_a, b, new_ca, pos, pos + mv_len, 0);	//copy a[pos,pos+mv_len) to b[0,mv_len);
			}
			else {
				array_move(a, ca_a, b, new_ca, pos, pos + seq_1, 0);	//copy a[pos,pos+seq_1) to b[0,seq_1);
				int left = mv_len - seq_1;
				if (left < pos)
				{
					array_move(a, ca_a, b, new_ca, 0, left, seq_1);		//copy a[0,left) to b[seq_1,seq_1+left)
				}
				else {
					array_move(a, ca_a, b, new_ca, 0, pos, seq_1);		//copy a[0,pos) to b[seq_1,seq_1+pos)
				}
			}
			delete[] a;
			a = b;
			b = nullptr;
			
		}


		/*
			将数组a中的[ab,ae)序列 移动到数组b中的[bb,be)处
		*/
		template<class T>
		int array_move(T*& a,int ca_a,T*& b,int ca_b,int begin_a,int end_a,int begin_b)
		{
			
			bool invalid_seq = begin_a<0||begin_b<0||end_a<0||end_a>ca_a||(end_a-begin_a+begin_b)>ca_b;
			if (invalid_seq)
			{
				//两组序列必须同时有效
				return -1;
			}
			for (int i = begin_a,pos=begin_b; i < end_a; ++i,++pos)
			{
				b[pos] = std::move(a[i]);
			}
			return 0;
		}
	}
}


#endif