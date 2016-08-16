#ifndef _NODE_H
#define _NODE_H

#include <iostream>
#include <string>
#include "ArrayList.h"

//Models nodes of a graph

//Node XML tags
const string nodeStartTag = "<node>";
const string nodeEndTag = "</node>";
const string nodeLocationStartTag = "<location>";
const string nodeLocationEndTag = "</location>";

class Edge; //forward declaration for Edge class

class Node{
private:
	int locationX;
	int locationY;
	int colorType;
	bool visited;
	ArrayList<Edge> edges; //incident edges

public:
	Node();
	bool getVisited();
	bool hasUnvisitedNeighbour();
	void setVisited(bool b);
	int getColorType();
	void setColorType(int c);
	int getLocationX();
	int getLocationY();
	void setLocationX(int x);
	void setLocationY(int y);
	ArrayList<Edge> getEdges();
	void addEdge(Edge * anEdge);
	void printOn(ostream & out) const;
	void parseDataFromFile(ifstream & input);
	bool up;
	bool down;
	bool left;
	bool right;
};

ostream & operator<<(ostream & out, const Node& n);



#endif