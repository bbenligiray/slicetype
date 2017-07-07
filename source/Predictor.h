#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <fstream>
#include <sstream>
#include "Trie.h"
using std::string;
using std::vector;
using std::stringstream;

class Predictor
{
	Trie *uniGrams, *biGrams;
	unsigned int uniNoOfWords, uniTotalFreq, biNoOfWords, biTotalFreq;
	unsigned int strToUint(string str);
	void readTextCorpus();
	string freqPredict(string word1, string word2);
	bool tryLoadingTries();
public:
	Predictor();
	~Predictor();
	string Predict(string word1, string word2);
};

Predictor::Predictor()
{
	if (!tryLoadingTries())
	{
		readTextCorpus();
		uniGrams->saveTrie("uniTrie.txt");
		biGrams->saveTrie("biTrie.txt");
	}
}

Predictor::~Predictor()
{
	delete uniGrams;
	delete biGrams;
}

void Predictor::readTextCorpus()
{
	std::ifstream uniCorpusFile("uniCorpus.txt");
	vector<ValidChar> words;
	string word;

	std::getline(uniCorpusFile, word);

	uniCorpusFile >> word;
	uniNoOfWords = strToUint(word);
	uniCorpusFile >> word;
	uniTotalFreq = strToUint(word);

	for (int i = 0; i < uniNoOfWords; i++)
	{
		uniCorpusFile >> word;
		unsigned int f = strToUint(word);

		uniCorpusFile >> word;

		words.push_back(ValidChar(' ', word, f));

	}

	uniGrams = new Trie(words);



	std::ifstream biCorpusFile("biCorpus.txt");
	words.clear();
	string biWord;

	std::getline(biCorpusFile, word);

	biCorpusFile >> word;
	biNoOfWords = strToUint(word);
	biCorpusFile >> word;
	biTotalFreq = strToUint(word);

	for (int i = 0; i < biNoOfWords; i++)
	{
		biCorpusFile >> word;
		unsigned int f = strToUint(word);

		biCorpusFile >> word;
		biCorpusFile >> biWord;

		words.push_back(ValidChar(' ', word + " " + biWord, f));
	}

	biGrams = new Trie(words);
}


unsigned int Predictor::strToUint(string str)
{
	stringstream ss(str);
	unsigned int num;
	ss >> num;
	return num;
}

string Predictor::Predict(string word1, string word2)
{
	ValidChar uniPrediction, biPrediction;

	if (word1 == "")
	{
		uniPrediction = uniGrams->searchWord(word2);
		return uniPrediction.getString();
	}
	else
	{
		uniPrediction = uniGrams->searchWord(word2);
		biPrediction = biGrams->searchWord(word1 + " " + word2);

		if (biPrediction.getString() == "")
			return uniPrediction.getString();

		string resultStr;

		resultStr = biPrediction.getString().substr(word1.size() + 1, biPrediction.getString().size());

		return resultStr;
	}
}

string Predictor::freqPredict(string word1, string word2)
{
	ValidChar uniPrediction, biPrediction;

	uniPrediction = uniGrams->searchWord(word2);
	biPrediction = biGrams->searchWord(word1 + " " + word2);

	/*
	if (biPrediction.getString()==" ")
	return uniPrediction.getString();
	else
	return biPrediction.getString().substr(word1.size()+1,biPrediction.getString().size());
	*/


	if ((uniPrediction.getFreq() / (double)uniTotalFreq) > (biPrediction.getFreq() / (double)biTotalFreq))
		return uniPrediction.getString();
	else
		return biPrediction.getString().substr(word1.size() + 1, biPrediction.getString().size());
}

bool Predictor::tryLoadingTries()
{
	std::ifstream inputFile("uniTrie.txt");

	if (inputFile.is_open())
	{
		delete uniGrams;
		uniGrams = new Trie();
		uniGrams->loadTrie(inputFile);
	}
	else
		return false;
	inputFile.close();

	inputFile.open("biTrie.txt");

	if (inputFile.is_open())
	{
		delete biGrams;
		biGrams = new Trie();
		biGrams->loadTrie(inputFile);
	}
	else
		return false;
	inputFile.close();

}

#endif