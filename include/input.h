#ifndef INPUT_H
#define INPUT_H

typedef struct InState InState;

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "editor.h"

struct InState {
    bool QUIT;
    bool keyboard;

    bool UP;
    bool DOWN;
    bool LEFT;
    bool RIGHT;

    bool W;
    bool X;
    bool C;
    bool V;
    bool V_prec;

    bool I;
    bool J;
    bool K;
    bool L;

    bool ToggledF3;
    bool ToggledF4;
    bool ToggledGrid;
    bool paused;

    bool ToggledF3_prec;
    bool ToggledF4_prec;
    bool ToggledGrid_prec;

    bool Tplace;
    bool Tdestroy;
    bool Tplace_prec;
    bool Tdestroy_prec;

    SDL_FPoint joystick_left;
    SDL_FPoint joystick_right;

    SDL_FPoint mouse;
    EntityArchetype selected_archetype;   
};

void InState_Init(InState *inState);
void InState_Update(InState *inState, SDL_Event event);
void InState_Update_gamecontroller(InState *inState, SDL_Event event,SDL_GameController *controller);

#endif