#pragma once
#include <stdbool.h>

typedef struct clist_element
{
	void *data;
	struct clist_element *previous;
	struct clist_element *next;
} clist_element_t;

typedef struct circular_list
{
	int size;
	void(*destroy)(void *data);

	clist_element_t *head;
} circular_list_t;


void circular_list_init(circular_list_t *list, void(*destroy)(void *data));

int circular_list_add_first(circular_list_t *list, const void *data);

int circular_list_add_last(circular_list_t *list, const void *data);

int circular_list_remove_first(circular_list_t *list, void **data);

int circular_list_remove_last(circular_list_t *list, void **data);

int circular_list_contains(circular_list_t *list, bool(*compare)(void *));

clist_element_t *circular_list_find(circular_list_t *list, bool(*compare)(void *));

int circular_list_remove(circular_list_t *list, clist_element_t *element, void **data);

void circular_list_destroy(circular_list_t *list);