#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <buddy.h>







int main(int argc, char *argv[])
{

	//buddy_init((size_t) 0 );

	void* ptr1 = buddy_malloc( (size_t) 1 << 22);
	void* ptr2 = buddy_malloc( (size_t) 1 << 12);

	printf("ptr1 &=%p, ptr2 &=%p\n", ptr1, ptr2);

	void* ptr3 = buddy_calloc( 4, 32);
	void* ptr4 = buddy_calloc( 0 , 5 );

	printf("ptr3 &=%p, ptr4 &=%p\n", ptr3, ptr4);

	ptr1 = buddy_realloc(ptr1, 1 << 30);
	ptr4 = buddy_realloc(ptr4, 1024);
	ptr2 = buddy_realloc(ptr2, 0);


	printf("ptr1 &=%p, ptr2 &=%p\n", ptr1, ptr2);
	printf("ptr3 &=%p, ptr4 &=%p\n", ptr3, ptr4);

	buddy_free(ptr1);	
	buddy_free(ptr2);

	buddy_free(ptr3);
	buddy_free(ptr4);


	printf("ptr1 &=%p, ptr2 &=%p\n", ptr1, ptr2);
	printf("ptr3 &=%p, ptr4 &=%p\n", ptr3, ptr4);

	exit(0);
}

