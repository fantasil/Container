#pragma once
#include"basic_queue.h"

#ifndef Basic_Queue_Array_Fantasil_H
#define Basic_Queue_Array_Fantasil_H


namespace sli {
	namespace cont {

		template<class T>
		class basic_queue_a :basic_queue<T> {
		public:
			virtual inline bool full() const = 0;
			virtual inline int size() const = 0;
			virtual ~basic_queue_a(){}
		};
	}
}


#endif // !Basic_Queue_Array_Fantasil_H
