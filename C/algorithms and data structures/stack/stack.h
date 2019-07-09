#pragma once
#include "linked_list.h"

typedef linked_list_t stack_t;

void stack_init(stack_t *stack, void(*destroy)(void *data));

void stack_destroy(stack_t *stack);

void stack_push(stack_t *stack, const void *data);

void stack_pop(stack_t *stack, void **data);

void *stack_peek(stack_t *stack);