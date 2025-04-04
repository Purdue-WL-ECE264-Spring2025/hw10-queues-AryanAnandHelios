
#include "linked_list.h"
#include <stdlib.h>

struct list_node *new_node(size_t nodeValue) {
  struct list_node *newNode = malloc(sizeof(struct list_node));
  if (newNode) {
    newNode->value = nodeValue;
    newNode->next = NULL;
  }
  return newNode;
}

void insert_at_head(struct linked_list *list, size_t nodeValue) {
  struct list_node *newNode = new_node(nodeValue);
  if (newNode) {
    newNode->next = list->head;
    list->head = newNode;
  }
}

void insert_at_tail(struct linked_list *list, size_t nodeValue) {
  struct list_node *newNode = new_node(nodeValue);
  if (!newNode) {
    return;
  }

  if (!list->head) {
    list->head = newNode;
    return;
  }

  struct list_node *lastNode = list->head;
  while (lastNode->next) {
    lastNode = lastNode->next;
  }
  lastNode->next = newNode;
}

size_t remove_from_head(struct linked_list *list) {
  if (!list->head) {
    return 0;
  }
  struct list_node *firstNode = list->head;
  size_t nodeValue = firstNode->value;
  list->head = firstNode->next;
  free(firstNode);
  return nodeValue;
}

size_t remove_from_tail(struct linked_list *list) {
  if (!list->head) {
    return 0;
  }

  if (!list->head->next) {
    size_t nodeValue = list->head->value;
    free(list->head);
    list->head = NULL;
    return nodeValue;
  }

  struct list_node *secondLastNode = list->head;
  while (secondLastNode->next->next) {
    secondLastNode = secondLastNode->next;
  }

  size_t nodeValue = secondLastNode->next->value;
  free(secondLastNode->next);
  secondLastNode->next = NULL;
  return nodeValue;
}

void free_list(struct linked_list list) {
  struct list_node *currentNode = list.head;
  while (currentNode) {
    struct list_node *nextNode = currentNode->next;
    free(currentNode);
    currentNode = nextNode;
  }
}
