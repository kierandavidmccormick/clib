#include <stdio.h>
#include <stdbool.h>

void bubbleSortGen(void *list[], int size, int(*comparator)(void *, void *));
int compareInt(int *a, int *b);
void swap(void **a, void **b);

struct blah {
	char junk[6];
	int val;
};

void printList(void *list[]) {
	for (int i = 0; i < 5; i++) {
		printf("%d, ", (*(struct blah *)list[i]).val);
	}
}

int main(int argc, char *argv[]) {
	int numArray[] = {3, 1, 23945, 0, -3};
	struct blah structArray[] = {{"AAAAA", 3}, {"AAAAA", 1}, {"AAAAA", 23945}, {"AAAAA", 0}, {"AAAAA", -3}};
	void *list[] = {(void *)&structArray[0], (void *)&structArray[1], (void *)&structArray[2], (void *)&structArray[3], (void *)&structArray[4]};
	//bubbleSortGen(list, 4, (int (*)(void *, void *)(compareInt)));
	int (*compPtr)(void *, void *) = &compareInt;
	bubbleSortGen(list, 5, (int (*)(void *, void *))compPtr);
	printList(list);
	printf("\n");
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