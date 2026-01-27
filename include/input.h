#ifndef INPUT_H
#define INPUT_H

typedef struct InState InState;

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "editor.h"

struct InState {
    bool QUIT;

    bool UP;
    bool DOWN;
    bool LEFT;
    bool RIGHT;

    bool W;
    bool X;
    bool C;
    bool V;

    bool I;
    bool J;
    bool K;
    bool L;

    bool ToggledF3;
    bool ToggledF4;
    bool ToggledGrid;

    SDL_FPoint mouse;
    EntityArchetype selected_archetype;   
};

void InState_Init(InState *inState);
void InState_Update(InState *inState, SDL_Event event);
void InState_Update_gamecontroller(InState *inState, SDL_Event event,SDL_GameController *controller);

#endif