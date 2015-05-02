
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "buddy.h"

#define MAX_MEM_SIZE 34359738368 // 32 Gigabytes
#define HIGH_EXPONENT 34 //2^35 = 32 Gigabytes
#define LOW_EXPONENT 5 // 2^5 = 32 bytes

typedef struct { // size of is 24 bytes
	unsigned tag;
	unsigned kval;
	void* prev;
	void* next;
} block;



static void *BASE = NULL;

static block *AVAIL[HIGH_EXPONENT + 1];

static unsigned MAX;


void split (unsigned k);
unsigned get_k(size_t size);



void buddy_init(size_t size)
{
	// sbrk for max size

	BASE = sbrk (1UL << HIGH_EXPONENT);
	if (BASE < 0 || errno == ENOMEM){
		perror("Could not allocate memory");
		errno = ENOMEM;
		exit(1);
	}
	
	void *end = sbrk(0);
	// initialize lists up to HIGH_EXPONENT (0 - m)
	unsigned m = 0;
	unsigned long long actualSize = (end - BASE);
	unsigned long long calculatedSize = 0;
	//calculatedSize = calculatedSize << 1;
	//printf("Init: actualSize = %llu, hex = 0x%llx \n", actualSize, actualSize);
	while ( actualSize > calculatedSize && m < 64 ){
		calculatedSize = (unsigned long long) 1 << ++m;
	//	printf("size? = %llx, last? %i\n", calculatedSize, actualSize >= 1 << k);
	} 		
	//printf ("&Base = %p, &end = %p\n", (void*)BASE,(void*) end);
	//printf ("Init: Size of block = %u\n", m );
	
	// Now have k value for block size.
	// Initialize Lists of m + 1 size
	MAX = m;
//	unsigned arraySize =  (m+1) * sizeof(block*);
	

//	AVAIL = sbrk(arraySize);
//	if (AVAIL < 0 || errno == ENOMEM){
//		perror("Could not allocate memory for AVAIL");
//		exit(1);
//	}

	//printf("Init: SizeofAVAIL= %ld\n", sbrk(0) - (void*)AVAIL);

	// Add block of max size to the mth list
	AVAIL[m] = BASE;
	AVAIL[m]->tag = 1;
	AVAIL[m]->kval = m;
	AVAIL[m]->next = NULL;
	AVAIL[m]->prev = NULL; 
	
}

void *buddy_calloc(size_t num, size_t size)
{
	if (num == 0 || size == 0) return NULL;

	// allocate block of memory num * size bytes
	if (BASE == NULL) buddy_init((size_t) MAX_MEM_SIZE);

	void* rval = buddy_malloc(num * size);
	// MUST INITIALIZE NEW DATA TO ZERO
	// USE memset(address, 0 , number of bytes);
	memset(rval, 0, num*size); 


	return rval;
}

void *buddy_malloc(size_t size)
{
	if (BASE == NULL) buddy_init((size_t) MAX_MEM_SIZE);

	// loop to find size of k
	unsigned k = get_k(size);
	//printf("b_malloc:  k= %u\n", k); 

	void *rval = NULL;
	
	// if kth list is null, split on k+1 if its not the max k
	if (k < MAX && AVAIL[k] == NULL){
	//	printf("b_malloc: Splitting on %u\n", k+1);
		split(k+1);
	}
	if (AVAIL[k] != NULL){
	//	printf("b_malloc: AVAIL[%u] not null\n", k);
	//	printf("b_malloc: AVAIL[%u] tag=%u\n", AVAIL[k]->kval , AVAIL[k]->tag);

		block *tmp = AVAIL[k];
		AVAIL[k] = AVAIL[k]->next;
		if ( AVAIL[k] != NULL) AVAIL[k]->prev = NULL;
		tmp->prev = NULL;
		tmp->next = NULL;
		tmp->tag = 0;
		tmp++;
		rval = (void*) tmp;
	}
	else {
		perror("Couldn't allocate memory, in buddy_malloc");
	}
	//printf("b_malloc: End of bmalloc\n");
	return rval;
}

void *buddy_realloc(void *ptr, size_t size)
{
	if (ptr == NULL){
		return  buddy_malloc(size);
	}

	if (size == 0) {
		buddy_free(ptr);
		return ptr;
	}

	if (BASE == NULL) buddy_init(MAX_MEM_SIZE);

	block* TMP = (block*) ptr;
	TMP--;
	
//	size_t settingSize;

	size_t oldSize = (1 << TMP->kval) - sizeof(block);

//	if (oldSize > size)
//		settingSize = size;
//	else
//		settingSize = oldSize; 

	void* rval;

	if (oldSize != size) {
		rval = buddy_malloc(size);
		memcpy(rval, ptr, size);
		buddy_free(ptr);		
	}
	else {
		rval = ptr;
	}

	return rval;
}

void buddy_free(void *ptr)
{	
	if (ptr == NULL) return;
	//printf("b_free: Start of free\n");

	block *TMP = (block*) ptr;
	TMP--;
	
	//if (TMP->tag) {
	//	perror("Attempting to free already freed pointer\n");
	//	exit(1);
	//}	
	//printf("b_free: Block->tag=%u, k=%u, &=%p\n", TMP->tag, TMP->kval, TMP);	

	unsigned buddy = 0;
	block* BUD = NULL;
	unsigned k = TMP->kval;

	if (k < MAX){
		//unsigned long x = (void*) TMP - BASE;
		//int addsub = 1;
		//if (x % (1<<(k+1)) == (1<<k))
		//	addsub = -1; 
		//if (addsub)
		//	BUD = (block*) ((((void*)TMP - BASE) + ((size_t)1<<k)) + BASE);
		//else
		BUD = (block*) ((((size_t)TMP - (size_t)BASE) ^ ((size_t)1<<k)) + (size_t) BASE );

		//BUD = (block*) ( ( ( (void*)TMP - BASE) ^ ( (size_t)1 << k) ) + BASE) ;
		//printf("b_free: exor=%zx\n", ( ( (void*)TMP - BASE) ^ ( (size_t)1 << k) )  );
		buddy = BUD->tag;
		//printf("b_free: buddy?=%u\n", buddy);
	}	
	if (buddy){
		block *PREV= BUD->prev;

		//printf("b_free: AV=%p T=%p B=%p P=%p\n", AVAIL[k],TMP, BUD, PREV); 
		if (BUD == AVAIL[k]){
			//printf("b_free: BUD was head\n");
			AVAIL[k] = BUD->next;
		}


		if (PREV == NULL){
			//printf("b_free: BUD PREV was NULL\n");
			AVAIL[k] = BUD->next;	
		}
		else {
			PREV->next = BUD->next;
		}
		block *NEXT = BUD->next;
		if (NEXT != NULL)
			NEXT->prev = BUD->prev;
		
		//printf("b_free: Buddy Available!\n");
		// Find the address that is smallest
		//block* MERGE = (block*) ((((void*) TMP - BASE) & ~((size_t) 1 << k )) + BASE);
		if (BUD < TMP) 
			TMP = BUD;
		//block *BUD = (block*) ((((void*)TMP - BASE) ^ (size_t) 1 << TMP->kval) + BASE);
		//printf("b_free: Mask=%zx, k=%u\n",((size_t) 1 << (TMP->kval)), TMP->kval);	
		//printf("b_free: Merged&=%p k=%u, T&=%p, B&=%p k=%u\n", MERGE, k+1, TMP, BUD, k);
		
		//if ( AVAIL[k] == BUD ){
		//	printf("AVL=%p  == BUD=%p\n", AVAIL[k], BUD);
		//	AVAIL[k] = AVAIL[k]->next;
		//	if (AVAIL[k] != NULL) 
		//		AVAIL[k]->prev = NULL;
		//}
		//printf("b_free: after fixing head\n");
		//block* FIX = AVAIL[k];
		//while (FIX != NULL && FIX->next != NULL){
		//	if ( FIX->next == BUD){
		//		FIX->next = BUD->next;
		//		if (BUD->next != NULL) {
		//			printf("b_free: BUD next isn't null\n");
		//			block* nxt =  BUD->next;
		//			nxt->prev = FIX;
		//		}
		//	}		
		//	FIX = FIX->next;
		//}
		//printf("b_free: after fixing list\n");
		//TMP->tag = 0;
		//TMP->next = NULL;
		//TMP->prev = NULL;
		//BUD->tag = 0;
		//BUD->next = NULL;
		//BUD->prev= NULL;
		k++;
		TMP->kval = k;
		TMP->tag = 0;
		//MERGE->tag = 1;
		//MERGE->kval = k;
		//MERGE->next = NULL;
		//MERGE->prev = NULL;
		//MERGE++;
		TMP++;
		buddy_free( (void*) TMP);
		
	}
	else {

		//TMP->tag = 1;
		//TMP->next = 



		//printf("b_free: no buddy\n");
		block *curr = (block*) AVAIL[k];
		//printf("b_free: curr=%p k=%u, TMP&=%p k=%u\n", curr, k, TMP, TMP->kval);
		if (curr != NULL){
			while (curr->next != NULL){
				curr = curr->next;
			}	
			curr->next = TMP;
		}
		else {
			AVAIL[k] = TMP;
		}
		TMP->kval = k;
		TMP->prev = curr;
		TMP->next = NULL;
		TMP->tag = 1;
	
	}
	
	//printf("b_free: End of buddy free\n");
}


/*
 * Returns the k for that size
 */
unsigned get_k (size_t size)
{
	//printf("get_k: Start of getk\n");
	unsigned k = LOW_EXPONENT;

	size_t calcSize = (1<<k) - sizeof(block);
	//if (!calcSize) printf ("get_k: k is too small\n");
	//printf("actualSize = %llu, hex = 0x%llx \n", actualSize, actualSize);
	while ( size > calcSize && k < 64 ){
		calcSize = ((size_t) 1 << ++k ) - sizeof(block);
	//	printf("calcSize = %lu\n", calcSize);
	} 		
	//printf("get_k: End of getk\n");
	return k;
}


/*
 *
 * Split a node.
 *
 */
void split(unsigned k)
{
	//printf("split: Start of split k=%u\n", k); 
	
	if (AVAIL[k] == NULL && k+1 <= MAX ){
	//	printf("split: splitting on %u\n", k+1);
		split (k + 1);
	}
	if (AVAIL[k] != NULL){
	//	printf("split: AVAIL[k] isn't NULL");
		block *tmp = AVAIL[k];
		AVAIL[k] = AVAIL[k]->next;
		if (AVAIL[k] != NULL){
			AVAIL[k]->prev = NULL;
		}
	//	printf(" 1 \n");
		AVAIL[k-1] = tmp;
	//	printf(" 2 \n");
		AVAIL[k-1]->tag = 1;
	//	printf(" 3 \n");
		AVAIL[k-1]->kval = k-1;
	//	printf(" 4 \n");
		AVAIL[k-1]->prev = NULL;
	//	printf(" 5 \n");
	//	(block*) ((((void*)TMP - BASE) + ((size_t)1<<k)) + BASE);
		block *SEC = (block*) ((((void*)AVAIL[k-1] - BASE) + ((size_t) 1 << (k-1))) + BASE) ;
		AVAIL[k-1]->next = SEC;
	//	printf(" 6 \n");
		SEC->tag = 1;
	//	printf(" 7 \n");
		SEC->kval = k-1;
		///g=1,kval=13,addr=0x1ee8018] --> [tag=1,kval=13,addr=0x1ea2018] --> <null>
	//	printf(" 8 \n");
		SEC->next = NULL;
	//	printf(" 9 \n");
		SEC->prev = AVAIL[k-1];
	//	printf("Split: AVAIL[%u] &=%p, AVAIL[%u]->next &%p\n", k-1, AVAIL[k-1], k-1,  SEC);
	}
	//printf("Split: Finished split at k= %u\n", k);
}

void printBuddyLists(void)
{

	unsigned k = 0;
	block* curr = NULL;
	printf("\n");
	while ( k <= MAX ) {
		printf("List %u: head = %p --> ", k, AVAIL+k);
		curr = AVAIL[k];
		while (curr != NULL){
			printf("[tag=%u,kval=%u,addr=%p] --> ", curr->tag, curr->kval, curr+1);
			curr = curr->next;
		}		
		printf("<null>\n");
		k++;
	}
}



