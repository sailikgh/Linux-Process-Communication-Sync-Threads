#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned n;
unsigned k;
unsigned faultCount;

typedef struct Node
{
	struct Node *prev, *next;
	unsigned pageNo;
} Node;

typedef struct List
{
	unsigned count;
	unsigned capacity;
	Node *head, *tail;	
} List;

typedef struct Pointers
{
	int cap;
	Node* *array;
} Pointers;

Node* newNode(unsigned pageNumber){
	Node* temp = (Node*)malloc(sizeof(Node));
	temp->pageNo = pageNumber;
	temp->prev = temp->next = NULL;
	return temp;
}

List* createList(int frames){
	List* list = (List*)malloc(sizeof(List));
	list->count = 0;
	list->head = list->tail = NULL;
	list->capacity = frames;
	return list;
}

Pointers* createPointers(int capacity){
	Pointers* pointers = (Pointers*)malloc(sizeof(Pointers));
	pointers->cap = capacity;

	pointers->array = (Node**)malloc(pointers->cap * sizeof(Node*));

	for (int p = 0; p < pointers->cap; p++){
		pointers->array[p] = NULL;
	}

	return pointers;
}

int isFull(List* l1){
	return l1->count == l1->capacity;
}

int isEmpty(List* l2){
	return l2->tail == NULL;
}

void out(List* list){
	if (isEmpty(list)){
		return;
	}

	if (list->head == list->tail){
		list->head = NULL;
	}

	Node* temp = list->tail;
	list->tail = list->tail->prev;

	if (list->tail){
		list->tail->next = NULL;
	}

	free(temp);
	list->count--;
}

void in(List* list, Pointers* pointers, unsigned pageNumber){
	if (isFull(list)){
		pointers->array[list->tail->pageNo] = NULL;
		out(list);
	}

	Node* temp = newNode(pageNumber);
	temp->next = list->head;

	if (isEmpty(list)){
		list->tail = list->head = temp;
	}
	else{
		list->head->prev = temp;
		list->head = temp;
	}

	pointers->array[pageNumber] = temp;
	list->count++;
}

void pageTrack(List* list, Pointers* pointers, unsigned pageNumber){
	Node* reqPage = pointers->array[pageNumber];

	if (reqPage == NULL){
		in(list, pointers, pageNumber);
		faultCount++;
	}
	else if (reqPage != list->head){
		reqPage->prev->next = reqPage->next;
		if (reqPage->next){
			reqPage->next->prev = reqPage->prev;
		}

		if (reqPage == list->tail){
			list->tail = reqPage->prev;
			list->tail->next = NULL;
		}

		reqPage->next = list->head;
		reqPage->prev = NULL;

		reqPage->next->prev = reqPage;

		list->head = reqPage;
	}
}

unsigned lru(unsigned trace[], int f){

	List* l = createList(f);
	Pointers* p = createPointers(k);
	faultCount = 0;
	
	for (int t = 0; t < n; t++){
		pageTrack(l, p, trace[t]);
	}

	return faultCount;
}

int main(int argc, char* argv[]){

	if (argc != 3){
		printf("Insufficient parameters passed.\n");
		return 1;
	}
	else{
		n = atoi(argv[1]);
		k = atoi(argv[2]);

		unsigned trace[n];
		unsigned faultCount[k-3];
		srand(time(0));

		for (int i = 0; i < n; i++){
			trace[i] = (rand() % k);
		}

		for (int f = 4; f <= k; f++){
			faultCount[f - 4] = lru(trace, f);
		}
		
		FILE *fptr;
		fptr = fopen("lab8.csv", "w");
		if (fptr == NULL){
			printf("Error in opening file\n");
			exit(1);
		}
		fprintf(fptr, "f,Fault Count\n");
		for (int f = 4; f <= k; f++){
			fprintf(fptr, "%d,%u\n", f, faultCount[f-4]);
		}

	}

	return 0;
}



