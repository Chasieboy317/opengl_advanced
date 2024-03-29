#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include <GL/glew.h>

#include <glm/vec3.hpp>
#include <vector>

#include "geometry.h"
#include "entity.h"
#include "camera.h"
#include "light.h"

class OpenGLWindow
{
public:
    OpenGLWindow(std::vector<std::string> objects, bool loadTextures);

    void initGL();
    void initOBJ();
    void render();
    bool handleEvent(SDL_Event e);
    void cleanup();

private:
    SDL_Window* sdlWin;
    
    GLuint shader;

    std::vector<std::string> objects;
    std::vector<Entity> entities; 
    std::vector<GeometryData> geometry;
    std::vector<light> lights;

    int translateDirection;
    int rotateDirection;
    int scaleDirection;
    int selection = 0;

    bool loadTextures=false;

};

#endif
