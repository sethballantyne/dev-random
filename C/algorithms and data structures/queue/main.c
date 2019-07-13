#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

typedef struct
{
	int num1;
	int num2;
} num_t;

int main(void)
{
	queue_t queue;
	
	num_t num1 = { 1, 2 };
	num_t num2 = { 3, 4 };
	num_t num3 = { 5, 6 };

	queue_init(&queue, NULL);
	queue_enqueue(&queue, &num1);
	queue_enqueue(&queue, &num2);
	queue_enqueue(&queue, &num3);

	printf("Size: %d\n", queue.size);
	for(int i = 0; i < 3; i++)
	{
		num_t *num = (num_t *)queue_peek(&queue);
		printf("\nPeek: %d %d\n", num->num1, num->num2);
		queue_dequeue(&queue, &num);
		printf("Dequeue: %d\n", num->num1, num->num2);
		printf("Size: %d\n", queue.size);
	}

	queue_destroy(&queue);
	system("PAUSE");
	return 0;
}