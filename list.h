#ifndef __INCLUDE_LIST_H
#define __INCLUDE_LIST_H

#include <stdbool.h>
#include <stddef.h>

#define INIT_BUF_SIZE 8
//Defines the initial size of the buffer used to store elements in the list.

typedef struct __list list;
struct __list {
	void *buf;			// actual pointer to the memory
	size_t mem;			// memory used
	size_t size_elem;	// the size of a single element
	unsigned num_elems; // number of elements
	bool empty;			// boolean flag indicating whether the list is empty
};

void listInit(list *self, size_t size_elem); // initialize the list
void listDestroy(list *self); // free all memory associated with the list
void listAppend(list *self,
				void *data); // add an element at the end of the list
void listInsertAt(list *self, unsigned index,
				  void *data); // insert an element at given index
void listDeleteAt(list *self,
				  unsigned index); // delete an element at the given index
void *listAt(list *self,
			 unsigned index); // return a pointer to element at an index
void listExpand(list *self);  // increase the memory for the container if needed
void listShrink(list *self);  // decrease the memory for the container if needed

// the do-while construct only runs once, and makes sure the macro doesn't face
// issues with semicolons placed after it when it is called
#define NULL_ERROR(name)                                                       \
	do {                                                                       \
		fprintf(stderr, "Error: %s called with invalid list!\n", #name);       \
	} while (0)

#endif // __INCLUDE_LIST_H
