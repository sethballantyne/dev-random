/* 
 * Example code for shuffling data in an array.
 * Written by Seth B <seth.ballantyne@gmail.com>
 * Do whatever you want with it.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_LENGTH 12

int gen_dest(int source_pos)
{
	int num;

	do
	{
		num = rand() % DATA_LENGTH;
	} while(num == source_pos);

	return num;
}

int main(void)
{
	int data[DATA_LENGTH];

	srand(time(0));

	for(int i = 0; i < DATA_LENGTH; i++)
	{
		data[i] = i;
	}

	int sort_count = 0;
	while(sort_count++ < DATA_LENGTH)
	{
		int source_pos = rand() % DATA_LENGTH;
		int target_pos = gen_dest(source_pos);
		int temp = data[target_pos];
		data[target_pos] = data[source_pos];
		data[source_pos] = temp;
	}

	for(int i = 0; i < DATA_LENGTH; i++)
	{
		printf("%d ", data[i]);
	}

	printf("\nAll done!\n");

#ifdef _DEBUG
	system("PAUSE");
#endif

	return 0;
}