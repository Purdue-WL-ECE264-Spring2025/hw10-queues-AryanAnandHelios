#include "linked_list.h"
#include <stdlib.h>

/* Creates a new linked list node with the given value
 * Returns NULL if memory allocation fails
 */
struct list_node *new_node(size_t value) {
  struct list_node *node = malloc(sizeof(struct list_node));
  if (node) {
    node->value = value;
    node->next = NULL;
  }
  return node;
}

/* Inserts a new node with given value at the start of the list
 * If memory allocation fails, the list remains unchanged
 */
void insert_at_head(struct linked_list *list, size_t value) {
  struct list_node *node = new_node(value);
  if (node) {
    node->next = list->head;
    list->head = node;
  }
}

/* Inserts a new node with given value at the end of the list
 * If memory allocation fails, the list remains unchanged
 */
void insert_at_tail(struct linked_list *list, size_t value) {
  struct list_node *node = new_node(value);
  if (!node) {
    return;
  }

  if (!list->head) {
    list->head = node;
    return;
  }

  struct list_node *current = list->head;
  while (current->next) {
    current = current->next;
  }
  current->next = node;
}

/* Removes and returns the value from the head of the list
 * Returns 0 if the list is empty
 */
size_t remove_from_head(struct linked_list *list) {
  if (!list->head) {
    return 0;
  }
  struct list_node *temp = list->head;
  size_t value = temp->value;
  list->head = temp->next;
  free(temp);
  return value;
}

/* Removes and returns the value from the tail of the list
 * Returns 0 if the list is empty
 */
size_t remove_from_tail(struct linked_list *list) {
  if (!list->head) {
    return 0;
  }

  if (!list->head->next) {
    size_t value = list->head->value;
    free(list->head);
    list->head = NULL;
    return value;
  }

  struct list_node *current = list->head;
  while (current->next->next) {
    current = current->next;
  }

  size_t value = current->next->value;
  free(current->next);
  current->next = NULL;
  return value;
}

/* Frees all nodes in the linked list
 * Should be called when the list is no longer needed
 */
void free_list(struct linked_list list) {
  struct list_node *current = list.head;
  while (current) {
    struct list_node *next = current->next;
    free(current);
    current = next;
  }
}
