#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

typedef struct
{
	int num1;
	int num2;
} num_t;

//void print_list_forwards_forever(circular_list_t *list)
//{
//	clist_element_t *element = list->head;
//	while(NULL != element)
//	{
//		printf("%d\n", (int)element->data);
//		element = element->next;
//	}
//}

//void print_list_backwards_forever(circular_list_t *list)
//{
//	clist_element_t *element = list->head;
//	while(NULL != element)
//	{
//		printf("%d\n", (int)element->data);
//		element = element->previous;
//	}
//}

//void dump_list_forwards(circular_list_t *list, int print_amount)
//{
//	for(int count = 0; count < print_amount; count++)
//	{
//		clist_element_t *element = list->head;
//		for(int i = 0; i < list->size; i++)
//		{
//			/*num_t *tmp = (num_t *)element->data;*/
//			printf("%d\n", (int)element->data);
//			element = element->next;
//		}
//
//		printf("\n");
//	}
//}

//void dump_list_backwards(circular_list_t *list, int print_amount)
//{
//	for(int count = 0; count < print_amount; count++)
//	{
//		clist_element_t *element = list->head;
//		for(int i = list->size - 1; i >= 0; i--)
//		{
//			/*num_t *tmp = (num_t *)element->data;*/
//			printf("%d\n", (int)element->data);
//			element = element->previous;
//		}
//
//		printf("\n");
//	}
//	
//}

//bool compare(void *data)
//{
//	if(data == (int *)1)
//	{
//		return true;
//	}
//
//	return false;
//}

int main(void)
{
	stack_t stack;
	
	stack_init(&stack, NULL);
	stack_push(&stack, (int *) 1);
	stack_push(&stack, (int *) 2);
	stack_push(&stack, (int *) 3);

	for(int i = 0; i < 3; i++)
	{
		int *num = (int *)stack_peek(&stack);
		printf("Peek: %d\n", num);
		stack_pop(&stack, &num);
		printf("Pop: %d\n", num);
	}

	system("PAUSE");
	return 0;
}