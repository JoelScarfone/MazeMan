#ifndef _MAZEGENERATOR_CPP
#define _MAZEGENERATOR_CPP

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "ArrayList.h"
#include "Node.h"
#include "Edge.h"
#include "MazeGenerator.h"

MazeGenerator::MazeGenerator(){
	startCreated = false;
}
MazeGenerator::~MazeGenerator(){}

ArrayList<Edge>* MazeGenerator::getEdges(){
	return &edges;
}

ArrayList<Node>*  MazeGenerator::getNodes(){
	return &nodes;
}

/*
	function:	generate()
	purpose:	takes in two integers and fills the Arraylists 'edges' and 'nodes' 
				with randomly generated edges that create a maze with a start and finish
*/
void MazeGenerator::generate(int x, int y){
	srand(time(NULL));
	buildBaseGraphs(x, y);
	int pos = rand() % baseNodes.size();
	nodes.elementAt(pos)->setColorType(2);
	nodesCreated = 0;
	recurse(baseNodes.elementAt(pos), x * y / 2);
	
	// regenerates the odd case no Start is created 
	// **Warning** if paramater on recurse 'minimumDistance' 
	// is too large, we will get an infinite loop here

	if (!startCreated){
		generate(x, y);
	}
}
			

//recursive function that generates the edges that form each randomly generated maze
void MazeGenerator::recurse(Node* n, int minimumDistance){
	n->setVisited(true);
	int index = rand() % n->getEdges().size();
	ArrayList<Node> neighbours;

	for (int i = 0; i < n->getEdges().size(); i++){
		neighbours.add(n->getEdges().elementAt(index)->getNodeOpposite(n));
		index++;
		index = index % n->getEdges().size();
	}

	int visitedEdges = 0;
	for (int i = 0; i < neighbours.size(); i++){
		if (!neighbours.elementAt(i)->getVisited()){
			nodesCreated++;
			edges.add(n->getEdges().elementAt((index + i) % (n->getEdges().size())));			

			int indexOfOppositeNode = baseNodes.positionOf(n->getEdges().elementAt((index + i) % (n->getEdges().size()))->getNodeOpposite(n));
			nodes.elementAt(baseNodes.positionOf(n))->addEdge(n->getEdges().elementAt((index + i) % (n->getEdges().size())));
			nodes.elementAt(indexOfOppositeNode)->addEdge(n->getEdges().elementAt((index + i) % (n->getEdges().size())));

			recurse(neighbours.elementAt(i), minimumDistance);
		}else{
			visitedEdges++;
		}
		if (visitedEdges == neighbours.size() && nodesCreated > minimumDistance && !startCreated){
			int indexOfFinish = baseNodes.positionOf(n);
			if (indexOfFinish != -1){
				nodes.elementAt(indexOfFinish)->setColorType(1);
				startCreated = true;
			}
		}
	}
}


//function to fill a x by y graph of edges that represents a grid
void  MazeGenerator::buildBaseGraphs(int x, int y){

	for (int i = 0; i < x; i++){
		for (int j = 0; j < y; j++){
			Node * node = new Node();
			node->setLocationX(i * MAZE_WIDTH);
			node->setLocationY(j * MAZE_WIDTH);
			baseNodes.add(node);

			Node * node2 = new Node();
			node2->setLocationX(i * MAZE_WIDTH);
			node2->setLocationY(j * MAZE_WIDTH);
			nodes.add(node2);
		}
	}	//baseNodes and nodes is now a template graph that is X by Y of nodes, space MAZE_WIDTH appart,
	



	// cover the edgeson the bottom of the graph
	for (int i = 0; i < (x - 1); i++){

		Edge * edge = new Edge();

		edge->setHeadNodeLocationX(i * MAZE_WIDTH);
		edge->setHeadNodeLocationY(0);
		edge->setTailNodeLocationX(i * MAZE_WIDTH + MAZE_WIDTH);
		edge->setTailNodeLocationY(0);

		Node * headNode = nodeWithLocation(edge->getHeadNodeLocationX(), edge->getHeadNodeLocationY());
		Node * tailNode = nodeWithLocation(edge->getTailNodeLocationX(), edge->getTailNodeLocationY());

		if (headNode == NULL || tailNode == NULL){
			cout << "EDGE GENERATOR ERROR: NO HEAD OR TAIL NODE FOUND\n";
		}
		else {
			// set edges nodes
			edge->setHeadNode(headNode);
			edge->setTailNode(tailNode);

			//add edge to out base Edge but not our acctual edges for graph
			baseEdges.add(edge);
			// make the proper nodes have this edge
			edge->getHeadNode()->addEdge(edge);
			edge->getTailNode()->addEdge(edge);
		}

	}

	// cover the edges on the left side of the graph
	for (int i = 0; i < (y - 1); i++){

		Edge * edge = new Edge();

		edge->setHeadNodeLocationX(0);
		edge->setHeadNodeLocationY(i * MAZE_WIDTH);
		edge->setTailNodeLocationX(0);
		edge->setTailNodeLocationY(i * MAZE_WIDTH + MAZE_WIDTH);

		Node * headNode = nodeWithLocation(edge->getHeadNodeLocationX(), edge->getHeadNodeLocationY());
		Node * tailNode = nodeWithLocation(edge->getTailNodeLocationX(), edge->getTailNodeLocationY());

		if (headNode == NULL || tailNode == NULL){
			cout << "EDGE GENERATOR ERROR: NO HEAD OR TAIL NODE FOUND\n";
		}
		else {
			// set edges nodes
			edge->setHeadNode(headNode);
			edge->setTailNode(tailNode);

			//add edge to out base Edge but not our acctual edges for graph
			baseEdges.add(edge);
			// make the proper nodes have this edge
			edge->getHeadNode()->addEdge(edge);
			edge->getTailNode()->addEdge(edge);
		}

	}

	for (int i = 1; i < x; i++){
		for (int j = 1; j < y; j++){
			// each other node needs to add two edges, one to its left node, and one down
			// Doing this will make every node connected in a full X by Y graph

			Edge * edge1 = new Edge();
			Edge * edge2 = new Edge();

			//one going to the edge below
			edge1->setHeadNodeLocationX(i * MAZE_WIDTH);
			edge1->setHeadNodeLocationY(j * MAZE_WIDTH);
			edge1->setTailNodeLocationX(i * MAZE_WIDTH);
			edge1->setTailNodeLocationY((j * MAZE_WIDTH) - MAZE_WIDTH);

			//one going to the edge to the left
			edge2->setHeadNodeLocationX(i * MAZE_WIDTH);
			edge2->setHeadNodeLocationY(j * MAZE_WIDTH);
			edge2->setTailNodeLocationX((i * MAZE_WIDTH) - MAZE_WIDTH);
			edge2->setTailNodeLocationY(j * MAZE_WIDTH);

			Node * headNode1 = nodeWithLocation(edge1->getHeadNodeLocationX(), edge1->getHeadNodeLocationY());
			Node * tailNode1 = nodeWithLocation(edge1->getTailNodeLocationX(), edge1->getTailNodeLocationY());

			Node * headNode2 = nodeWithLocation(edge2->getHeadNodeLocationX(), edge2->getHeadNodeLocationY());
			Node * tailNode2 = nodeWithLocation(edge2->getTailNodeLocationX(), edge2->getTailNodeLocationY());

			if (headNode1 == NULL || tailNode1 == NULL || headNode2 == NULL || tailNode2 == NULL){
				cout << "EDGE GENERATOR ERROR: NO HEAD OR TAIL NODE FOUND\n";
			}
			else {
				// set edges nodes
				edge1->setHeadNode(headNode1);
				edge1->setTailNode(tailNode1);
				edge2->setHeadNode(headNode2);
				edge2->setTailNode(tailNode2);
				//add edges to out base Edge but not our acctual edges for graph
				baseEdges.add(edge1);
				baseEdges.add(edge2);
				// make the proper nodes have this edge
				edge1->getHeadNode()->addEdge(edge1);
				edge1->getTailNode()->addEdge(edge1);
				edge2->getHeadNode()->addEdge(edge2);
				edge2->getTailNode()->addEdge(edge2);
			}

		}

	}
}

//returns a node pointer to the node that has the location x,y in the graph 
Node * MazeGenerator::nodeWithLocation(int x, int y){
	//answer the node with location co-ordinates x,y or null if none exists
	//delete the nodes nodes of the graph
	for (int i = 0; i < baseNodes.size(); i++){
		Node * nodePtr = baseNodes.elementAt(i);
		if (nodePtr->getLocationX() == x && nodePtr->getLocationY() == y) return nodePtr;
	}
	return NULL;
}


#endif	