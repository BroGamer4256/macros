#pragma once
namespace diva {
#pragma pack(push, 8)
FUNCTION_PTR_H (void *, operatorNew, u64);
FUNCTION_PTR_H (void *, operatorDelete, void *);
template <typename T>
T *
allocate (u64 count) {
	return (T *)(operatorNew (count * sizeof (T)));
}

inline void
deallocate (void *p) {
	operatorDelete (p);
}

template <typename T>
struct listElement {
	listElement<T> *next;
	listElement<T> *previous;
	T current;

	bool operator== (const listElement<T> &rhs) { return this->current == rhs->current; }
	bool operator== (const T &rhs) { return this->current == rhs; }
};

template <typename T>
struct list {
	listElement<T> *root;
	u64 length;

	listElement<T> *begin () { return this->length ? this->root->next : this->root; }
	listElement<T> *end () { return this->root; }

	list () {
		this->length         = 0;
		this->root           = allocate<listElement<T>> (1);
		this->root->next     = this->root;
		this->root->previous = this->root;
	}

	void push_back (T value) {
		auto elem                  = allocate<listElement<T>> (1);
		elem->current              = value;
		elem->next                 = this->root;
		elem->previous             = this->root->previous;
		this->root->previous->next = elem;
		this->root->previous       = elem;
		this->length += 1;
	}
};

template <typename K, typename V>
struct mapElement {
	mapElement<K, V> *left;
	mapElement<K, V> *parent;
	mapElement<K, V> *right;
	bool isBlack;
	bool isNull;
	alignas (8) K key;
	V value;

	mapElement<K, V> *next () {
		auto ptr = this;
		if (ptr->right->isNull) {
			while (!ptr->parent->isNull && ptr == ptr->parent->right)
				ptr = ptr->parent;
			ptr = ptr->parent;
		} else {
			ptr = ptr->right;
			while (!ptr->left->isNull)
				ptr = ptr->left;
		}
		return ptr;
	}
};

template <typename K, typename V>
struct map {
	mapElement<K, V> *root;
	u64 length;

	map () {
		this->root          = allocate<mapElement<K, V>> (1);
		this->root->left    = this->root;
		this->root->parent  = this->root;
		this->root->right   = this->root;
		this->root->isBlack = true;
		this->root->isNull  = true;
		this->length        = 0;
	}

	~map () {
		for (auto it = this->begin (); it != this->end (); it = it->next ())
			deallocate (it);
		deallocate (this->root);
	}

	std::optional<V *> find (K key) {
		auto ptr = this->root->parent;
		while (!ptr->isNull)
			if (key == ptr->key) return std::optional (&ptr->value);
			else if (key > ptr->key) ptr = ptr->right;
			else if (key < ptr->key) ptr = ptr->left;
		return std::nullopt;
	}

	mapElement<K, V> *begin () { return this->length ? this->root->left : this->root; }
	mapElement<K, V> *end () { return this->root; }
	mapElement<K, V> *bottom () {
		auto ptr = this->root->parent;
		while (!ptr->right->isNull && !ptr->left->isNull)
			if (!ptr->right->isNull) ptr = ptr->right;
			else ptr = ptr->left;
		return ptr;
	}
	void insert (K key, V value) {
		auto elem     = allocate<mapElement<K, V>> (1);
		elem->isNull  = false;
		elem->isBlack = false;
		elem->key     = key;
		elem->value   = value;
		elem->left    = this->root;
		elem->right   = this->root;
		auto ptr      = this->root->parent;
		while (!ptr->isNull) {
			if (key == ptr->key) return;
			else if (key > ptr->key) {
				if (ptr->right->isNull) {
					elem->parent = ptr;
					ptr->right   = elem;
				} else ptr = ptr->right;
			} else if (key < ptr->key) {
				if (ptr->left->isNull) {
					elem->parent = ptr;
					ptr->left    = elem;
				} else ptr = ptr->left;
			}
		}
		this->length += 1;
	}
};

template <typename T>
struct vector {
	T *first;
	T *last;
	void *capacity_end;

	vector () {}
	vector (u64 n) {
		this->first        = diva::allocate<T> (n);
		this->last         = this->first;
		this->capacity_end = (void *)((u64)this->first + (n * sizeof (T)));
	}

	~vector () { deallocate (this->first); }

	std::optional<T *> at (u64 index) {
		if (index >= this->length ()) return std::nullopt;
		return std::optional (&this->first[index]);
	}

	void push_back (T value) {
		if (this->remaining_capcity () > 0) {
			this->first[this->length ()] = value;
			this->last++;
			return;
		}

		u64 new_length = this->length () + (this->length () / 2);
		T *new_first   = diva::allocate<T> (new_length);
		u64 old_length = (u64)this->last - (u64)this->first;
		memcpy (new_first, this->first, old_length);
		deallocate (this->first);

		this->first        = new_first;
		this->last         = (T *)((u64)new_first + old_length);
		this->capacity_end = (void *)((u64)new_first + (new_length * sizeof (T)));
		this->push_back (value);
	}

	u64 length () { return ((u64)this->last - (u64)this->first) / sizeof (T); }
	u64 capacity () { return ((u64)this->capacity_end - (u64)this->first) / sizeof (T); }
	u64 remaining_capcity () { return this->capacity () - this->length (); }

	T *begin () { return this->first; }
	T *end () { return this->last; }

	void allocate (u64 n) {
		if (this->length () >= n) return;
		vector<T> replacement (n);
		for (u64 i = 0; i < this->length (); i++)
			*replacement.at (i).value () = *this->at (i).value ();
		this->first        = replacement.first;
		this->last         = replacement.last;
		this->capacity_end = replacement.capacity_end;
	}
};

enum class ButtonType : i32 {
	CIRCLE      = 0,
	CROSS       = 1,
	TRIANGLE    = 2,
	SQUARE      = 3,
	SLIDE_LEFT  = 4,
	SLIDE_RIGHT = 7,
};

enum class Key : i32 {
	CONTROLLER_DPAD_UP     = 0x03,
	CONTROLLER_DPAD_DOWN   = 0x04,
	CONTROLLER_DPAD_LEFT   = 0x05,
	CONTROLLER_DPAD_RIGHT  = 0x06,
	CONTROLLER_X           = 0x07,
	CONTROLLER_Y           = 0x08,
	CONTROLLER_B           = 0x09,
	CONTROLLER_A           = 0x0A,
	CONTROLLER_L1          = 0x0B,
	CONTROLLER_R1          = 0x0C,
	CONTROLLER_L2          = 0x0D,
	CONTROLLER_R2          = 0x0E,
	CONTROLLER_L3          = 0x0F,
	CONTROLLER_R3          = 0x10,
	CONTROLLER_TOUCHPAD    = 0x11,
	CONTROLLER_LEFT_UP     = 0x98,
	CONTROLLER_LEFT_DOWN   = 0x99,
	CONTROLLER_LEFT_LEFT   = 0x9A,
	CONTROLLER_LEFT_RIGHT  = 0x9B,
	CONTROLLER_RIGHT_UP    = 0x9C,
	CONTROLLER_RIGHT_DOWN  = 0x9D,
	CONTROLLER_RIGHT_LEFT  = 0x9E,
	CONTROLLER_RIGHT_RIGHT = 0x9F,

	KEYBOARD_1           = 0x1D,
	KEYBOARD_2           = 0x1E,
	KEYBOARD_3           = 0x1F,
	KEYBOARD_4           = 0x20,
	KEYBOARD_5           = 0x21,
	KEYBOARD_6           = 0x22,
	KEYBOARD_7           = 0x23,
	KEYBOARD_8           = 0x24,
	KEYBOARD_9           = 0x25,
	KEYBOARD_0           = 0x26,
	KEYBOARD_MINUS       = 0x27,
	KEYBOARD_BACKSPACE   = 0x29,
	KEYBOARD_TAB         = 0x2A,
	KEYBOARD_Q           = 0x2B,
	KEYBOARD_W           = 0x2C,
	KEYBOARD_E           = 0x2D,
	KEYBOARD_R           = 0x2E,
	KEYBOARD_T           = 0x2F,
	KEYBOARD_Y           = 0x30,
	KEYBOARD_U           = 0x31,
	KEYBOARD_I           = 0x32,
	KEYBOARD_O           = 0x33,
	KEYBOARD_P           = 0x34,
	KEYBOARD_LSQB        = 0x35,
	KEYBOARD_RSQB        = 0x36,
	KEYBOARD_A           = 0x39,
	KEYBOARD_S           = 0x3A,
	KEYBOARD_D           = 0x3B,
	KEYBOARD_F           = 0x3C,
	KEYBOARD_G           = 0x3D,
	KEYBOARD_H           = 0x3E,
	KEYBOARD_J           = 0x3F,
	KEYBOARD_K           = 0x40,
	KEYBOARD_L           = 0x41,
	KEYBOARD_LSHIFT      = 0x45,
	KEYBOARD_Z           = 0x47,
	KEYBOARD_X           = 0x48,
	KEYBOARD_C           = 0x49,
	KEYBOARD_V           = 0x4A,
	KEYBOARD_B           = 0x4B,
	KEYBOARD_N           = 0x4C,
	KEYBOARD_M           = 0x4D,
	KEYBOARD_COMMA       = 0x4E,
	KEYBOARD_PERIOD      = 0x4F,
	KEYBOARD_SLASH       = 0x50,
	KEYBOARD_RSHIFT      = 0x51,
	KEYBOARD_SPACE       = 0x54,
	KEYBOARD_ARROW_UP    = 0x7B,
	KEYBOARD_ARROW_DOWN  = 0x7C,
	KEYBOARD_ARROW_LEFT  = 0x7D,
	KEYBOARD_ARROW_RIGHT = 0x7E,
};

#pragma pack(pop)

extern vector<i32> *buttonIds;
extern map<i32, list<Key>> **buttonKeys;
} // namespace diva
