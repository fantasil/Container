#pragma once
#include"basic_stack.h"

#ifndef Basic_Stack_Array_Fantasil_H
#define Basic_Stack_Array_Fantasil_H

namespace sli {
	namespace cont {
		template<class T>
		class basic_stack_a :basic_stack<T> {
		public:
			virtual inline bool full() const = 0;
			virtual inline int size() const = 0;
			virtual ~basic_stack_a() {};
		};
	}
}




#endif
