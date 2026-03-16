#include "Zuma_Prefix.pch"

#include "CircleShootApp.h"
#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
    Sexy::CircleShootApp app;

    app.Init();
    app.Start();
    app.Shutdown();
    
    return 0;
}
