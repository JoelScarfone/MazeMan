#include "Level.h"

Level::Level()
{
	startPoint = new Point2D(0, 0);
	endPoint = new Point2D(0, 0);
	levelNum = 1;
	nodes = new ArrayList<Node>();
	edges = new ArrayList<Edge>();
	randomGenerateLevel(levelNum  + 5, levelNum + 5, startPoint , endPoint);
	walls = new ArrayList<Entity>();
	floors = new ArrayList<Entity>();
    nodeWalls= new ArrayList<Entity>();
}

Level::Level(int x)
{
	startPoint = new Point2D(0,0);
	endPoint = new Point2D(0, 0);
	levelNum = x;
	nodes = new ArrayList<Node>();
	edges = new ArrayList<Edge>();
	randomGenerateLevel(x+5, x+5, startPoint, endPoint);
	walls = new ArrayList<Entity>();
	floors = new ArrayList<Entity>();
	nodeWalls = new ArrayList<Entity>();
}

Level::~Level()
{
	for (int i = 0; i < numWalls; i++){
		delete walls->elementAt(i);
	}
	for (int i = 0; i < numFloors; i++){
		delete floors->elementAt(i);
	}
	for (int i = 0; i < numNodeWalls; i++){
		delete nodeWalls->elementAt(i);
	}
	delete nodeWalls;
	delete walls;
	delete floors;

	delete startPoint;
	delete endPoint;
}
/*
Function randomGenerateLevel()
Purpose: generate a random X by Y maze in a graph form.
*/
void Level::randomGenerateLevel(int x, int y, Point2D* start, Point2D* finish)
{
	maze.generate(x,y);
	nodes = maze.getNodes();
	edges = maze.getEdges();

	for (int i = 0; i < nodes->size(); i++){
		if (nodes->elementAt(i)->getColorType() == 1){
			start->x = nodes->elementAt(i)->getLocationX();
			start->y = nodes->elementAt(i)->getLocationY();
			startNode = nodes->elementAt(i);
		}
		if (nodes->elementAt(i)->getColorType() == 2){
			finish->x = nodes->elementAt(i)->getLocationX();
			finish->y = nodes->elementAt(i)->getLocationY();

		}
	}
}


/*
Function: initLevel()
Purpose: initialize components of a level(walls floors corners etc... 
		 and have them ready to draw when rendering level).
*/
void Level::initLevel(Renderable* c, Renderable* f){
	for (int i = 0; i <edges->size(); i++){
		drawEdge(edges->elementAt(i), c, f);
	}
	for (int i = 0; i <nodes->size(); i++){
		initNode(nodes->elementAt(i));
		drawNode(nodes->elementAt(i), c, f);
	}
	cubeStart = new Entity(Vector3(startPoint->x, 0, startPoint->y), c);
	cubeFinish = new Entity(Vector3(endPoint->x, 30, endPoint->y), c);
	cubeFinish->setScale(0.5, 30, 0.5);
}

/*
Function: drawlevel()
Purpose: draw all necesairy components of a level(walls floors corners etc...).
*/
void Level::drawLevel(Matrix4 vp){
	for (int i = 0; i < numFloors; i++){
		floors->elementAt(i)->render(vp);
	}
	for (int i = 0; i < numWalls; i++){
		walls->elementAt(i)->render(vp);
	}
	for (int i = 0; i < numNodeWalls; i++){
		nodeWalls->elementAt(i)->render(vp);
	}
	cubeStart->render(vp);
	cubeFinish->render(vp);
}

/*
Function: drawEdge(Edge* e)
Purpose: draws the walls and floors arround an edge on the graph
*/
void Level::drawEdge(Edge* e, Renderable* c, Renderable*f)
{
	drawFloor(e->getHeadNode(), e->getTailNode(),f);
	drawWalls(e->getHeadNode(), e->getTailNode(),c);
}

/*
Funbdtion: drawNode(Node*n)
Purpose: draws walls on the corner piece or node depending on
		 wheather or not there is an edge coming from that side
*/
void Level::drawNode(Node* n, Renderable* c, Renderable* f)
{

	if (!(n->up)){
		drawWall(n, 1, c);
	}

	if (!(n->down)){
		drawWall(n, 2, c);
	}

	if (!(n->left)){
		drawWall(n, 3, c);
	}

	if (!(n->right)){
		drawWall(n, 4, c);
	}

	Entity* floor = new Entity(Vector3(0, 0, 0), f);
	floor->setPosition(Vector3(n->getLocationX(), -5, n->getLocationY()));
	floor->setScale(5,.2,5);
	floors->add(floor);
	numFloors++;
}



/*  
Function: drawFloor(Node*, Node*)between two nodes in the level graph
Purpose: render the floor between two nodes to the screen
*/
void Level::drawFloor(Node* start, Node* end, Renderable* c)
{
	floors->add(new Entity(Vector3(0, 0, 0), c));
	float width = 5;
	if (start->getLocationX() == end->getLocationX()){

		float dist = sqrtf(((start->getLocationY()) - (end->getLocationY()))  *((start->getLocationY()) - (end->getLocationY())));
		dist = dist/2 - 5;
		Vector3 temp =  Vector3(start->getLocationX(), -width,
					(start->getLocationY() + end->getLocationY()) / 2);

		floors->elementAt(numFloors)->setScale(width, .2, dist);
		floors->elementAt(numFloors)->setPosition(temp);

		
	}
	else{
		float dist = sqrtf(((start->getLocationX()) - (end->getLocationX()))*((start->getLocationX()) - (end->getLocationX())));
		dist = dist/2 - 5;
		
			Vector3 temp = Vector3((start->getLocationX() + end->getLocationX())/2, -width,
																	start->getLocationY());
		floors->elementAt(numFloors)->setScale(dist, 0.2, width);
		floors->elementAt(numFloors)->setPosition(temp);


	}
	numFloors++;
}



/*
Function: drawWalls(Node*, Node*)
Purpose: render walls on the screen allong an edge n the level graph
*/
void Level::drawWalls(Node* start, Node* end, Renderable* c)
{

	Entity *wall1, *wall2;
	wall1 = new Entity(Vector3(0,0,0),c);
	wall2 = new Entity(Vector3(0, 0, 0), c);
	if (start->getLocationX() == end->getLocationX()){
		float dist = sqrtf((start->getLocationY() - end->getLocationY())  *(start->getLocationY() - end->getLocationY()));
		dist = dist/2 - 5;
		wall1->setScale(0.2, 5, dist);
		wall1->setPosition(Vector3(start->getLocationX() - 5, 0,
			(start->getLocationY() + end->getLocationY()) / 2));


		wall2->setScale(0.2, 5, dist);
		wall2->setPosition(Vector3(start->getLocationX() + 5, 0,
			(start->getLocationY() + end->getLocationY()) / 2));
		
	}
	else{
		
		float dist = sqrtf((start->getLocationX() - end->getLocationX())*(start->getLocationX() - end->getLocationX()));
		dist = dist/2 - 5;
		wall1->setScale(dist, 5, 0.2);
		wall1->setPosition(Vector3(((start->getLocationX() + end->getLocationX()) / 2), 0,start->getLocationY() - 5));

		wall2->setScale(dist, 5, 0.2);
		wall2->setPosition(Vector3(((start->getLocationX() + end->getLocationX()) / 2), 0, start->getLocationY() + 5));

	}
	walls->add(wall1);
	walls->add(wall2);
	numWalls += 2;

}
/*
Function: drawWall(Node*, int)
Purpose: render a wall on the edge of a corner  
*/
void Level::drawWall(Node* n, int side, Renderable* c)
{
	Entity* wall = new Entity(Vector3(0, 0, 0), c);
	if (side == 1){
		wall->setPosition(Vector3(n->getLocationX(), 0, n->getLocationY()+5));
		wall->setScale(5,5,.2);
	}
	if (side == 2){
		wall->setPosition(Vector3(n->getLocationX(), 0, n->getLocationY()-5));
		wall->setScale(5, 5, .2);
	}
	if (side == 3){
		wall->setPosition(Vector3(n->getLocationX()-5, 0, n->getLocationY()));
		wall->setScale(0.2,5, 5);
	}
	if (side == 4){
		wall->setPosition(Vector3(n->getLocationX()+5, 0, n->getLocationY()));
		wall->setScale(0.2, 5, 5);
	}
	nodeWalls->add(wall);
	numNodeWalls++;
}

void Level::initNode(Node* n){
	for (int i = 0; i < n->getEdges().size(); i++){
		Node* otherNode;


		if (n->getEdges().elementAt(i)->getHeadNode()->getLocationX() == n->getLocationX() && n->getEdges().elementAt(i)->getHeadNode()->getLocationY() == n->getLocationY()){
			otherNode = n->getEdges().elementAt(i)->getTailNode();
		}
		else{
			otherNode = n->getEdges().elementAt(i)->getHeadNode();
		}


		if (n->getLocationX() == otherNode->getLocationX()){
			if (otherNode->getLocationY() > n->getLocationY()){
				n->up = true;
			}
			else{
				n->down = true;
			}
		}

		if (n->getLocationY() == otherNode->getLocationY()){
			if (otherNode->getLocationX() > n->getLocationX()){
				n->right = true;
			}
			else{
				n->left = true;
			}
		}
	}
}