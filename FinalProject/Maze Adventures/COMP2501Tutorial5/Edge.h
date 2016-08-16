#ifndef _EDGE_H
#define _EDGE_H

#include <iostream>
#include <string>
#include "ArrayList.h"

//Models edges of a graph


//Edge XML tags
const string edgeStartTag = "<edge>";
const string edgeEndTag = "</edge>";

const string headNodeLocationStartTag = "<startLocation>";
const string headNodeLocationEndTag = "</startLocation>";
const string tailNodeLocationStartTag = "<endLocation>";
const string tailNodeLocationEndTag = "</endLocation>";

class Edge{

private:

	//temporary locations used during input parsing from a file
	//these are replaced with actual headNode and tailNode
	//references when parsing is over
	int headNodeLocationX, 
		headNodeLocationY, 
		tailNodeLocationX, 
		tailNodeLocationY;

	//nodes the edge goes between
	Node * headNode;
	Node * tailNode;

public:
	Edge();
	int getHeadNodeLocationX();
	int getHeadNodeLocationY();
	int getTailNodeLocationX();
	int getTailNodeLocationY();
	void setHeadNodeLocationX(int x);
	void setHeadNodeLocationY(int y);
	void setTailNodeLocationX(int x);
	void setTailNodeLocationY(int y);
	Node * getHeadNode();
	void setHeadNode(Node * aNode);
	Node * getTailNode();
	void setTailNode(Node * aNode);
	Node * getNodeOpposite(Node* n);

	void printOn(ostream & out) const;

	void parseDataFromFile(ifstream & input);
};

ostream & operator<<(ostream & out, const Edge& e);

#endif