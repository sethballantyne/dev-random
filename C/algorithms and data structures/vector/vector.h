#pragma once

#define VECTOR_DEFAULT_CAPACITY 16

typedef struct
{
	unsigned int capacity;
	unsigned int size;
	void **elements;
} vector_t;

int vector_init(vector_t *vector);

int vector_add_last(vector_t *vector, const void *item);

int vector_set_item_at(vector_t *vector, unsigned int index, const void *item);

int vector_get_item_at(vector_t *vector, unsigned int index, void **item);

void vector_remove_item_at(vector_t *vector, unsigned int index);

void vector_destroy(vector_t *vector);