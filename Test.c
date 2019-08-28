#include <stdio.h>
#include <stdbool.h>

void bubbleSortGen(void *list[], int size, int(*comparator)(void *, void *));
int compareInt(int *a, int *b);
void swap(void **a, void **b);
void printList(void *list[]);

typedef struct linkedListElement {
	struct ll_element *next;
	void *value;
} ll_element;

typedef struct linkedListHead {
	int size;
	struct ll_element *first;
	struct ll_element *last;
} ll_head;

//creates a new linked list in memory; make sure to free before termination
ll_head *ll_create() {
	ll_head *list = malloc(sizeof(ll_head));
	list.size = 0;
	list.first = 0;
	list.last = 0;
	return list;
}

ll_element *ll_getIndex(int index, ll_head *head) {
	if (index < 0 || index > head->size - 1) {
		return 0;
	}
	ll_element *elementptr = head->first;
	for (int i = 0; i < index; i++) {
		elementptr = elementptr->next;
	}
	return elementptr;
}

ll_element **ll_getPrevPtr(int index, ll_head *head) {
	if (index < 0 || index > head->size) {
		return 0;
	}
	ll_element **elementptr = &(head->first);
	for (int i = 0; i =< index; i++) {
		elementptr = &((**elementptr).next);		//can be -> ?
	}
	return elementptr;
}

void ll_destroy(ll_head *head) {
	while(size > 0) {
		ll_removeElement(0, head);
	}
	free(*head);
}

int ll_addElement(void *element, ll_head *head) {
	return ll_addElement(element, 0, head);
}

int ll_addElement(void *element, int index, ll_head *head) {
	ll_element **prevptr = ll_getPrevPtr(index, head);
	if (prevptr == 0) {
		return 1;
	}
	ll_element *nextptr = *prevptr != 0 ? (**prevptr) : 0;

	ll_element *newElement = malloc(sizeof(ll_element));
	newElement->next = nextptr;
	newElement->value = value;
	*prevptr = newElement;

	head->size--;
	return 0;
}

//TODO: deallocate element contents as well; how to do with nested lists
//solution: supply a pointer to a destructor function that takes a void * pointing to the relevant object and destroys it as necessary; add to the whole list
int ll_removeElement(int index, ll_head *head) {
	ll_element **prevptr = ll_getPrevPtr(index, head);
	if (prevptr == 0) {
		return 1;
	}
	ll_element *nextptr = *prevptr != 0 ? (**prevptr).next : 0;
	free (**prevptr)
	*prevptr = nextptr;
	head->size--;
	return 0;
}













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