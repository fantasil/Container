#pragma once

#ifndef Node_Fantasil_H
#define Node_Fantasil_H

namespace sli {
	namespace node {

		template<class T>
		struct bi_node {
			T val;
			bi_node<T>* _prev;
			bi_node<T>* _next;
		};
	}
 }

#endif