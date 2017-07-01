#ifndef MOUSESTATUS_H
#define MOUSESTATUS_H

#define M_PI 3.14159265358979323846

#include <cmath>
#include "enums.h"


class MouseStatus
{
	int loop;
	int row;
	void convertCartToPolar (int x, int y, double &angle, double &radius);
	void ConvertPolartoField(double angle, double radius, int &loop, int &row);
public:
	MouseStatus();
	bool updateCell (int x, int y);
	void getCell (int &outLoop, int &outRow);
};

MouseStatus::MouseStatus()
{
	loop = -1;
	row = -1;
}

bool MouseStatus::updateCell (int x, int y)
{
	x = (x / (double)SCREEN_SIZE_X) * SCREEN_SIZE;
	y = (y / (double)SCREEN_SIZE_Y) * SCREEN_SIZE;


	int border = 5;
	int newLoop = 0;
	int newRow = 0;

	if ((x < border) || (x > SCREEN_SIZE - border) || (y < border) || (y > SCREEN_SIZE - border))
	{
		newLoop = -1;
		newRow = -1;
	}
	else
	{
		double angle = 0;
		double radius = 0;
		convertCartToPolar (x, y, angle, radius);
		ConvertPolartoField(angle, radius, newLoop, newRow);
	}

	if ((loop != newLoop) || (row != newRow))
	{
		loop = newLoop;
		row = newRow;
		return true;
	}
	else
		return false;
}

void MouseStatus::getCell (int &outLoop, int &outRow)
{
	outLoop = loop;
	outRow = row;
}

void MouseStatus::convertCartToPolar (int x, int y, double &angle, double &radius)
{
	


	angle = -(atan2 (y - SCREEN_SIZE / 2, x - SCREEN_SIZE / 2) / M_PI) * 180;
	if (angle < 0)
		angle += 360;

	radius = sqrt (pow (y - SCREEN_SIZE / 2, 2) + pow (x -  SCREEN_SIZE / 2, 2));

	/*angle = -(atan2 (y - SCREEN_SIZE / 2, x - SCREEN_SIZE / 2) / M_PI) * 180;
	if (angle < 0)
		angle += 360;

	radius = sqrt (pow (y - SCREEN_SIZE / 2, 2) + pow (x -  SCREEN_SIZE / 2, 2));*/
}

void MouseStatus::ConvertPolartoField(double angle, double radius, int &loop, int &row)
{
	if (radius < radi0)
	{
		loop = 0;
		if (angle < 180)
			row = 0;
		else
			row = 1;
		return;
	}
	else if (radius < radi1)
		loop = 1;
	else if (radius < radi2)
		loop = 2;
	else
	{
		loop = 3;
		if (angle < 90)
			row = 0;
		else if (angle < 180)
			row = 1;
		else if (angle < 270)
			row = 2;
		else
			row = 3;
		return;
	}
	row = floor (angle / 30);
}

#endif