#ifndef BUTTON_H
#define BUTTON_H

#include "DiskPart.h"
#include "ValidChar.h"
#include "PosChart.h"
#include "enums.h"

int getPosOfChar (const char *array, char c);
void getLocOfChar (char c, int &loop, int &row);

//Holds every aspect of a button
class Button 
{
	DiskPart buttonShape;
	ValidChar writing;

public:
	Button ();
	Button (ValidChar inpWriting);
	DiskPart& getButtonShape();
	ValidChar& getWriting();
};

Button::Button ()
{
}

Button::Button (ValidChar inpWriting)
{
	writing=inpWriting;

	int loop, row;
	getLocOfChar (writing.getChar(), loop, row);

	buttonShape = DiskPart(loop,row);
}

DiskPart& Button::getButtonShape()
{
	return buttonShape;
}

ValidChar& Button::getWriting()
{
	return writing;
}

int getPosOfChar (const char *array, char c)
{
	size_t size = 26;	//not cool
	const char* end = array + size;
	const char* match = std::find (array, end, c);
	return (end == match) ? -1 : (match-array);
}

void getLocOfChar (char c, int &loop, int &row)
{
	int noOfChar = getPosOfChar (firstScreenChars, c);

	if (noOfChar == -1)
		noOfChar = getPosOfChar (secondScreenChars, c);

	//middle loop
	if (noOfChar < 2)
	{
		loop = 0;
		//first half
		if (noOfChar == 0)
			row = 0;
		//second half
		else
			row = 1;
	}
	//second ring
	else if (noOfChar < 14)
	{
		loop = 1;
		row = (noOfChar - 2) % 12;
	}
	//third ring
	else
	{
		loop = 2;
		row = (noOfChar - 2) % 12;
	}
}


#endif