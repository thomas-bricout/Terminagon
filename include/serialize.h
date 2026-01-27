#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <jansson.h>

#include "entity_pool.h"
#include "game.h"
#include "input.h"

void JSON_EntityToFileAll(EntityPool *pool, const char *filepath);
EntityID JSON_ToEntityFromFile(EntityPool *pool, const char *filepath);

void JSON_Save(Game *game, const char *filepath);
void JSON_Load(Game *game, const char *filepath, InState *inState);

#endif