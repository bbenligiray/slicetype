#ifndef VALIDCHAR_H
#define VALIDCHAR_H

#include <string>

using std::string;

//holds a char and its matching prediction
class ValidChar
{
	char character;
	string prediction;
	unsigned int freq;
public:
	ValidChar();
	ValidChar(char ch, string str, unsigned int fq);
	const char ValidChar::getChar() const;
	const string& getString() const;
	const unsigned int getFreq() const;
	void updatePred(string newPred, unsigned int newFreq);
	string getPrediction();
};

ValidChar::ValidChar()
{
	character = ' ';
	prediction = "";
	freq = 0;
}

ValidChar::ValidChar(char ch, string str, unsigned int fq)
{
	character = ch;
	prediction = str;
	freq = fq;
}

const char ValidChar::getChar() const
{
	return character;
}
const string& ValidChar::getString() const
{
	return prediction;
}
const unsigned int ValidChar::getFreq() const
{
	return freq;
}
void ValidChar::updatePred(string newPred, unsigned int newFreq)
{
	prediction = newPred;
	freq = newFreq;
}

string ValidChar::getPrediction()
{
	return prediction;
}

#endif