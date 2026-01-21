#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <jansson.h>

#include "entity_pool.h"

void JSON_EntityToFile(EntityPool *pool, int loc, const char *filepath);
EntityID JSON_ToEntityFromFile(EntityPool *pool, const char *filepath);

#endif