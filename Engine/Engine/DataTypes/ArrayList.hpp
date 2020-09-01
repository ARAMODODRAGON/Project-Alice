#ifndef _DATATYPES_ARRAY_LIST_HPP
#define _DATATYPES_ARRAY_LIST_HPP
#include <initializer_list>
typedef unsigned int uint;

template<class T>
class ArrayList {

	T* arr;
	uint sz;
	uint cap;

	uint block_sz;

public:

	ArrayList(uint block_size = 1)
		: arr(0), sz(0), cap(block_size), block_sz(block_size) {
		// make array
		arr = new T[cap];

	}

	ArrayList(std::initializer_list<T> list)
		: arr(nullptr), sz(list.size()), cap(sz + 1), block_sz(1) {
		// create array
		arr = new T[cap];

		uint i = 0;
		// fill array
		for (T item : list) {
			arr[i] = item;
			++i;
		}
	}

	template<uint S>
	ArrayList(const T(&array_)[S])
		: arr(nullptr), sz(S), cap(sz + 1), block_sz(1) {
		// create array
		arr = new T[cap];

		// copy values
		for (uint i = 0; i < S; ++i) {
			arr[i] = array_[i];
		}
	}

	ArrayList(const ArrayList& other)
		: arr(nullptr), sz(other.sz), cap(other.cap), block_sz(other.block_sz) {
		// make array
		arr = new T[cap];

		// copy over values
		for (uint i = 0; i < sz; ++i)
			arr[i] = other.arr[i];

	}

	ArrayList& operator=(const ArrayList& other) {
		// destroy old array
		if (arr) delete[] arr;
		arr = nullptr;

		// copy values
		sz = other.sz;
		cap = other.cap;
		block_sz = other.block_sz;

		// make array
		arr = new T[cap];

		// copy over values
		for (uint i = 0; i < sz; ++i)
			arr[i] = other.arr[i];

		return *this;
	}

	ArrayList(ArrayList&& other)
		: arr(other.arr), sz(other.sz), cap(other.cap), block_sz(other.block_sz) {
		// erase others refernce to the array 
		other.arr = nullptr;
		other.sz = 0;
		other.cap = 0;
		other.block_sz = 0;
	}

	ArrayList& operator=(ArrayList&& other) {
		// delete current array
		if (arr) delete[] arr;
		arr = nullptr;

		// copy array over
		arr = other.arr;
		sz = other.sz;
		cap = other.cap;
		block_sz = other.block_sz;

		// erase others refernce to the array 
		other.arr = nullptr;
		other.sz = 0;
		other.cap = 0;
		other.block_sz = 0;

		return *this;
	}

	~ArrayList() {
		// delete the array
		if (arr) delete[] arr;
		arr = nullptr;
		sz = cap = 0;
		block_sz = 1;
	}

	uint size() const { return sz; }

	uint capacity() const { return cap; }

	uint block_size() const { return block_sz; }


	void set_block_size(uint block_size_) { block_sz = (block_size_ != 0 ? block_size_ : 1); }

	void trim() {
		if (cap > sz) {

			T* old_arr = arr;
			arr = new T[cap = sz];

			// copy values
			for (uint i = 0; i < sz; ++i)
				arr[i] = old_arr[i];

			// delete old array
			delete[] old_arr;
		}
	}

	void clear() { sz = 0; }

	void clear_and_trim() {
		clear(); trim();
	}

	void reserve(uint sz_) {
		if (sz_ > cap) {

			T* old_arr = arr;
			arr = new T[cap = sz_];

			// copy values
			for (uint i = 0; i < sz; ++i)
				arr[i] = old_arr[i];

			// delete old array
			delete[] old_arr;
		}
	}


	T& operator[](uint index) {
		return arr[index];
	}

	const T& operator[](uint index) const {
		return arr[index];
	}


	void push_back(T value) {
		// add one to the back
		if (sz != cap)
			arr[sz++] = value;
		// cant add one so increase size and then add one
		else {

			T* old_arr = arr;
			arr = new T[cap += block_sz];

			// copy values
			for (uint i = 0; i < sz; ++i)
				arr[i] = old_arr[i];

			// delete old array
			delete[] old_arr;

			arr[sz++] = value;
		}
	}

	void pop_back() {
		// we only really need to set it one less, deletion isnt nessisary
		if (sz > 0) --sz;
	}


	class Iterator {
		friend ArrayList;

		uint pos;
		ArrayList* list;

	public:

		Iterator(uint pos_, ArrayList* list_)
			: pos(pos_), list(list_) { }

		Iterator& operator++() {
			pos++;
			return *this;
		}

		Iterator operator++(int) {
			return Iterator(pos++, list);
		}

		Iterator& operator--() {
			pos--;
			return *this;
		}

		Iterator operator--(int) {
			return Iterator(pos--, list);
		}

		bool operator==(const Iterator& other) const {
			return pos == other.pos;
		}

		bool operator!=(const Iterator& other) const {
			return pos != other.pos;
		}

		T& operator*() {
			return (*list)[pos];
		}

		T* operator->() {
			return &(*list)[pos];
		}

		bool is_null() const {
			if (pos < 0 || pos >= list->size())
				return true;
			return false;
		}

	};


	Iterator begin() {
		return Iterator(0, this);
	}

	Iterator end() {
		return Iterator(sz, this);
	}


	void erase(Iterator position) {

		// start iterating and moving everything
		Iterator lastpos = position++;
		while (position != end()) {
			// copy
			(*lastpos) = (*position);

			// next
			lastpos = position++;
		}

		// decrease the size
		--sz;
	}

	void insert(Iterator position, T value) {
		if (position.is_null()) return;

		// increase size
		sz += 1;

		// the array must be resized
		if (sz > cap) {

			T* old_array = arr;
			arr = new T[cap += block_sz];

			uint i = 0;
			uint old_i = 0;
			for (; i < sz; ++i) {
				if (i == position.pos) {
					arr[i] = value;
				} else {
					arr[i] = old_array[old_i++];
				}
			}

			delete[] old_array;
		}
		// the array does not need to be resized
		else {

			// move back the values
			for (uint i = cap - 1; i > position.pos; --i)
				arr[i] = arr[i - 1];

			// add the new value
			(*position) = value;

		}
	}

	Iterator find(const T value) {
		auto iter = begin();
		for (; iter != end(); ++iter)
			if (*iter == value)
				return iter;
		return iter;
	}

};

#endif // !_DATATYPES_ARRAY_LIST_HPP