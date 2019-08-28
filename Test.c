#include <stdio.h>
#include <stdbool.h>

void bubbleSortGen(void *list[], int size, int(*comparator)(void *, void *));
int compareInt(int *a, int *b);
void swap(void **a, void **b);
void printList(void *list[]);


int main(int argc, char *argv[]) {
	//for sorting
	//int numArray[] = {3, 1, 23945, 0, -3};
	//void *list[] = {(void *)&numArray[0], (void *)&numArray[1], (void *)&numArray[2], (void *)&numArray[3], (void *)&numArray[4]};
	//int (*compPtr)(void *, void *) = &compareInt;
	//bubbleSortGen(list, 5, (int (*)(void *, void *))compPtr);
	//printList(list);
	//printf("\n");
	return 0;
}

//performs bubbleSort on generic array, using generic comparator
//takes in array of pointers to elements, array size, and pointer to comparator function
//TODO: replace with quicksort or something to make it reasonably efficient
void bubbleSortGen(void *list[], int size, int (*comparator)(void *, void *)) {
	if (size < 2) {
		return;
	}
	bool swapped;
	do {
		swapped = false;
		for (int i = 0; i < size - 1; i++) {
			if ((*comparator)(list[i], list[i+1]) < 0){
				//printf("Swapping %d and %d:   ", *(int *)list[i], *(int *)list[i+1]);
				swap(&list[i], &list[i+1]);
				swapped = true;
				//printList(list);
				//printf("\n");
			}
		}
	} while (swapped == true);
	//printf("Final List:   ");
	//printList(list);
	//printf("\n");
}

//if a and b are not ints, c will stubbornly insist that they are and perform the bitwise comparison identically
int compareInt(int *a, int *b) {
	return *a - *b;
}

void swap(void **a, void **b) {
	void *temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void printList(void *list[]) {
	for (int i = 0; i < 5; i++) {
		printf("%d, ", (*(struct blah *)list[i]).val);
	}
}