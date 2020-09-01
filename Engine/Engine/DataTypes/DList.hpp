#ifndef _DATATYPES_DLIST_HPP
#define _DATATYPES_DLIST_HPP

template <typename T>
class DList {
	// DList node
	struct Node {
		T value;		// the value
		Node* next;		// next node
		Node* prev;		// prev node

						// constructor
						// Define the contructor for the node
		Node(const T& v) : value(v), next(nullptr), prev(nullptr) { }
	};

	// pointers to first and last nodes in the list
	Node* head, * tail;
	size_t sz;

public:

	// default constructor
	DList() : head(nullptr), tail(nullptr), sz(0) { }

	// destructor
	~DList() {
		clear();
	}

	// remove all nodes
	void clear() {
		// remove a node until empty
		while (!empty())
			pop_front();
	}

	// check for empty list
	bool empty() const {
		return sz == 0;
	}

	size_t size() const {
		return sz;
	}

	// return reference to first value in list
	// precondition: list is not empty
	T& front() {
		return head->value;
	}

	// return reference to last value in list
	// precondition: list is not empty
	T& back() {
		return tail->value;
	}

	// insert a value at the beginning of the list
	void push_front(const T& value) {
		// create new node with value
		Node* new_node = new Node(value);

		if (empty()) {
			// if the list is empty, new node becomes both head and tail
			head = tail = new_node;
		} else {
			// if the list is not empty, insert new node before head
			head->prev = new_node; // set the head to point back to this node
			new_node->next = head; // this node should point to the old head
			head = new_node; // this node is the new head
		}

		++sz;
	}

	// insert a value at the end of the list
	void push_back(const T& value) {
		// create new node with value
		Node* new_node = new Node(value);

		if (empty()) {
			// if the list is empty, new node becomes both head and tail
			head = tail = new_node;
		} else {
			// if the list is not empty, insert new node after tail

			tail->next = new_node; // tail points to this node
			new_node->prev = tail; // this node should point to the old tail
			tail = new_node; // this node is now the new tail
		}

		++sz;
	}

	// remove first element from the list
	// precondition: list is not empty
	void pop_front() {
		// dont delete head if there is no head
		if (empty()) return;

		// save pointer to old head
		Node* old_head = head;

		if (head == tail)
			// if list contained only one element, list is now empty
			head = tail = nullptr;
		else {
			// otherwise, reposition head pointer to next element in list
			head = head->next;
			head->prev = nullptr;
		}

		// blow away old head node
		delete old_head;

		--sz;
	}

	// remove last element from the list
	// precondition: list is not empty
	void pop_back() {
		// dont delete tail if there is no tail
		if (empty()) return;

		// save old list tail
		Node* old_tail = tail;

		if (head == tail) {
			// if list contained only one element, list is now empty
			head = tail = nullptr;
		} else {
			// otherwise, reposition tail pointer to previous element in list
			tail = tail->prev;
			tail->next = nullptr;
		}

		// blow away old tail node
		delete old_tail;

		--sz;
	}


	// Iterator class -- represents a position in the list.
	// Used to gain access to individual elements, as well as
	// insert, find, and erase elements
	class Iterator {
	protected:
		// the iterator holds a pointer to the "current" list node
		Node* node;

		// DList class needs access to private members of Iterator
		friend class DList;

		// private parameterized constructor: used by DList::begin() method
		Iterator(Node* n) : node(n) { }

	public:

		// default constructor: iterator not valid until initialized
		Iterator() : node(nullptr) { }

		// equality operator (==)
		// checks whether both iterators represent the same position
		bool operator== (const Iterator& rhs) const {
			return node == rhs.node;
		}

		// inequality operator (!=)
		// checks whether iterators represent different positions
		bool operator!= (const Iterator& rhs) const {
			return node != rhs.node;
		}

		// dereference operator (unary *)
		// returns a reference to the data value at the iterator position
		// precondition: iterator is valid
		T& operator* () const {
			return node->value;
		}

		// preincrement operator
		// advances to next node and returns itself
		// precondition: iterator is valid
		Iterator& operator++() {
			node = node->next;
			return *this;
		}

		// postincrement operator
		// advances to next node and returns old copy of itself
		// precondition: iterator is valid
		Iterator operator++(int) {
			Iterator tmp = *this;
			node = node->next;
			return tmp;
		}

		// predecrement operator
		// advances to prev node and returns itself
		// precondition: iterator is valid
		Iterator& operator--() {
			// assign the node to the previous node
			node = node->prev;
			// return this
			return *this;
		}

		// postdecrement operator
		// advances to prev node and returns old copy of itself
		// precondition: iterator is valid
		Iterator operator--(int) {
			// assign old iterator value
			Iterator tmp = *this;
			// assign the node to the previous node
			node = node->prev;
			// return the old value
			return tmp;
		}
	};

	// return iterator set to first element in list
	Iterator begin() {
		return head;
	}

	// return iterator set to last element in list (reverse begin)
	Iterator rbegin() {
		return tail;
	}

	// return iterator past last element in list
	Iterator end() {
		return nullptr;
	}

	// return iterator past first element in list (reverse end)
	Iterator rend() {
		return nullptr;
	}

	// erase the list element at pos
	// precondition: pos is a valid list iterator
	// returns: an iterator at the element immediately _after_ pos
	Iterator erase(Iterator pos) {
		// if its trys to erase nothing
		if (pos.node == nullptr) throw "the iterator was not valid";

		Node* target = pos.node;  // save target to be erased

		++pos;  // advance iterator

		if (target == head)
			pop_front();
		else if (target == tail)
			pop_back();
		else {
			// unlink target node

			// 1 <-> target <-> 2

			target->prev->next = target->next; // set 1 to point to 2
			target->next->prev = target->prev; // set 2 to point to 1

											   // delete target node
			delete target;
			--sz;
		}

		return pos; // return advanced iterator
	}

	// insert a value before pos
	// precondition: pos is a valid iterator or end()
	// returns: an iterator at the inserted value
	Iterator insert(Iterator pos, const T& value) {
		if (pos == begin()) {
			// insert new node before head
			push_front(value);
			return head;
		} else if (pos == end()) {
			push_back(value);
			return tail;
		} else {
			Node* new_node = new Node(value);

			// insert new_node between pos.node->prev and pos.node
			pos.node->prev->next = new_node;
			pos.node->prev = new_node;

			new_node->prev = pos.node->prev;
			new_node->next = pos.node;


			++sz;

			return new_node;
		}
	}

	// find a value in the list
	// returns: a valid iterator if found, end() if not found
	Iterator find(const T& value) {
		for (Node* node = head; node; node = node->next)
			if (node->value == value)
				return node;
		return end();
	}

	Iterator findL(bool(*lambda)(const T&)) {
		for (Node* node = head; node; node = node->next)
			if (lambda(node->value))
				return node;
		return end();
	}

};

#endif // !_DATATYPES_DLIST_HPP