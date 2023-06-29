#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) 
{ 
  long posicion = hash(key,map->capacity);
  
  while(map->buckets[posicion] != NULL)
  {
    posicion = (posicion+1)%map->capacity;
  }
  
  Pair * nodo = createPair(key,value); 
  
  map->buckets[posicion] = nodo;  
  map->size++;
  map->current = posicion;

}


void enlarge(HashMap * map) 
{
  enlarge_called = 1; //no borrar (testing purposes)
  Pair** anteriorRegistro = map->buckets;
  map->capacity *= 2;
  map->buckets = (Pair*) calloc(map->capacity, sizeof(Pair));

  map->size = 0;

  for (int i = 0; i < map->capacity/2; i++) 
  {
    if (anteriorRegistro[i] != NULL) 
    {
      insertMap(map, anteriorRegistro[i]->key, anteriorRegistro[i]->value);
    }
  }
  enlarge_called++;
}

  


}


HashMap * createMap(long capacity) {
  HashMap* map = (HashMap*) malloc(sizeof(HashMap));
  map->buckets = (Pair**)calloc(capacity,sizeof(Pair));
  map->size = 0;
  map->current = -1;
  map->capacity = capacity; 
  
  return map;
}


void eraseMap(HashMap * map,  char * key) 
{    
  Pair* nodo = searchMap(map,key);
  if(nodo != NULL)
  {
    nodo->key = NULL;
    map->size--;
  }
}

Pair * searchMap(HashMap * map,  char * key) {   
  long posicion = hash(key,map->capacity);
  long posicion_1 = posicion;

  while(map->buckets[posicion] != NULL)
    {
      if(strcmp(map->buckets[posicion]->key, key) == 0)
      {
        map->current = posicion;
        return map->buckets[posicion];
      }
      posicion = (posicion + 1) % map->capacity;
      if (posicion == posicion_1) break;
    }

  return NULL;
}

Pair * firstMap(HashMap * map) {
  for (int i = 0; i < map->capacity; i++) 
  {
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) 
    {
      map->current = i;
      return map->buckets[i];
     }
  }
  return NULL;
}

Pair * nextMap(HashMap * map) 
{
  for (int i = map->current+1; i < map->capacity; i++) 
  {
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)   {
        map->current = i;
        return map->buckets[i];
      }
  }
return NULL;
}