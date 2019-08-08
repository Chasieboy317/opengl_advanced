#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include <GL/glew.h>

#include <glm/vec3.hpp>
#include <vector>

#include "geometry.h"
#include "entity.h"
#include "camera.h"

class OpenGLWindow
{
public:
    OpenGLWindow();
    OpenGLWindow(std::vector<std::string> objects);

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
    std::vector<glm::vec3> lights;

    int translateDirection;
    int rotateDirection;
    int scaleDirection;
};

#endif
