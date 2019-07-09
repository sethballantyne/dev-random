#include <stdlib.h>
#include "stack.h"

void stack_init(stack_t *stack, void(*destroy)(void *data))
{
	linked_list_init(stack, destroy);
}

void stack_destroy(stack_t *stack)
{
	linked_list_clear(stack);
}

void stack_push(stack_t *stack, const void *data)
{
	return linked_list_add_first(stack, data);
}

void stack_pop(stack_t *stack, void **data)
{
	return linked_list_remove_first(stack, data);
}

void *stack_peek(stack_t *stack)
{
	if(NULL != stack && NULL != stack->head)
	{
		return stack->head->data;
	}

	return NULL;
}

