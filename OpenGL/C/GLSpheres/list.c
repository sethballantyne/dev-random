/*Copyright (c) 2016 Seth Ballantyne <seth.ballantyne@gmail.com>
*
*Permission is hereby granted, free of charge, to any person obtaining a copy
*of this software and associated documentation files (the "Software"), to deal
*in the Software without restriction, including without limitation the rights
*to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*copies of the Software, and to permit persons to whom the Software is
*furnished to do so, subject to the following conditions:
*
*The above copyright notice and this permission notice shall be included in
*all copies or substantial portions of the Software.
*
*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*THE SOFTWARE.
*/

#include <stdarg.h>
#include <string.h>
#include "list.h"

void List_Create(list_t *list, void (*destroy)(void *data))
{
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;
	list->tail = NULL;
}

void List_Destroy(list_t *list)
{
	void *data;

	while(list->size > 0)
	{
		int result = List_RemoveItem(list, NULL, (void **) &data);
		if(result == 0 && list->destroy != NULL)
		{
			list->destroy(data);
		}
	}

	memset(list, 0, sizeof(list_t));
}

int List_AppendItem(list_t *list, element_t *element,  void *data)
{
	element_t *new_element;
	element_t *temp;

	new_element = (element_t *) malloc(sizeof(element_t));
	if(NULL == new_element)
	{
		return -1;
	}

	new_element->data = (void *) data;
	new_element->next = NULL;

	if(NULL == list->tail)
	{
		list->head = new_element;
		list->tail = new_element;
	}
	else
	{
		temp = list->head;

		while (NULL != temp->next) 
		{
			temp = temp->next;
		}

		temp->next = new_element;
		list->tail = new_element;
    }

	list->size++;

	return 0;
}

int List_AddItem(list_t *list, element_t *element, void *data)
{
	element_t *new_element;

	new_element = (element_t *) malloc(sizeof(element_t));
	if(NULL == new_element)
	{
		return -1;
	}

	new_element->data = (void *) data;

	if(NULL == element)
	{
		if(list->size == 0)
		//if(LIST_SIZE(list) == 0)
		{
			list->tail = new_element;
		}

		new_element->next = list->head;
		list->head = new_element;
	}
	else
	{
		if(NULL == element->next)
		{
			list->tail = new_element;
		}

		new_element->next = element->next;
		element->next = new_element;
	}

	list->size++;

	return 0;
}

int List_RemoveItem(list_t *list, element_t *element, void **data)
{
	element_t *old_element;

	if(0 == list->size)
	//if(LIST_SIZE(list) == 0)
	{
		return -1;
	}

	if(NULL == element)
	{
		*data = list->head->data;
		old_element = list->head;
		list->head = list->head->next;

		if(list->size == 1)
		{
			list->tail = NULL;
		}
	}
	else
	{
		if(NULL == element->next)
		{
			return -1;
		}

		*data = (element_t *) element->next->data;
		old_element = element->next;
		element->next = element->next->next;

		if(NULL == element->next)
			list->tail = element;
	}

	free(old_element);

	list->size--;

	return 0;
}