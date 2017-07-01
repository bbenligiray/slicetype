#ifndef DRAWER_H
#define DRAWER_H

#define M_PI 3.14159265358979323846

#include <cmath>
#include <string>
#include <sstream>
#include <cctype>
#include <windows.h>
#include "openGL.h"
#include "enums.h"
#include "ButtonSet.h"

class Drawer
{
	vector<Button> butFadeIn;
	vector<Button> butFadeOut;
	vector<Button> butSame;
	vector<Button> butMorphNew;
	vector<Button> butMorphOld;

	void drawText(Button but, double countDouble, bool FadeIn, bool incPred);
	void morphText(Button oldBut, Button newBut, kbState state, double morphDouble, bool incPred);
	void setColor (colorTypes colorCode, float alpha);
	void convertPolartoCart(double angle, double radius, float &x, float &y);
	void extractShapeParams (Button but, double &innerRad, double &outerRad, double &startAngle, double &stopAngle, double &sweepAngle);
	double fanBetweenTwoAngles (double angle1, double angle2);


	void drawMorphingInnerButton (GLUquadric* obj,double startAngle1 , double startAngle2, double sweepAngle1, colorTypes color, double morphRatio);
	void drawInnerButton (GLUquadric* obj, double startAngle, double sweepAngle, double fillRatio, colorTypes frontColor, colorTypes backColor, double alpha);
	void drawFillingButton (GLUquadric* obj, double innerRad, double outerRad, double startAngle, double sweepAngle, double fillRatio, colorTypes frontColor);
	void drawRegularButton (GLUquadric* obj, double innerRad, double outerRad, double startAngle, double sweepAngle, colorTypes frontColor, double alpha);
	void updateButtonVectors (ButtonSet *newButtons, ButtonSet *oldButtons);
	bool areButtonShapesSame (Button &but1, Button &but2);
	void drawBackground(GLUquadric* obj, kbState state, double countDouble, char currChar, string currWord);


public:
	void drawButtons (ButtonSet *newButtons, ButtonSet *oldButtons, kbState state, bool boolMorph, double countDouble, char currChar, string currentWord);
};

void Drawer::drawButtons(ButtonSet *newButtons, ButtonSet *oldButtons, kbState state, bool boolMorph, double countDouble, char currChar, string currentWord)
{
	double innerradi1, outerradi1, startAngle1, stopAngle1, sweepAngle1, innerradi2, outerradi2, startAngle2, stopAngle2, sweepAngle2;

	//// mouse dislocation

	double limit = 0.2;

	if ((boolMorph) && (currChar != '\n') && (countDouble < limit))
	{
		int indexNew=-1;
		bool skip = false;
		for (int i = 0; i < newButtons->getButtons().size(); i++)
		{
			if (newButtons->getButtons()[i].getWriting().getChar() == currChar)
				indexNew=i;
		}
		if (indexNew==-1)
			skip = true;
		
		if (!skip)
		{
			extractShapeParams(newButtons->getButtons()[indexNew], innerradi2, outerradi2, startAngle2, stopAngle2, sweepAngle2);

			float newX, newY;
			if ((innerradi2==0) && (sweepAngle2 == 360))
			{
				newX=0;
				newY=0;
			}
			else
			{
				double radius = (outerradi2 + innerradi2) / 2;
				double angle = startAngle2 + (sweepAngle2 / 2);
				convertPolartoCart (angle, radius, newX, newY);
			}

			HWND tW = GetActiveWindow();
			RECT rect;
			GetWindowRect(tW, &rect);

			//newX += (SCREEN_SIZE / 2 + 4);
			//newY += (SCREEN_SIZE / 2 + 16);
			newX += SCREEN_SIZE / 2;
			newY += SCREEN_SIZE / 2;

			//added to rectify resize bug
			newX = (newX/SCREEN_SIZE)*(rect.right-rect.left-38);
			newY = (newY/SCREEN_SIZE)*(rect.bottom-rect.top-16);
			newX += 4;
			newY += 16;
			//added to rectify resize bug

			newX+=rect.left;
			newY+=rect.top;

			POINT mPoint;
			GetCursorPos(&mPoint);
			float deltaX = (newX - mPoint.x) * (countDouble / limit);
			float deltaY = (newY - mPoint.y) * (countDouble / limit);


			SetCursorPos(mPoint.x + deltaX, mPoint.y + deltaY);
		}

	}

	//// mouse dislocation

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();
	glTranslatef (SCREEN_SIZE / 2.f, SCREEN_SIZE / 2.f, 0.f);
	GLUquadric* obj = gluNewQuadric ();

	

	drawBackground(obj, state, countDouble, currChar, currentWord);

	if (boolMorph)
	{
		if (countDouble == 0)
			updateButtonVectors (newButtons, oldButtons);

		for (vector<Button>::size_type i = 0; i <butMorphNew.size(); i ++)
		{
			extractShapeParams(butMorphOld[i], innerradi1, outerradi1, startAngle1, stopAngle1, sweepAngle1);
			extractShapeParams(butMorphNew[i], innerradi2, outerradi2, startAngle2, stopAngle2, sweepAngle2);

			if (innerradi1 == 0)
			{
				if (butMorphOld[i].getWriting().getChar() == currChar)
					drawMorphingInnerButton(obj, startAngle1, startAngle2, sweepAngle1, colorBack, countDouble);
				else
					drawMorphingInnerButton(obj, startAngle1, startAngle2, sweepAngle1, colorFront, countDouble);
			}
			else
			{
				double innerRad, outerRad, startAngle, sweepAngle;
				innerRad = innerradi1 + (innerradi2 - innerradi1) * countDouble;
				outerRad = outerradi1 + (outerradi2 - outerradi1) * countDouble;

				double angleFan = fanBetweenTwoAngles (startAngle1, startAngle2);

				if (startAngle1 < startAngle2)															
				{
					if (startAngle1 + 180 > startAngle2)
					{

						startAngle = startAngle1 + angleFan * countDouble;
					}
					else
					{
						startAngle = startAngle1 - angleFan * countDouble;
					}
				}
				else
				{
					if (startAngle1 - 180 < startAngle2)
					{
						startAngle = startAngle1 - angleFan * countDouble;
					}
					else
					{
						startAngle = startAngle1 + angleFan * countDouble;
					}
				}
				if (startAngle > 360)
					startAngle -= 360;

				sweepAngle = sweepAngle1 + (sweepAngle2 - sweepAngle1) * countDouble;

				if (butMorphOld[i].getWriting().getChar() == currChar)
					drawRegularButton(obj, innerRad, outerRad, startAngle, sweepAngle, colorBack, 1);
				else
					drawRegularButton(obj, innerRad, outerRad, startAngle, sweepAngle, colorFront, 1);
			}
		}

		for (vector<Button>::size_type i = 0; i <butSame.size(); i ++)
		{
			extractShapeParams(butSame[i], innerradi1, outerradi1, startAngle1, stopAngle1, sweepAngle1);

			if (innerradi1 == 0)
			{
				if (butSame[i].getWriting().getChar() == currChar)
					drawInnerButton(obj, startAngle1, sweepAngle1, 0, colorBack, colorBack, 1);
				else
					drawInnerButton(obj, startAngle1, sweepAngle1, 0, colorFront, colorFront, 1);
			}
			else
			{
				if (butSame[i].getWriting().getChar() == currChar)
					drawRegularButton(obj, innerradi1, outerradi1, startAngle1, sweepAngle1, colorBack, 1);
				else
					drawRegularButton(obj, innerradi1, outerradi1, startAngle1, sweepAngle1, colorFront, 1);
			}
		}

		for (vector<Button>::size_type i = 0; i <butFadeIn.size(); i ++)
		{
			extractShapeParams(butFadeIn[i], innerradi1, outerradi1, startAngle1, stopAngle1, sweepAngle1);

			if (innerradi1 == 0)
			{
				if (butFadeIn[i].getWriting().getChar() == currChar)
					drawInnerButton(obj, startAngle1, sweepAngle1, 0, colorBack, colorBack, countDouble);
				else
					drawInnerButton(obj, startAngle1, sweepAngle1, 0, colorFront, colorFront, countDouble);
			}
			else
			{
				if (butFadeIn[i].getWriting().getChar() == currChar)
					drawRegularButton(obj, innerradi1, outerradi1, startAngle1, sweepAngle1, colorBack, countDouble);
				else
					drawRegularButton(obj, innerradi1, outerradi1, startAngle1, sweepAngle1, colorFront, countDouble);
			}
		}

		for (vector<Button>::size_type i = 0; i <butFadeOut.size(); i ++)
		{
			extractShapeParams(butFadeOut[i], innerradi1, outerradi1, startAngle1, stopAngle1, sweepAngle1);

			if (innerradi1 == 0)
			{
				if (butFadeOut[i].getWriting().getChar() == currChar)
					drawInnerButton(obj, startAngle1, sweepAngle1, 0, colorBack, colorBack, 1 - countDouble);
				else
					drawInnerButton(obj, startAngle1, sweepAngle1, 0, colorFront, colorFront, 1 - countDouble);
			}
			else
			{
				if (butFadeOut[i].getWriting().getChar() == currChar)
					drawRegularButton(obj, innerradi1, outerradi1, startAngle1, sweepAngle1, colorBack, 1 - countDouble);
				else
					drawRegularButton(obj, innerradi1, outerradi1, startAngle1, sweepAngle1, colorFront, 1 - countDouble);
			}
		}

		//text
		for (vector<Button>::size_type i = 0; i <butMorphNew.size(); i ++)
		{
			if (butMorphOld[i].getWriting().getChar() == currChar)
				morphText(butMorphOld[i], butMorphNew[i], state, countDouble, true);
			else
				morphText(butMorphOld[i], butMorphNew[i], noSelect, countDouble, false);
		}
		for (vector<Button>::size_type i = 0; i <butSame.size(); i ++)
		{
			if (butSame[i].getWriting().getChar() == currChar)
				drawText (butSame[i], 1, true, true);
			else
				drawText (butSame[i], 1, true, false);
		}
		for (vector<Button>::size_type i = 0; i <butFadeIn.size(); i ++)
		{
			if (butFadeIn[i].getWriting().getChar() == currChar)
				drawText (butFadeIn[i], countDouble, true, true);
			else
				drawText (butFadeIn[i], countDouble, true, false);
		}
		for (vector<Button>::size_type i = 0; i <butFadeOut.size(); i ++)
			drawText (butFadeOut[i], countDouble, false, false);


	}
	else
	{
		for (vector<Button>::size_type i = 0; i < newButtons->getButtons().size(); i ++)
		{
			extractShapeParams(newButtons->getButtons()[i], innerradi1, outerradi1, startAngle1, stopAngle1, sweepAngle1);

			if (innerradi1 == 0)
			{
				if (newButtons->getButtons()[i].getWriting().getChar() == currChar)
				{
					if (state == select1)
						drawInnerButton(obj, startAngle1, sweepAngle1, countDouble, colorS1, colorBack, 1);
					else if (state == select2)
						drawInnerButton(obj, startAngle1, sweepAngle1, countDouble, colorS2, colorBack, 1);
					else
						drawInnerButton(obj, startAngle1, sweepAngle1, 1, colorFront, colorFront, 1); //shouldn't happen anyways
				}
				else
					drawInnerButton(obj, startAngle1, sweepAngle1, 1, colorFront, colorFront, 1);
			}
			else
			{
				if (newButtons->getButtons()[i].getWriting().getChar() == currChar)
				{
					if (state == select1)
						drawFillingButton(obj, innerradi1, outerradi1, startAngle1, sweepAngle1, countDouble, colorS1);
					else if (state == select2)
						drawFillingButton(obj, innerradi1, outerradi1, startAngle1, sweepAngle1, countDouble, colorS2);
					else
						drawFillingButton(obj, innerradi1, outerradi1, startAngle1, sweepAngle1, countDouble, colorFront); //shouldn't happen anyways
				}
				else
					drawRegularButton(obj, innerradi1, outerradi1, startAngle1, sweepAngle1, colorFront, 1);
			}
		}
		if (currentWord == "~")
		{
			for (vector<Button>::size_type i = 0; i < newButtons->getButtons().size(); i ++)
			{

				drawText (newButtons->getButtons()[i], 1, true, false);
			}
		}
		else
		{
			for (vector<Button>::size_type i = 0; i < newButtons->getButtons().size(); i ++)
			{
				if (newButtons->getButtons()[i].getWriting().getChar() == currChar)
					drawText (newButtons->getButtons()[i], 1, true, true);
				else
					drawText (newButtons->getButtons()[i], 1, true, false);
			}
		}

	}

	gluDeleteQuadric (obj);
}


void Drawer::drawText(Button but, double countDouble, bool FadeIn, bool incPred)
{
	double innerRad, outerRad, startAngle, stopAngle, sweepAngle;
	extractShapeParams (but, innerRad, outerRad, startAngle, stopAngle, sweepAngle);

	float rastX, rastY;
	if ((outerRad == radi0) && (sweepAngle == 360))
	{
		rastX = 0;
		rastY = 0;
	}
	else
	{
		double radius = (outerRad+innerRad) / 2;
		double angle = startAngle + (sweepAngle / 2);
		convertPolartoCart (angle, radius, rastX, rastY);
	}

	glLineWidth (textWidth);
	glLoadIdentity ();
	glTranslatef (SCREEN_SIZE / 2.f, SCREEN_SIZE / 2.f, 0.f);

	if (FadeIn)
		setColor (black, countDouble);
	else
		setColor (black, 1-countDouble);

	float totLength = glutStrokeWidth(GLUT_STROKE_ROMAN, but.getWriting ().getChar ()) * charScale;
	float totHeight = 119.05 * charScale;
	rastX -= totLength / 2;
	rastY += totHeight / 2;

	glTranslatef(rastX, rastY, 0);
	glScalef(charScale, -charScale, charScale);
	glutStrokeCharacter(GLUT_STROKE_ROMAN, toupper (but.getWriting ().getChar ()));

	if (incPred)
	{
		glLineWidth (textSmallWidth);
		rastX += totLength / 2;
		rastY += 33.33 * charScale;

		string pred = but.getWriting ().getPrediction ();
		totLength = 0;
		for (int i = 0; i < pred.size (); i++)
			totLength += (glutStrokeWidth(GLUT_STROKE_ROMAN, pred[i]) + textSpacing);
		totLength -= textSpacing;
		totLength *= predScale;
		totHeight = 119.05 * predScale;

		rastX -= totLength / 2;
		rastY += totHeight;


		glLoadIdentity ();
		glTranslatef (SCREEN_SIZE / 2.f, SCREEN_SIZE / 2.f, 0.f);
		glTranslatef(rastX, rastY, 0);
		glScalef(predScale, -predScale, predScale);

		for (int i = 0; i < pred.size (); i++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, pred[i]);
			glTranslatef(glutStrokeWidth(GLUT_STROKE_ROMAN, pred[i]) * predScale + textSpacing, 0, 0);
		}
	}
}


void Drawer::setColor (colorTypes colorCode, float alpha)
{
	switch (colorCode)
	{
	case cornflowerBlue:
		glColor4f( 0.392f, 0.584f, 0.929f, alpha);
		break;
	case paleOrange:
		glColor4f( 1.f, 0.898f, 0.706f, alpha);
		break;
	case orange:
		glColor4f( 1.f, 0.5f, 0.f, alpha);
		break;
	case black:
		glColor4f( 0.f, 0.f, 0.f, alpha);
		break;
	case green:
		glColor4f( 0.f, 1.f, 0.f, alpha);
		break;
	case purple:
		glColor4f( 0.624f, 0.475f, 0.933f, alpha);
		break;
	case coral:
		glColor4f( 0.933f, 0.415f, 0.314f, alpha);
		break;
	case warmgrey:
		glColor4f( 0.5f, 0.5f, 0.418f, alpha);
		break;
	case olivedrab:
		glColor4f( 0.420f, 0.557f, 0.137f, alpha);
		break;
	case goldenrod:
		glColor4f( 0.803f, 0.584f, 0.047f, alpha);
		break;
	case turquoise:
		glColor4f( 0.f, 0.773f, 0.804f, alpha);
		break;

	}

}

void Drawer::convertPolartoCart(double angle, double radius, float &x, float &y)
{
	x = cos((angle / 180) * M_PI) * radius;
	y = sin((-angle / 180) * M_PI) * radius;
	if (angle == 0)
	{
		x = radius;
		y = 0;
	}
	else if (angle == 90)
	{
		x = 0;
		y = -radius;
	}
	else if (angle == 180)
	{
		x = -radius;
		y = 0;
	}
	else if (angle == 270)
	{
		x = 0;
		y = radius;
	}
}

void Drawer::extractShapeParams (Button but, double &innerRad, double &outerRad, double &startAngle, double &stopAngle, double &sweepAngle)
{
	int loopStart, loopEnd, rowStart, rowEnd;
	but.getButtonShape ().getShape (loopStart, loopEnd, rowStart, rowEnd);

	startAngle = rowStart * 30;
	stopAngle = rowEnd * 30;

	switch (loopStart)
	{
	case 0:
		innerRad = 0;
		break;
	case 1:
		innerRad = radi0;
		break;
	case 2:
		innerRad = radi1;
		break;
	}

	switch (loopEnd)
	{
	case 1:
		outerRad = radi0;
		break;
	case 2:
		outerRad = radi1;
		break;
	case 3:
		outerRad = radi2;
		break;
	}

	if (rowEnd - rowStart > 0)
		sweepAngle = stopAngle - startAngle;
	else
		sweepAngle = 360 + stopAngle - startAngle;
}



double Drawer::fanBetweenTwoAngles (double angle1, double angle2)
{
	double high, low;
	if (angle1 < angle2)
	{
		high = angle2;
		low = angle1;
	}
	else
	{
		high = angle1;
		low = angle2;
	}

	double fan1 = high-low;
	double fan2 = 360-high+low;

	if (fan1 < fan2)
		return fan1;
	else
		return fan2;
}
void Drawer::morphText(Button oldBut, Button newBut, kbState state, double morphDouble, bool incPred)
{
	glLoadIdentity ();
	glTranslatef (SCREEN_SIZE / 2.f, SCREEN_SIZE / 2.f, 0.f);

	double innerradi1, outerradi1, startAngle1, stopAngle1, sweepAngle1, innerradi2, outerradi2, startAngle2, stopAngle2, sweepAngle2;
	extractShapeParams (oldBut, innerradi1, outerradi1, startAngle1, stopAngle1, sweepAngle1);
	extractShapeParams (newBut, innerradi2, outerradi2, startAngle2, stopAngle2, sweepAngle2);

	double innerRad, outerRad, startAngle, stopAngle, sweepAngle;
	innerRad = innerradi1 + (innerradi2 - innerradi1) * morphDouble;
	outerRad = outerradi1 + (outerradi2 - outerradi1) * morphDouble;

	double angleFan = fanBetweenTwoAngles (startAngle1, startAngle2);

	if (startAngle1 < startAngle2)															
	{
		if (startAngle1 + 180 > startAngle2)
		{

			startAngle = startAngle1 + angleFan * morphDouble;
		}
		else
		{
			startAngle = startAngle1 - angleFan * morphDouble;
		}
	}
	else
	{
		if (startAngle1 - 180 < startAngle2)
		{
			startAngle = startAngle1 - angleFan * morphDouble;
		}
		else
		{
			startAngle = startAngle1 + angleFan * morphDouble;
		}
	}
	if (startAngle > 360)
		startAngle -= 360;

	sweepAngle = sweepAngle1 + (sweepAngle2 - sweepAngle1) * morphDouble;
	stopAngle = startAngle + sweepAngle;
	if (stopAngle > 360)
		stopAngle -= 360;



	float rastX, rastY;
	if (innerradi1 == 0)
	{
		float y1, y2;

		if (sweepAngle1 == 180)
		{
			if (startAngle1 == 0)
				y1 = -radi0 / 2;
			else
				y1 = radi0 / 2;
		}
		else
			y1 = 0;

		if (sweepAngle2 == 180)
		{
			if (startAngle2 == 0)
				y2 = -radi0 / 2;
			else
				y2 = radi0 / 2;
		}
		else
			y2 = 0;

		rastX = 0;
		rastY = y1 + (y2 - y1) * morphDouble;
	}
	else
	{
		double radius = (outerRad+innerRad) / 2;
		double angle = startAngle + (sweepAngle / 2);
		convertPolartoCart (angle, radius, rastX, rastY);
	}

	glLineWidth (textWidth);


	setColor (black, 1);

	float totLength = glutStrokeWidth(GLUT_STROKE_ROMAN, newBut.getWriting ().getChar ()) * charScale;
	float totHeight = 119.05 * charScale;
	rastX -= totLength / 2;
	rastY += totHeight / 2;

	glTranslatef(rastX, rastY, 0);
	glScalef(charScale, -charScale, charScale);
	glutStrokeCharacter(GLUT_STROKE_ROMAN, toupper (newBut.getWriting ().getChar ()));

	if (incPred)
	{
		glLineWidth (textSmallWidth);
		rastX += totLength / 2;
		rastY += 33.33 * charScale;

		string pred = newBut.getWriting ().getPrediction ();
		totLength = 0;
		for (int i = 0; i < pred.size (); i++)
			totLength += (glutStrokeWidth(GLUT_STROKE_ROMAN, pred[i]) + textSpacing);
		totLength -= textSpacing;
		totLength *= predScale;
		totHeight = 119.05 * predScale;

		rastX -= totLength / 2;
		rastY += totHeight;


		glLoadIdentity ();
		glTranslatef (SCREEN_SIZE / 2.f, SCREEN_SIZE / 2.f, 0.f);
		glTranslatef(rastX, rastY, 0);
		glScalef(predScale, -predScale, predScale);

		for (int i = 0; i < pred.size (); i++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, pred[i]);
			glTranslatef(glutStrokeWidth(GLUT_STROKE_ROMAN, pred[i]) * predScale + textSpacing, 0, 0);
		}
	}
}




void Drawer::drawMorphingInnerButton (GLUquadric* obj,double startAngle1 , double startAngle2, double sweepAngle1, colorTypes color, double morphRatio)
{

	setColor (color, 1);

	gluPartialDisk (obj,  0,  radi0,  slices,  loops,  0,  360);

	setColor (black, 1);

	if (sweepAngle1 == 180)	
	{
		if (startAngle1 == 0) //up to full	
		{
			glBegin(GL_POLYGON);
			glVertex3f(-radi0, radi0 * morphRatio, 0.0f);
			glVertex3f(radi0, radi0 * morphRatio, 0.0f);
			glVertex3f(radi0, radi0, 0.0f);
			glVertex3f(-radi0, radi0, 0.0f);
			glEnd();
		}
		else // down to full
		{
			glBegin(GL_POLYGON);
			glVertex3f(-radi0, -radi0 * morphRatio, 0.0f);
			glVertex3f(radi0, -radi0 * morphRatio, 0.0f);
			glVertex3f(radi0, -radi0, 0.0f);
			glVertex3f(-radi0, -radi0, 0.0f);
			glEnd();
		}
	}
	else
	{
		if (startAngle2 == 0) // full to up
		{
			glBegin(GL_POLYGON);
			glVertex3f(-radi0, radi0 - radi0 * morphRatio, 0.0f);
			glVertex3f(radi0, radi0 - radi0 * morphRatio, 0.0f);
			glVertex3f(radi0, radi0, 0.0f);
			glVertex3f(-radi0, radi0, 0.0f);
			glEnd();
		}
		else //full to down
		{
			glBegin(GL_POLYGON);
			glVertex3f(-radi0, -radi0 + radi0 * morphRatio, 0.0f);
			glVertex3f(radi0, -radi0 + radi0 * morphRatio, 0.0f);
			glVertex3f(radi0, -radi0, 0.0f);
			glVertex3f(-radi0, -radi0, 0.0f);
			glEnd();
		}
	}

}

void Drawer::drawInnerButton (GLUquadric* obj, double startAngle, double sweepAngle, double fillRatio, colorTypes frontColor, colorTypes backColor, double alpha)
{
	double innerRad, outerRad, fillOuterRad;
	innerRad = 0;
	outerRad = radi0;
	fillOuterRad = innerRad + (outerRad - innerRad) * fillRatio;

	startAngle += 90;
	double stopAngle = startAngle + sweepAngle;
	if (stopAngle > 360)
		stopAngle -= 360;


	if (frontColor == backColor)
	{
		setColor (colorFront, alpha);
		gluPartialDisk (obj,  innerRad,  outerRad,  slices,  loops,  startAngle,  sweepAngle);
	}
	else
	{
		setColor (backColor, alpha);
		gluPartialDisk (obj,  innerRad,  outerRad,  slices,  loops,  startAngle,  sweepAngle);
		setColor (frontColor, alpha);
		gluPartialDisk (obj,  innerRad,  fillOuterRad,  slices,  loops,  startAngle,  sweepAngle);
	}


	setColor (black, alpha);

	gluPartialDisk (obj,  outerRad - circleBoundary,  outerRad,  slices,  loops,  startAngle,  sweepAngle);

	if (sweepAngle != 360)
	{
		glLineWidth (lineWidth);

		if (outerRad == radi0)
		{
			glBegin (GL_LINES);
			glVertex3f (innerRad + radi0, 0, 0);
			glVertex3f (innerRad - radi0, 0, 0);
			glEnd ();
		}
		else
		{
			float sinStartAngle = sin ((startAngle / 180) * M_PI);
			float cosStartAngle = cos ((startAngle / 180) * M_PI);
			float sinStopAngle = sin ((stopAngle / 180) * M_PI);
			float cosStopAngle = cos ((stopAngle / 180) * M_PI);

			glBegin (GL_LINES);
			glVertex3f (innerRad * sinStartAngle, innerRad * cosStartAngle, 0.0);
			glVertex3f (outerRad * sinStartAngle, outerRad * cosStartAngle, 0.0);
			glVertex3f (innerRad * sinStopAngle, innerRad * cosStopAngle, 0.0);
			glVertex3f (outerRad * sinStopAngle, outerRad * cosStopAngle, 0.0);
			glEnd ();
		}
	}


}

void Drawer::drawFillingButton (GLUquadric* obj, double innerRad, double outerRad, double startAngle, double sweepAngle, double fillRatio, colorTypes frontColor)
{
	double fillOuterRad = innerRad + (outerRad - innerRad) * fillRatio;

	startAngle += 90;
	double stopAngle = startAngle + sweepAngle;
	if (stopAngle > 360)
		stopAngle -= 360;

	setColor (colorBack, 1);
	gluPartialDisk (obj,  innerRad,  outerRad,  slices,  loops,  startAngle,  sweepAngle);

	setColor (frontColor, 1);
	gluPartialDisk (obj,  innerRad,  fillOuterRad,  slices,  loops,  startAngle,  sweepAngle);

	setColor (black, 1);
	gluPartialDisk (obj,  outerRad - circleBoundary,  outerRad,  slices,  loops,  startAngle,  sweepAngle);
	if (innerRad != 0)
		gluPartialDisk (obj,  innerRad,  innerRad + circleBoundary,  slices,  loops,  startAngle,  sweepAngle);

	if (sweepAngle != 360)
	{
		glLineWidth (lineWidth);

		if (outerRad == radi0)
		{
			glBegin (GL_LINES);
			glVertex3f (innerRad + radi0, 0, 0);
			glVertex3f (innerRad - radi0, 0, 0);
			glEnd ();
		}
		else
		{
			float sinStartAngle = sin ((startAngle / 180) * M_PI);
			float cosStartAngle = cos ((startAngle / 180) * M_PI);
			float sinStopAngle = sin ((stopAngle / 180) * M_PI);
			float cosStopAngle = cos ((stopAngle / 180) * M_PI);

			glBegin (GL_LINES);
			glVertex3f (innerRad * sinStartAngle, innerRad * cosStartAngle, 0.0);
			glVertex3f (outerRad * sinStartAngle, outerRad * cosStartAngle, 0.0);
			glVertex3f (innerRad * sinStopAngle, innerRad * cosStopAngle, 0.0);
			glVertex3f (outerRad * sinStopAngle, outerRad * cosStopAngle, 0.0);
			glEnd ();
		}
	}

}

void Drawer::drawRegularButton (GLUquadric* obj, double innerRad, double outerRad, double startAngle, double sweepAngle, colorTypes frontColor, double alpha)
{
	startAngle += 90;
	double stopAngle = startAngle + sweepAngle;
	if (stopAngle > 360)
		stopAngle -= 360;

	setColor (frontColor, alpha);
	gluPartialDisk (obj,  innerRad,  outerRad,  slices,  loops,  startAngle,  sweepAngle);

	setColor (black, alpha);
	gluPartialDisk (obj,  outerRad - circleBoundary,  outerRad,  slices,  loops,  startAngle,  sweepAngle);
	if (innerRad != 0)
		gluPartialDisk (obj,  innerRad,  innerRad + circleBoundary,  slices,  loops,  startAngle,  sweepAngle);

	if (sweepAngle != 360)
	{
		glLineWidth (lineWidth);

		if (outerRad == radi0)
		{
			glBegin (GL_LINES);
			glVertex3f (innerRad + radi0, 0, 0);
			glVertex3f (innerRad - radi0, 0, 0);
			glEnd ();
		}
		else
		{
			float sinStartAngle = sin ((startAngle / 180) * M_PI);
			float cosStartAngle = cos ((startAngle / 180) * M_PI);
			float sinStopAngle = sin ((stopAngle / 180) * M_PI);
			float cosStopAngle = cos ((stopAngle / 180) * M_PI);

			glBegin (GL_LINES);
			glVertex3f (innerRad * sinStartAngle, innerRad * cosStartAngle, 0.0);
			glVertex3f (outerRad * sinStartAngle, outerRad * cosStartAngle, 0.0);
			glVertex3f (innerRad * sinStopAngle, innerRad * cosStopAngle, 0.0);
			glVertex3f (outerRad * sinStopAngle, outerRad * cosStopAngle, 0.0);
			glEnd ();
		}
	}

}

void Drawer::updateButtonVectors (ButtonSet *newButtons, ButtonSet *oldButtons)
{
	butFadeIn.clear();
	butFadeOut.clear();
	butSame.clear();
	butMorphNew.clear();
	butMorphOld.clear();

	for (vector<Button>::size_type i = 0; i < oldButtons->getButtons().size(); i++)
	{
		int foundInd = -1;

		for (vector<Button>::size_type j = 0; j < newButtons->getButtons().size(); j++)
		{
			if (oldButtons->getButtons()[i].getWriting().getChar() == newButtons->getButtons()[j].getWriting().getChar())
			{
				foundInd = j;
				break;
			}
		}

		if (foundInd == -1)
			butFadeOut.push_back(oldButtons->getButtons()[i]);
		else if (areButtonShapesSame(oldButtons->getButtons()[i], newButtons->getButtons()[foundInd]))
			butSame.push_back(newButtons->getButtons()[foundInd]);
		else
		{
			butMorphOld.push_back(oldButtons->getButtons()[i]);
			butMorphNew.push_back(newButtons->getButtons()[foundInd]);
		}
	}

	for (vector<Button>::size_type i = 0; i < newButtons->getButtons().size(); i++)
	{
		int foundInd = -1;

		for (vector<Button>::size_type j = 0; j < oldButtons->getButtons().size(); j++)
		{
			if (newButtons->getButtons()[i].getWriting().getChar() == oldButtons->getButtons()[j].getWriting().getChar())
			{
				foundInd = j;
				break;
			}
		}

		if (foundInd == -1)
			butFadeIn.push_back(newButtons->getButtons()[i]);
	}
}

bool Drawer::areButtonShapesSame (Button &but1, Button &but2)
{
	int loopStart1, loopEnd1, rowStart1, rowEnd1, loopStart2, loopEnd2, rowStart2, rowEnd2;

	but1.getButtonShape().getShape(loopStart1, loopEnd1, rowStart1, rowEnd1);
	but2.getButtonShape().getShape(loopStart2, loopEnd2, rowStart2, rowEnd2);

	if ((loopStart1 == loopStart2) && (loopEnd1 == loopEnd2) && (rowStart1 == rowStart2) && (rowEnd1 == rowEnd2))
		return true;
	else
		return false;
}

void Drawer::drawBackground(GLUquadric* obj, kbState state, double countDouble, char currChar, string currWord)
{
	if (currWord == "~")
		return;
	double halfSize = SCREEN_SIZE / 2;
	setColor (colorFront, 1);
	glBegin (GL_POLYGON);
	glVertex3f (-halfSize, -halfSize, 0);
	glVertex3f (halfSize, -halfSize, 0);
	glVertex3f (halfSize, halfSize, 0);
	glVertex3f (-halfSize, halfSize, 0);
	glEnd ();

	if (state == selectOuter)
	{
		setColor (colorS1, 1);
		double radius = sqrt (pow (halfSize,2) * 2);
		if (currChar == 1)
			gluPartialDisk (obj,  halfSize,  halfSize + (radius - halfSize) * countDouble,  slices,  loops,  90,  90);
		else if (currChar == 2)
			gluPartialDisk (obj,  halfSize,  halfSize + (radius - halfSize) * countDouble,  slices,  loops,  180,  90);
		else if (currChar == 3)
			gluPartialDisk (obj,  halfSize,  halfSize + (radius - halfSize) * countDouble,  slices,  loops,  270,  90);
		else if (currChar == 4)
			gluPartialDisk (obj,  halfSize,  halfSize + (radius - halfSize) * countDouble,  slices,  loops,  0,  90);
	}
	else if (state == secondClear)
	{
		setColor (colorS2, 1);
		double radius = sqrt (pow (halfSize,2) * 2);
		gluPartialDisk (obj,  halfSize,  halfSize + (radius - halfSize) * countDouble,  slices,  loops,  90,  90);
	}

	setColor (black, 1);
	gluDisk(obj, 0, halfSize, slices, loops);
	glLineWidth (textWidth);

	setColor (black, 1);
	float totLength = 0;
	for (int i = 0; i < currWord.size (); i++)
	{
		currWord[i] = toupper (currWord[i]);
		totLength += (glutStrokeWidth(GLUT_STROKE_ROMAN, currWord[i]) + textSpacing);
	}
	totLength -= textSpacing;
	totLength *= charScale;
	float totHeight = 119.05 * charScale;
	float rastX = -halfSize + textSpacing * 5;
	float rastY = -halfSize + totHeight + textSpacing * 5;
	glTranslatef(rastX, rastY, 0);
	glScalef(charScale, -charScale, charScale);
	for (int i = 0; i < currWord.size (); i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, currWord[i]);
		glTranslatef(glutStrokeWidth(GLUT_STROKE_ROMAN, currWord[i]) * charScale + textSpacing, 0, 0);
	}

	setColor (black, 1);
	string bksp = "Backspace";
	glLineWidth (textSmallWidth);
	totLength = 0;
	for (int i = 0; i < bksp.size (); i++)
		totLength += (glutStrokeWidth(GLUT_STROKE_ROMAN, bksp[i]) + textSpacing);
	totLength -= textSpacing;
	totLength *= predScale;
	rastX = halfSize - totLength*1.25;
	rastY = -halfSize + totHeight*1.5;
	glLoadIdentity ();
	glTranslatef (SCREEN_SIZE / 2.f, SCREEN_SIZE / 2.f, 0.f);
	glTranslatef(rastX, rastY, 0);
	glScalef(predScale, -predScale, predScale);
	for (int i = 0; i < bksp.size (); i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, bksp[i]);
		glTranslatef(glutStrokeWidth(GLUT_STROKE_ROMAN, bksp[i]) * predScale + textSpacing, 0, 0);
	}

	string otk = "Other Keys";
	glLineWidth (textSmallWidth);
	totLength = 0;
	for (int i = 0; i < otk.size (); i++)
		totLength += (glutStrokeWidth(GLUT_STROKE_ROMAN, otk[i]) + textSpacing);
	totLength -= textSpacing;
	totLength *= predScale;
	rastX = -halfSize + textSpacing * 5;
	rastY = halfSize - totHeight*0.5 - textSpacing;
	glLoadIdentity ();
	glTranslatef (SCREEN_SIZE / 2.f, SCREEN_SIZE / 2.f, 0.f);
	glTranslatef(rastX, rastY, 0);
	glScalef(predScale, -predScale, predScale);
	for (int i = 0; i < otk.size (); i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, otk[i]);
		glTranslatef(glutStrokeWidth(GLUT_STROKE_ROMAN, otk[i]) * predScale + textSpacing, 0, 0);
	}

	string opt = "Options";
	glLineWidth (textSmallWidth);
	totLength = 0;
	for (int i = 0; i < opt.size (); i++)
		totLength += (glutStrokeWidth(GLUT_STROKE_ROMAN, otk[i]) + textSpacing);
	totLength -= textSpacing;
	totLength *= predScale;
	rastX = halfSize -totLength*1.5;
	rastY = halfSize - totHeight*0.5 - textSpacing;
	glLoadIdentity ();
	glTranslatef (SCREEN_SIZE / 2.f, SCREEN_SIZE / 2.f, 0.f);
	glTranslatef(rastX, rastY, 0);
	glScalef(predScale, -predScale, predScale);
	for (int i = 0; i < opt.size (); i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, opt[i]);
		glTranslatef(glutStrokeWidth(GLUT_STROKE_ROMAN, opt[i]) * predScale + textSpacing, 0, 0);
	}

	glLoadIdentity ();
	glTranslatef (SCREEN_SIZE / 2.f, SCREEN_SIZE / 2.f, 0.f);
}


#endif