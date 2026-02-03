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
    inState->ToggledF4 = false;
    inState->ToggledGrid = false;

    inState->selected_archetype = ARCHETYPE_DEBUG;

    inState->mouse = (SDL_FPoint) {0., 0.};
}

void InState_Update(InState *inState, SDL_Event event) {
    SDL_EventType type = event.type;
    SDL_Scancode scancode = event.key.keysym.scancode;

    switch (type) {
        case SDL_MOUSEMOTION:
            inState->mouse.x = event.motion.x;
            inState->mouse.y = event.motion.y;
            break;
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
                case SDL_SCANCODE_V:
                    inState->V = true;
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
                case SDL_SCANCODE_F4:
                    inState->ToggledF4 = !inState->ToggledF4;
                    break;
                case SDL_SCANCODE_G:
                    inState->ToggledGrid = !inState->ToggledGrid;
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
                case SDL_SCANCODE_V:
                    inState->V = false;
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

    // Editor keybindings
    if ((type == SDL_MOUSEWHEEL && event.wheel.y > 0) || (type == SDL_KEYDOWN && scancode == SDL_SCANCODE_P)) {
        inState->selected_archetype ++;
        if (inState->selected_archetype == ARCHETYPE_NUMBER) {
            inState->selected_archetype = 0;
        }
    }

    if ((type == SDL_MOUSEWHEEL && event.wheel.y < 0) || (type == SDL_KEYDOWN && scancode == SDL_SCANCODE_M)) {
        inState->selected_archetype --;
        if (inState->selected_archetype == -1) {
            inState->selected_archetype = ARCHETYPE_NUMBER - 1;
        }
    }
}


void InState_Update_gamecontroller(InState *inState, SDL_Event event,SDL_GameController *controller) {
    inState->UP     = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP);
    inState->DOWN   = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
    inState->LEFT   = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
    inState->RIGHT  = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
    inState->W = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
    inState->X = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B);
    inState->C = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
    int V_temp = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A);
    inState->V=0;
    if(V_temp && inState->V_prec){
        inState->V= 0;
    }else{
        inState->V= V_temp;
    }
    inState->V_prec= V_temp;

    int alx = SDL_GameControllerGetAxis(controller,SDL_CONTROLLER_AXIS_LEFTX);
    if(alx<3200 && alx>-3200){
        alx=0;
    }
    int aly = SDL_GameControllerGetAxis(controller,SDL_CONTROLLER_AXIS_LEFTY);
    if(aly<3200 && aly>-3200){
        aly=0;
    }
    
    inState->joystick_left = (SDL_FPoint){alx,aly};
}

void PlayerAddSwordCollision(EntityPool *pool, EntityID id) {
    // TODO make it be in front of the player ( where they are currently facing )
    pool->damage_box[id.location] = (SDL_FRect) {100., 100., 100., 100.};
    POOL_AddComponentFlags(pool, COMPONENT_DAMAGEBOX, id.location);
}