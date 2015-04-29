
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "buddy.h"

#define MAX_MEM_SIZE 34359738368; // 32 Gigabytes
#define HIGH_EXPONENT 35; //2^35 = 32 Gigabytes
#define LOW_EXPONENT 5; // 2^5 = 16 bytes

struct block{ // size of is 32 bytes
	unsigned TAG;
	unsigned kval;
	void* LINKF;
	void* LINKB;
};

static void *mem = NULL

static struct node *find_next(unsigned userSize);
static struct node *split (unsigned userSize, struct node *curr);



void buddy_init(size_t size)
{
	// sbrk for max size
	

	void *pool = sbrk (1 << MAX_EXPONENT);
	if (pool < 0 || errno == ENOMEM){
		perror("Could not allocate memory");
		exit(1);
	}

	// initialize lists up to HIGH_EXPONENT (0 - m)

	// Add block of max size to the mth list

}

void *buddy_calloc(size_t, size_t)
{
	if (mem == NULL) buddy_init((size_t) MAX_MEM_SIZE);


}

void *buddy_malloc(size_t size)
{
	if (mem == NULL) buddy_init((size_t) MAX_MEM_SIZE);

	// loop to find size of k
	unsigned k = get_k(size);

	// search kth list, if null, split on k+1


}

void *buddy_realloc(void *ptr, size_t size)
{



}

void buddy_free(void *)
{




}


/*
 * Returns the k for that size
 */
unsigned get_k (size_t size)
{

	unsigned k = 0;

	while (size <  ((1 << k) ) + sizeof(block) ) {
		k++;
	}

	return k;
}


/*
 *
 * Find the next node with user specified size
 *
 */
static struct node *find_next(unsigned k)
{
	

}


/*
 *
 * Split a node.
 *
 */
void split(unsigned k)
{




}




