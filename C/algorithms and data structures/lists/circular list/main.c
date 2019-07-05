#include <stdlib.h>
#include <stdio.h>
#include "circular_list.h"

typedef struct
{
	int num1;
	int num2;
} num_t;

void print_list_forwards_forever(circular_list_t *list)
{
	clist_element_t *element = list->head;
	while(NULL != element)
	{
		printf("%d\n", (int)element->data);
		element = element->next;
	}
}

void print_list_backwards_forever(circular_list_t *list)
{
	clist_element_t *element = list->head;
	while(NULL != element)
	{
		printf("%d\n", (int)element->data);
		element = element->previous;
	}
}

void dump_list_forwards(circular_list_t *list, int print_amount)
{
	for(int count = 0; count < print_amount; count++)
	{
		clist_element_t *element = list->head;
		for(int i = 0; i < list->size; i++)
		{
			/*num_t *tmp = (num_t *)element->data;*/
			printf("%d\n", (int)element->data);
			element = element->next;
		}

		printf("\n");
	}
}

void dump_list_backwards(circular_list_t *list, int print_amount)
{
	for(int count = 0; count < print_amount; count++)
	{
		clist_element_t *element = list->head;
		for(int i = list->size - 1; i >= 0; i--)
		{
			/*num_t *tmp = (num_t *)element->data;*/
			printf("%d\n", (int)element->data);
			element = element->previous;
		}

		printf("\n");
	}
	
}

bool compare(void *data)
{
	if(data == (int *)1)
	{
		return true;
	}

	return false;
}

int main(void)
{
	circular_list_t list;

	circular_list_init(&list, NULL);
	circular_list_add_last(&list, 5);
	/*circular_list_add_last(&list, 6);
	circular_list_add_last(&list, 7);
	circular_list_add_last(&list, 8);
	circular_list_add_last(&list, 9);*/

	circular_list_remove_last(&list, NULL);
	//circular_list_remove_last(&list, NULL);
	circular_list_add_first(&list, 1);
	circular_list_add_first(&list, 2);
	circular_list_add_first(&list, 3);
	//print_list_forwards_forever(&list);
	//print_list_backwards_forever(&list);
	dump_list_forwards(&list, 2);

	if(-1 != circular_list_contains(&list, compare))
	{
		printf("Element exists!\n");
	}

	clist_element_t *element = circular_list_find(&list, compare);
	if(element)
	{
		printf("Element found!\n");
	}

	//circular_list_remove(&list, element, NULL);
	circular_list_add_first(&list, 6);
	dump_list_forwards(&list, 1);

	circular_list_destroy(&list);

	//dump_list_backwards(&list, 2);
	//linked_list_clear(&list);
	system("PAUSE");
	return 0;
}