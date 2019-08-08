#include <iostream>
#include <stdio.h>

#include "SDL.h"
#include <GL/glew.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glwindow.h"
#include "geometry.h"
#include "camera.h"

using namespace std;

camera c(0.5f, 0.1f, 45.0f); 

GLint64 timer;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

const char* glGetErrorString(GLenum error)
{
    switch(error)
    {
    case GL_NO_ERROR:
        return "GL_NO_ERROR";
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
    default:
        return "UNRECOGNIZED";
    }
}

void glPrintError(const char* label="Unlabelled Error Checkpoint", bool alwaysPrint=false)
{
    GLenum error = glGetError();
    if(alwaysPrint || (error != GL_NO_ERROR))
    {
        printf("%s: OpenGL error flag is %s\n", label, glGetErrorString(error));
    }
}

//method to read shaders from a shader file, create a shader based on the type specified and then compile it
GLuint loadShader(const char* shaderFilename, GLenum shaderType)
{
    FILE* shaderFile = fopen(shaderFilename, "r");
    if(!shaderFile)
    {
        return 0;
    }

    fseek(shaderFile, 0, SEEK_END);
    long shaderSize = ftell(shaderFile);
    fseek(shaderFile, 0, SEEK_SET);

    char* shaderText = new char[shaderSize+1];
    size_t readCount = fread(shaderText, 1, shaderSize, shaderFile);
    shaderText[readCount] = '\0';
    fclose(shaderFile);

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, (const char**)&shaderText, NULL);
    glCompileShader(shader);

    delete[] shaderText;

    return shader;
}

//method to create a program, create shaders and then attach it to the program
//finally link the program and return it 
GLuint loadShaderProgram(const char* vertShaderFilename,
                       const char* fragShaderFilename)
{
    GLuint vertShader = loadShader(vertShaderFilename, GL_VERTEX_SHADER);
    GLuint fragShader = loadShader(fragShaderFilename, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if(linkStatus != GL_TRUE)
    {
        GLsizei logLength = 0;
        GLchar message[1024];
        glGetProgramInfoLog(program, 1024, &logLength, message);
        cout << "Shader load error: " << message << endl;
        return 0;
    }

    return program;
}

OpenGLWindow::OpenGLWindow(std::vector<std::string> objects) : objects(objects) 
{
    geometry.resize(objects.size());
    
    for (int i=0; i<objects.size(); i++) {
	geometry[i].loadFromOBJFile(objects[i]); //load the geometry data for each object and store it in the vector

	//set the position of each entity to be used when creating the models
        Entity temp;
	temp.position = glm::vec3(i, i, i);
	temp.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	temp.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	temp.color = glm::vec3(i, i+1, i+2);
        entities.push_back(temp);
    }

    translateDirection = 0;
    rotateDirection = 0;
    scaleDirection = 0;
}
//default constructor for a window, set the default position and rotation of the parent and child entities
OpenGLWindow::OpenGLWindow()
{
    translateDirection = 0;
    rotateDirection = 0;
    scaleDirection = 0;
}


void OpenGLWindow::initGL()
{
    // We need to first specify what type of OpenGL context we need before we can create the window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    sdlWin = SDL_CreateWindow("OpenGL Prac 1",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              1366, 768, SDL_WINDOW_OPENGL);
    if(!sdlWin)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Error", "Unable to create window", 0);
    }
    SDL_GLContext glc = SDL_GL_CreateContext(sdlWin);
    SDL_GL_MakeCurrent(sdlWin, glc);
    SDL_GL_SetSwapInterval(1);

    glewExperimental = true;
    GLenum glewInitResult = glewInit();
    glGetError(); // Consume the error erroneously set by glewInit()
    if(glewInitResult != GLEW_OK)
    {
        const GLubyte* errorString = glewGetErrorString(glewInitResult);
        cout << "Unable to initialize glew: " << errorString;
    }

    int glMajorVersion;
    int glMinorVersion;
    glGetIntegerv(GL_MAJOR_VERSION, &glMajorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &glMinorVersion);
    cout << "Loaded OpenGL " << glMajorVersion << "." << glMinorVersion << " with:" << endl;
    cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
    cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
    cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
    cout << "\tGLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0,0,0,1);

    shader = loadShaderProgram("phong.vert", "phong.frag");
    glUseProgram(shader);

    //setup projection matrix 
    glm::mat4 projectionMat = glm::perspective(glm::radians(90.0f), 4.0f/3.0f, 0.1f, 10.0f);
    int projectionMatrixLoc = glGetUniformLocation(shader, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLoc, 1, false, &projectionMat[0][0]);

    //setup camera
    int viewingMatrixLoc = glGetUniformLocation(shader, "viewingMatrix");
    glUniformMatrix4fv(viewingMatrixLoc, 1, false, &c.getViewMatrix()[0][0]);

    //setup each object
    for (int i=0; i<geometry.size(); i++) {
	glGenVertexArrays(1, &geometry[i].vao);
	glGenBuffers(1, &geometry[i].vbo);
	glGenBuffers(1, &geometry[i].ebo);
	glGenBuffers(1, &geometry[i].nbo);

	glBindVertexArray(geometry[i].vao);
	glBindBuffer(GL_ARRAY_BUFFER, geometry[i].vbo);

	glBufferData(GL_ARRAY_BUFFER, geometry[i].vertexCount()*3*sizeof(float), geometry[i].vertexData(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry[i].nbo);
	glBufferData(GL_ARRAY_BUFFER, geometry[i].normalCount()*sizeof(float), geometry[i].normalData(), GL_STATIC_DRAW); 

	//position attribute
    	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, geometry[i].vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float)*3, 0);

	//normal attribute
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, geometry[i].nbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(float)*3, 0);

	glBindVertexArray(0);
    }

}

void OpenGLWindow::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // NOTE: glm::translate/rotate/scale apply the transformation by right-multiplying by the
    //       corresponding transformation matrix (T). IE glm::translate(M, v) = M * T, not T*M
    //       This means that the transformation you apply last, will effectively occur first
    
    glGetInteger64v(GL_TIMESTAMP, &timer);
    float currentFrame = timer/100000000.0f;
    deltaTime = currentFrame-lastFrame;
    lastFrame = currentFrame;

    for (int i=0; i<entities.size(); i++){

	glm::mat4 modelMat(1.0f);
    	modelMat = glm::translate(modelMat, entities[i].position);
   	modelMat = glm::rotate(modelMat, entities[i].rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
  	modelMat = glm::rotate(modelMat, entities[i].rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    	modelMat = glm::rotate(modelMat, entities[i].rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    	modelMat = glm::scale(modelMat, entities[i].scale);

    	int modelMatrixLoc = glGetUniformLocation(shader, "modelMatrix");
    	glUniformMatrix4fv(modelMatrixLoc, 1, false, &modelMat[0][0]);	    

	glm::mat4 projectionMat = glm::perspective(glm::radians(90.0f), 4.0f/3.0f, 0.1f, 10.0f);
    	int projectionMatrixLoc = glGetUniformLocation(shader, "projectionMatrix");
   	glUniformMatrix4fv(projectionMatrixLoc, 1, false, &projectionMat[0][0]);

    	int viewingMatrixLoc = glGetUniformLocation(shader, "viewingMatrix");
    	glUniformMatrix4fv(viewingMatrixLoc, 1, false, &c.getViewMatrix()[0][0]);

    	glm::vec3 lpos(1.3f, 1.0f, 2.0f);
    	int lposLoc = glGetUniformLocation(shader, "lpos");
    	glUniform3fv(lposLoc, 1, &lpos[0]);

    	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    	int lightColorLoc(glGetUniformLocation(shader, "lightColor"));
    	glUniform3fv(lightColorLoc, 1, &lightColor[0]);

    	glm::vec3 objectColor (1.0, 0.5f, 0.3f);
    	int colorLoc = glGetUniformLocation(shader, "objectColor");
    	glUniform3fv(colorLoc, 1, &objectColor[0]);

	glBindVertexArray(geometry[i].vao);
	glDrawArrays(GL_TRIANGLES,0, geometry[i].vertexCount());
	glBindVertexArray(0);
    }

    // Swap the front and back buffers on the window, effectively putting what we just "drew"
    // onto the screen (whereas previously it only existed in memory)
    SDL_GL_SwapWindow(sdlWin);
}

// The program will exit if this function returns false
bool OpenGLWindow::handleEvent(SDL_Event e)
{
    // A list of keycode constants is available here: https://wiki.libsdl.org/SDL_Keycode
    // Note that SDL provides both Scancodes (which correspond to physical positions on the keyboard)
    // and Keycodes (which correspond to symbols on the keyboard, and might differ across layouts)
    int selection = 0;
    movement direction;
    if(e.type == SDL_KEYDOWN)
    {
        if(e.key.keysym.sym == SDLK_ESCAPE)
        {
            return false;
        }
	else if (e.key.keysym.sym == SDLK_y) {
	    if (selection==geometry.size()) {selection=0;}
	    else {selection++;}
	}
        else if(e.key.keysym.sym == SDLK_q)
        {
            entities[selection].position[translateDirection] -= 0.5f;
        }
        else if(e.key.keysym.sym == SDLK_w)
        {
		direction = FORWARD;
		c.translate(direction, deltaTime);
        }
        else if(e.key.keysym.sym == SDLK_e)
        {
            entities[selection].position[translateDirection] += 0.5f;
        }

        else if(e.key.keysym.sym == SDLK_a)
        {
	    direction = LEFT;
	    c.translate(direction, deltaTime);
        }
        else if(e.key.keysym.sym == SDLK_s)
        {
	    direction = BACK;
	    c.translate(direction, deltaTime);
        }
        else if(e.key.keysym.sym == SDLK_d)
        {
	    direction = RIGHT;
	    c.translate(direction, deltaTime);
        }

        else if(e.key.keysym.sym == SDLK_z)
        {
            entities[selection].scale[scaleDirection] -= 0.2f;
        }
        else if(e.key.keysym.sym == SDLK_x)
        {
            scaleDirection = (scaleDirection+1)%3;
        }
        else if(e.key.keysym.sym == SDLK_c)
        {
            entities[selection].scale[scaleDirection] += 0.2f;
        }
    }
    return true;
}

void OpenGLWindow::cleanup()
{
    for (int i=0; i<geometry.size(); i++) {
	    glDeleteVertexArrays(1, &geometry[i].vao);
	    glDeleteBuffers(1, &geometry[i].vbo);
	    glDeleteBuffers(1, &geometry[i].ebo);
    }
    SDL_DestroyWindow(sdlWin);
}
