#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "debuglib.h"

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

typedef struct arrayListHead {
	int size;
	int capacity;		//make sure that this is never negative
	void **arrptr;
	void (*dealloc)(void *);
} al_head;

//TODO: make things that need to be static or private static or private
//TODO: get my debug lib, use

void bubbleSortGen(void *list[], int size, int(*comparator)(void *, void *));
int compareInt(int *a, int *b);
void swap(void **a, void **b);
void printList(al_head *list);

ll_head *ll_create(void (*dealloc)(void *));
ll_element *ll_getIndex(int index, ll_head *head);
ll_element **ll_getPrevPtr(int index, ll_head *head);
void ll_destroy(ll_head *head);
int ll_addElement(void *element, int index, ll_head *head);
int ll_removeElement(int index, ll_head *head);

al_head *al_create(int capacity, void (*deallocFn)(void *));
void al_destroy(al_head *head);
void al_destroyList(al_head *head);
int al_add(void *elem, int index, al_head *head);
int al_remove(int index, al_head *head);
void al_handleShift(int neededCapacity, int index, bool add, al_head *head);
void *al_ensureCapacity(int neededCapacity, al_head *head);

void llArrayListDestructor(void *target);
void llRecDestructor(void *target);
void basicDestructor(void *target);
void stubDestructor(void *target);

al_head *al_create(int capacity, void (*deallocFn)(void *)) {
	if (capacity < 1) {
		return NULL;
	}
	al_head *head = malloc(sizeof(al_head));
	head->size = 0;
	head->capacity = capacity;
	head->arrptr = malloc(sizeof(void *) * capacity);
	head->dealloc = deallocFn;
	return head;
}

void al_destroy(al_head *head) {
	al_destroyList(head);
	free(head->arrptr);
	free(head);
}

void al_destroyList(al_head *head) {
	for (int i = 0; i < head->size; i++) {
		head->dealloc(head->arrptr[i]);
	}
}

int al_add(void *elem, int index, al_head *head) {
	debug3("=> al_add", *(int *)elem, index, (int)head);
	if (index < 0 || index > head->size) {
		debug0("<= al_add RETURN NULL");
		return 1;
	}
	al_handleShift(head->size + 1, index, true, head);
	head->arrptr[index] = elem;
	debug3("== al_add", (int)elem, (int)head->arrptr[index], *(int *)head->arrptr[index]);
	head->size++;
	debug0("<= al_add");
	return 0;
}

int al_remove(int index, al_head *head) {
	if (index < 0 || index >= head->size) {
		return 1;
	}
	al_handleShift(head->size - 1, index, false, head);
	head->size--;
	return 0;
}

void movemem(void *destination, void *source, size_t nbytes) {
	char *destbytes = (char *)destination;
	char *sourcebytes = (char *)source;
	char buffer[nbytes];
	for (int i = 0; i < nbytes; i++) {
		buffer[i] = sourcebytes[i];
	}
	for (int i = 0; i < nbytes; i++) {
		destbytes[i] = buffer[i];
	}
	debug0("<= movemem");
}

//deals with resizing the array and making space for or removing and element with the minimum number of shifts
//in dire need of simplification due to complexity
void al_handleShift(int neededCapacity, int index, bool add, al_head *head) {
	debug4("=> al_handleShift", neededCapacity, index, add, (int)head);
	if (index < 0) {
		debug0("<= al_handleShift RETURN EARLY");
		return;
	}
	void **newArr = al_ensureCapacity(neededCapacity, head);
	bool changedArray = newArr != head->arrptr;
	debug3("== al_handleShift", changedArray, (int)head->arrptr, (int)newArr);
	if (changedArray) {
		debug1("== al_handleShift full copy of", head->size);
		//memcpy(newArr, head->arrptr, sizeof(void *) * head->size);
		memcpy(newArr, head->arrptr, sizeof(void *) * index);
		if (index < head->size) {
			memcpy(newArr + index + 1, head->arrptr + index, sizeof(void *) * (head->size - index));
		}
	} else if (!add && index < head->size - 1) {
		debug2("== al_handleShift shifting down", head->size - (index + 1), index + 1);
		memmove(head->arrptr + index, head->arrptr + index + 1, sizeof(void *) * (head->size - (index + 1)));
	} else if (add && index < head->size) {
		debug2("== al_handleShift shifting up", index, head->size - index);
		memmove(head->arrptr + index + 1, head->arrptr + index, sizeof(void *) * (head->size - index));
		//head->arrptr[index + 1] = head->arrptr[index];
		//movemem(head->arrptr + 1, head->arrptr, sizeof(void *) * (head->size - index));
	} else {
		debug0("== al_handleShift no shift needed");
	}
	if (changedArray) {
		debug0("== al_handleShift destroying old array");
		al_destroyList(head);									//need to deallocate elements of old list before deallocating old list
		free(head->arrptr);
	}
	head->arrptr = newArr;
	debug1("<= al_handleShift", (int)head->arrptr);
}

void *al_ensureCapacity(int neededCapacity, al_head *head) {
	debug3("=> al_ensureCapacity", neededCapacity, head->capacity, (int)head);
	if (head->capacity >= neededCapacity) {
		debug0("<= al_ensureCapacity adequate");
		return head->arrptr;												//don't want to create a new block of memory if existing capacity works fine
	}
	int newCapacity = head->capacity;
	while (newCapacity < neededCapacity) {
		newCapacity = (newCapacity * 3) / 2 + 1;			//copying behavior from java version; they probably know what they are doing
	}
	head->capacity = newCapacity;
	debug1("== al_ensureCapacity", newCapacity);
	debug0("<= al_ensureCapacity");
	return malloc(sizeof(void *) * newCapacity);
}

void printList(al_head *list) {
	debug3("=> printList", (int)list, list->size, list->capacity);
	debug2("== printList", (int)list->arrptr, (int)list->arrptr[0])
	printf("size: %d, capacity: %d, contents: ", list->size, list->capacity);
	//debug0("== printList");
	for (int i = 0; i < list->size; i++) {
		//debug2("== printList", i, (int)list->arrptr[i]);
		printf("(%d, ", (int)(list->arrptr[i]));
		printf("%d) ", *(int *)(list->arrptr[i]));
	}
	printf("\n");
	debug0("<= printList");
}










int main(int argc, char *argv[]) {

	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	int e = 5;

	debug0("=> main");

	al_head *head = al_create(2, &stubDestructor);

	al_add((void *)&a, 0, head);
	printList(head);
	al_add((void *)&b, 1, head);
	printList(head);
	al_add((void *)&c, 2, head);
	printList(head);
	al_add((void *)&d, 2, head);
	printList(head);
	al_add((void *)&e, 0, head);

	debug3("== main", (int)head, head->size, head->capacity);

	printList(head);

	debug0("== main");

	al_remove(4, head);
	al_remove(0, head);
	al_remove(1, head);

	printList(head);

	//for sorting
	//int numArray[] = {3, 1, 23945, 0, -3};
	//void *list[] = {(void *)&numArray[0], (void *)&numArray[1], (void *)&numArray[2], (void *)&numArray[3], (void *)&numArray[4]};
	//int (*compPtr)(void *, void *) = &compareInt;
	//bubbleSortGen(list, 5, (int (*)(void *, void *))compPtr);
	//printList(list);
	//printf("\n");

	/*

	for linked list

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
	*/



	return 0;
}

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

void llArrayListDestructor(void *target) {
	al_destroy((al_head *) target);
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