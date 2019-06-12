#ifndef item_h
#define item_h

#include <iostream>
#include <string>
#include <string.h>
#include <mutex>
#include "utils.h"

using namespace std;

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
	Item(T item);
	~Item();

private:
	T item;
public:
	mutex mtx;

public:
	T get();					// use mutex
	void set(T t);				// use mutex


	// consider if keeping them or not...
	T get_noMutex();			// NO mutex used
	void set_noMutex(T t);		// NO mutex used

	Item<T> & operator=(Item<T>& item_to_copy);


};






/*
 * Items
 */

template <class T>
class Items
{
public:
	Items();
	Items(string name);
	Items(int max_len);
	Items(string name, int max_len);
	~Items();

private:
	vector<T> items;
	mutex mtx;
	int MAX_LEN = 10;				// maximum length of the vector
	string name;				// identifier

public:
	int get_MAX_LEN();
	void set_MAX_LEN(int i);

	string get_name();
	void set_name(string s);


	vector<T> get_items();					// use mutex
	void add_item(T t);						// use mutex
	void add_unique_item(T t);				// use mutex (only add unique items)

	void copy(vector<T> new_items);			// use mutex
	void clear();							// use mutex

	vector<T> get_items_noMutex();			// NO mutex used
	void add_item_noMutex(T t);				// NO mutex used

	

	T get_last_item();
	T get_last_item_noMutex();

	bool remove_item(T t);					// use mutex

	void print_items();
	string to_string_cs(string delimiter);	// cs = coma separated (delimiter separeted)
	
	Items<T> & operator=(Items<T>& items_to_copy);
	
};




/*
 * The methods from the previous classes are
 * coded in the following file included here
 */
#include "item.tpp"

#endif