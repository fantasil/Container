#pragma once

#ifndef Basic_Stack_Fantasil_H
#define Basci_Stack_Fantasil_H


template<class T>
class basic_stack {
public:
	virtual inline bool empty() const = 0;
	virtual int push(const T& val) = 0;
	virtual int top(T& val) = 0;
	virtual int pop(T& val) = 0;
	virtual ~basic_stack() {}
};



#endif // !Basic_Stack_Fantasil_H
