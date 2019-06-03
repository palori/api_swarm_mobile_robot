

/*
 * Item methods
 */
template<class T>
Item<T>::Item(){}

template<class T>
Item<T>::Item(T item){set(item);}

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

template<class T>
Item<T> & Item<T>::operator=(Item<T>& item_to_copy){
	set(item_to_copy.get());//get_noMutex();
	return *this;
}




//***************************************************************
//***************************************************************


/*
 * Items methods
 */
template<class T>
Items<T>::Items(){}

template<class T>
Items<T>::Items(string name){set_name(name);}

template<class T>
Items<T>::Items(int max_len){set_MAX_LEN(max_len);}

template<class T>
Items<T>::Items(string name, int max_len){
	set_name(name);
	set_MAX_LEN(max_len);
}

template<class T>
Items<T>::~Items(){}




template<class T>
int Items<T>::get_MAX_LEN(){return MAX_LEN;}

template<class T>
void Items<T>::set_MAX_LEN(int i){MAX_LEN = i;}




template<class T>
string Items<T>::get_name(){return name;}

template<class T>
void Items<T>::set_name(string s){name = s;}








template<class T>
vector<T> Items<T>::get_items(){
	mtx.lock();
	vector<T> items_m = items;
	mtx.unlock();
	return items_m;
}

template<class T>
void Items<T>::add_item(T t){
	mtx.lock();
	add_item_noMutex(t);
	mtx.unlock();
}






template<class T>
vector<T> Items<T>::get_items_noMutex(){return items;}

template<class T>
void Items<T>::add_item_noMutex(T t){ 	// modification of 'utils/add2vector()'
	cout << "" << endl;
	if (items.size() >= get_MAX_LEN()){
		items.erase (items.begin());    	// Only when it's full, delete the first position
	}
	items.push_back(t);
}










template<class T>
T Items<T>::get_last_item(){
	return get_items().back();
}

template <class T>
T Items<T>::get_last_item_noMutex(){
	return get_items_noMutex().back();
}



/*
template <class T>
Items<T> Items<T>::operator=(Items<T> items_to_copy){
	int max_len = items_to_copy.get_MAX_LEN();

}*/





template <class T>
void Items<T>::print_items(){
	cout << "\nVector '" + get_name() + "' [" + to_string(get_MAX_LEN()) + "]:" << endl;
	
	vector<T> v = get_items();
	for (int i = 0; i < v.size(); i++){
		T t = v.at(i);
		cout << "  - " << t << endl;
	}
}


template<class T>
Items<T> & Items<T>::operator=(Items<T>& items_to_copy){
	set_name(items_to_copy.get_name());
	set_MAX_LEN(items_to_copy.get_MAX_LEN());
	this->items = items_to_copy.get_items();
	return *this;
}