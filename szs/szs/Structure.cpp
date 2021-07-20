#pragma once
#include <vector>
#include <string>
#include <iostream>
#define AlphabetSize 33
#define FirstLetter 32

class Node {
public:
	Node(const int level) : nextLevelWords(0), level(level) {}
	void initNextLevel() {
		if (nextLevel.empty())
			for (int i = 0; i < AlphabetSize; i++)
				nextLevel.push_back(new Node(level + 1));
	}
	int nextLevelWords, level;
	bool isEndWord = false;
	std::vector<Node*> nextLevel;
};

class Structure {
public:
	Structure(){
		if (startLevel.empty())
			for (int i = 0; i < AlphabetSize; i++)
				startLevel.push_back(new Node(0));
	}
	
	void putWord(const std::string & word) {
		Node* c = startLevel[word[0] + FirstLetter];
		for (int i = 1; i < word.size(); i++) {
			c->nextLevelWords++;
			if (c->nextLevel.empty())c->initNextLevel();
			c = c->nextLevel[word[i] + FirstLetter];
		}
		c->isEndWord = true;
		size++;
	}

	Node * checkWord(const std::string & word) {
		Node* c = startLevel[word[0] + FirstLetter];
		for (int i = 1; i < word.size() && !c->nextLevel.empty(); i++)
			c = c->nextLevel[word[i] + FirstLetter];
		return c;
	}

	int getSize() {
		return size;
	}

private:
	std::vector<Node*> startLevel;
	int size = 0;
};
