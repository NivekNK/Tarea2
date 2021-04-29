#ifndef MAP_H
#define MAP_H

typedef struct Map Map;

Map * createMap(int (*is_equal)(void* key1, void* key2));

void insertMap(Map * map, void * key, void * value);

void * eraseMap(Map * map, void * key);

void * searchMap(Map * map, void * key);

void * firstMap(Map * map);

void * tailMap(Map * list);

void * nextMap(Map * map);

void setSortFunction(Map* map, int (*lower_than)(void* key1, void* key2));

int is_equal_string(void * key1, void * key2);

int lower_than_string(void * key1, void * key2);

int is_equal_int(void * key1, void * key2);

int lower_than_int(void * key1, void * key2);

int upper_than_int(void * key1, void * key2);

#endif /* MAP_h */
