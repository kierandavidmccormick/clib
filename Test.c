#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct linkedListElement {
	struct linkedListElement *next;
	void *value;
} ll_element;

typedef struct linkedListHead {
	int size;
	struct linkedListElement *first;
	struct linkedListElement *last;
	void (*dealloc)(void *);
} ll_head;

//TODO: make things that need to be static or private static or private
//TODO: get my debug lib, use

void bubbleSortGen(void *list[], int size, int(*comparator)(void *, void *));
int compareInt(int *a, int *b);
void swap(void **a, void **b);
void printList(ll_head *list);
ll_head *ll_create(void (*dealloc)(void *));
ll_element *ll_getIndex(int index, ll_head *head);
ll_element **ll_getPrevPtr(int index, ll_head *head);
void ll_destroy(ll_head *head);
int ll_addElement(void *element, int index, ll_head *head);
int ll_removeElement(int index, ll_head *head);

//creates a new linked list in memory; make sure to free before termination
ll_head *ll_create(void (*deallocFn)(void *)) {
	ll_head *list = malloc(sizeof(ll_head));
	list->size = 0;
	list->first = NULL;
	list->last = NULL;				//TODO: use, implement, or remove
	list->dealloc = deallocFn;
	return list;
}

//returns pointer to numbered index, or null if invalid
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

//returns pointer of previous element to element at index (or head)
ll_element **ll_getPrevPtr(int index, ll_head *head) {
	if (index < 0 || index > head->size) {
		return NULL;
	}
	ll_element **elementptr = &(head->first);
	for (int i = 1; i <= index; i++) {
		elementptr = &((*elementptr)->next);
	}
	return elementptr;
}

//destroys the list; deallocates all memory used
void ll_destroy(ll_head *head) {
	while(head->size > 0) {
		ll_removeElement(0, head);
	}
	free(head);
}

//creates new entry from pointer and adds to list
int ll_addElement(void *element, int index, ll_head *head) {
	ll_element **prevptr = ll_getPrevPtr(index, head);
	if (prevptr == NULL) {
		return 1;
	}

	ll_element *nextptr = *prevptr != NULL ? (*prevptr) : NULL;
	ll_element *newElement = malloc(sizeof(ll_element));
	newElement->next = nextptr;
	newElement->value = element;
	*prevptr = newElement;

	head->size++;
	return 0;
}

//removes entry from list; deallocates memory
int ll_removeElement(int index, ll_head *head) {
	ll_element **prevptr = ll_getPrevPtr(index, head);
	if (prevptr == NULL) {
		return 1;
	}
	ll_element *nextptr = *prevptr != NULL ? (**prevptr).next : NULL;
	ll_element *delptr = &(**prevptr);
	//printf("Freed element\n");
	head->dealloc(delptr->value);
	free (delptr);
	*prevptr = nextptr;
	head->size--;
	return 0;
}

void *getElement(int index, ll_head *head) {
	return ll_getIndex(index, head)->value;
}

//destroys a linked list stored in another data structure
void llRecDestructor(void *target) {
	ll_destroy((ll_head *)target);
}

//destroys a basic object (struct, primitive, etc) stored in a data structure
void basicDestructor(void *target) {
	free(target);
}

//used when a destructor is needed but the contents do not need to be destroyed (are in stack, destroyed elsewhere, etc)
void stubDestructor(void *target) {
	//do nothing
}












int main(int argc, char *argv[]) {
	//for sorting
	//int numArray[] = {3, 1, 23945, 0, -3};
	//void *list[] = {(void *)&numArray[0], (void *)&numArray[1], (void *)&numArray[2], (void *)&numArray[3], (void *)&numArray[4]};
	//int (*compPtr)(void *, void *) = &compareInt;
	//bubbleSortGen(list, 5, (int (*)(void *, void *))compPtr);
	//printList(list);
	//printf("\n");

	int a = 1;
	int b = 2;
	int c = 3;

	ll_head *head1 = ll_create(&stubDestructor);

	ll_addElement((void *)&a, 0, head1);
	ll_addElement((void *)&b, 1, head1);
	ll_addElement((void *)&c, 0, head1);

	int d = 4;
	int e = 5;

	ll_head *head2 = ll_create(&stubDestructor);

	ll_addElement((void *)&d, 0, head2);
	ll_addElement((void *)&e, 1, head2);

	int *f = malloc(sizeof(int));
	*f = 6;
	int *g = malloc(sizeof(int));
	*g = 7;

	ll_head *head3 = ll_create(&basicDestructor);

	ll_addElement((void *)f, 0, head3);
	ll_addElement((void *)g, 1, head3);

	ll_head *head0 = ll_create(&llRecDestructor);

	ll_addElement((void *) head1, 0, head0);
	ll_addElement((void *) head2, 0, head0);
	ll_addElement((void *) head3, 0, head0);

	ll_destroy(head0);

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