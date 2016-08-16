#ifndef _EDGE_CPP
#define _EDGE_CPP

#include <iostream>
#include <fstream>
#include <string>
#include "ArrayList.h"
#include "Node.h"
#include "Edge.h"

//Models edges of a graph




	Edge::Edge(){

		headNode = 0;
		tailNode = 0;

		headNodeLocationX = -1;
		headNodeLocationY = -1;
		tailNodeLocationX = -1;
		tailNodeLocationY = -1;

	}

	int Edge::getHeadNodeLocationX(){ return headNodeLocationX; }
	int Edge::getHeadNodeLocationY(){ return headNodeLocationY; }
	int Edge::getTailNodeLocationX(){ return tailNodeLocationX; }
	int Edge::getTailNodeLocationY(){ return tailNodeLocationY; }
	void Edge::setHeadNodeLocationX(int x){ headNodeLocationX = x; }
	void Edge::setHeadNodeLocationY(int y){ headNodeLocationY = y; }
	void Edge::setTailNodeLocationX(int x){ tailNodeLocationX = x; }
	void Edge::setTailNodeLocationY(int y){ tailNodeLocationY = y; }
	Node * Edge::getHeadNode() { return headNode; }
	void Edge::setHeadNode(Node * aNode) { headNode = aNode; }
	Node * Edge::getTailNode() { return tailNode; }
	void Edge::setTailNode(Node * aNode) { tailNode = aNode; }

	void Edge::printOn(ostream & out) const {
		if (headNode == NULL || tailNode == NULL){
			out << "Edge[" << headNodeLocationX << "," << headNodeLocationY << "-->" <<
				tailNodeLocationX << "," << tailNodeLocationY << "]";
		}
		else{
			out << "Edge[" << *headNode << "-->" << *tailNode << "]";

		}
	}
	
	Node* Edge::getNodeOpposite(Node* n){
		if (n->getLocationX() == getHeadNodeLocationX() && n->getLocationY() == getHeadNodeLocationY()){
			return tailNode;
		}
		else{
			return headNode;
		}
	}

ostream & operator<<(ostream & out, const Edge& e){
	e.printOn(out);
	return out;
}



#endif