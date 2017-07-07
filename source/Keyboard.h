#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <string>
#include "ButtonSet.h"
#include "Predictor.h"

#include "enums.h"

using std::vector;
using std::string;



class Keyboard
{
	ButtonSet *buttons;
	ButtonSet *otherKeys;
	Predictor predict;
	string lastWord, currentWord;
	vector<ValidChar> firstScreenVC, secondScreenVC;
	void initCharVecs();
	void resetVC();
	void cleanVC();

public:
	Keyboard();
	~Keyboard();
	ButtonSet* getButtonSet();
	void enterChar(char inpChar);
	void exitChar(char inpChar);
	string Keyboard::enterWord();
	string getCurrWord();
	void clearCurrWord();
	void setLastWord(string lw);
	ButtonSet* getOtherButtons();
};

Keyboard::Keyboard()
{
	resetVC();
	lastWord = "";
	currentWord = "";
	initCharVecs();
	buttons = new ButtonSet(firstScreenVC);
	otherKeys = new ButtonSet(secondScreenVC);
}

Keyboard::~Keyboard()
{
	delete buttons;
	delete otherKeys;
}

void Keyboard::initCharVecs()
{
	for (vector<ValidChar>::size_type i = 0; i < 26; i++)
		firstScreenVC[i].updatePred(predict.Predict(lastWord, currentWord + firstScreenVC[i].getChar()), 0);
}

//entered a char. prediction for the entered char shouldn't change.
void Keyboard::enterChar(char inpChar)
{
	resetVC();
	currentWord = currentWord + inpChar;
	for (vector<ValidChar>::size_type i = 0; i < 26; i++)
	{
		if (firstScreenVC[i].getChar() != inpChar)
			firstScreenVC[i].updatePred(predict.Predict(lastWord, currentWord + firstScreenVC[i].getChar()), 0);
		else
			firstScreenVC[i].updatePred(predict.Predict(lastWord, currentWord), 0);
	}
	cleanVC();
	buttons = new ButtonSet(firstScreenVC);
}

//entered a char. exited its cell before select2 ends. its prediction must be updated.
void Keyboard::exitChar(char inpChar)
{
	resetVC();
	initCharVecs();
	cleanVC();

	if (firstScreenVC.size() == 0)
	{
		currentWord = "";
		resetVC();
		initCharVecs();
		cleanVC();
	}
	buttons = new ButtonSet(firstScreenVC);
}

ButtonSet* Keyboard::getButtonSet()
{
	return buttons;
}

void Keyboard::resetVC()
{
	firstScreenVC.clear();
	secondScreenVC.clear();
	for (vector<ValidChar>::size_type i = 0; i < 26; i++)
	{
		firstScreenVC.push_back(ValidChar(firstScreenChars[i], "", 0));
		secondScreenVC.push_back(ValidChar(secondScreenChars[i], "", 0));
	}
}

void Keyboard::cleanVC()
{
	vector<ValidChar> tempVC;

	for (vector<ValidChar>::size_type i = 0; i < 26; i++)
	{
		if (firstScreenVC[i].getPrediction() != "")
			tempVC.push_back(firstScreenVC[i]);
	}
	firstScreenVC = tempVC;
}

string Keyboard::enterWord()
{
	currentWord = predict.Predict(lastWord, currentWord);
	lastWord = currentWord;
	currentWord = "";
	resetVC();
	initCharVecs();
	buttons = new ButtonSet(firstScreenVC);
	return lastWord;
}

string Keyboard::getCurrWord()
{
	return currentWord;
}

void Keyboard::clearCurrWord()
{
	currentWord = "";
	resetVC();
	initCharVecs();
	buttons = new ButtonSet(firstScreenVC);
}

void Keyboard::setLastWord(string lw)
{
	lastWord = lw;
}

ButtonSet* Keyboard::getOtherButtons()
{
	return otherKeys;
}



#endif