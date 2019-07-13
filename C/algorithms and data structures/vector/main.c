#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

typedef struct
{
	int num1;
	int num2;
} num_t;

int main(void)
{
	vector_t vector;

	vector_init(&vector);
	
	for(int i = 0; i < 16; i++)
	{
		vector_add_last(&vector, i);
	}

	for(int i = 0; i < vector.size; i++)
	{
		int num;
		vector_get_item_at(&vector, i, &num);
		printf("%d\n", num);
	}
	
	vector_remove_item_at(&vector, 7);

	printf("\n\n\n");
	for(int i = 0; i < vector.size; i++)
	{
		int num;
		vector_get_item_at(&vector, i, &num);
		printf("%d\n", num);
	}

	vector_destroy(&vector);
	system("PAUSE");
	return 0;
}