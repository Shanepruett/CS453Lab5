
#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include <pthread.h>
#include <unistd.h>

/*

	list.c 
		Contains functions to manipulate a doubly-linked list.
 
*/

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

/* private methods */
static void print (const NodePtr node, char *(*toString) (const void *));

ListPtr
createList (Boolean (*equals) (const void *, const void *),
	    char *(*toString) (const void *),
	    void (*freeObject) (const void *), int maxSize)
{
  ListPtr list;
  list = (ListPtr) malloc (sizeof (List));
  list->maxSize = maxSize;
  list->size = 0;
  list->head = NULL;
  list->tail = NULL;
  list->equals = equals;
  list->toString = toString;
  list->freeObject = freeObject;
  //pthread_mutex_init(&lock, NULL);

  return list;
}

void
finishUp (const ListPtr list)
{
  // Wait for size of list to fall to zero
  while (TRUE)
    {
      pthread_mutex_lock (&lock);
      if (list->size)
	{
	  pthread_mutex_unlock (&lock);
	  usleep (1000);
	}
      else
	{
	  pthread_mutex_unlock (&lock);
	  break;
	}
    }
  //printf("\n");
}


void
freeList (const ListPtr list)
{
  while (list->head != NULL)
    {
      NodePtr tmp = removeFront (list);
      freeNode (tmp, list->freeObject);
    }
  free (list);
}

int
getSize (const ListPtr list)
{
  return list->size;
}

Boolean
isEmpty (const ListPtr list)
{
  if (list->size == 0)
    return TRUE;
  else
    return FALSE;
}

void
addAtFront (ListPtr list, NodePtr node)
{

  if (list == NULL)
    {
      if (node != NULL)
	{
	  freeNode (node, list->freeObject);
	}
      return;
    }
  if (node == NULL)
    return;



  pthread_mutex_lock (&lock);	// lock
  while (list->size >= list->maxSize)
    pthread_cond_wait (&condition, &lock);
  list->size++;

  if (list->head == NULL)
    {
      list->head = node;
      list->tail = node;
      node->next = node;
      node->prev = node;
    }
  else
    {
      NodePtr tmp = list->head;
      list->head = node;

      node->next = tmp;
      node->prev = list->tail;
      list->tail->next = node;
      tmp->prev = node;
    }
  pthread_mutex_unlock (&lock);	// unlock
}

void
addAtRear (ListPtr list, NodePtr node)
{
  if (list == NULL)
    {
      if (node != NULL)
	{
	  freeNode (node, list->freeObject);
	}
      return;
    }
  if (node == NULL)
    return;

  pthread_mutex_lock (&lock);	// lock 

  while (list->size >= list->maxSize)
    pthread_cond_wait (&condition, &lock);
  list->size++;

  if (list->head == NULL)
    {
      list->head = node;
      list->tail = node;
      node->next = node;
      node->prev = node;

    }
  else
    {
      list->tail->next = node;
      node->next = list->head;
      node->prev = list->tail;
      list->head->prev = node;
      list->tail = node;
    }
  pthread_mutex_unlock (&lock);	// unlock
}

NodePtr
removeFront (ListPtr list)
{

  pthread_mutex_lock (&lock);	// lock
  if (list->size == 0)
    {
      pthread_mutex_unlock (&lock);
      return NULL;
    }
  //printf("rF, ");
  NodePtr tmp = list->head;


  list->head->next->prev = list->tail;
  list->tail->next = list->head->next;
  list->head = list->head->next;

  tmp->next = NULL;
  tmp->prev = NULL;

  list->size--;

  if (list->size == 0)
    {
      list->head = NULL;
      list->tail = NULL;
    }
  pthread_cond_signal (&condition);
  pthread_mutex_unlock (&lock);	// unlock
  return tmp;

}

NodePtr
removeRear (ListPtr list)
{

  pthread_mutex_lock (&lock);	// lock
  if (list->size == 0)
    {
      pthread_mutex_unlock (&lock);
      return NULL;
    }
  //printf("rR, ");

  NodePtr tmp = list->tail;


  list->head->prev = list->tail->prev;
  list->tail->prev->next = list->head;
  list->tail = list->tail->prev;


  tmp->next = NULL;
  tmp->prev = NULL;


  list->size--;

  if (list->size == 0)
    {
      list->head = NULL;
      list->tail = NULL;
    }
  pthread_cond_signal (&condition);
  pthread_mutex_unlock (&lock);	// unlock
  return tmp;
}


NodePtr
removeNode (ListPtr list, NodePtr node)
{
  if (list == NULL)
    return NULL;
  if (node == NULL)
    return NULL;
  pthread_mutex_lock (&lock);	// lock
  printf ("rN, ");
  NodePtr tmp = node;

  if (node == list->head)
    list->head = node->next;
  if (node == list->tail)
    list->tail = node->prev;

  node->next->prev = node->prev;
  node->prev->next = node->next;

  tmp->next = NULL;
  tmp->prev = NULL;

  list->size--;

  if (list->size == 0)
    {
      list->head = NULL;
      list->tail = NULL;
    }
  pthread_mutex_unlock (&lock);	// unlock
  return tmp;

}

NodePtr
search (const ListPtr list, const void *obj)
{
  if (list == NULL)
    return NULL;

  int i;
  pthread_mutex_lock (&lock);	// lock
  NodePtr tmp = list->head;
  for (i = 0; i < list->size; i++)
    {
      if (tmp->obj == obj)
	{

	  return tmp;
	}
      tmp = tmp->next;
    }
  pthread_mutex_unlock (&lock);	// unlock

  return NULL;
}

void
reverseList (ListPtr list)
{
  pthread_mutex_lock (&lock);	// lock
  NodePtr tmp = list->head;
  NodePtr nex = NULL;

  list->tail = list->head;

  int i;
  for (i = 0; i < list->size; i++)
    {
      nex = tmp->next;
      tmp->next = tmp->prev;
      tmp->prev = nex;

      tmp = nex;
    }

  list->head = list->tail->next;
  pthread_mutex_unlock (&lock);	// unlock
}

void
printList (const ListPtr list)
{
  if (list && list->size > 0)
    print (list->head, list->toString);
}

static void
print (const NodePtr node, char *(*toString) (const void *))
{

  //printf("inside printList/printi\n");
  //int count = 0;
  pthread_mutex_lock (&lock);	// lock
  char *output;
  NodePtr temp = node;
  while (temp->next != node)
    {
      output = (*toString) (temp->obj);
      printf ("%s\n", output);
      free (output);
      temp = temp->next;
      //count++;
      //if ((count % 6) == 0)
      //      printf("\n");
    }
  output = (*toString) (temp->obj);
  printf ("%s\n", output);
  free (output);
  pthread_mutex_unlock (&lock);	// unlock
}
