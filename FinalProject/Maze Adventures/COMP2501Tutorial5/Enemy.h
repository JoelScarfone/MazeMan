#ifndef _ENEMY_H
#define _ENEMY_H

#include "Entity.h"
#include "Node.h"
#include "Edge.h"
#include "ArrayList.h"

class Enemy : public Entity{
private:
	bool alive;
	int numIncrements;
	Node *currentNode;
	Node *targetNode;
	Vector3 toTarget;
	ArrayList<Edge>* edges;

public:
	Enemy(Vector3 location, Renderable* renderable);
	void setCurrentNode(Node* n){ currentNode = n; };
	void setAlive(bool b){ alive = b; };
	bool getAlive(){ return alive; };
	Vector3 getPosition(){ return position; };
	void getRandomTarget();
	void move();
};

#endif

