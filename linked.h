#pragma once
#include <stdio.h>
#include <stdlib.h>

// node creation
typedef struct Node {
  SDL_Surface* data;
  double real;
  double im;
  double zoom;
  double scroll;
  struct Node* next;
  struct Node* prev;
}Node;

void insert(struct Node** head, SDL_Surface* data, double real, double im,
		double zoom, double scroll);
void deleteNode(struct Node** head, struct Node* del_node);
