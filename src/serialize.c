#include <jansson.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "entity_pool.h"

json_t *JSON_FPoint(SDL_FPoint point) {
    json_t *json = json_object();
    json_object_set_new(json, "x", json_real(point.x));
    json_object_set_new(json, "y", json_real(point.y));
    return json;
}

SDL_FPoint JSON_ToFPoint(json_t *json) {
    SDL_FPoint point;
    json_t *x = json_object_get(json, "x");
    json_t *y = json_object_get(json, "y");

    point.x = json_real_value(x);
    point.y = json_real_value(y);

    json_decref(x);
    json_decref(y);
    return point;
}

SDL_FRect JSON_ToFRect(json_t *json) {
    SDL_FRect rect;
    json_t *x = json_object_get(json, "x");
    json_t *y = json_object_get(json, "y");
    json_t *h = json_object_get(json, "h");
    json_t *w = json_object_get(json, "w");

    rect.x = json_real_value(x);
    rect.y = json_real_value(y);
    rect.h = json_real_value(h);
    rect.w = json_real_value(w);

    json_decref(x);
    json_decref(y);
    json_decref(h);
    json_decref(w);
    return rect;
}

SDL_Rect JSON_ToRect(json_t *json) {
    SDL_Rect rect;
    json_t *x = json_object_get(json, "x");
    json_t *y = json_object_get(json, "y");
    json_t *h = json_object_get(json, "h");
    json_t *w = json_object_get(json, "w");

    rect.x = json_integer_value(x);
    rect.y = json_integer_value(y);
    rect.h = json_integer_value(h);
    rect.w = json_integer_value(w);

    json_decref(x);
    json_decref(y);
    json_decref(h);
    json_decref(w);
    return rect;
}

json_t *JSON_FRect(SDL_FRect rect) {
    json_t *json = json_object();
    json_object_set_new(json, "x", json_real(rect.x));
    json_object_set_new(json, "y", json_real(rect.y));
    json_object_set_new(json, "w", json_real(rect.w));
    json_object_set_new(json, "h", json_real(rect.h));
    return json;
}

json_t *JSON_Rect(SDL_Rect rect) {
    json_t *json = json_object();
    json_object_set_new(json, "x", json_integer(rect.x));
    json_object_set_new(json, "y", json_integer(rect.y));
    json_object_set_new(json, "w", json_integer(rect.w));
    json_object_set_new(json, "h", json_integer(rect.h));
    return json;
}

json_t *JSON_EntityID(EntityID id) {
    json_t *json = json_object();
    json_object_set_new(json, "unique_id", json_integer(id.unique_id));
    json_object_set_new(json, "location" , json_integer(id.location));
    return json;
}

json_t *JSON_PlayerComponent(PlayerComponent player_c) {
    json_t *json = json_object();
    // TODO
    return json;
}

json_t *JSON_FromEntity(EntityPool *pool, int loc) {
    json_t *entity = json_object();

    json_object_set_new(entity, "components", json_integer(pool->component_flags[loc]));
    
    json_object_set_new(entity, "texture", json_integer(pool->tex_location[loc]));

    json_object_set_new(entity, "displayrect", JSON_Rect(pool->display_rect[loc]));

    json_object_set_new(entity, "collisionbox", JSON_FRect(pool->collision_box[loc]));
    json_object_set_new(entity, "damagebox",    JSON_FRect(pool->damage_box[loc]));
    json_object_set_new(entity, "hitbox",       JSON_FRect(pool->hit_box[loc]));

    json_object_set_new(entity, "position", JSON_FPoint(pool->position[loc]));
    json_object_set_new(entity, "velocity", JSON_FPoint(pool->velocity[loc]));

    return entity;
}

void JSON_ToEntity(EntityPool *pool, json_t *json) {
    EntityID id = POOL_NewEntity(pool);
    int i = id.location;
    
    json_t *components = json_object_get(json, "components");
    json_t *texture = json_object_get(json, "texture");
    json_t *displayrect = json_object_get(json, "displayrect");
    json_t *collisionbox = json_object_get(json, "collisionbox");
    json_t *damagebox = json_object_get(json, "damagebox");
    json_t *hitbox = json_object_get(json, "hitbox");
    json_t *position = json_object_get(json, "position");
    json_t *velocity = json_object_get(json, "velocity");

    pool->component_flags[i] = json_integer_value(components);
    if (!POOL_LacksComponentFlags(pool, COMPONENT_TEXTURE       , i)) { pool->tex_location[i] = json_integer_value(texture);     }

    if (!POOL_LacksComponentFlags(pool, COMPONENT_DISPLAYRECT   , i)) { pool->display_rect[i] = JSON_ToRect(displayrect); }

    if (!POOL_LacksComponentFlags(pool, COMPONENT_COLLISIONBOX  , i)) { pool->collision_box[i] = JSON_ToFRect(collisionbox); }
    if (!POOL_LacksComponentFlags(pool, COMPONENT_DAMAGEBOX     , i)) { pool->damage_box[i]    = JSON_ToFRect(damagebox); }
    if (!POOL_LacksComponentFlags(pool, COMPONENT_HITBOX        , i)) { pool->hit_box[i]       = JSON_ToFRect(hitbox); }

    if (!POOL_LacksComponentFlags(pool, COMPONENT_POSITION      , i)) { pool->position[i]      = JSON_ToFPoint(position); }
    if (!POOL_LacksComponentFlags(pool, COMPONENT_VELOCITY      , i)) { pool->velocity[i]      = JSON_ToFPoint(velocity); }

    json_decref(components  );
    json_decref(texture     );
    json_decref(displayrect );
    json_decref(collisionbox);
    json_decref(damagebox   );
    json_decref(hitbox      );
    json_decref(position    );
    json_decref(velocity    );
}

void JSON_ToEntityAll(EntityPool *pool, json_t *json) {
    size_t i;
    json_t *entity;

    json_array_foreach(json, i, entity) {
        JSON_ToEntity(pool, entity);
    }
}

void JSON_Save(Game *game, const char *filepath) {
    FILE *fp = fopen(filepath, "w");
    json_t *json = json_object();
    json_t *entities = json_array();

    if ( fp == NULL ) {
        fprintf(stderr, "Failed to open file: %s", filepath);
    }

    // Save entity list
    for (int i = 0; i < game->pool->lastEntitylocation; i++) {
        json_array_append_new(entities, JSON_FromEntity(game->pool, i));
    }
    json_object_set_new(json, "entities", entities);

    // Save other fields

    // Dump json to file and free up space
    json_dumpf(json, fp, JSON_INDENT(2));
    json_decref(json);
    fclose(fp);
}

void JSON_Load(Game *game, const char *filepath, InState *inState) {
    // Empty current entity list
    POOL_Init(game->pool);

    // Open file
    FILE *fp = fopen(filepath, "r");
    json_t *json = NULL;
    json_error_t error;

    if ( fp != NULL ) {
        json = json_load_file(filepath, 0, &error);
        fclose(fp);
    } else {
        fprintf(stderr, "Failed to open file: %s", filepath);
    }

    // Load entities
    json_t *entities = json_object_get(json, "entities");
    JSON_ToEntityAll(game->pool, entities);

    // Load other fiels
    // TODO: Replace hard coded values
    game->pool->player_component[0] = PLAYER_NewComponent(inState);
    game->pool->player_id[0] = (EntityID) {0, 0};

    json_decref(json);
}