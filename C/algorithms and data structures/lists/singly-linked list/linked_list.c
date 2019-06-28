#include <stdlib.h>
#include "linked_list.h"

void linked_list_init(linked_list_t *list, void(*destroy)(void *data))
{
	list->size = 0;
	list->destroy = destroy;
	list->head = list->tail = NULL;
}

int linked_list_add_first(linked_list_t *list, const void *data)
{
	list_element_t *new_element = malloc(sizeof(list_element_t));
	if(!new_element)
	{
		return -1;
	}

	new_element->data = (void *)data;
	new_element->next = NULL;

	if(0 == list->size)
	{
		list->head = new_element;
		list->tail = new_element;
	}
	else
	{
		new_element->next = list->head;
		list->head = new_element;
	}

	list->size++;
	return 0;
}

int linked_list_add_last(linked_list_t *list, const void *data)
{
	list_element_t *new_element = malloc(sizeof(list_element_t));
	if(!new_element)
	{
		return -1;
	}

	new_element->data = (void *)data;
	new_element->next = NULL;

	if(0 == list->size)
	{
		list->head = new_element;
		list->tail = new_element;
	}
	else
	{
		list->tail->next = new_element;
		list->tail = new_element;
	}

	list->size++;

	return 0;
}

int linked_list_remove_first(linked_list_t *list, void **data)
{
	list_element_t *old_element;

	if(0 == list->size)
	{
		return -1;
	}

	if(NULL != data)
	{
		*data = list->head->data;
	}

	old_element = list->head;
	list->head = list->head->next;

	if(1 == list->size)
	{
		list->tail = NULL;
	}

	free(old_element);
	list->size--;

	return 0;
}

void linked_list_clear(linked_list_t *list)
{
	void *data;

	while(list->size > 0)
	{
		int result = linked_list_remove_first(list, (void **)&data);
		if(0 == result && NULL != list->destroy)
		{
			// call the user-specified clean up function on elements data.
			list->destroy(data);
		}
	}
}