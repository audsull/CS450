// assn1.cpp
//
// A simple 2D OpenGL program

#define GL3_PROTOTYPES

// Include the vector and matrix utilities from the textbook, as well as some
// macro definitions.
#include "../include/Angel.h"

#ifdef __APPLE__
#  include <OpenGL/gl3.h>
#endif

#include <stdio.h>

const int NumPoints = 25;

void init()
{
    vec2 verticies[] = {
        vec2(-0.7, -1.0),
        vec2(-1.0, 0.3),
        vec2(0.0, 1.0),
        vec2(-0.7, -1.0),
        vec2(0.0, 1.0),
        vec2(1.0, 0.3),
        vec2(-0.7, -1.0),
        vec2(1.0, 0.3),
        vec2(0.7, -1.0),
        
        vec2(-0.25, -0.25),
        vec2(-0.25, 0.25),
        vec2(0.25, 0.25),
        vec2(0.25, 0.25),
        vec2(-0.25, -0.25),
        vec2(0.25, -0.25)
    };
    
    vec4 colors[] = {
        vec4(1.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.0, 0.0, 1.0),
        vec4(0.0, 1.0, 0.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.0,0.0, 1.0)
    };

    GLuint vao[1];
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]); //bind to make it active

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

    // Create and link the shaders into a program
    GLuint shaders = InitShader("vertshader.glsl", "fragshader.glsl");

    // You can bind the shader anytime before drawing.
    glUseProgram(shaders);

    // Get position and color attribute locations from the vertex shader
    GLuint vPositionLoc = glGetAttribLocation(shaders, "vPosition");
    GLuint vColorLoc = glGetAttribLocation(shaders, "vColor");

    // ...
    glEnableVertexAttribArray(vPositionLoc);
    glVertexAttribPointer(vPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glClearColor(1.0, 1.0, 1.0, 1.0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        // Quit when ESC is pressed
        exit(EXIT_SUCCESS);
        break;
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw each object here
    glDrawArrays(GL_TRIANGLES, 0, 9);
    glDrawArrays(GL_TRIANGLES, 9, 6);

    glFlush();
    glutSwapBuffers();
}


//------------------------------------------------------------------------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
#ifdef __APPLE__
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE);
#else
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitContextVersion (3, 2);
    glutInitContextFlags (GLUT_FORWARD_COMPATIBLE);
#endif
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(500, 300);
    glutCreateWindow("Simple Open GL Program");
    printf("%s\n%s\n", glGetString(GL_RENDERER), glGetString(GL_VERSION));

#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    glewInit();
#endif

    init();

    //NOTE:  callbacks must go after window is created!!!
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutMainLoop();

    return(0);
}
