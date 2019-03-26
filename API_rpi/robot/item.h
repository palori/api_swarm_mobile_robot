#ifndef item_h
#define item_h

#include <iostream>
#include <mutex>
#include "utils.h"

/*
 * This file contains the following template
 * classes:
 *		- Item
 *		- Items
 */





/*
 * Item
 */
template <class T>
class Item
{
public:
	Item();
	~Item();

private:
	vector<T> items;
	mutex mtx;
	int MAX_LEN;				// maximum length of the vector

public:
	T get();					// use mutex
	void set(T t);				// use mutex

	T get_noMutex();			// NO mutex used
	void set_noMutex(T t);		// NO mutex used

	int get_MAX_LEN();
	void set_MAX_LEN();
};






/*
 * Items
 */

template <class T>
class Items
{
public:
	Items();
	~Items();

private:
	vector<item<T>> items;
	mutex mtx;

public:
	T get();					// use mutex
	void set(T t);				// use mutex

	T get_noMutex();			// NO mutex used
	void set_noMutex(T t);		// NO mutex used
};




/*
 * The methods from the previous classes are
 * coded in the following file included here
 */
#include "item.tpp"

#endif