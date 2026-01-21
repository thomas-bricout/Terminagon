#include <stdbool.h>
#include <SDL2/SDL.h>

#include "input.h"

void InState_Init(InState *inState) {
    inState->QUIT    = false;

    inState->UP      = false;
    inState->DOWN    = false;
    inState->LEFT    = false;
    inState->RIGHT   = false;
    inState->W       = false;
    inState->X       = false;
    inState->C       = false;

    inState->ToggledF3 = false;
}

void InState_Update(InState *inState, SDL_EventType type, SDL_Scancode scancode) {
    switch (type) {
        case SDL_QUIT:
            inState->QUIT = true;
            break;
        case SDL_KEYDOWN:
            switch(scancode) {
                case SDL_SCANCODE_UP:
                    inState->UP = true;
                    break;
                case SDL_SCANCODE_DOWN:
                    inState->DOWN = true;
                    break;
                case SDL_SCANCODE_LEFT:
                    inState->LEFT = true;
                    break;
                case SDL_SCANCODE_RIGHT:
                    inState->RIGHT = true;
                    break;
                case SDL_SCANCODE_W:
                    inState->W = true;
                    break;
                case SDL_SCANCODE_X:
                    inState->X = true;
                    break;
                case SDL_SCANCODE_C:
                    inState->C = true;
                    break;
                case SDL_SCANCODE_I:
                    inState->I = true;
                    break;
                case SDL_SCANCODE_J:
                    inState->J = true;
                    break;
                case SDL_SCANCODE_K:
                    inState->K = true;
                    break;
                case SDL_SCANCODE_L:
                    inState->L = true;
                    break;

                case SDL_SCANCODE_F3:
                    inState->ToggledF3 = !inState->ToggledF3;
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch(scancode) {
                case SDL_SCANCODE_UP:
                    inState->UP = false;
                    break;
                case SDL_SCANCODE_DOWN:
                    inState->DOWN = false;
                    break;
                case SDL_SCANCODE_LEFT:
                    inState->LEFT = false;
                    break;
                case SDL_SCANCODE_RIGHT:
                    inState->RIGHT = false;
                    break;
                case SDL_SCANCODE_W:
                    inState->W = false;
                    break;
                case SDL_SCANCODE_X:
                    inState->X = false;
                    break;
                case SDL_SCANCODE_C:
                    inState->C = false;
                    break;
                case SDL_SCANCODE_I:
                    inState->I = false;
                    break;
                case SDL_SCANCODE_J:
                    inState->J = false;
                    break;
                case SDL_SCANCODE_K:
                    inState->K = false;
                    break;
                case SDL_SCANCODE_L:
                    inState->L = false;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}