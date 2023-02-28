#pragma once

/*
* | ext.h - intellectual property of Rohan Shashank (jammy3662)
* |
* | A header-only software library called Ext, featuring
* | a series of extensions for c and c++ functionality.
* | Includes doubly-linked-list, array, string, and other
* | wrappers for common constructs, among other things.
* |
* | EXT IS LICENSED AS PRIVATE SOFTWARE.
* | Copying, modifying, or redistributing this software is NOT PERMISSIBLE.
* | This licensing is SUBJECT TO CHANGE WITHOUT NOTICE.
* |
* | Ext is planned to eventually be relicensed under a more permissive open-source license,
* | such as the GNU LGPL, or something similar. For now, it remains private.
*/

// TODO: add preprocessor options to compile c-friendly code
// (currently only valid in c++)

#ifndef EXT_H
#define EXT_H

#include <stdlib.h>
#include <string.h>

#include <math.h>

#define uint   unsigned int

#define int8   char
#define int16  short
#define int32  int
#define int64  long

#define uint8  unsigned char
#define uint16 unsigned short
#define uint32 unsigned int
#define uint64 unsigned long

#define i8   char
#define i16  short
#define i32  int
#define i64  long

#define u8  unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long

/* --- ASCII CHARACTERS --- */
#define VT 11  // vertical tab
#define FF 12  // new page
#define CR 13  // carriage return
#define DL 127 // backspace/delete

#define PI M_PI

// iterate over range _MIN_ to _MAX_
#define fromto(ID, MIN, MAX) for (int ID = MIN; ID < MAX; ID++)

// iterate over each element in stack array
#define forls(ID, LST, BLOCK)\
{\
	__typeof__(*LST) ID;\
	for (int i = 0; i < (sizeof(LST)/sizeof(*LST)); i++)\
	{\
		ID = LST[i];\
		{ BLOCK }\
	}\
}

// iterate over each element in Ext array
#define forin(ID, LST, BLOCK)\
{\
	__typeof__(*LST.data) ID;\
	for (int i = 0; i < LST.size; i++)\
	{\
		ID = LST[i];\
		{ BLOCK }\
	}\
}

// get rid of code without commenting it out
// (for syntax highlighting)
#define __cancel(BLOCK)

#ifdef ZEXT_IO
	#include <stdio.h>
	#ifdef ZEXT_SILENCE_PRINTF
		#define printf(...)
	#endif
	#ifdef ZEXT_CONTROL_PRINTF
		#define printf(...) ZALGO_EXT::conditional_printf(__VA_ARGS__)
	#endif
#endif


namespace ZALGO_EXT
{
	// Set i to 0 to silence printf
	int (*conditional_printf) (const char*, ...) = printf;
	int silent_printf(const char* _, ...) { return 0; }
	void printf_verbosity(int verbose)
	{
		if (!verbose) conditional_printf = silent_printf;
		else conditional_printf = printf;
	}
};

// type based allocation (using malloc)
#define alloc(TYPE) (TYPE*)malloc(sizeof(TYPE))
#define talloc(TYPE, SIZE) (TYPE*)malloc(SIZE * sizeof(TYPE))
#define tcalloc(TYPE, SIZE) (TYPE*)calloc(SIZE, sizeof(TYPE))
#define trealloc(PTR, SIZE) (__typeof__(PTR))realloc(PTR, SIZE * sizeof(PTR))

// returns whether _str2_ begins with the entirety of _str1_
bool strmatch(char* str1, char* str2)
{
	while (*str1 != 0)
	{
		if (*str2 == 0) return false;
		if (*str1 != *str2) return false;
		str1++; str2++;
	}
	return true;
}

// returns whether _str1_ exactly matches _str2_
bool streq(char* str1, char* str2)
{
	while (*str1 != 0 and *str2 != 0)
	{
		if (!(*str1 == *str2)) return 0;
		str1++; str2++;
	}
	return (*str1 == *str2);
}

// returns true if _str_ contains EXACTLY 1 or EXACTLY 0 characters (followed by implicit null terminator)
bool str1char(char* str)
{
	if (*str == 0) return true;
	else if (*(++str) == 0) return true;
	return false;
}

// returns position of first occurence of _c_ in _str_
// if _c_ not found in _str_, returns 0
int strhaschar(char* str, char c)
{
	int pos = 1;
	while (*str != 0)
	{
		if (*str == c) return pos;
		str++; pos++;
	}
	return 0;
}
/*
char* pathGetDir(char* path, char delimeter)
{
	uint i = 0;
	uint last = 0;
	
	while (1)
	{
		if (path[i] == 0) break;
		if (path[i] == delimeter) last = i;
		
		i++;
	}
	
	char* ret = (char*) malloc((last+2) * sizeof(char));
	memcpy(ret, path, (last+1) * sizeof(char));
	ret[last+1] = 0;
	
	return ret;
}

char* pathGetName(char* path, char delimeter)
{
	uint i = 0;
	uint last = 0;
	
	while (1)
	{
		if (path[i] == 0) break;
		if (path[i] == delimeter) last = i;
		
		i++;
	}
	
	char* ret = (char*) malloc((i-last+1) * sizeof(char));
	last++;
	
	uint j = 0;	
	while (1)
	{
		if (last == i) break;
		if (path[last] != '.')
		{
			ret[j] = path[last];
			j++;
		}
		last++;
	}
	ret[j] = 0;
	
	return ret;
}
*/
char* printbin(int bits, char bitc)
{
	char* str = (char*) malloc(bitc + 1);
	str[bitc] = '\0';
	for (int i = 0; i < bitc; i++)
	{
		str[bitc - 1 - i] = '0' + (0 != (bits & (1 << i)));
	}
	return str;
}

int binint(char* n)
{
	int ret = 0;
	int i = 0;
	for (char* bit = n; *bit != '\0'; bit++)
	{
		if (*bit - '0') ret |= 1 << i;
		i++;
	}
	return ret;
}

#define flagset(field, bit) (field |= 1 << bit)
#define flagclear(field, bit) (field &= ~(1 << bit))
#define flag(field, bit) (field & (1 << bit))

inline float rad(float deg) {
	return deg * PI / 180;
}

inline float deg(float rad) {
	return rad * 180 / PI;
}

constexpr float RAD(float DEG) {
	return DEG * PI / 180;
}

constexpr float DEG(float RAD) {
	return RAD * 180 / PI;
}

#define CLAMP(LO, X, HI) (MIN(MAX(LO, X), HI))
#define MIN(A, B) ((A < B) ? A : B)
#define MAX(A, B) ((A > B) ? A : B)

struct string
{
	char* str = 0;
	int size = 0;
	int last = 0;
	
	// initialize empty string
	string()
	{
		str = (char*)0;
		size = 0;
		last = 0;
	}

	// initialize string with specific size
	string(int s)
	{
		size = s;
		str = talloc(char, s);
		last = 0;
		str[0] = (char)0; // null terminator
	}

	// initialize string with specific str
	string(char* chars)
	{
		size = strlen(chars) + 1;
		last = size - 1;
		str = talloc(char, size);
		memcpy(str, chars, last * sizeof(char));
		str[last] = 0;
	}

	// initialize string with specific data and size
	string(char* chars, int s)
	{
		int strl = strlen(chars) + 1;
		size = MAX(s, strl);
		str = talloc(char, size);
		memcpy(str, chars, strl * sizeof(char));
		last = strl - 1;
		str[last] = 0;
	}
	
	// initialize string with another string and specific size
	// (faster than raw char pointer because skip the call to 'strlen')
	string(string str2, int s)
	{
		size = MAX(s, str2.size);
		str = talloc(char, size);
		memcpy(str, str2.str, str2.size * sizeof(char));
		last = str2.last;
	}
	
	// returns a copy of this string with independent data
	string clone()
	{
		char* str2 = talloc(char, size);
		memcpy(str2, str, size * sizeof(char));
		
		// c++ is hellish
		string ret = *this;
		ret.str = str;
		return ret;
	}
	
	void clear()
	{
		free(str);
		str = (char*)0;
		size = 0;
		last = 0;
	}

	char& operator[] (int idx)
	{
		#ifdef ZALGO_EXT_ERR
			if (idx >= capacity) fprintf(stderr, "string %p: out of bounds index %i\n", this, idx);
		#endif
		return str[idx];
	}
	
	inline int space()
	{
		// last is zero-indexed while size is not
		return size - last - 1;
	}
	
	void append(char c)
	{
		if (space()) goto write;
		
		str = trealloc(str, size + 1);
		
		write:
		str[last] = c;
		last++;
		str[last] = 0;
	}
	
	void append(char* str2)
	{
		int strl = strlen(str2);
		if (space() < strl)
			str = trealloc(str, size + strl);
		
		memcpy(str + last, str2, strl * sizeof(char));
		last += strl;
		str[last] = 0;
	}
	
	// writes data to the end of this string
	// resizes to fit exactly
	void append(string str2)
	{
		if (space() < str2.size)
			str = trealloc(str, size + str2.size);
		
		memcpy(str + last, str2.str, str2.size * sizeof(char));
		last += str2.size;
	}
	
	// increases size by factor of 2
	void grow()
	{
		size = size << 1;
		str = trealloc(str, size);
	}
	
	// frees unused characters
	void trim()
	{
		str = trealloc(str, last + 1);
	}
	
	// shrink array by _s_ characters
	void cut(int s)
	{
		str = trealloc(str, size - s);
		last = size - 1;
		str[last] = 0;
	}
	
	// get substring of this string
	string substr(int from, int to)
	{
		string ret;
		
		ret.size = to - from + 2;
		ret.last = ret.size - 1;
		ret.str = talloc(char, size);
		memcpy(ret.str, str + from, ret.size * sizeof(char));
		ret.str[size - 1] = 0;
		
		return ret;
	}
};

template <class T>
struct dlnode { // Doubly linked node
	T value;
	dlnode* next = NULL;
	dlnode* prev = NULL;
};

template <class T>
struct List {

	dlnode<T>* first;
	dlnode<T>* last;
	int size;

	List() {
		first = NULL;
		last = NULL;
		size = 0;
	}

	int append(T val) {
		size++;
		dlnode<T>* add = new dlnode<T>;
		add->value = val;
		if (first == NULL) {
			first = add;
			last = add;
			return size;
		}
		last->next = add;
		add->prev = last;
		last = add;
		return size;
	}

	int prepend(T val) {
		size++;
		dlnode<T>* add = new dlnode<T>;
		add->value = val;
		if (last == NULL) {
			first = add;
			last = add;
			return size;
		}
		first->prev = add;
		add->next = first;
		first = add;
		return size;
	}

	void insert(T val, int idx) {
		if (idx < 0 or idx > size - 1) return;
		size++;
		dlnode<T>* add = new dlnode<T>;
		add->value = val;
		if (first == NULL) {
			first = add;
			last = add;
			return;
		}
		dlnode<T>* current = first;
		for (int i = 0; i < idx - 1; i++) {
			current = current->next;
		}
		dlnode<T>* following = current->next;
		current->next = add;
		add->prev = current;
		add->next = following;
		following->prev = add;
	}

	int remove(int idx) {
		if (idx < 0 or idx > size - 1) return -1;
		size--;
		dlnode<T>* current = first;
		for (int i = 0; i < idx; i++) {
			current = current->next;
		}
		T val = current->value;
		if (current->prev == NULL) {
			if (current->next == NULL) {
				first = NULL;
				last = NULL;
				free(current);
				return idx;
			}
			first = current->next;
			current->next->prev = NULL;
			free(current);
			return idx;
		}
		if (current->next == NULL) {
			if (current->prev == NULL) {
				first = NULL;
				last = NULL;
				free(current);
				return idx;
			}
			last = current->prev;
			current->prev->next = NULL;
			free(current);
			return idx;
		}
		current->prev->next = current->next;
		current->next->prev = current->prev;
		free(current);
		return size;
	}

	int find(T val) {
		if (size == 0) return -1;
		dlnode<T>* current = first;
		for (int idx = 0; current != NULL; idx++) {
			if (current->value == val) return idx;
			current = current->next;
		}
		return -1;
	}

	T get(int idx) {
		if (idx < 0 or idx > size - 1 or size == 0) return NULL;
		dlnode<T>* current = first;
		for (int i = 0; i < idx; i++) {
			if (current != NULL) current = current->next;
		}
		if (current == NULL) return NULL;
		return current->value;
	}

	T operator[](int idx)
	{
		return get(idx);
	}

};

template <class T>
struct Array
{
	T* data = 0;
	uint cap = 0;
	uint size = 0; // next available index

	// create an empty array of given size
	void allocate(int capacity)
	{
		data = (T*) malloc(capacity * sizeof(T));
		if (!data) return;
		cap = capacity;
		size = 0;
	}

	// completely overwrites array buffer with a given byte
	void fill(int value)
	{
		memset(data, value, cap * sizeof(T));
	}
	
	// frees trailing bytes
	// (does not clean fragmentation!)
	void shrink()
	{
		data = (T*) realloc(data, size * sizeof(T));
	}

	// returns size of array data in bytes
	uint blocksize()
	{
		return cap * sizeof(T);
	}
	
	// move the internal 'cursor' by an offset
	inline
	uint seek(uint i)
	{
		size += i;
	}
	
	// move the internal 'cursor' to a specific index
	// essentially deletes trailing elements
	// without freeing memory
	inline
	uint seekTo(uint i)
	{
		size = i;
	}

	// returns 0 for not enough memory,
	// otherwise returns new array size
	uint append(T value)
	{	
		if (size == cap)
		{
			if (cap == 0) cap = 1;
			T* attempt = (T*) realloc(data, sizeof(T) * (cap*2));
			if (!attempt) return 0;
			data = attempt;
			cap *= 2;
		}
		data[size] = value;
		size++;
		return cap;
	}
	
	// returns 0 for not enough memory,
	// otherwise returns new array size
	uint prepend(T value)
	{	
		if (size == cap)
		{
			T* attempt = (T*) realloc(data, sizeof(T) * (cap*2));
			if (!attempt) return 0;
			data = attempt;
			cap *= 2;
		}
		memmove(data, data + 1, sizeof(T) * size);
		data[0] = value;
		size++;
		return cap;
	}

	// append an entire array to the end of this one
	// (does not free memory of concatenated array)
	// returns 0 on failure, otherwise returns new size
	// UNSTABLE API!
	uint concatenate(Array<T> array)
	{
		if (! realloc(data, (this->size + array.size) * sizeof(T)) )
			return 0;
		
		memcpy((data + size), array.data, array.size * sizeof(T));
		size += array.size;
		return size;
	}

	// returns -1 for not enough memory
	// UNSTABLE API!
	uint insert(T value, int idx) {
		T* attempt = (T*) realloc(data, sizeof(T) * (size + 1));
		if (!attempt) return -1;
		memmove(data + idx, data + idx + 1, size - idx);
		data[idx] = value;
		size++;
		return idx;
	}

	// returns new size of array
	uint remove(int idx)
	{
		memmove(data + idx + 1, data + idx, size - 1 - idx);
		
		size--;
		if (size < cap/2)
		{
			cap = cap/2;
			data = (T*) realloc(data, sizeof(T) * cap);
		}
		return cap;
	}

	// removes all elements and frees/nulls data pointer
	void empty()
	{
		if (data) free(data);
		data = 0;
		cap = 0;
		size = 0;
	}

	// returns NULL for invalid index
	inline T & get(int idx) {
		return data[idx];
	}

	T & operator[](int idx)
	{
		return data[idx];
	}

	// does nothing for invalid index
	inline void set(int idx, T value) {
		if (idx < 0 or idx > size - 1) return;
		data[idx] = value;
	}

	// returns index of first matching value
	// -1 = not found
	// -2 = array empty
	int find(T value) {
		if (size < 1) return -2;
		for (int idx = 0; idx < size; idx++) {
			if (data[idx] == value) return idx;
		}
		return -1;
	}

	#ifdef ZALGO_EXT_IO
	void dump() {
		for (int i = 1; i < size; i++) {
			printf("index %i, value %i\n",i,get(i - 1));
		}
	}
	#endif
};

/*
template <int size, class T>
struct Table
{
	T data[size];
	int next = 0;
	int last = 0;

	void add(T item)
	{
		data[next] = item;
	}
};
*/

struct nenum: Array<char*>
{
	nenum() {}

	nenum(char** keys, int ct)
	{
		int keybytes = sizeof(char*) * ct;
		allocate(ct);
		memcpy(data, keys, keybytes);
	}

	inline int operator[](char* key)
	{
		for (int i = 0; i < size; i++)
		{
			if (strcmp(key, data[i]) == 0) return i;
		}
		return -1;
	}
};

/*
template <class K, class V>
struct Dict
{
	struct Pair
	{
		K key;
		V value;
	}
	
	Array<Pair> entries;

	void add(char* key, T value)
	{
		keys.append(key);
		values.append(value);
	}

	// removes first element with 'key'
	void remove(char* key)
	{
		for (int i = 0; i < keys.size; i++)
		{

		}
	}

	// returns index of first element matching 'key'
	// returns -1 if key not found
	int find(K key)
	{
		for (int i = 0; i < keys.size; i++)
		{
			if (strcmp(keys[i], key) == 0) return i;
		}
		return -1;
	}

	// returns index of first element matching 'value'
	// returns -1 if value not found
	int find(V value)
	{
		for (int i = 0; i < values.size; i++)
		{
			if (values[i] == value) return i;
		}
		return -1;
	}
};
*/

#undef PI

#endif
