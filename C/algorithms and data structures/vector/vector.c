#include <stdlib.h>
#include "vector.h"

static int vector_resize(vector_t *vector, int capacity)
{
	void **temp = realloc(vector->elements, sizeof(void *) * capacity);
	if(NULL != temp)
	{
		vector->elements = temp;
		vector->capacity = capacity;

		return 0;
	}

	return 1; // failed to allocate memory
}

int vector_init(vector_t *vector)
{
	if(NULL != vector)
	{
		vector->capacity = VECTOR_DEFAULT_CAPACITY;
		vector->size = 0;
		vector->elements = malloc(sizeof(void *) * vector->capacity);
		if(!vector->elements)
		{
			return -1;
		}

		return 0;
	}

	return -1;
}

int vector_add_last(vector_t *vector, const void *item)
{
	if(NULL != vector)
	{
		if(vector->size == vector->capacity)
		{
			int result = vector_resize(vector, vector->capacity * 2);
			if(result != 0)
			{
				return -1; // failed to allocate memory when resizing;
				// item not added.
			}
		}

		vector->elements[vector->size] = item;
		vector->size++;

		return 0;
	}

	return -1;
}

int vector_set_item_at(vector_t *vector, unsigned int index, const void *item)
{
	if(NULL != vector)
	{
		if(index < vector->size)
		{
			vector->elements[index] = item;
			
			return 0;
		}
		else
		{
			return -1; // invalid index;
		}
	}

	return -1; // vector was NULL
}

int vector_get_item_at(vector_t *vector, unsigned int index, void **item)
{
	if(NULL != vector)
	{
		if(index < vector->size)
		{
			*item = vector->elements[index];
			return 0;
		}

		return -1; // index invalid
	}

	return -1; // vector != NULL
}

void vector_remove_item_at(vector_t *vector, unsigned int index)
{
	if(NULL != vector)
	{
		if(index < vector->size)
		{
			vector->elements[index] = NULL;

			// shift all the items right of the deleted element
			// one element to the left.
			for(int i = index; i < (vector->size - 1); i++)
			{
				vector->elements[i] = vector->elements[i + 1];
				vector->elements[i + 1] = NULL;
			}

			vector->size--;

			return 0;
		}

		return -1; // invalid index
	}

	return -1; // vector is NULL
}

void vector_destroy(vector_t *vector)
{
	if(NULL != vector && NULL != vector->elements)
	{
		free(vector->elements);
		vector->elements = NULL;
		vector->capacity = 0;
	}
}
