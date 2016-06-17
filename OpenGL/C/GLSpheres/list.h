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

#pragma once

#include <stdlib.h>

/* Represents an individual node within the linked list */
typedef struct element_
{
	void *data;
	struct element_ *next;
} element_t;

//
typedef struct
{
	// the number of elements contained within the list
	int size;

	//
	int (*match)(const void *key1, const void *key2);

	//
	void (*destroy)(void *data);

	//
	element_t *head;

	//
	element_t *tail;
} list_t;

//
void List_Create(list_t *list, void(*destroy)(void *data));

//
void List_Destroy(list_t *list);

//
int List_AddItem(list_t *list, element_t *element, void *data);

//
int List_AppendItem(list_t *list, element_t *element,  void *data);

//
int List_RemoveItem(list_t *list, element_t *element, void **data);

//#define LIST_SIZE(list) ((list)->size)
//#define LIST_HEAD(list) ((list)->head)
//#define LIST_TAIL(list) ((list)->tail)
//#define LIST_IS_HEAD(list, element) ((element) == (list)->head ? 1 : 0)
//#define LIST_IS_TAIL(element) ((element)->next == NULL ? 1 : 0)
//#define LIST_DATA(element) ((element)->data)
//#define LIST_NEXT(element) ((element)->next)


