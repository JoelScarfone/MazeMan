#include "Enemy.h"

Enemy::Enemy(Vector3 location, Renderable* renderable) :Entity(location, renderable){
	alive = true;
	numIncrements = 0;
}

void Enemy::getRandomTarget(){	
	int targetArrayPos;
	targetArrayPos = rand() % currentNode->getEdges().size();
	targetNode = currentNode->getEdges().elementAt(targetArrayPos)->getNodeOpposite(currentNode);
	toTarget = Vector3(currentNode->getLocationX() - targetNode->getLocationX(), 0, currentNode->getLocationY() - targetNode->getLocationY());
	numIncrements = 0;
}

void Enemy::move(){
	numIncrements++;
	position = Vector3(position.x - 0.0002 * toTarget.x, position.y, position.z - 0.0002 * toTarget.z);	
	if (numIncrements == 5000){
		currentNode = targetNode;
		getRandomTarget();
	}
}