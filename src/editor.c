#include <SDL2/SDL.h>

#include "editor.h"
#include "geometry.h"
#include "game.h"
#include "assets.h"

// Translate mouse position to world position
SDL_FPoint EDITOR_MouseToWorld(SDL_FPoint mouse, SDL_FPoint camera) {
    SDL_FPoint res;
    res.x = mouse.x + camera.x;
    res.y = mouse.y + camera.y;
    return res;
}

void EDITOR_PlaceEntity(Game *game) {
    SDL_FPoint mouse_position = EDITOR_MouseToWorld(game->inState->mouse, game->camera_pos);
    SDL_FPoint position = game->inState->ToggledGrid ? FPOINT_NearestSquare(mouse_position) : mouse_position;

    // TODO: get entity bundle from player selection
    EDITOR_EntityFromArchetype(game, game->inState->selected_archetype, position);
}


void EDITOR_EntityFromArchetype(Game *game, EntityArchetype archetype, SDL_FPoint pos) {
    EntityPool *pool = game->pool;
    EntityID id = POOL_NewEntity(game->pool);
    int i = id.location;

    pool->position[i] = pos;
    POOL_AddComponentFlags(pool, COMPONENT_POSITION, i);

    switch (archetype) {
        case ARCHETYPE_PLAYER:
            pool->tex_location[i] = TEX_DEBUG;
            pool->display_rect[i] = GRID_RECT;
            pool->collision_box[i] = GRID_FRECT;
            pool->velocity[i] = (SDL_FPoint) {0., 0.};

            POOL_AddComponentFlags(pool, COMPONENT_TEXTURE | COMPONENT_DISPLAYRECT | COMPONENT_COLLISIONBOX | COMPONENT_VELOCITY, i);
            break;   
        default:
        case ARCHETYPE_DEBUG:
            pool->tex_location[i] = TEX_DEBUG;
            pool->display_rect[i] = (SDL_Rect) GRID_RECT;
            pool->collision_box[i] = (SDL_FRect) GRID_FRECT;

            POOL_AddComponentFlags(pool, COMPONENT_TEXTURE | COMPONENT_DISPLAYRECT | COMPONENT_COLLISIONBOX, i);
            break;
        case ARCHETYPE_OCTOROK:
            ENEMY_SpawnOctorok(pool, pos);
    }
} 

void EDITOR_DisplayInfo(Game *game) {
    // Compiling things to print
    if (!game->inState->ToggledF4) { return; }

    // Draw the grid ( if applicable )
    if (game->inState->ToggledGrid) {
        EDITOR_DrawGrid(game);
    }

    SDL_FPoint inGameMouseCoordinates = EDITOR_MouseToWorld(game->inState->mouse, game->camera_pos);
    SDL_FPoint nearestSquare = FPOINT_NearestSquare(inGameMouseCoordinates);

    char str[500];
    sprintf(
        str,
        "SELECTED ARCHETYPE: %s\n"
        "MOUSE POSITION: (%4.f, %4.f)\n"
        "NEAREST SQUARE: (%4.f, %4.f)\n"
        ,
        EDITOR_ArchetypeToString(game->inState->selected_archetype),
        inGameMouseCoordinates.x, inGameMouseCoordinates.y,
        nearestSquare.x, nearestSquare.y
    );

    // Display the string on the screen
    SDL_Color white = {255, 255, 255};
    SDL_Surface *messageSurface = TTF_RenderUTF8_Solid_Wrapped(game->asset_manager->debug_font, str, white, 0);
    if (messageSurface == NULL) {
        fprintf(stderr, "Erreur TTF_RenderUTF8_Solid_Wrapped: %s\n", SDL_GetError());
     }

    SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(game->renderer, messageSurface);
    if (messageSurface == NULL) {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
    }

    SDL_Rect dst = {0, 0, 0, 0};
    if (0 != SDL_QueryTexture(messageTexture, NULL, NULL, &dst.w, &dst.h)) {
        fprintf(stderr, "Erreur SDL_QueryTexture : %s\n", SDL_GetError());
    }

    // Place the text in the bottom right corner of the window
    int window_h = 100;
    int window_w = 100;
    SDL_GetWindowSize(game->window, &window_w, &window_h);
    dst.x = window_w - dst.w;
    dst.y = window_h - dst.h;

    if (SDL_RenderCopy(game->renderer, messageTexture, NULL, &dst) != 0) {
        fprintf(stderr, "STD_RenderCopy Error: %s", SDL_GetError());
    }

    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);



    //mouse

    SDL_FRect rect = {game->inState->mouse.x-5, game->inState->mouse.y-5,10, 10};
    SDL_RenderFillRectF(game->renderer, &rect);
}

char* EDITOR_ArchetypeToString(EntityArchetype archetype) {
    char *strs[] = {"DEBUG", "PLAYER", "OCTOROK"};
    return strs[archetype];
}

void EDITOR_DrawGrid(Game *game) {
    // Compute grid offset and number of vertical/horizontal lines
    SDL_Renderer *renderer = game->renderer;
    SDL_Point camera = FPOINT_ToPoint(game->camera_pos);
    SDL_Point offset = { - camera.x % GRID_SIZE, - camera.y % GRID_SIZE };

    // Get number of lines
    int window_h = 1000;
    int window_w = 1000;
    SDL_GetWindowSize(game->window, &window_w, &window_h);
    int vertical_count = window_w / GRID_SIZE + 1;
    int horizontal_count = window_h / GRID_SIZE + 1;

    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 100);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Draw horizontal lines
    for (int i = -1; i < horizontal_count; i++) {
        SDL_RenderDrawLine(renderer, 0, i * GRID_SIZE + offset.y, window_w, i * GRID_SIZE + offset.y);
    }
    
    // Draw vertical lines
    for (int i = -1; i < vertical_count; i++) {
        SDL_RenderDrawLine(renderer, i * GRID_SIZE + offset.x, 0, i * GRID_SIZE + offset.x, window_h);
    }
}

// Renvoie la première entité dans le point et dans le display_box dans *id
// Renvoie true sur succés, false si il n'y a pas d'entité en ce point
bool EDITOR_EntityAtPoint(EntityPool *pool, SDL_Point point, EntityID *id) {
    for (int i = 0; i < pool->lastEntitylocation; i++) {
        if (POOL_LacksComponentFlags(pool, COMPONENT_DISPLAYRECT | COMPONENT_POSITION, i)) { continue; }
        SDL_Rect offset_display_rect = RECT_Offset(pool->display_rect[i], FPOINT_ToPoint(pool->position[i]));
        if (SDL_PointInRect(&point, &offset_display_rect)) {
            *id = pool->id[i];
            return true;
        }
    }
    return false;
}

void EDITOR_DeleteUnderMouse(Game *game) {
    SDL_Point point = FPOINT_ToPoint(EDITOR_MouseToWorld(game->inState->mouse, game->camera_pos));
    EntityID id = {0, 0};
    EDITOR_EntityAtPoint(game->pool, point, &id);
    POOL_DestroyEntity(game->pool, id);
}