#include <stdlib.h>
#include "circular_list.h"

void circular_list_init(circular_list_t *list, void(*destroy)(void *data))
{
	list->head = NULL;
	list->destroy = destroy;
	list->size = 0;
}

int circular_list_add_first(circular_list_t *list, const void *data)
{
	clist_element_t *new_element = NULL;

	if(NULL != list && NULL != data)
	{
		new_element = malloc(sizeof(clist_element_t));
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
			list->head->previous = list->head;
			list->head->next = list->head;
		}
		else
		{
			new_element->next = list->head;
			new_element->previous = list->head->previous;
			list->head->previous->next = new_element;
			list->head->previous = new_element;
			//list->head->previous->next = new_element;
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

int circular_list_add_last(circular_list_t *list, const void *data)
{
	if(NULL != list && NULL != data)
	{
		clist_element_t *new_element = malloc(sizeof(clist_element_t));
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
			list->head->next = new_element;
			list->head->previous = new_element;
		}
		else
		{
			new_element->previous = list->head->previous;
			list->head->previous->next = new_element;
			new_element->next = list->head;
			list->head->previous = new_element;
		}

		list->size++;

		return 0;
	}
	else
	{
		return -1;
	}
}

int circular_list_remove_first(circular_list_t *list, void **data)
{
	if(NULL != list)
	{
		clist_element_t *old_element;

		if(0 == list->size)
		{
			return -1;
		}

		if(NULL != data)
		{
			*data = list->head->data;
		}

		old_element = list->head;
		list->head = old_element->next;
		list->head->previous = old_element->previous;
		list->head->previous->next = list->head;

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

int circular_list_remove_last(circular_list_t *list, void **data)
{
	if(NULL != list && 0 != list->size)
	{
		if(NULL != data)
		{
			*data = list->head->previous->data;
		}

		clist_element_t *old_element = list->head->previous;
		list->head->previous = old_element->previous;
		old_element->previous->next = list->head;

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

int circular_list_contains(circular_list_t *list, bool(*compare)(void *))
{
	if(NULL != list && NULL != compare)
	{
		clist_element_t *element = list->head;
		for(int i = 0; i < list->size; i++)
		{
			if(true == compare(element->data))
			{
				return i;
			}

			element = element->next;
		}
	}

	return -1;
}

clist_element_t *circular_list_find(circular_list_t *list, bool(*compare)(void *))
{
	if(NULL != list && NULL != compare)
	{
		clist_element_t *element = list->head;
		for(int i = 0; i < list->size; i++)
		{
			if(true == compare(element->data))
			{
				return element;
			}

			element = element->next;
		}

		return NULL;
	}
}

int circular_list_remove(circular_list_t *list, clist_element_t *element, void **data)
{
	if(NULL != list && NULL != element)
	{
		if(NULL != data)
		{
			*data = element->data;
		}

		if(NULL != list->head)
		{
			element->previous->next = element->next;
			element->next->previous = element->previous;

			free(element);
			list->size--;

			if(0 == list->size)
			{
				list->head = NULL;
			}
			return 0;
		}
	}

	return -1;
}

void circular_list_destroy(circular_list_t *list)
{
	if(NULL != list)
	{
		void *data = NULL;

		while(list->size > 0)
		{
			int result = circular_list_remove_first(list, (void **)&data);
			if(0 == result && NULL != list->destroy)
			{
				// call the user-specified clean up function on elements data.
				list->destroy(data);
			}
		}
	}
}