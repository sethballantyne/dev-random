#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

typedef struct
{
	int num1;
	int num2;
} num_t;

void dump_list(linked_list_t *linked_list)
{
	list_element_t *element = linked_list->head;
	for(int i = 0; i < linked_list->size; i++)
	{
		num_t *tmp = (num_t *) element->data;
		printf("%d %d\n", tmp->num1, tmp->num2);
		if(NULL != element->next)
		{
			element = element->next;
		}
	}
}

int main(void)
{
	linked_list_t linked_list;
	num_t foo = { 1, 2 };
	num_t foo1 = { 3, 4 };
	num_t foo2 = { 5, 6 };
	num_t *dynamic_num = (num_t *) malloc(sizeof(num_t));
	dynamic_num->num1 = 7;
	dynamic_num->num2 = 8;

	linked_list_init(&linked_list, NULL);
	/*linked_list_insert_next(&linked_list, NULL, "test");
	linked_list_insert_next(&linked_list, NULL, "boo");*/
	linked_list_add_first(&linked_list, &foo);
	linked_list_add_first(&linked_list, &foo1);
	linked_list_add_first(&linked_list, &foo2);
	linked_list_add_first(&linked_list, dynamic_num);
	

	dump_list(&linked_list);
	printf("\n\n\n\n");

	linked_list_remove_first(&linked_list, NULL);
	linked_list_remove_first(&linked_list, NULL);
	linked_list_remove_first(&linked_list, NULL);
	linked_list_remove_first(&linked_list, NULL);

	if(dynamic_num != NULL)
	{
		printf("freeing dynamic_num\n");
		free(dynamic_num);
		dynamic_num = NULL;
	}

	dump_list(&linked_list);

	linked_list_clear(&linked_list);

	system("PAUSE");
	return 0;
}