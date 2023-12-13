#pragma once
#include <stdio.h>
#include <stdlib.h>

// node creation
typedef struct Node {
  SDL_Surface* data;
  struct Node* next;
  struct Node* prev;
}Node;

void insert(struct Node** head, SDL_Surface* data);
void deleteNode(struct Node** head, struct Node* del_node);
