#include <stdlib.h>
#include <stdio.h>
#include "dlinked_list.h"

typedef struct
{
	int num1;
	int num2;
} num_t;

bool find_nums(void *list_data)
{
	num_t **num = (num_t *)list_data;
	
	if((*(num))->num1 == 1 && (*(num))->num2 == 2)
	{
		return true;
	}

	return false;
}

void dump_list_forwards(dlinked_list_t *linked_list)
{
	dlist_element_t *element = linked_list->head;
	for(int i = 0; i < linked_list->size; i++)
	{
		num_t *tmp = (num_t *)element->data;
		printf("%d %d\n", tmp->num1, tmp->num2);
		if(NULL != element->next)
		{
			element = element->next;
		}
	}
}

void dump_list_backwards(dlinked_list_t *linked_list)
{
	dlist_element_t *element = linked_list->tail;
	for(int i = linked_list->size; i > 0; i--)
	{
		num_t *tmp = (num_t *) element->data;
		printf("%d %d\n", tmp->num1, tmp->num2);
		if(NULL != element->previous)
		{
			element = element->previous;
		}
	}
}

int main(void)
{
	dlinked_list_t linked_list;
	num_t foo = { 1, 2 };
	num_t foo1 = { 3, 4 };
	num_t foo2 = { 5, 6 };
	num_t foo3 = { 7, 8 };
	num_t foo4 = { 9, 10 };
	num_t foo5 = { 11, 12 };
	dlinked_list_init(&linked_list, NULL);

	dlinked_list_add_first(&linked_list, &foo);
	//printf("foo address: %p\n", &foo);
	dlinked_list_add_first(&linked_list, &foo1);
	dlinked_list_add_first(&linked_list, &foo2);
	dlinked_list_add_last(&linked_list, &foo3);
	dlinked_list_add_last(&linked_list, &foo4);
	printf("dumping forwards\n");
	dump_list_forwards(&linked_list);

	printf("contains searched data: %d\n", dlinked_list_contains(&linked_list, find_nums));
	dlist_element_t *element = dlinked_list_find(&linked_list, find_nums);
	if(element != NULL)
	{
		printf("removing element.\n");
		//printf("element address: %p\n", *element);
		num_t *num_test;
		dlinked_list_remove(&linked_list, element, (void**) &num_test);
		printf("element containing %d %d removed.\n", num_test->num1, num_test->num2);
	}

	//printf("%d %d\n\n", ((num_t *)element->data)->num1, ((num_t *)element->data)->num2);

	dump_list_forwards(&linked_list);
	printf("removing first.\n");
	dlinked_list_remove_first(&linked_list, NULL);
	dump_list_forwards(&linked_list);
	printf("\n");
	dump_list_backwards(&linked_list);

	printf("\nremoving last\n");
	dlinked_list_remove_last(&linked_list, NULL);
	dump_list_forwards(&linked_list);
	printf("\n");
	dump_list_backwards(&linked_list);

	printf("\nadding new numbers to the head.\n");
	dlinked_list_add_first(&linked_list, &foo5);
	dump_list_forwards(&linked_list);
	printf("\n");
	dump_list_backwards(&linked_list);

	printf("\nadding new numbers to the tail.\n");
	dlinked_list_add_last(&linked_list, &foo5);
	dump_list_forwards(&linked_list);
	printf("\n");
	dump_list_backwards(&linked_list);

	printf("clearing list\n");
	dlinked_list_clear(&linked_list);

	system("PAUSE");
	return 0;
}