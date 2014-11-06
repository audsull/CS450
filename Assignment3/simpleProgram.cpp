//Includes vec, mat, and other include files as well as macro defs
#define GL3_PROTOTYPES

// Include the vector and matrix utilities from the textbook, as well as some
// macro definitions.
#include "Angel.h"
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <iosfwd>
#include <string>
#include <cstring>
#include <iostream>
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/glm.hpp"
#include "objloader.hpp"
#include "objloader.cpp"
#ifdef __APPLE__
#  include <OpenGL/gl3.h>
#endif


typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

GLuint  model_view;  // model-view matrix uniform shader variable location
GLuint  projection; // projection matrix uniform shader variable location

GLuint vao[3];

std::vector<glm::vec4> vertices1;
std::vector<glm::vec4> vertices2;
std::vector<glm::vec4> vertices3;

std::vector<glm::vec4> normals1;
std::vector<glm::vec4> normals2;
std::vector<glm::vec4> normals3;

float eyex, eyey, eyez;
float atx, aty, atz;
float upx, upy, upz;

char proj = 'O';
int wireFlag = 1;

float yfov, aspect, near, far, right, left, top, bottom;

float startAR = (right-left)/(top-bottom);

int wh = 512;
int currentHeight = wh;

std::string scnfile = "";
const char* filename1 = "";
const char* filename2 = "";
const char* filename3 = "";

std::string oldfilename1 = "";
std::string oldfilename2 = "";
std::string oldfilename3 = "";

GLuint shaderProgram;
GLuint program;

//----------------------------------------------------------------------------

void load(){
    //open scn file
    std::ifstream scn;
    scn.open(scnfile);
    if (scn.is_open()) {
    
        //read info
        scn >> oldfilename1 >> oldfilename2 >> oldfilename3
            >> eyex >> eyey >> eyez
            >> atx >> aty >> atz
            >> upx >> upy >> upz
            >> proj;
        
        if(proj == 'P'){
           scn >> yfov >> aspect >> near >> far;
        }
        else if (proj == 'O'){
            scn >> left >> right >> bottom >> top >> near >> far;
        }
 
        //close file
        scn.close();
    }
    
    else std::cout << "Unable to open file";
 
    filename1 = oldfilename1.c_str();
    filename2 = oldfilename2.c_str();
    filename3 = oldfilename3.c_str();
}

void myReshape(int w, int h){
    
    GLuint projectionLoc = glGetUniformLocation(shaderProgram , "Projection");
    currentHeight = h;
    mat4 p2;
    glViewport(0,0,w,h);
    
    float ar = w/h;
    
    if ( ar < startAR ){ //taller
        if (proj == 'O') {
            p2 = Ortho(left, right, left * (GLfloat) h / (GLfloat) w,
                       right * (GLfloat) h / (GLfloat) w, 0.1, 10.0);
        }
        else if (proj == 'P'){
            p2 = Frustum(left, right, left * (GLfloat) h / (GLfloat) w,
                         right * (GLfloat) h / (GLfloat) w, 0.1, 10.0);
        }
    }
    else { //wider
        if (proj == 'O') {
            p2 = Ortho(bottom * (GLfloat) w / (GLfloat) h,
                       top * (GLfloat) w / (GLfloat) h, bottom, top, 0.1, 10.0);
        }
        else if (proj == 'P'){
            p2 = Frustum(bottom * (GLfloat) w / (GLfloat) h,
                         top * (GLfloat) w / (GLfloat) h, bottom, top, 0.1, 10.0);
        }
    }
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &p2[0][0]);
}

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
    GLuint vbo1[2];
    GLuint vbo2[2];
    GLuint vbo3[2];
    
    GLuint vPosition;
    GLuint vNormal;
    
    bool res1 = loadOBJ(filename1, vertices1, normals1);
    bool res2 = loadOBJ(filename2, vertices2, normals2);
    bool res3 = loadOBJ(filename3, vertices3, normals3);

    // Load shaders and use the resulting shader program
    program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program );
    shaderProgram = program;
    
    int choiceInt = glGetUniformLocation(program, "choose");
    glUniform1i(choiceInt, 0);
    
    // Create a vertex array object
    glGenVertexArrays( 1, &vao[1] );
    glBindVertexArray( vao[1] );
    
    glGenBuffers( 2, vbo1 );
    glBindBuffer( GL_ARRAY_BUFFER, vbo1[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices1.size() * sizeof(glm::vec4), &vertices1[0], GL_STATIC_DRAW);
    
    vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    
    glBindBuffer( GL_ARRAY_BUFFER, vbo1[1]);
    glBufferData(GL_ARRAY_BUFFER, normals1.size() * sizeof(glm::vec4), &normals1[0], GL_STATIC_DRAW);
    
    vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    
    
    glGenVertexArrays( 1, &vao[2] );
    glBindVertexArray( vao[2] );
    
    glGenBuffers( 2, vbo2 );
    glBindBuffer( GL_ARRAY_BUFFER, vbo2[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(glm::vec4), &vertices2[0], GL_STATIC_DRAW);
    
    vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    
    glBindBuffer( GL_ARRAY_BUFFER, vbo2[1]);
    glBufferData(GL_ARRAY_BUFFER, normals2.size() * sizeof(glm::vec4), &normals2[0], GL_STATIC_DRAW);
    
    vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    
    
    glGenVertexArrays( 1, &vao[3] );
    glBindVertexArray( vao[3] );
    
    glGenBuffers( 2, vbo3 );
    glBindBuffer( GL_ARRAY_BUFFER, vbo3[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices3.size() * sizeof(glm::vec4), &vertices3[0], GL_STATIC_DRAW);
    
    vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    
    glBindBuffer( GL_ARRAY_BUFFER, vbo3[1]);
    glBufferData(GL_ARRAY_BUFFER, normals3.size() * sizeof(glm::vec4), &normals3[0], GL_STATIC_DRAW);

    vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    
    
    
    // Initialize shader lighting parameters
    point4 light_position( 1.5, 1.5, 2.0, 1.0 );
    color4 light_ambient( 0.2, 0.2, 0.2, 1.0 );
    color4 light_diffuse( 1.0, 1.0, 1.0, 1.0 );
    color4 light_specular( 1.0, 1.0, 1.0, 1.0 );

    color4 material_ambient( 1.0, 0.0, 1.0, 1.0 );
    color4 material_diffuse( 1.0, 0.8, 0.0, 1.0 );
    color4 material_specular( 1.0, 0.8, 0.0, 1.0 );
    float  material_shininess = 100.0;

    color4 ambient_product = light_ambient * material_ambient;
    color4 diffuse_product = light_diffuse * material_diffuse;
    color4 specular_product = light_specular * material_specular;

    glUniform4fv( glGetUniformLocation(program, "AmbientProduct"),
		  1, ambient_product );
    glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"),
		  1, diffuse_product );
    glUniform4fv( glGetUniformLocation(program, "SpecularProduct"),
		  1, specular_product );

    glUniform4fv( glGetUniformLocation(program, "LightPosition"),
		  1, light_position );

    glUniform1f( glGetUniformLocation(program, "Shininess"),
		 material_shininess );


    model_view = glGetUniformLocation( program, "ModelView" );
    projection = glGetUniformLocation( program, "Projection" );

//---------------------------------------------------------------------------

    mat4 p;
    
    if (proj == 'P') {
        p = Perspective(yfov, aspect, near, far);
    }
    else if (proj == 'O'){
        p = Ortho(left, right, bottom, top, near, far);
    }

    point4  eye( eyex, eyey, eyez, 1.0 );
    point4  at( atx, aty, atz, 1.0 );
    Angel::vec4    up( upx, upy, upz, 0.0 );


    mat4  mv = LookAt( eye, at, up );

    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    glUniformMatrix4fv( projection, 1, GL_TRUE, p );


    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
}
//----------------------------------------------------------------------------


void myMouse(GLint button, GLint state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        GLuint pickingColorID = glGetUniformLocation(program, "PickingColor");
        int choiceInt = glGetUniformLocation(program, "choose");
        glUniform1i(choiceInt, 1);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        for (int i = 2; i <= 3; i++) {
            int r = (i & 0x000000FF) >> 0;
            int g = (i & 0x0000FF00) >> 8;
            int b = (i & 0x00FF0000) >> 16;
            
            
            glBindVertexArray(vao[i]);
            glUniform4f(pickingColorID, r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
            
            if(i == 1){
                glDrawArrays(GL_TRIANGLES, 0, vertices1.size());
            }
            if(i == 2) {
                glDrawArrays(GL_TRIANGLES, 0, vertices2.size());
            }
            if(i == 3) {
                glDrawArrays(GL_TRIANGLES, 0, vertices3.size());
            }
        }
        
        GLubyte pixel[4];
        glReadPixels(x, currentHeight - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
        wireFlag = pixel[0] + (pixel[1] * 256) + (pixel[2] * 256 * 256);

        printf("mouse: %d, %d\n", x, y);
        printf("color: %d %d %d %d\n", pixel[0], pixel[1], pixel[2], pixel[3]);
        printf("Wireflag: %d\n\n", wireFlag);
        
        
    }
    //glutSwapBuffers();
}

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    int choiceInt = glGetUniformLocation(program, "choose");
    
    for(int i = 2; i <= 3; i++) {
        if(wireFlag == i) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glPolygonOffset(1.0, 2);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        
        glUniform1i(choiceInt, 0);
        glBindVertexArray(vao[i]);
        if(i == 1) {
            glDrawArrays( GL_TRIANGLES, 0, vertices1.size() );
        }
        if(i == 2) {
            glDrawArrays( GL_TRIANGLES, 0, vertices2.size() );
        }
        if(i == 3) {
            glDrawArrays( GL_TRIANGLES, 0, vertices3.size() );
        }
        
    }
    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
	case 033:  // Escape key
	case 'q': case 'Q':
	    exit( EXIT_SUCCESS );
	    break;
    }
}

//----------------------------------------------------------------------------

/* glut.h includes gl.h and glu.h*/

int main(int argc, char** argv)
{
    //read in scn file
    if(argc == 2){
        scnfile = argv[1];
    }
    
    load();
    
    glutInit(&argc, argv);
#ifdef __APPLE__
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DEPTH);
#else
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
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
    glutReshapeFunc(myReshape);
    glutMouseFunc(myMouse);
    glutDisplayFunc(display);

    glutMainLoop();

    return(0);
}
