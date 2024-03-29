#include "SDL.h"
#include "glwindow.h"
#include <iostream>

// In order to make cross-platform development and deployment easy, SDL implements its own main
// function, and instead calls out to our code at this SDL_main, however on linux this is not
// needed (since the entrypoint in linux is already called main) so to keep things portable
// we need to check our environment at compile-time
#ifdef __linux__
int main(int argc, char** argv)
#else
int SDL_main(int argc, char** argv)
#endif
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Error", "Unable to initialize SDL", 0);
        return 1;
    }

    //initialize vector containing all objects to be used in the program
    std::vector<std::string> objects;
    for (int i=0; i<argc-1; i++) {
	objects.push_back(std::string(argv[i+1]));
    }

    bool loadTextures=false;
    if (std::string(argv[argc-1])=="true") loadTextures=true; 
    OpenGLWindow window(objects, loadTextures);
    window.initGL();

    bool running = true;
    while(running)
    {
        // Check for a quit event before passing to the GLWindow
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                running = false;
            }
            else if(!window.handleEvent(e))
            {
                running = false;
            }
        }
        window.render();

        // We sleep for 10ms here so as to prevent excessive CPU usage
        SDL_Delay(10);
    }

    window.cleanup();
    SDL_Quit();
    return 0;
}

