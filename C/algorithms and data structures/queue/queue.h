#pragma once
#include "linked_list.h"

typedef linked_list_t queue_t;

void queue_init(queue_t *queue, void(*destroy)(void *data));

void queue_destroy(queue_t *queue);

int queue_dequeue(queue_t *queue, void **data);

int queue_enqueue(queue_t *queue, const void **data);

void *queue_peek(queue_t *queue);