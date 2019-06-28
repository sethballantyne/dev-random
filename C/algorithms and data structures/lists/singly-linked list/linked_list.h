//-----------------------------------------------------------------------------
// Implementation of a singly-linked list.
//-----------------------------------------------------------------------------
#pragma once

typedef struct list_element
{
	void *data;
	struct list_element *next;
} list_element_t;

typedef struct linked_list
{
	int size;
	void (*destroy)(void *data);

	list_element_t *head;
	list_element_t *tail;
} linked_list_t;

// initialises the linked list pointed to by list. 
// This must be called before the linked list can be used with any other 
// linked_list_* function. 
// destroy provides a means of performing additional tasks against each element
// during clean up when linked_list_clear() is called. If you need to free memory, that's a 
// good place to do it.
void linked_list_init(linked_list_t *list, void(*destroy)(void *data));

// creates a new element containing the specified data and places it at the start of the list.
// returns 0 on succcess, or -1 on failure.
int linked_list_add_first(linked_list_t *list, const void *data);

// creates a new element containing the specified data and appends it to the list.
// returns 0 if the data is successfully appended to the list, otherwise -1.
int linked_list_add_last(linked_list_t *list, const void *data);

// removes the first element from the list. If the element contains data that was dynamically
// allocated, this will need to be freed by the caller. The data argument will point to the data
// contained in the element that was removed. It's up to the caller to free any memory that was
// dynamically allocated to whatever is stored in data.
// returns 0 on success, -1 on failure.
int linked_list_remove_first(linked_list_t *list, void **data);

// removes the last element from the list. If the element contains data that was dynamically
// allocated, this will need to be freed by the caller. The data argument will point to the data
// contained in the element that was removed. It's up to the caller to free any memory that was
// dynamically allocated to whatever is stored in data.
// returns 0 if the element was successfully removed, otherwise -1.
int linked_list_remove_last(linked_list_t *list, void **data);

// Removes all the elements from the linked list and calls the cleanup function passed to 
// linked_list_init() when the linked list was initialised, provided it wasn't set to NULL.
void linked_list_clear(linked_list_t *list);