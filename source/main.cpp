#include "main.h"

//text addition
#include <fstream>
#include <sstream>
//text addition

int main( int argc, char* args[] )
{	
	
	string line;
	ifstream myfile ("options.txt");
	std::istringstream convert;
	int number = 1000;
	if (myfile.is_open())
	{
		getline (myfile,line);
		getline (myfile,line);
		convert = std::istringstream(line);
		convert >> number;
		selectTime = (float)number / 1000;

		getline (myfile,line);
		getline (myfile,line);
		convert = std::istringstream(line);
		convert >> number;
		morphTime = (float)number / 1000;

		myfile.close();

		selectCount = (int)(selectTime * SCREEN_FPS);
		morphCount = (int)(morphTime * SCREEN_FPS);
	}

	else
		return 1;

	glutInit(&argc, args);
	initGL ();

	//Set functions
	glutDisplayFunc (render);
	glutPassiveMotionFunc (handleMousePos);
	glutMouseFunc (handleMouseClick);
	glutKeyboardFunc (handleKeys);

	//Set loop
	glutTimerFunc (1000 / SCREEN_FPS, runMainLoop, 0);
	glutMainLoop ();



	return 0;
}

void runMainLoop (int val)
{
	render();

	glutTimerFunc (1000 / SCREEN_FPS, runMainLoop, val);
}



void render ()
{

	kbMain.tickCounter ();
	kbMain.TEMPDRAW();
	glutSwapBuffers();
}

void handleMousePos (int x, int y)
{
	kbMain.updateMouseCell (x, y);
}

void handleMouseClick (int button, int state, int x, int y)
{
	switch (button)
	{
	case 0:
		cout << "left button ";
		break;
	case 2:
		cout << "right button ";
		break;
	default:
		return;
	}
	switch (state)
	{
	case 0:
		cout << "down" << endl;
		break;
	case 1:
		cout << "up" << endl;
		break;
	}
}

void handleKeys (unsigned char key, int x, int y)
{
	cout << key << endl;
}

