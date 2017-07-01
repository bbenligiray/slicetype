#ifndef OPENGL_H
#define OPENGL_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <iostream>
#include "enums.h"

void initGL()
{
	//Create OpenGL 2.1 context
    glutInitContextVersion(2, 1);

    //Create Double Buffered Window with multisampled buffer
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowSize (SCREEN_SIZE + 16, SCREEN_SIZE + 16);
    glutCreateWindow ("SliceType");

	//Initialize GLEW
	glewInit();

	//Set the viewport
	glViewport( 0.f, 0.f, SCREEN_SIZE, SCREEN_SIZE);

	//Initialize Projection Matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0.0, SCREEN_SIZE + 8, SCREEN_SIZE, -8, 1.0, -1.0 );

	//Initialize Modelview Matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	//Initialize clear color
	glClearColor( 0.f, 0.f, 0.f, 1.f );

	//Set blending
	glEnable( GL_BLEND );
	glDisable( GL_DEPTH_TEST );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	//Set antialiasing/multisampling
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	glEnable( GL_LINE_SMOOTH );
	glEnable( GL_POLYGON_SMOOTH );
	glEnable( GL_MULTISAMPLE );
	glDisable(GL_TEXTURE_2D);
}

#endif
