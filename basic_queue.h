#pragma once

#ifndef Basic_Queue_Fantasil_H
#define Basic_Queue_Fantasil_H



namespace sli {
	namespace cont {

		template<class T>
		class basic_queue {
		public:
			virtual inline bool empty() const = 0;
			virtual int enqueue(const T& val) = 0;
			virtual int dequeue(T& val) = 0;
			virtual ~basic_queue() {}
		};
	}
}



#endif // !Basic_Queue_Fantasil_H
