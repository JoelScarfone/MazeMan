#ifndef _MAZEGENERATOR_H
#define _MAZEGENERATOR_H

#include <iostream>
#include <string>
#include "ArrayList.h"
#include "Node.h"
#include "Edge.h"

#define MAZE_WIDTH 30

class MazeGenerator{

private:
	
	// Nodes with randomly generated Edges
	ArrayList<Node> nodes; //nodes of the graph
	ArrayList<Edge> edges; //edgess of the graph

	//Nodes with 'full' edges between them. 
	ArrayList<Node> baseNodes;
	ArrayList<Edge> baseEdges;

	bool startCreated;
	int nodesCreated;
	
public:
	MazeGenerator(); 
	~MazeGenerator();
	ArrayList<Node>* getNodes();
	ArrayList<Edge>* getEdges();
	void generate(int x, int y);
	void recurse(Node* n, int minimumDistance);
	void buildBaseGraphs(int x, int y);
	void addNode(Node * nodePtr);
	void addEdge(Edge * edgePtr);
	Node * nodeWithLocation(int x, int y);
};


#endif