// SimpleProgram.cpp
//
// A simple 2D OpenGL program

#define GL3_PROTOTYPES

// Include the vector and matrix utilities from the textbook, as well as some
// macro definitions.
#include "Angel.h"

#ifdef __APPLE__
#  include <OpenGL/gl3.h>
#endif

#include <stdio.h>

// A global constant for the number of points that will be in our object.
const int NumPoints = 27;
const int NumPointsDot = 28;
const int NumPointsLine = 30;


//----------------------------------------------------------------------------
void init(void)
{
    // Specify the vertices for a rectangle.  The first and last vertex are
    // duplicated to close the box.
    vec2 vertices[] = {
        vec2(-0.7, -1.0), //pentagon1
        vec2(-1.0, 0.3),
        vec2(0.0, 1.0),
        vec2(-0.7, -1.0),
        vec2(0.0, 1.0),
        vec2(1.0, 0.3),
        vec2(-0.7, -1.0),
        vec2(1.0, 0.3),
        vec2(0.7, -1.0),
        
        vec2(-0.6, -0.9), //pentagon2
        vec2(-0.9, 0.2),
        vec2(0.0, 0.9),
        vec2(-0.6, -0.9),
        vec2(0.0, 0.9),
        vec2(0.9, 0.2),
        vec2(-0.6, -0.9),
        vec2(0.9, 0.2),
        vec2(0.6, -0.9),
        
        vec2(-0.5, -0.5), //triangle
        vec2(0.5, -0.5),
        vec2(0.0, 0.5),
        
        vec2(-0.25, -0.25), //square
        vec2(-0.25, 0.25),
        vec2(0.25, 0.25),
        vec2(0.25, 0.25),
        vec2(-0.25, -0.25),
        vec2(0.25, -0.25),
        
        vec2(-0.15, 0.0), //point in the middle
        
        vec2(0.0, 1.0), //line
        vec2(0.0, -1.0)
    };
    
    vec4 colors[] = {
        vec4(0.0, 1.0, 0.0, 1.0), //pentagon1
        vec4(0.0, 1.0, 0.0, 1.0),
        vec4(0.0, 1.0, 0.0, 1.0),
        vec4(0.0, 1.0, 0.0, 1.0),
        vec4(0.0, 1.0, 0.0, 1.0),
        vec4(0.0, 1.0, 0.0, 1.0),
        vec4(0.0, 1.0, 0.0, 1.0),
        vec4(0.0, 1.0, 0.0, 1.0),
        vec4(0.0, 1.0, 0.0, 1.0),
        
        vec4(0.0, 0.0, 1.0, 1.0), //pentagon2
        vec4(0.0, 0.0, 1.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0),
        
        vec4(1.0, 0.0, 0.0, 1.0), //triangle
        vec4(0.0, 1.0, 0.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0),
        
        vec4(1.0, 0.0, 0.0, 1.0), //square
        vec4(1.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.0, 0.0, 1.0),
        
        vec4(0.0, 0.0, 0.0, 1.0), //point in the middle
        
        vec4(0.0, 0.0, 0.0, 1.0), //line
        vec4(0.0, 0.0, 0.0, 1.0)
    };
    
    
    // Create a vertex array object---OpenGL needs this to manage the Vertex
    // Buffer Object
    GLuint vao;
    
    // Generate the vertex array and then bind it to make make it active.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // Create and initialize buffer objects---that's the memory buffer that
    // will be on the card!  We'll need one for each array in this example
    GLuint buffer[2];
    
    glGenBuffers(2, buffer);
    
    // Bind makes buffer[0] the active VBO
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    
    // Here we copy the vertex data into our buffer on the card.  The parameters
    // tell it the type of buffer object, the size of the data in bytes, the
    // pointer for the data itself, and a hint for how we intend to use it.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    
    
    //Color buffer setup
    // Bind makes buffer[1] the active buffer now
    glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    
    
    // Load the shaders.  Note that this function is not offered by OpenGL
    // directly, but provided as a convenience.
    GLuint program = InitShader("vertshader.glsl", "fragshader.glsl");
    
    // Make that shader program active.
    glUseProgram(program);
    
    
    //tie to the shader
    // Let's make buffer[0] active so we can do vertices first.
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    
    // Initialize the vertex position attribute from the vertex shader.  When
    // the shader and the program are linked, a table is created for the shader
    // variables.  Here, we get the index of the vPosition variable.
    GLuint loc = glGetAttribLocation(program, "vPosition");
    
    // We want to set this with an array!
    glEnableVertexAttribArray(loc);
    
    // We map it to this offset in our current buffer (VBO) So, our position
    // data is going into loc and contains 2 floats.  The parameters to this
    // function are the index for the shader variable, the number of components,
    // the type of the data, a boolean for whether or not this data is
    // normalized (0--1), stride (or byte offset between consective attributes),
    // and a pointer to the first component.  Note that BUFFER_OFFSET is a macro
    // defined in the Angel.h file.  It's the offset into the vertex buffer data
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    
    // Tie the color vertices to shader variable
    //Now let's work on the color buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
    loc = glGetAttribLocation(program, "vColor");
    
    glEnableVertexAttribArray(loc);
    //Note offset is still 0 because it's a second buffer, the color buffer in this case.
    glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    // Make the background white
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

//----------------------------------------------------------------------------
void
display(void)
{
    // clear the window
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw the points.  The parameters to the function are: the mode, the first
    // index, and the count.
    glDrawArrays(GL_TRIANGLES, 0, NumPoints);
    glDrawArrays(GL_POINTS, NumPoints, NumPointsDot);
    glDrawArrays(GL_LINES, NumPointsDot, NumPointsLine);
    glFlush();
    glutSwapBuffers();
}

//----------------------------------------------------------------------------
void
keyboard(unsigned char key, int x, int y)
{
    switch (key) {
            
            // Quit when ESC is pressed
        case 27:
            exit(EXIT_SUCCESS);
            break;
    }
}

//------------------------------------------------------------------------------
// This program draws a red rectangle on a white background, but it's still
// missing the machinery to move to 3D.
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
