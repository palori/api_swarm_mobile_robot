
//#include <iostream>
//#include <mutex>


/*
 * Item methods
 */
template<class T>
Item<T>::Item(){}

template<class T>
Item<T>::~Item(){}

template<class T>
T Item<T>::get(){
	mtx.lock();
	T item_m = item;
	mtx.unlock();
	return item_m;
}

template<class T>
void Item<T>::set(T t){
	mtx.lock();
	item = t;
	mtx.unlock();
}

template<class T>
T Item<T>::get_noMutex(){return item;}

template<class T>
void Item<T>::set_noMutex(T t){item = t;}







/*
 * Items methods
 */
template<class T>
Item<T>::Items(){}

template<class T>
Item<T>::~Items(){}

template<class T>
T Item<T>::get(){
	mtx.lock();
	vector<T> items_m = items;
	mtx.unlock();
	return items_m;
}

template<class T>
void Item<T>::set(T t){
	mtx.lock();
	items = add2vector(items, t, get_MAX_LEN());
	mtx.unlock();
}

template<class T>
T Item<T>::get_noMutex(){return items;}

template<class T>
void Item<T>::set_noMutex(T t){items = add2vector(items, t, get_MAX_LEN());}

