#include <stdbool.h>
#include <SDL2/SDL.h>

#include "input.h"

void InState_Init(InState *inputSituation) {
    inputSituation->QUIT    = false;

    inputSituation->UP      = false;
    inputSituation->DOWN    = false;
    inputSituation->LEFT    = false;
    inputSituation->RIGHT   = false;
    inputSituation->W       = false;
    inputSituation->X       = false;
    inputSituation->C       = false;

    inputSituation->ToggledF3 = false;
}

void InState_Update(InState *inputSituation, SDL_EventType type, SDL_Scancode scancode) {
    switch (type) {
        case SDL_QUIT:
            inputSituation->QUIT = true;
            break;
        case SDL_KEYDOWN:
            switch(scancode) {
                case SDL_SCANCODE_UP:
                    inputSituation->UP = true;
                    break;
                case SDL_SCANCODE_DOWN:
                    inputSituation->DOWN = true;
                    break;
                case SDL_SCANCODE_LEFT:
                    inputSituation->LEFT = true;
                    break;
                case SDL_SCANCODE_RIGHT:
                    inputSituation->RIGHT = true;
                    break;
                case SDL_SCANCODE_W:
                    inputSituation->W = true;
                    break;
                case SDL_SCANCODE_X:
                    inputSituation->X = true;
                    break;
                case SDL_SCANCODE_C:
                    inputSituation->C = true;
                    break;

                case SDL_SCANCODE_F3:
                    inputSituation->ToggledF3 = !inputSituation->ToggledF3;
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch(scancode) {
                case SDL_SCANCODE_UP:
                    inputSituation->UP = false;
                    break;
                case SDL_SCANCODE_DOWN:
                    inputSituation->DOWN = false;
                    break;
                case SDL_SCANCODE_LEFT:
                    inputSituation->LEFT = false;
                    break;
                case SDL_SCANCODE_RIGHT:
                    inputSituation->RIGHT = false;
                    break;
                case SDL_SCANCODE_W:
                    inputSituation->W = false;
                    break;
                case SDL_SCANCODE_X:
                    inputSituation->X = false;
                    break;
                case SDL_SCANCODE_C:
                    inputSituation->C = false;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}