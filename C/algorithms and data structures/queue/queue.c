#include <stdlib.h>
#include "queue.h"

void queue_init(queue_t *queue, void(*destroy)(void *data))
{
	linked_list_init(queue, destroy);
}

void queue_destroy(queue_t *queue)
{
	linked_list_clear(queue);
}

int queue_dequeue(queue_t *queue, void **data)
{
	return linked_list_remove_first(queue, data);
}

int queue_enqueue(queue_t *queue, const void **data)
{
	return linked_list_add_last(queue, data);
}

void *queue_peek(queue_t *queue)
{
	if(NULL != queue && NULL != queue->head)
	{
		return queue->head->data;
	}

	return NULL;
}