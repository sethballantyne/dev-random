//-----------------------------------------------------------------------------
// Implementation of a doubly-linked list.
//-----------------------------------------------------------------------------
#pragma once
#include <stdbool.h>

typedef struct dlist_element
{
	void *data;
	struct dlist_element *previous;
	struct dlist_element *next;
} dlist_element_t;

typedef struct dlinked_list
{
	int size;
	void(*destroy)(void *data);

	dlist_element_t *head;
	dlist_element_t *tail;
} dlinked_list_t;

// initialises the doubly-linked list pointed to by list. 
// This must be called before the linked list can be used with any other 
// linked_list_* function. 
// destroy provides a means of performing additional tasks against each element
// during clean up when dlinked_list_clear() is called. If you need to free memory, that's a 
// good place to do it.
void dlinked_list_init(dlinked_list_t *list, void(*destroy)(void *data));

// creates a new element containing the specified data and places it at the start of the list.
// returns 0 on succcess, or -1 on failure.
int dlinked_list_add_first(dlinked_list_t *list, const void *data);

// creates a new element containing the specified data and appends it to the list.
// returns 0 if the data is successfully appended to the list, otherwise -1.
int dlinked_list_add_last(dlinked_list_t *list, const void *data);

// removes the first element from the list. If the element contains data that was dynamically
// allocated, this will need to be freed by the caller. The data argument will point to the data
// contained in the element that was removed. It's up to the caller to free any memory that was
// dynamically allocated to whatever is stored in data.
// returns 0 on success, -1 on failure.
int dlinked_list_remove_first(dlinked_list_t *list, void **data);

// removes the last element from the list. If the element contains data that was dynamically
// allocated, this will need to be freed by the caller. The data argument will point to the data
// contained in the element that was removed. It's up to the caller to free any memory that was
// dynamically allocated to whatever is stored in data.
// returns 0 if the element was successfully removed, otherwise -1.
int dlinked_list_remove_last(dlinked_list_t *list, void **data);

// Uses a user-defined function to compare each element in the list for the desired
// data. The function pointed to by compare will be executed against the data member of each element
// and should compare the desired value against data, returning true if there's a match or 
// false if there's not. 
// As an example, say you had a list of structs that contained two integer members:
// bool find_nums(void *list_data)
// {
//	num_t **num = (num_t *)list_data;
//
//	if((*(num))->num1 == 0 && (*(num))->num2 == 2)
//	{
//		return true;
//	}
//
//	return false;
// }
//
// ... 
//
// bool contains = dlinked_list_contains(&list, find_num);
// 
// dlinked_list_contains returns true if the specified value is found, otherwise false.
bool dlinked_list_contains(dlinked_list_t *list, bool(*compare)(void *));

// returns the first element that matches the comparison performed in compare. 
// See the comments for dlinked_list_contains() for details on how compare should work.
// returns NULL if no element could be found.
dlist_element_t *dlinked_list_find(dlinked_list_t *list, bool(*compare)(void *));

// removes the specified element from the list.
// returns 0 if the element was removed, otherwise returns -1.
int dlinked_list_remove(dlinked_list_t *list, dlist_element_t *element, void **data);

// Removes all the elements from the linked list and calls the cleanup function passed to 
// dlinked_list_init() when the linked list was initialised, provided it wasn't set to NULL.
void dlinked_list_clear(dlinked_list_t *list);