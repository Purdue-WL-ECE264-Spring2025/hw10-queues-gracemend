#include "linked_list.h"

#include <stdlib.h>

struct list_node *new_node(size_t value) { return NULL; }

void insert_at_head(struct linked_list *list, size_t value) {
  struct list_node * node = new_node(value);
  node->next = list->head;
  list->head = node;
}

void insert_at_tail(struct linked_list *list, size_t value) {
  struct list_node * node = new_node(value);
  struct list_node* curr = list->head;
  while(curr->next != NULL){
    curr = curr->next;
  }
  curr->next = node;
}

size_t remove_from_head(struct linked_list *list) { 
  struct list_node* del = list->head;
  list->head = list->head->next;
  free(del);
  return 0; 
}

size_t remove_from_tail(struct linked_list *list) { 
  struct list_node* curr = list->head->next;
  struct list_node* last = list->head;
  while(curr->next != NULL){
    last = curr;
    curr = curr->next;
  }
  last->next = NULL;
  free(curr);
  return 0; 
}

void free_list(struct linked_list list) {
  struct list_node *curr = list.head;
  while (curr != NULL) {
    struct list_node *del = curr;
    curr = curr->next;
    free(del);
  }
  free(list.head);
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
