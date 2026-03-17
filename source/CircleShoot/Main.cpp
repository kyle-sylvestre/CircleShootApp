#include "Zuma_Prefix.pch"

#include "CircleShootApp.h"
#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
	const char *path = "/Users/ksylvestre/dev/CircleShootApp/ignore";
    Sexy::CircleShootApp app;
	Sexy::SetResourceFolder(path);
	Sexy::ChDir(path);

    app.Init();
    app.Start();
    app.Shutdown();
    
    return 0;
}
