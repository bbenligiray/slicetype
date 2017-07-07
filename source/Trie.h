#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "ValidChar.h"

enum TrieMode { uni, bi };

using std::vector;
using std::string;
using std::ofstream;
using std::ifstream;
using std::stringstream;
using std::endl;

vector<string> splitLine(string& line);

class Node
{
	ValidChar content;

public:
	vector<Node*> children;
	Node();
	Node(ValidChar initChar);
	Node* findChild(char c);
	const ValidChar& getContent() const;
	ValidChar& getContentUnprotected();
	void appendChild(Node* child);
	void Node::writeContent(ofstream& outputFile, string front);
	void changeContent(ValidChar inpCon);
};

class Trie
{
	Node* root;
	void addWord(string start, string pred, unsigned int freq);
	void appendWord(ValidChar newWord);
	void ascendWord(ValidChar newWord);
	void fillWord(ValidChar newWord);
public:
	Trie();
	Trie(const vector<ValidChar> &words);
	~Trie();
	ValidChar searchWord(string s);
	void saveTrie(string fileName);
	void loadTrie(ifstream& inputFile);
};

Trie::Trie()
{
	root = new Node();
}

Trie::Trie(const vector<ValidChar> &words)
{
	root = new Node();

	for (vector<ValidChar>::size_type i = 0; i < words.size(); i++)
	{
		appendWord(words[i]);
		ascendWord(words[i]);
		fillWord(words[i]);
	}
}

Trie::~Trie()
{
	delete root;
}

void Trie::addWord(string start, string pred, unsigned int freq)
{
	Node* currentNode = root;

	for (string::size_type i = 0; i < start.size(); i++)
	{
		Node* childNode = currentNode->findChild(start[i]);
		if (childNode != NULL)
		{
			currentNode = childNode;
		}
		else
		{
			Node* temp = new Node(ValidChar(start[i], pred, freq));
			currentNode->appendChild(temp);
			currentNode = temp;
		}
	}
}

void Trie::appendWord(ValidChar newWord)
{
	Node* currentNode = root;

	for (string::size_type i = 0; i < newWord.getString().length(); i++)
	{
		Node* childNode = currentNode->findChild(newWord.getString()[i]);
		if (childNode != NULL)
		{
			currentNode = childNode;
			if ((i == newWord.getString().length() - 1) && (currentNode->getContent().getString() == ""))
				currentNode->getContentUnprotected().updatePred(newWord.getString(), newWord.getFreq());
		}
		else
		{
			Node* temp;
			if (i == newWord.getString().length() - 1)
				temp = new Node(ValidChar(newWord.getString()[i], newWord.getString(), newWord.getFreq()));
			else
				temp = new Node(ValidChar(newWord.getString()[i], "", 0));

			currentNode->appendChild(temp);
			currentNode = temp;
		}
	}
}
void Trie::ascendWord(ValidChar ascWord)
{
	Node* currentNode = root;

	bool couldAscend = false;

	int firstWordLength = 0;
	for (string::size_type i = 0; i < ascWord.getString().length() - 1; i++)
	{
		if (ascWord.getString()[i] == ' ')
		{
			firstWordLength = i;
			break;
		}
	}

	for (string::size_type i = 0; i < ascWord.getString().length() - 1; i++)
	{
		Node* childNode = currentNode->findChild(ascWord.getString()[i]);

		if ((childNode->getContent().getString() == "") && (i > firstWordLength))
		{
			childNode->getContentUnprotected().updatePred(ascWord.getString(), ascWord.getFreq());
			couldAscend = true;
			break;
		}
		currentNode = childNode;
	}
	if (couldAscend)
	{
		currentNode = root;
		for (string::size_type i = 0; i < ascWord.getString().length(); i++)
		{
			Node* childNode = currentNode->findChild(ascWord.getString()[i]);
			currentNode = childNode;
		}
		currentNode->getContentUnprotected().updatePred("", 0);
	}
}

void Trie::fillWord(ValidChar fillWord)
{
	Node* currentNode = root;

	for (string::size_type i = 0; i < fillWord.getString().length(); i++)
	{
		Node* childNode = currentNode->findChild(fillWord.getString()[i]);

		if (childNode->getContent().getString() == "")
		{
			childNode->getContentUnprotected().updatePred(fillWord.getString(), fillWord.getFreq());
		}
		currentNode = childNode;
	}
}

ValidChar Trie::searchWord(string str)
{
	Node* currentNode = root;


	for (string::size_type i = 0; i < str.length(); i++)
	{
		Node* temp = currentNode->findChild(str[i]);
		if (temp == NULL)
			return ValidChar(' ', "", 0);
		currentNode = temp;
	}

	return currentNode->getContent();

}

void Trie::saveTrie(string fileName)
{
	ofstream outputFile;
	outputFile.open(fileName);

	for (vector<Node>::size_type i = 0; i != root->children.size(); ++i)
		root->children[i]->writeContent(outputFile, "");

	outputFile.close();
}

void Trie::loadTrie(ifstream& inputFile)
{
	string line;
	vector<string> splortLine;
	getline(inputFile, line);
	root->changeContent(ValidChar(' ', "", 0));
	while (getline(inputFile, line))
	{
		splortLine = splitLine(line);
		addWord(splortLine[0], splortLine[1], atoi(splortLine[2].c_str()));
	}
}

void Node::writeContent(ofstream& outputFile, string front)
{
	stringstream ss;
	ss << content.getFreq();
	string newFront = front + content.getChar();

	outputFile << newFront + "~" + content.getPrediction() + "~" + ss.str() << endl;

	for (vector<Node>::size_type i = 0; i != children.size(); ++i)
		children[i]->writeContent(outputFile, newFront);
}




Node::Node()
{

}

Node::Node(ValidChar initChar)
{
	content = initChar;
}

const ValidChar& Node::getContent() const
{
	return content;
}

ValidChar& Node::getContentUnprotected()
{
	return content;
}

Node* Node::findChild(char c)
{
	for (vector<Node>::size_type i = 0; i < children.size(); i++)
	{
		Node* tmp = children[i];
		if (tmp->getContent().getChar() == c)
			return tmp;
	}
	return NULL;
}

void Node::appendChild(Node* child)
{
	children.push_back(child);
}

vector<string> splitLine(string& line)
{
	vector<string> ret;
	string::size_type i = 0;

	while (i != line.size())
	{
		if (line[i] == '~')
			i++;

		string::size_type j = i;

		while (j != line.size() && line[j] != '~')
			++j;

		if (i != j)
		{
			ret.push_back(line.substr(i, j - i));
			i = j;
		}
	}
	return ret;
}

void Node::changeContent(ValidChar inpCon)
{
	content = inpCon;
}

#endif