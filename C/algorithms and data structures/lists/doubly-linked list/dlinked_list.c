#include <stdlib.h>
#include "dlinked_list.h"

void dlinked_list_init(dlinked_list_t *list, void(*destroy)(void *data))
{
	if(NULL != list)
	{
		list->size = 0;
		list->destroy = destroy;
		list->head = list->tail = NULL;
	}
}

int dlinked_list_add_first(dlinked_list_t *list, const void *data)
{
	dlist_element_t *new_element = NULL;

	if(NULL != list && NULL != data)
	{
		new_element = malloc(sizeof(dlist_element_t));
		if(!new_element)
		{
			return -1;
		}

		new_element->data = (void *)data;
		new_element->next = NULL;
		new_element->previous = NULL;

		if(0 == list->size)
		{
			list->head = new_element;
			list->tail = new_element;
		}
		else
		{
			new_element->next = list->head;
			list->head->previous = new_element;
			list->head = new_element;
		}

		list->size++;

		return 0;
	}
	else
	{
		return -1;
	}
}

int dlinked_list_add_last(dlinked_list_t *list, const void *data)
{
	if(NULL != list && NULL != data)
	{
		dlist_element_t *new_element = malloc(sizeof(dlist_element_t));
		if(!new_element)
		{
			return -1;
		}

		new_element->data = (void *)data;
		new_element->next = NULL;
		new_element->previous = NULL;

		if(0 == list->size)
		{
			list->head = new_element;
			list->tail = new_element;
		}
		else
		{
			new_element->previous = list->tail;
			list->tail->next = new_element;
			list->tail = new_element;
		}

		list->size++;

		return 0;
	}
	else
	{
		return -1;
	}
}

int dlinked_list_remove_first(dlinked_list_t *list, void **data)
{
	if(NULL != list)
	{
		dlist_element_t *old_element;

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

		// if we're in the process of removing the last element in the list,
		// this will be NULL.
		if(NULL != list->head)
		{
			list->head->previous = NULL;
		}

		if(1 == list->size)
		{
			list->tail = NULL;
		}

		free(old_element);
		list->size--;

		return 0;
	}
	else
	{
		return -1;
	}
}

int dlinked_list_remove_last(dlinked_list_t *list, void **data)
{
	if(NULL != list && 0 != list->size)
	{
		if(NULL != data)
		{
			*data = list->tail->data;
		}

		dlist_element_t *old_element = list->tail;
		list->tail = list->tail->previous;

		if(NULL != list->tail)
		{
			list->tail->next = NULL;
		}

		free(old_element);
		list->size--;

		if(0 == list->size)
		{
			list->head = NULL;
		}

		return 0;
	}
	else
	{
		return -1;
	}
}

bool dlinked_list_contains(dlinked_list_t *list, bool(*compare)(void *))
{
	if(NULL != list && NULL != compare)
	{
		dlist_element_t *element = list->head;
		for(int i = 0; i < list->size; i++)
		{
			if(true == compare(element))
			{
				return true;
			}

			element = element->next;
		}
	}

	return false;
}

dlist_element_t *dlinked_list_find(dlinked_list_t *list, bool(*compare)(void *))
{
	if(NULL != list && NULL != compare)
	{
		dlist_element_t *element = list->head;
		for(int i = 0; i < list->size; i++)
		{
			if(true == compare(element))
			{
				return element;
			}

			element = element->next;
		}

		return NULL;
	}
}

int dlinked_list_remove(dlinked_list_t *list, dlist_element_t *element, void **data)
{
	if(NULL != list && NULL != element)
	{
		if(NULL != data)
		{
			*data = element->data;
		}

		if(NULL != element->previous)
		{
			// not removing a head element.
			element->previous->next = element->next;
					
			if(NULL == element->next)
			{
				list->tail = element->previous;
			}
			else
			{
				element->next->previous = element->previous;
			}
		}
		else
		{
			// removing the head element
			list->head = element->next;

			if(NULL == list->head)
			{
				list->tail = NULL;
			}
			else
			{
				element->next->previous = NULL;
			}
		}

		free(element);
		list->size--;

		return 0;
	}

	return -1;
}

void dlinked_list_clear(dlinked_list_t *list)
{
	if(NULL != list)
	{
		void *data;

		while(list->size > 0)
		{
			int result = dlinked_list_remove_first(list, (void **)&data);
			if(0 == result && NULL != list->destroy)
			{
				// call the user-specified clean up function on elements data.
				list->destroy(data);
			}
		}
	}
}