#include <stdlib.h>

#define NOT_A_CELL NULL

struct CELL {
  void *elm;
  struct CELL *next  
};

typedef struct CELL cell_t;

struct LIST {
  cell_t head;
};

typedef struct LIST list_t; 

list_t list_create()
{
  list_t list;

  list.head = NOT_A_CELL;

  return list;
}

int list_append(list_t *list, void *elm)
{
  if (list->head == NOT_A_CELL) {
    list->head      = malloc(sizeof(cell_t));
    list->head.elm  = elm;
    list->head.next = NOT_A_CELL;
      
    return 0;
  }

  cell_t cell = list->head;
  
  while (cell.next != NOT_A_CELL)
    cell = cell->next;

  cell.next = malloc(sizeof(cell_t));
  cell = cell.next;
  
  cell.elm = elm;
  cell.next = NOT_A_CELL;

  return 0;
}

int list_remove(list_t *list, void *elm)
{
  if (list->head == NOT_A_CELL)
    return 1;

  cell_t current = list->head;

  if (current.elm == elm) {
    list->head = current.next;
    free(&current);

    return 0;
  }

  cell_t prev = current;
  current     = current.next;
  
  while (current.elm != elm) {
    if (current.next == NOT_A_CELL)
      return 1;

    prev    = current;
    current = current.next;
  }

  prev.next = current.next;
  free(&current);

  return 0;
}

int list_concat(list_t *l1, list_t *l2)
{
  
}

