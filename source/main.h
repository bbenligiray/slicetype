#ifndef MAIN_H
#define MAIN_H

#include "openGL.h"
#include "KeyMain.h"

void runMainLoop( int val );
void update();
void render();

void handleMousePos (int x, int y);
void handleMouseClick (int button, int state, int x, int y);
void handleKeys (unsigned char key, int x, int y);

KeyMain kbMain;

using std::string;
using std::cout;
using std::endl;


#endif