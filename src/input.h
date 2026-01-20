#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

typedef struct {
    bool QUIT;

    bool UP;
    bool DOWN;
    bool LEFT;
    bool RIGHT;

    bool W;
    bool X;
    bool C;

    bool ToggledF3;
} InputSituation;

void InputSituation_Init(InputSituation *inputSituation);
void InputSituation_Update(InputSituation *inputSituation, SDL_EventType type, SDL_Scancode scancode);

#endif