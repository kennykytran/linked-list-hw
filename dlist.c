#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "dlist.h"
#include "utils.h"

dnode* dnode_create(int data, dnode* prev, dnode* next){
  dnode* p = (dnode*)malloc(sizeof(dnode));
  
  p-> data = data;
  p-> prev = prev;
  p-> next = next;
  return p;
}
dlist* dlist_create(void){
  dlist* p = (dlist*)malloc(sizeof(dlist));
  
  p-> head = NULL;
  p-> tail = NULL;
  p-> size = 0;
  return p;
}

bool dlist_empty(dlist* list){
  return list->size == 0;}
size_t dlist_size(dlist* list){
  return list->size;}

void dlist_popfront(dlist* list){
  if(dlist_empty(list)){
    return;}
  
  if(list-> head-> next != NULL){
    list-> head-> next-> prev = NULL;}
    
  dnode* p = list-> head-> next;
  free(list-> head);
  list-> head = p;
  --list-> size;
}

void dlist_popback(dlist* list){
  if(dlist_empty(list)) {return;}

  dnode* p = list-> head;
  dnode* prev = p;
  while(p-> next != NULL){
    prev = p;
    p = p-> next;}
    
  prev-> next = NULL;
  free(p);
  list-> tail = prev;
  --list-> size;
}

void dlist_pushfront(dlist* list, int data){
  dnode* p = dnode_create(data, NULL, list-> head);

  if(list-> head != NULL) {list-> head-> prev= p;}
  list-> head = p;
  ++list-> size;
}

void dlist_pushback(dlist* list, int data){
  dnode* p = dnode_create(data, list-> tail, NULL);
  dnode* i = list-> head;
  if(list-> head == NULL){
    p-> prev = NULL;
    list-> head = p;
    ++list-> size;
    return;}
  while(i-> next != NULL) {
    i = i-> next;}
  p-> prev = i;
  i-> next = p;
  ++list->size;
}

void dlist_clear(dlist* list){
  while(!(dlist_empty(list))){
    dlist_popfront(list);}
}
void dlist_print(dlist* list, const char* msg){
  printf("%s", msg);
  dnode* p = list->head;
  printf("   %p  <-- %d (%p) --> %p\n", p-> prev, p-> data, p, p-> next);
  p= p-> next;
  while(p != NULL){
    printf("   %p <-- %d (%p) --> %p\n", p-> prev, p-> data, p, p-> next);
    p= p-> next;}
}

int dlist_front(dlist* list){
  return list->head->data;}
int dlist_back(dlist* list){
  dnode* p = list-> head;
  while(p-> next != NULL) p= p-> next;
  return p-> data;
}

//TESTS BELOW

static void dlist_check_frontback(dlist* list, int front, int back) {
  printf("assert front of list is: %d\n", front);
  assert(dlist_front(list) == front);
  printf("assert back of list is: %d\n", back);
  assert(dlist_back(list) == back);
}

static void dlist_test_once(dlist* list, int start, int end, int step, void (*fp)(dlist*, int)) {
  for (int i = start; i < end; i += step) {
    fp(list, i);
  }
  dlist_print(list, "list is created and is...\n");
  printf("list size is: %zu\n", dlist_size(list));
  printf("list is empty? %s\n", yesorno(dlist_empty(list)));

  int firstadded = start;
  int lastadded = start;
  while (lastadded + step < end) {
    lastadded += step;
  }
  if (fp == dlist_pushback)  { dlist_check_frontback(list, firstadded, lastadded);  }
  if (fp == dlist_pushfront) { dlist_check_frontback(list, lastadded, firstadded ); }

  dlist_clear(list);
  printf("after clearing the list, is the list now empty? %s\n\n", yesorno(dlist_empty(list)));
}

void dlist_test() {
  printf("//===================== TESTING DLIST =========================\n\n");
  dlist* list = dlist_create();
  dlist_test_once(list, 10, 50, 10, dlist_pushfront);
  dlist_test_once(list, 10, 100, 20, dlist_pushback);
  dlist_test_once(list, 0, 100, 1, dlist_pushback);

  printf("      All Assertions passed!...\n");
  free(list);
  printf("//===================== END TESTING DLIST =========================\n\n");
}
