#ifndef _NODE_CPP
#define _NODE_CPP

#include <iostream>
#include <fstream>
#include <string>
#include "ArrayList.h"
#include "Node.h"
#include "Edge.h"


Node::Node(){
	locationX = 0;
	locationY = 0;
	visited = false; // for Maze Generation Purposes
	colorType = 0;
	up = false;
	down = false;
	left = false;
	right = false;
}

int Node::getLocationX(){ return locationX; }

int Node::getLocationY(){ return locationY; }

void Node::setLocationX(int x){ locationX = x; }
void Node::setLocationY(int y){ locationY = y; }

int Node::getColorType(){ return colorType; }
void Node::setColorType(int c){ colorType = c; }
bool Node::getVisited(){ return visited;}
void Node::setVisited(bool b){ visited = b; }
ArrayList<Edge> Node::getEdges(){return edges;}

bool Node::hasUnvisitedNeighbour(){
	for (int i = 0; i < edges.size(); i++){
		if (!edges.elementAt(i)->getNodeOpposite(this)->getVisited()){
			return true;
		}
	}
	return false;
}
void Node::addEdge(Edge * anEdge){ edges.add(anEdge); }


void Node::printOn(ostream & out) const {
	out << "Node[" << locationX << "," << locationY << "]";
}

ostream & operator<<(ostream & out, const Node& n){
	n.printOn(out);
	return out;
}

#endif