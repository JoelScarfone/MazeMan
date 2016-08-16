#pragma once
#include "Shader.h"
#include "Node.h"
#include "Edge.h"
#include "ArrayList.h"
#include "MazeGenerator.h"
#include "Enemy.h"

class Level
{
public:
	typedef struct Point2D{
		Point2D(int _x, int _y){ x = _x; y = _y; }
		int x;
		int y;
	}Point2D;

public:
	Level();
	Level(int);
	~Level();
	void drawLevel(Matrix4);
	void initLevel(Renderable*, Renderable*);
	void randomGenerateLevel(int, int, Point2D*, Point2D*);
	Point2D* startPoint;
	Point2D* endPoint;
	Node* startNode;
	ArrayList<Edge>* getEdges(){ return edges; };

private:
	void drawWall(Node*, int, Renderable* );
	void drawWalls(Node*, Node*, Renderable*);
	void drawFloor(Node*, Node*,  Renderable*);
	void drawEdge(Edge*, Renderable*, Renderable*);
	void drawNode(Node*, Renderable*, Renderable*);
	void initNode(Node*);

	Entity* cubeFinish;
	Entity* cubeStart;
	int levelNum;
	int numWalls = 0;
	int numFloors = 0;
	int numNodeWalls = 0;
	ArrayList<Node> *nodes;
	ArrayList<Edge> *edges;
	ArrayList<Entity> *walls;
	ArrayList<Entity> *floors;
	ArrayList<Entity> *nodeWalls;
	MazeGenerator maze;
};

