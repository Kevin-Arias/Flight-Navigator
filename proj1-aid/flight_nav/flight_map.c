#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "flight_map.h"

// A safe version of malloc, that will exit the program in case your allocation
// fails.
void *checked_malloc(size_t size) {
     void *ptr = malloc(size);
     if (ptr == NULL) {
          fprintf(stderr, "memory allocation failed\n");
          exit(1);
     }
     return ptr;
}

typedef struct node {
  int marker;
  const char *name;
  struct node *nextcity;
  struct node *linkedcity;
} node;

typedef struct map_t {
     // YOUR CODE HERE
     int size;
     node *head;
} map_t;

map_t* map_create() {
     // YOUR CODE HERE
     map_t *newmap = malloc(sizeof(map_t));
     newmap->size = 0;
     newmap->head = NULL;
     return newmap;
}

void map_free(map_t* map) {
     // YOUR CODE HERE
     node *deleter = map->head;
     while(deleter->nextcity != NULL) {
       node *temp = deleter->nextcity;
       remove_city(map, deleter->name);
       deleter = temp;
     }
     remove_city(map, deleter->name);
     free(map->head);
     free(map);
}

node* makecity(const char* cityname) {
  node *newcity = malloc(sizeof(node));
  int marker = 0;
  int namelength = strlen(cityname) + 1;
  newcity->name = (char*)malloc(namelength * sizeof(char));
  strcpy(newcity->name, cityname);
  newcity->nextcity = NULL;
  newcity->linkedcity = NULL;
  return newcity;
}

int add_city(map_t* map, const char* name) {
     // YOUR CODE HERE

     if (map->head == NULL) {
       map->head = makecity(name);
       map->size = (map->size) + 1;
       return 1;
     } else {
       node *temp = map->head;
       while (temp->nextcity != NULL) {
         if (strcmp(name, temp->name) == 0) {
           return 0;
         }
         temp = temp->nextcity;
       }
       temp->nextcity = makecity(name);
       map->size = (map->size) + 1;
       return 1;
     }
}
void free_linkedcities(map_t* map, const char* cityname) {
  node *temp = map->head;
  while (temp != NULL) {
    if (strcmp(cityname, temp->name) == 0) {
      node *linktemp = temp->linkedcity;

      while (linktemp != NULL) {
        unlink_cities(map, temp->name, linktemp->name);
        linktemp = temp->linkedcity;
      }
      free(linktemp);
      return;
    }
    temp = temp->nextcity;
  }
  return;
}

int remove_city(map_t* map, const char* name) {
     // YOUR CODE HERE
     node *curr = map->head;
     node *prev = curr;
     while(curr != NULL) {
       if(strcmp(curr->name, name) == 0) {
         if(curr == map->head) {
           free_linkedcities(map, curr->name);
           map->head = curr->nextcity;
           free(curr->name);
           free(curr);
           map->size = (map->size) - 1;
           return 1;
         }
         free_linkedcities(map, curr->name);
         prev->nextcity = curr->nextcity;
         free(curr->name);
         free(curr);
         map->size = (map->size) - 1;
         return 1;
       }
       prev = curr;
       curr = curr -> nextcity;
     }
     return 0;
}

int num_cities(map_t* map) {
     // YOUR CODE HERE
     return (map->size);
}

int link_cities(map_t* map, const char* city1_name, const char* city2_name) {
     // YOUR CODE HERE
     if (link_cities_helper(map, city1_name, city2_name) == 1) {
        if (link_cities_helper(map, city2_name, city1_name) == 1) {
          return 1;
        }
        return 0;
     }
     return 0;
}

int link_cities_helper(map_t* map, const char* city1_name, const char* city2_name) {
  node *cityfinder = map->head;
  while (cityfinder != NULL) {
    if (strcmp(city1_name, cityfinder->name) == 0) {
      while (cityfinder->linkedcity != NULL) {
        if (strcmp(city2_name, cityfinder->name) == 0) {
          return 0;
        }
        cityfinder = cityfinder->linkedcity;
      }
      cityfinder->linkedcity = makecity(city2_name);
      return 1;
    }
    cityfinder = cityfinder->nextcity;
  }
  return 0;
}

int unlink_cities_helper(map_t* map, const char* city1_name, const char* city2_name) {
  //check for same names
  node *unlinker = map->head;
  while (unlinker != NULL) {
    if (strcmp(city1_name, unlinker->name) == 0) {
      node *prev = unlinker;
      while (unlinker != NULL) {
        if (strcmp(city2_name, unlinker->name) == 0) {
          //Found it
          if (unlinker->linkedcity == NULL) {
            prev->linkedcity = NULL;
            free(unlinker->name);
            free(unlinker->nextcity);
            free(unlinker->linkedcity);
            free(unlinker);
            return 1;
          }
          prev->linkedcity = unlinker->linkedcity;
          free(unlinker->name);
          free(unlinker->nextcity);
          free(unlinker);
          return 1;
        }
        prev = unlinker;
        unlinker = unlinker->linkedcity;
      }
      return 0;
    }
    unlinker = unlinker->nextcity;
  }
  //Does not exist
  return 0;
}

int unlink_cities(map_t* map, const char* city1_name, const char* city2_name) {
     // YOUR CODE HERE
     if (unlink_cities_helper(map, city1_name, city2_name) == 1) {
        if (unlink_cities_helper(map, city2_name, city1_name) == 1) {
          return 1;
        }
        return 0;
     }
     return 0;
}
int linkedcity_counter(map_t* map, const char* city_name) {
  node *counter = map->head;
  int size = 0;
  while (counter != NULL) {
    if (strcmp(city_name, counter->name) == 0) {
      while (counter->linkedcity != NULL) {
        size = size + 1;
        counter = counter->linkedcity;
      }
      return size;
    }
    counter = counter->nextcity;
  }
  return size;
}

const char** linked_cities(map_t* map, const char* city_name) {
     // YOUR CODE HERE
     int size = linkedcity_counter(map, city_name) + 1;
     char **arr = malloc(sizeof(*arr) * size);
     for (int i = 0; i < size; i++) {
       arr[i] = NULL;
     }
     node *temp = map->head;
     while (temp != NULL) {
       if (strcmp(city_name, temp->name) == 0) {
         if (size == 1) {
           return arr;
         }
         temp = temp->linkedcity;
         for (int i = 0; i < size-1; i++) {
           int namesize = strlen(temp->name) + 1;
           arr[i] = (char*)malloc(namesize * sizeof(char));
           strcpy(arr[i], temp->name);
           temp = temp->linkedcity;
         }
         return arr;
       }
       temp = temp->nextcity;
     }
     return NULL;

}

void markerclearer(map_t* map) {
  node *temp = map->head;
  while (temp->nextcity != NULL) {
    node *linker = temp;
    while (linker->linkedcity != NULL) {
      linker->marker = 0;
      linker = linker->linkedcity;
    }
    temp = temp->nextcity;
  }
  return;
}

// const char** dfs(map_t* map, const char* src_name, const char* dst_name) {
//
// }
const char** find_path(map_t* map, const char* src_name, const char* dst_name) {
     // YOUR CODE HERE
     markerclearer(map);
     const char** arr = dfs(map, src_name, dst_name);
     return arr;


}

void map_export(map_t* map, FILE* f) {
     // YOUR CODE HERE
}

map_t* map_import(FILE* f) {
     // YOUR CODE HERE
     return NULL;
}
