#ifndef KEYMAIN_H
#define KEYMAIN_H

#include <iostream>
#include <sstream>
#include <algorithm>
#include <windows.h>
#include "Drawer.h"
#include "Keyboard.h"
#include "Button.h"
#include "MouseStatus.h"
#include "enums.h"

using std::cout;
using std::endl;

string progName = "SliceType";
HWND currentWindow, newWindow, thisWindow;
char windowText[1024];
bool halfEntry = false;
unsigned int lastEnteredWordLength = 1;

void changeScreenSize(int newWidth, int newHeight);
string windowTitle = "SliceType";

class KeyMain
{
	kbState state;
	char currChar;
	Keyboard kb;
	ButtonSet* prevButtons;
	Drawer dr;
	MouseStatus mouseStat;
	int counter, mCounter;
	bool boolMorph;
	bool isSameButtonset(ButtonSet* bs1, ButtonSet* bs2);
	void sendString(string s);
	void deleteLastWord();
public:
	void updateMouseCell(int x, int y);
	void tickCounter();
	KeyMain();
	void TEMPDRAW();
};

KeyMain::KeyMain()
{
	state = noSelect;
	boolMorph = false;
	currChar = '\n';
	counter = 0;
	mCounter = 0;
	cout << "Init with no selection." << endl;

}

void KeyMain::updateMouseCell(int x, int y)
{
	if (mouseStat.updateCell(x, y))
	{
		int inpLoop, inpRow;
		mouseStat.getCell(inpLoop, inpRow);

		if (inpLoop == -1)
		{
			if (state == selectingOtherKey)
			{
				state = selectOtherKey;
				counter = 0;
				currChar = '\n';
				return;
			}
			if (state == select2)
			{
				prevButtons = kb.getButtonSet();
				cout << "Exited screen during 2nd selection phase." << endl;
				kb.exitChar(currChar);
				boolMorph = true;
				mCounter = -1;
			}
			else if (state == select1)
			{
				//won't do anything here
				cout << "Exited screen during 1st selection phase." << endl;
			}
			state = noSelect;
			counter = 0;
			currChar = '\n';
			return;
		}
		else if (inpLoop == 3)
		{
			if (state == selectOtherKey)
			{
				state = selectOtherKey;
				counter = 0;
				currChar = '\n';
				return;
			}
			if ((state == noSelect) || (state == select1) || (state == select2))
			{
				state = selectOuter;
				counter = 0;
				if (inpRow == 0)
					currChar = 1;
				else if (inpRow == 1)
					currChar = 2;
				else if (inpRow == 2)
					currChar = 3;
				else
					currChar = 4;
				counter = 0;
				return;
			}
		}

		Button onButton;

		if ((state == selectOtherKey) || (state == selectingOtherKey))
		{
			kb.getOtherButtons()->getButtonWithField(inpLoop, inpRow, onButton);

			if (onButton.getWriting().getChar() != currChar)
			{
				currChar = onButton.getWriting().getChar();
				state = selectingOtherKey;
				counter = 0;
				return;
			}
		}

		if (!kb.getButtonSet()->getButtonWithField(inpLoop, inpRow, onButton))
		{
			if (state == select2)
			{
				prevButtons = kb.getButtonSet();
				cout << "Exited screen during 2nd selection phase." << endl;
				kb.exitChar(currChar);
				boolMorph = true;
				mCounter = -1;
			}
			else if (state == select1)
			{
				//won't do anything here
				cout << "Exited screen during 1st selection phase." << endl;
			}
			state = noSelect;
			counter = 0;
			currChar = '\n';
			return;
		}

		if (onButton.getWriting().getChar() != currChar)
		{
			if (state == select2)
			{
				prevButtons = kb.getButtonSet();
				cout << currChar << " -> " << onButton.getWriting().getChar() << " select2" << endl;
				kb.exitChar(currChar);
				boolMorph = true;
				mCounter = -1;
			}
			else if (state == select1)
			{
				//won't do anything here
				cout << currChar << " -> " << onButton.getWriting().getChar() << " select1" << endl;
			}
			else if (state == noSelect)
			{
				//won't do anything here
				cout << " "  " -> " << onButton.getWriting().getChar() << " noSelect" << endl;
			}
			currChar = onButton.getWriting().getChar();
			state = select1;
			counter = 0;
		}
	}
}

void KeyMain::tickCounter()
{
	thisWindow = FindWindow(NULL, windowTitle.c_str());
	if (thisWindow == NULL)
		return;


	RECT thisWindowRect;
	GetWindowRect(thisWindow, &thisWindowRect);

	POINT p;
	GetCursorPos(&p);
	if ((p.x < thisWindowRect.left) || (p.x > thisWindowRect.right) || (p.y < thisWindowRect.top) || (p.y > thisWindowRect.bottom))
		updateMouseCell(-1, -1);




	int width, height;
	width = thisWindowRect.right - thisWindowRect.left - 16;
	height = thisWindowRect.bottom - thisWindowRect.top - 38;

	SCREEN_SIZE_X = width;
	SCREEN_SIZE_Y = height;



	newWindow = GetForegroundWindow();
	if (newWindow == thisWindow)
	{
	}
	else if (newWindow != currentWindow)
	{
		currentWindow = newWindow;
		GetWindowText(currentWindow, windowText, 1024);
		windowTitle = string(windowText);
		if (windowTitle.size() == 0)
		{
			windowTitle = progName;
			glutSetWindowTitle(windowTitle.c_str());
		}
		else
		{
			windowTitle = progName + " >>> " + windowTitle;
			glutSetWindowTitle(windowTitle.c_str());
		}
	}



	if ((state != noSelect) && (state != selectOtherKey) && (!boolMorph))
		counter++;

	mCounter++;

	if ((boolMorph) && (mCounter == morphCount))
	{
		boolMorph = false;
		mCounter = 0;
	}


	if (counter == selectCount)
	{
		if (state == secondClear)
		{
			deleteLastWord();
			counter = 0;
			state = noSelect;
			return;
		}
		if (state == selectingOtherKey)
		{
			stringstream ss;
			string stringToSend;
			ss << currChar;
			ss >> stringToSend;
			sendString(stringToSend);
			kb.setLastWord("");
			counter = 0;
			if (isdigit(currChar))
				state = selectOtherKey;
			else
				state = noSelect;
			return;
		}
		if (state == selectOuter)
		{
			counter = 0;
			if (currChar == 1)
			{
				prevButtons = kb.getButtonSet();
				kb.clearCurrWord();
				boolMorph = !isSameButtonset(prevButtons, kb.getButtonSet());
				counter = 0;
				mCounter = 0;
				state = secondClear;
				currChar = '\n';
			}
			else if (currChar == 2)
			{
				kb.setLastWord(kb.getCurrWord());
				prevButtons = kb.getButtonSet();
				string stringToSend = kb.getCurrWord();
				halfEntry = true;
				sendString(stringToSend);
				kb.clearCurrWord();
				kb.setLastWord(stringToSend);
				boolMorph = !isSameButtonset(prevButtons, kb.getButtonSet());
				counter = 0;
				mCounter = 0;
				state = noSelect;
				currChar = '\n';

			}
			else if (currChar == 3)
			{
				state = selectOtherKey;
			}
			else
			{

			}
			return;
		}


		if (state == select1)
		{
			prevButtons = kb.getButtonSet();
			state = select2;
			cout << "Finish select1 " << currChar << endl;
			kb.enterChar(currChar);
		}
		else if (state == select2)
		{
			prevButtons = kb.getButtonSet();
			state = noSelect;
			cout << "Finish select2 " << currChar << endl;


			string stringToSend = kb.enterWord();

			sendString(stringToSend);
			currChar = '\n';
		}
		boolMorph = !isSameButtonset(prevButtons, kb.getButtonSet());
		counter = 0;
		mCounter = 0;
	}
}

void KeyMain::TEMPDRAW()
{
	if (state == secondClear)
		int a = 2;

	if (boolMorph)
		dr.drawButtons(kb.getButtonSet(), prevButtons, state, boolMorph, (double)mCounter / morphCount, currChar, kb.getCurrWord());
	else if ((state == selectingOtherKey) || (state == selectOtherKey))
		dr.drawButtons(kb.getOtherButtons(), prevButtons, select1, false, (double)counter / selectCount, currChar, "~");
	else
		dr.drawButtons(kb.getButtonSet(), prevButtons, state, boolMorph, (double)counter / selectCount, currChar, kb.getCurrWord());
}

bool KeyMain::isSameButtonset(ButtonSet* bs1, ButtonSet* bs2)
{
	if (bs1->getButtons().size() != bs2->getButtons().size())
		return false;
	for (vector<Button>::size_type i = 0; i < bs1->getButtons().size(); i++)
	{
		if (bs1->getButtons()[i].getWriting().getChar() != bs2->getButtons()[i].getWriting().getChar())
			return false;

		int loopStart1, loopEnd1, rowStart1, rowEnd1, loopStart2, loopEnd2, rowStart2, rowEnd2;
		bs1->getButtons()[i].getButtonShape().getShape(loopStart1, loopEnd1, rowStart1, rowEnd1);
		bs2->getButtons()[i].getButtonShape().getShape(loopStart2, loopEnd2, rowStart2, rowEnd2);

		if ((loopStart1 != loopStart2) || (loopEnd1 != loopEnd2) || (rowStart1 != rowStart2) || (rowEnd1 != rowEnd2))
			return false;
	}
	return true;
}

void KeyMain::sendString(string s)
{
	SetForegroundWindow(currentWindow);

	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.time = 0;
	ip.ki.dwFlags = KEYEVENTF_UNICODE;
	ip.ki.wVk = 0;
	ip.ki.dwExtraInfo = 0;

	lastEnteredWordLength = s.size();

	if (s == ".")
	{
		lastEnteredWordLength = 1;
		deleteLastWord();
		lastEnteredWordLength = s.size();
	}


	for (string::size_type i = 0; i < s.size(); i++)
	{
		ip.ki.wScan = s[i];
		SendInput(1, &ip, sizeof(INPUT));
		if (s[i] == s[i + 1] && i + 1 != s.size())
			Sleep(25);
	}
	if (true)
	{
		ip.ki.wScan = ' ';
		SendInput(1, &ip, sizeof(INPUT));
		lastEnteredWordLength++;
		halfEntry = false;
	}
	if (halfEntry)
		halfEntry = false;

}

void KeyMain::deleteLastWord()
{
	SetForegroundWindow(currentWindow);


	INPUT ip;

	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	ip.ki.wVk = 0x08; // bksp

	for (unsigned int i = 0; i < lastEnteredWordLength; i++)
	{
		ip.ki.dwFlags = 0;
		SendInput(1, &ip, sizeof(INPUT));
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));
	}
	lastEnteredWordLength = 1;
}

#endif