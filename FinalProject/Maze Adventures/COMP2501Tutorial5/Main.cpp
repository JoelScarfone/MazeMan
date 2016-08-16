
//include statments
#include <windows.h>	   
#include <glew.h>
#include <freeglut.h>	   
#include <iostream>
#include <SOIL.h>
#include <unordered_map>
#include <unordered_map>
#include <cmath>
#include "MathHelper.h"
#include "Camera.h"
#include "Level.h"
#include "Edge.h"
#include "Skybox.h"
#include "Text.h"
#include "Enemy.h"
#include "sound.h"
#pragma comment(lib, "glew32.lib")

#define KEY_ESCAPE 27

// ************************************************
// struct to hold window and projection information
struct Window{
	GLuint id;       // glut window id
	char* title;     // window title

	int width;       // window width
	int height;      // window height
	float fovAngle;  // field of view for projection
	float zNear;     // near plane for projection
	float zFar;      // far plane for projection
};

//struct to hold an x and y value 
struct v2{
	v2(float _x, float _y){ x = _x; y = _y; }
	v2(){ x = 0; y = 0; }
	float x;
	float y;
};
// ************************************************

//forward declarations for functoins
float checkCollision(Vector3&);
void checkNextLevel();
Vector3 findPosition(Level *l);
float angleBetween(Vector3 a, Vector3 b);
//global variables 
char * VSFileName = "basicshader.vert";
char * FSFileName = "basicshader.frag";
char * VSTextShaderFileName = "textShader.vert";
char * FSTextShaderFileName = "textShader.frag";
char * VSSkyboxFileName = "SkyboxShader.vert";
char * FSSkyboxFileName = "SkyboxShader.frag";
char* VSTextureFileName = "textureShader.vert";
char* FSTextureFileName = "textureShader.frag";
Sound* sound;
Window win1;
Shader *shader1 = 0, *textShader = 0, *skyboxShader = 0, *wallShader = 0, *buttonShader = 0;
Matrix4 projectionMatrix = Matrix4::IDENTITY;
Camera *camera1 = 0;
Level *levels[15];
int currLevel = 0;
int MenOption = 0;
int TimeMin = 0;
int mPosX, mPosY;
GLuint vao, vio;
GLuint mvpMatrixLoc;
Vector3 toClosestEdge;
Vector3 savePoint;
Vector3 saveUP, saveFOR;
Skybox *skybox;
ArrayList<Edge>* levelEdges;
bool jumping = false;
bool isPaused = false;
bool hasSavePoint = false;
bool keys[256];
bool special[256];
float gravity = 0.0000005;
float Time = 0;
float distanceFromMaze;
float Stamina = 100;
float boost = 0;
float legAngle = 4;
float moveTime = 15;
Renderable *cubeModel, *sphereModel, *cylinderModel, *enemyModel;
Renderable *wallModel;
Renderable *floorModel;
Renderable *flagModel;
Entity* saveFlag;
Entity savePlayer, startPlayer;
Entity cylinder1;
Entity thighLeft, thighRight;
Entity kneeLeft, kneeRight;
Entity legLeft, legRight;
Text *PlayBut, *ControlsBut, *Title, *CreditsBut, *Credits1, *Credits2, *Controls1,*Controls2, *Controls3 ,*Controls4, *Controls5, *Controls6, *Paused ,*Back ,*Loading, *Exitbut, *Restartbut , *time,*notice;
Enemy* enemies[30];
std::string State = "Menu";
std::unordered_map<std::string, GLuint> images;

// ****Controll Flow Function Including - initalizatoin, rendering, and collision detection****

void updateProjection(Window win)
{
	glutSetWindow(win.id);                                                      // tell opengl which window you want to change
	win.height = (win.height < 1) ? 1 : win.height;                            // make sure we are not dividing by zero for the aspect
	glViewport(0, 0, win.width, win.height);								    // set the viewport

	glMatrixMode(GL_PROJECTION);									// select projection matrix
	glLoadIdentity();												// reset projection matrix

	GLfloat aspect = (GLfloat)win.width / win.height;               // calculate aspect ratio
	projectionMatrix = createPerspectiveProjectionMatrix(win.fovAngle, aspect, win.zNear, win.zFar); // set up a perspective projection matrix
}

void initShaders()
{

	GLuint vertexLoc, colorLoc;
	GLuint buffers[3];

	buttonShader = wallShader = new Shader(VSTextureFileName, FSTextureFileName);
	shader1 = new Shader(VSFileName, FSFileName);
	textShader = new Shader(VSTextShaderFileName, FSTextShaderFileName);
	skyboxShader = new Shader(VSSkyboxFileName, FSSkyboxFileName);
	wallShader = new Shader(VSTextureFileName, FSTextureFileName);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Generate slots for the vertex and color buffers
	glGenBuffers(3, buffers);

	// bind buffer for vertices and copy data into buffer
	//vertexLoc = glGetAttribLocation(shader1->id(), "position");
	//glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	//glBufferData(GL_ARRAY_BUFFER, numvertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(vertexLoc);
	//glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0);

	// bind buffer for colors and copy data into buffer
	//colorLoc = glGetAttribLocation(shader1->id(), "color");
	//glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	//glBufferData(GL_ARRAY_BUFFER, numvertices * sizeof(Color), colors, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(colorLoc);
	//glVertexAttribPointer(colorLoc, 4, GL_FLOAT, 0, 0, 0);

	// bind buffer for indices and copy data into buffer
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, numindices * sizeof(GLushort), indices, GL_STATIC_DRAW);
	//vio = buffers[2];

	//mvpMatrixLoc = glGetUniformLocation(shader1->id(), "mvpMatrix");

	
}

//set the two global variables to keep track of mouse position
void setMouse(int x, int y){
	mPosX = x;
	mPosY = y;
}

//handles a collision between the character and a wall
void handleCollision(){
	float distanceOver = distanceFromMaze - 4.0;
	camera1->setPosition(Vector3(camera1->getPosition().x - ((distanceOver / distanceFromMaze)*toClosestEdge.x), camera1->getPosition().y, camera1->getPosition().z - ((distanceOver / distanceFromMaze)*toClosestEdge.z)));
}

//used to handle all user input
void handleInput(int deltatime)
{
	
	float deltaseconds = deltatime / 1000.0f;

	if (State == "Play"){

		Time += deltaseconds;
		glutSetCursor(GLUT_CURSOR_NONE);
		glutPassiveMotionFunc(setMouse);
		glutWarpPointer(win1.width / 2, win1.height / 2);
		float mouseSpeed = 5;
		float xAngle = mouseSpeed * deltaseconds * float(win1.width / 2 - mPosX);
		float yAngle = mouseSpeed * deltaseconds * float(win1.height / 2 - mPosY);

		camera1->mouseLook(xAngle, yAngle);
		cylinder1.yaw(-xAngle);

		if (keys['w']) {
			if (!jumping)
				PlaySound(TEXT("step.wav"), NULL, SND_FILENAME | SND_ASYNC| SND_NOSTOP);
			camera1->move(deltaseconds * (15 + boost));
			distanceFromMaze = checkCollision(toClosestEdge);
			if (distanceFromMaze > 4.0){
				handleCollision();
			}
			moveTime++;
			if (moveTime > 30){
				legAngle *= -1;
				moveTime = 0;
			}
			thighLeft.pitch(legAngle);
			thighRight.pitch(-legAngle);

		}
		else{
			moveTime = 15;
			thighLeft.resetOrientation();
			thighRight.resetOrientation();


		}
		if (keys['f']){
			savePlayer = cylinder1;
			savePoint = camera1->getPosition();
			saveUP = camera1->getCameraUp();
			saveFOR = camera1->getForward();
			savePoint.y = 2;
			hasSavePoint = true;
			keys['f'] = false;
		}
		if (keys['g'] && hasSavePoint){
			cylinder1 = savePlayer;
			camera1->setPosition(savePoint);
			camera1->setCameraUp(saveUP);
			camera1->setForward(saveFOR);
			keys['g'] = false;
		}
		if (keys['s']){
			if (!jumping)
				PlaySound(TEXT("step.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
			camera1->move(-deltaseconds * (15 + boost));
			distanceFromMaze = checkCollision(toClosestEdge);
			if (distanceFromMaze > 4.0){
				handleCollision();
			}

		}
		if (keys['a']) {
			if (!jumping)
				PlaySound(TEXT("step.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
			camera1->strafe(-deltaseconds * (15 + boost));
			distanceFromMaze = checkCollision(toClosestEdge);
			if (distanceFromMaze > 4.0){
				handleCollision();
			}
		}
		if (keys['d']) {
			if (!jumping)
				PlaySound(TEXT("step.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
			camera1->strafe(deltaseconds * (15 + boost));
			distanceFromMaze = checkCollision(toClosestEdge);
			if (distanceFromMaze > 4.0){
				handleCollision();
			}
		}
		if (keys['p']) {
			State = "Menu";
			isPaused = true;
		}
		if (keys[' '] && Stamina > 5 && !jumping) {
			PlaySound(TEXT("jump.wav"), NULL, SND_FILENAME | SND_ASYNC );
			jumping = true;
			Stamina -= 5;
			keys[' '] = false;
		}
		if (jumping) camera1->jump(deltaseconds * 10);
	
		
		if (camera1->getPosition().y <= 2){
			camera1->setPosition(Vector3(camera1->getPosition().x, 2, camera1->getPosition().z));
			jumping = false;
			camera1->gravity = 0.25;
		}

		if (special[GLUT_KEY_SHIFT_L] && Stamina > 0){
			Stamina -= (0.001 / deltaseconds);
			if (Stamina < 0){
				Stamina = 0;
				boost = 0;
			}
			else{
				boost = 10;
			}
		}
		else{
			boost = 0;
			Stamina += (0.001 / deltaseconds);
			if (Stamina >= 100){
				Stamina = 100;
			}
		}
	
	}
	if (State == "Menu"){
		if (!isPaused){
			if (special[GLUT_KEY_UP]){
				MenOption--;
				PlaySound(TEXT("switch.wav"), NULL, SND_FILENAME | SND_ASYNC);
				special[GLUT_KEY_UP] = false;
				if (MenOption < 0){
					MenOption = 3;
				}
			}

			if (special[GLUT_KEY_DOWN]){
				MenOption++;
				PlaySound(TEXT("switch.wav"), NULL, SND_FILENAME | SND_ASYNC);
				special[GLUT_KEY_DOWN] = false;
				if (MenOption > 3){
					MenOption = 0;
				}
			}
		}
		else{
			if (special[GLUT_KEY_UP]){
				PlaySound(TEXT("switch.wav"), NULL, SND_FILENAME | SND_ASYNC);
				MenOption--;
				special[GLUT_KEY_UP] = false;
				if (MenOption < 0){
					MenOption = 4;
				}
			}

			if (special[GLUT_KEY_DOWN]){
				PlaySound(TEXT("switch.wav"), NULL, SND_FILENAME | SND_ASYNC);
				MenOption++;
				special[GLUT_KEY_DOWN] = false;
				if (MenOption > 4){
					MenOption = 0;
				}
			}
		}

		if (keys[' '] && MenOption == 0){
			PlaySound(TEXT("play.wav"), NULL, SND_FILENAME | SND_ASYNC);
			if (!isPaused)
				State = "Loading";
			else
				State = "Play";
			keys[' '] = false;
			isPaused = false;
		}
		if (keys[' '] && MenOption == 1){
			PlaySound(TEXT("switch.wav"), NULL, SND_FILENAME | SND_ASYNC);
			State = "Controls";
			keys[' '] = false;
		}
		if (keys[' '] && MenOption == 2){
			PlaySound(TEXT("switch.wav"), NULL, SND_FILENAME | SND_ASYNC);
			State = "Credits";
			keys[' '] = false;
		}
		if (keys[' '] && MenOption == 3){
			PlaySound(TEXT("switch.wav"), NULL, SND_FILENAME | SND_ASYNC);
			glutExit();
		}
		if (keys[' '] && MenOption == 4){
			PlaySound(TEXT("switch.wav"), NULL, SND_FILENAME | SND_ASYNC);
			Stamina = 100;
			camera1 = new Camera(Vector3(levels[currLevel]->startPoint->x, 2, levels[currLevel]->startPoint->y), findPosition(levels[currLevel]), Vector3(0, 1, 0));
			State = "Loading";
			keys[' '] = false;
			isPaused = false;
		}
	}

	if (State == "Credits"){
		if (keys[' ']){
			PlaySound(TEXT("switch.wav"), NULL, SND_FILENAME | SND_ASYNC);
			State = "Menu";
			keys[' '] = false;
		}
	}
	if (State == "Controls"){
		if (keys[' ']){
			PlaySound(TEXT("switch.wav"), NULL, SND_FILENAME | SND_ASYNC);
			State = "Menu";
			keys[' '] = false;
		}
	}
	if (State == "Loading"){
		if (keys[' ']){
			PlaySound(TEXT("switch.wav"), NULL, SND_FILENAME | SND_ASYNC);
			State = "Play";
			keys[' '] = false;
		}
	}
}

//returns  f * f
float sqr(float f) { 
	return f*f;
}

// retuns the distance squared between two v2's
// avoids square rooting 
float dist2(v2 v, v2 w){
	return (sqr(v.x - w.x) + sqr(v.y - w.y));
}

//takes in the players position, and two vectors (v, and w) to form a line segments
float distancetoSegmentSquared(v2 p,v2 v, v2 w, Vector3& vector){
	float l2 = dist2(v,w);
	float t = ((p.x - v.x) * (w.x - v.x) + (p.y - v.y) * (w.y - v.y)) / l2;
	if (t < 0.0){
		vector = Vector3(p.x-v.x,2,p.y-v.y);
		return dist2(v, p);
	}
	if (t > 1.0){
		vector = Vector3(p.x - w.x, 2, p.y - w.y);
		return dist2(w, p);
	}
	v2 proj;
	proj.x = v.x + t * (w.x - v.x);
	proj.y = v.y + t * (w.y - v.y); 
	vector = Vector3(p.x - proj.x, 2,  p.y - proj.y);
	return dist2(p,proj);
}

//detects player - walls collision
float checkCollision(Vector3& v){	
	Vector3 tempHold;

	v2 playerPos;
	playerPos.x = camera1->getPosition().x;
	playerPos.y = camera1->getPosition().z;

	v2 temp1;
	temp1.x = levelEdges->elementAt(0)->getHeadNode()->getLocationX();
	temp1.y = levelEdges->elementAt(0)->getHeadNode()->getLocationY();

	v2 temp2;
	temp2.x = levelEdges->elementAt(0)->getTailNode()->getLocationX();
	temp2.y = levelEdges->elementAt(0)->getTailNode()->getLocationY();
	
	float dist = distancetoSegmentSquared(playerPos, temp1, temp2, tempHold);
	v = Vector3(tempHold);

	for (int i = 0; i < levelEdges->size(); i++){
		temp1.x = levelEdges->elementAt(i)->getHeadNode()->getLocationX();
		temp1.y = levelEdges->elementAt(i)->getHeadNode()->getLocationY();

		temp2.x = levelEdges->elementAt(i)->getTailNode()->getLocationX();
		temp2.y = levelEdges->elementAt(i)->getTailNode()->getLocationY();
		//we now have a point, and a line segment(formed by temp1 and temp2)
		//now we need to get the distance from the player to the closest line segment
		
		float tempDist = distancetoSegmentSquared(playerPos, temp1, temp2, tempHold);
		if (tempDist < dist){
			v = Vector3(tempHold);
			dist = tempDist;
		}
	}	
	return sqrt(dist);
}

//move the Enemies based on a given index in the Enemies array
void moveEnemies(int enemy){
	for (int i = 0; i < 30; i++)
	{
		if (enemies[i]->getAlive()){
			enemies[i]->move();
		}
	}
}

//randomizes enemies locations and sets their 'alive' state to true based on the current level
void resetEnemies(){
	for (int i = 0; i < 30; i++){
		float temp;
		do{
			temp = rand() % levelEdges->size();
		} while (levelEdges->elementAt(temp)->getHeadNode()->getLocationX() == camera1->getPosition().x &&
			levelEdges->elementAt(temp)->getHeadNode()->getLocationY() == camera1->getPosition().z);
		
		enemies[i]->setPosition(Vector3(levelEdges->elementAt(temp)->getHeadNode()->getLocationX(), -.5 , levelEdges->elementAt(temp)->getHeadNode()->getLocationY()));
		enemies[i]->setCurrentNode(levelEdges->elementAt(temp)->getHeadNode());
		enemies[i]->getRandomTarget();
		
		if (i>currLevel*2){
			enemies[i]->setAlive(false);
		}
	}
}

//detects player - enemy collision
void checkEnemyCollision(){
	for (int i = 0; i < 30; i++){
		if (enemies[i]->getAlive()){
			if (dist2(v2(enemies[i]->getPosition().x, enemies[i]->getPosition().z), v2(camera1->getPosition().x, camera1->getPosition().z)) < 14){
				if (camera1->getPosition().y > 5){
					enemies[i]->setAlive(false);
					camera1->gravity = 0.25;
					jumping = true;
					PlaySound(TEXT("splat.wav"), NULL, SND_FILENAME | SND_ASYNC);		
				}
				else{
					resetEnemies();
					PlaySound(TEXT("death.wav"), NULL, SND_FILENAME | SND_ASYNC);
					Stamina = 100;
					camera1 = new Camera(Vector3(levels[currLevel]->startPoint->x, 2, levels[currLevel]->startPoint->y), findPosition(levels[currLevel]), Vector3(0, 1, 0));
					cylinder1 = startPlayer;
					State = "Loading";					
				}
			}
		}
	}
}

//function that is repeatedly called to render the window
void renderWin1(int currentTime, int deltaTime)
{
	Matrix4 vpMatrix, viewMatrix, modelMatrix;
	handleInput(deltaTime);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewMatrix = camera1->getViewMatrix();
	vpMatrix = projectionMatrix * viewMatrix;
	Matrix4 orthographicMatrix = Matrix4::IDENTITY;
	orthographicMatrix = createOrthographicProjectionMatrix(win1.width, win1.height, -1, 1);
	char buffer[256];

	if (State == "Play"){
		checkEnemyCollision();
		skybox->render_self(projectionMatrix*viewMatrix, camera1->getPosition(), win1.zFar);
		levels[currLevel]->drawLevel(vpMatrix);
		time->setPosition(Vector3(0, -330, 0));

		if (Time >= 59){
			Time -= 59;
			TimeMin++;
		}
		
		if (Time >= 9.0f){
			sprintf(buffer, "Total time : %d:%0.0f", TimeMin, Time);
		}
		else{
			sprintf(buffer, "Total time : %d:0%0.0f", TimeMin, Time);
		}

		std::string timeM(buffer);
		time->render(orthographicMatrix, timeM);
		cylinder1.setPosition((camera1->getPosition()));
		cylinder1.render(vpMatrix);

		for (int i = 0; i < 30; i++){
			if (enemies[i]->getAlive())
				enemies[i]->render(vpMatrix);
			moveEnemies(i);
		}

		if (hasSavePoint){
			saveFlag->setPosition(savePoint);
			saveFlag->render(vpMatrix);
		}
		buffer[256];
		sprintf(buffer, "Stamina: %0.2f", Stamina);
		std::string message(buffer);
		Paused->setPosition(Vector3(-390, -330, 0));
		if (Stamina < 20){
			Paused->setColor(Color(1, 0, 0, 1));
		}
		else if (Stamina < 60){
			Paused->setColor(Color(1, 1, 0, 1));
		}
		else{
			Paused->setColor(Color(0, 1, 0, 1));
		}

		Paused->render(orthographicMatrix, message);
			
		checkNextLevel();
	} else if (State == "Menu") {
		notice->setColor(Color(1, 0, 1, 0));
		notice->setPosition(Vector3(-260, -180, 0));
		notice->render(orthographicMatrix, "ARROWS TO NAVIGATE SPACE TO SELECT");
		Title->render(orthographicMatrix, "Maze Adventures!");
		PlayBut->render(orthographicMatrix, "Play");
		
		if (MenOption == 0)
			PlayBut->setColor(Color(1, 1, 0, 1));
		else
			PlayBut->setColor(Color(1, 1, 1, 200));

		ControlsBut->render(orthographicMatrix, "Controls");
		if (MenOption == 1)
			ControlsBut->setColor(Color(1, 1, 0, 1));
		else
			ControlsBut->setColor(Color(1, 1, 1, 200));

		CreditsBut->render(orthographicMatrix, "Credits");
		if (MenOption == 2)
			CreditsBut->setColor(Color(1, 1, 0, 1));
		else
			CreditsBut->setColor(Color(1, 1, 1, 200));

		Exitbut->render(orthographicMatrix, "Exit");
		if (MenOption == 3)
			Exitbut->setColor(Color(1, 1, 0, 0.1f));
		else
			Exitbut->setColor(Color(1, 1, 1, 200));

		if (isPaused){
			Paused->setPosition(Vector3(-50, -150, 0));
			Paused->setColor(Color(1, 0, 0, 1));
			Paused->render(orthographicMatrix, "PAUSED");
			if (MenOption == 4)
				Restartbut->setColor(Color(1, 1, 0, 1));
			else
				Restartbut->setColor(Color(1, 1, 1, 200));
			Restartbut->render(orthographicMatrix, "Restart Level");

		}
	}
	else if (State == "Controls"){
		Back->setPosition(Vector3(-260, 200, 0));
		Back->render(orthographicMatrix, "BACK");
		Controls1->render(orthographicMatrix, "MOVE: W - Forward, S - Backward");
		Controls2->render(orthographicMatrix, "STRAFE: A - Left, D - Right");
		Controls3->render(orthographicMatrix, "LOOK/TURN: Mouse");
		Controls4->render(orthographicMatrix, "JUMP: Spacebar");
		Controls5->render(orthographicMatrix, "SPRINT: L-Shift");
		Controls6->render(orthographicMatrix, "PLACE SAVE: F - RETURN TO SAVE: G");
	}
	else if (State == "Credits"){
		Back->setPosition(Vector3(-260, 200, 0));
		Credits1->render(orthographicMatrix, "Maze Adventures Created By:");
		Credits2->render(orthographicMatrix, "Joel Scarfone & Alex Carlucci");
		Back->render(orthographicMatrix, "BACK");
	}
	else if (State == "Loading"){
		
		time->setPosition(Vector3(-50, -150, 0));
		sprintf(buffer, "Total time", Time);
		std::string timeM(buffer);
		//time->render(orthographicMatrix, timeM);
		char buffer[256];
		sprintf(buffer, "Level: %d", currLevel + 1);
		std::string message(buffer);
		

		Loading->render(orthographicMatrix, message);
		Back->setPosition(Vector3(-50, -150, 0));
		Back->render(orthographicMatrix, "Start");
	}
	else if (State == "Game_Finished"){
		Paused->setPosition(Vector3(-220, 120, 0));
		Paused->setColor(Color(0, 1, 0, 0));
		Paused->render(orthographicMatrix, "CONGRAGULATION YOU WON!");
		if (Time >= 59){
			Time -= 59;
			TimeMin++;
		}

		if (Time >= 9.0f){
			sprintf(buffer, "Total time: %d:%0.0f", TimeMin, Time);
		}
		else{
			sprintf(buffer, "Total time: %d:0%0.0f", TimeMin, Time);
		}

		std::string timeM(buffer);
		time->render(orthographicMatrix, timeM);
		time->setPosition(Vector3(-150, -150, 0));
	}
	glutSwapBuffers(); //works with GL_DOUBLE. use glFlush(); instead, if using GL_SINGLE
}

//called when a key is pressed
void keyboardDown(unsigned char key, int mousePositionX, int mousePositionY)
{
	key = tolower(key);
	keys[key] = true;
	if (key == KEY_ESCAPE) glutExit();
	if (key == ' ' && MenOption == 3 && State=="Menu"){
		glutExit();
	}
}

//called when a key is released
void keyboardUp(unsigned char key, int mousePositionX, int mousePositionY)
{
	key = tolower(key);
	keys[key] = false;
}

//called when a special key is pressed
void specialDown(int key, int mousePositionX, int mousePositionY)
{
	special[key] = true;
}

//called when a special key is released
void specialUp(int key, int mousePositionX, int mousePositionY)
{
	special[key] = false;
}

//deals with resizing the window
void reshape(GLint newWidth, GLint newHeight) {
	win1.width = newWidth;
	win1.height = newHeight;

	updateProjection(win1);
}

// Function called when timer ends
void timer(int id) {
	glutPostRedisplay(); // Tell GLUT to call it's glutDisplayFunc
}

// Set new timer and render
void display(void)
{
	static int oldTime = 0;
	int currentTime = 0, deltaTime = 0;

	currentTime = glutGet(GLUT_ELAPSED_TIME);      //Time since beginning
	deltaTime = currentTime - oldTime;             //Time since last render
	oldTime = currentTime;                         //Set last render time

	glutTimerFunc(17, timer, 1);                   //Call timer function in at least 17 milliseconds
	glutSetWindow(win1.id);						   //Tell glut which window we are working on (only needed for multiple windows)
	renderWin1(currentTime, deltaTime);            //Call our render function
}

// callback function when glut ends
void close()
{
	if (wallModel) delete wallModel;
	//if (level1) delete level1;
	if (shader1) delete shader1;
	if (camera1) delete camera1;
}

//loads images 
void loadImages()
{
	images["skybox"] = SOIL_load_OGL_cubemap("skybox_xp.png", "skybox_xn.png", "skybox_yp.png", "skybox_yn.png", "skybox_zp.png", "skybox_zn.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);
	images["font1"] = SOIL_load_OGL_texture("courier.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);
	images["cubeTexture"] = SOIL_load_OGL_texture("cubeTexture.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);
	images["cubeTexture2"] = SOIL_load_OGL_texture("cubeTexture2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);
	images["flagTexture"] = SOIL_load_OGL_texture("flagTexture.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);
	images["slimeTexture"] = SOIL_load_OGL_texture("slime.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);
}

void makeEntites()
{



	sphereModel = createSphere(shader1, 0, 30, 30, 1.0f);
	cylinderModel = createCylinder(shader1, 0, 30, 2.0f, 1.0f, 1.0f);


	legLeft = Entity(Vector3(0, -1.5, 0), cylinderModel);
	legRight = Entity(Vector3(0, -1.5, 0), cylinderModel);

	kneeLeft = Entity(Vector3(0, -1.5, 0), sphereModel);
	kneeRight = Entity(Vector3(0, -1.5, 0), sphereModel);

	thighLeft = Entity(Vector3(-.5, -1.5, 0), cylinderModel);
	thighRight = Entity(Vector3(.5, -1.5, 0), cylinderModel);

	cylinder1 = Entity(Vector3(0, 0, 0), cylinderModel);
	

	cylinder1.setScale(.2, .2, .2);
	thighLeft.setScale(.4, .4, .4);
	thighRight.setScale(.4, .4, .4);

	
	cylinder1.addChild(&thighLeft);
	cylinder1.addChild(&thighRight);
	thighLeft.addChild(&kneeLeft);
	thighRight.addChild(&kneeRight);
	kneeLeft.addChild(&legLeft);
	kneeRight.addChild(&legRight);


	skybox = new Skybox(skyboxShader, images["skybox"]);
	wallModel = createCube(wallShader, images["cubeTexture"]);
	enemyModel = createCube(wallShader, images["slimeTexture"]);
	floorModel = createCube(wallShader, images["cubeTexture2"]);
	flagModel = createCube(wallShader, images["flagTexture"]);


	saveFlag = new Entity(Vector3(0,0,0), flagModel);
	saveFlag->setScale(.2, 5, .2);

	levels[currLevel]->initLevel(wallModel, floorModel);

	Title = new Text(textShader, images["font1"], 30, 35);
	Title->setPosition(Vector3(-220, 120, 0));

	PlayBut = new Text(textShader, images["font1"], 20,25);
	PlayBut->setPosition(Vector3(-210, 20, 0));

	ControlsBut = new Text(textShader, images["font1"], 20, 25);
	ControlsBut->setPosition(Vector3(-210, -10, 0));
	notice = new Text(textShader, images["font1"], 15, 20);
	CreditsBut = new Text(textShader, images["font1"], 20, 25);
	CreditsBut->setPosition(Vector3(-210, -40, 0));
	Exitbut = new Text(textShader, images["font1"], 20, 25);
	Exitbut->setPosition(Vector3(-210, -70, 0));
	Restartbut = new Text(textShader, images["font1"], 20, 25);
	Restartbut->setPosition(Vector3(-210, -100, 0));
	Paused = new Text(textShader, images["font1"], 20, 25);
	Paused->setColor(Color(1, 0, 0,1));
	Back = new Text(textShader, images["font1"], 20, 25);
	Back->setColor(Color(1, 1, 0, 1));
	Credits1 = new Text(textShader, images["font1"], 20, 25);
	Credits1->setPosition(Vector3(-250, 50, 0));

	time = new Text(textShader, images["font1"], 20, 25);


	Credits2 = new Text(textShader, images["font1"], 20, 25);
	Credits2->setPosition(Vector3(-270, 0, 0));

	Controls1 = new Text(textShader, images["font1"], 15, 20);
	Controls1->setPosition(Vector3(-250, 120, 0));
	Controls2 = new Text(textShader, images["font1"], 15, 20);
	Controls2->setPosition(Vector3(-250, 60, 0));
	Controls3 = new Text(textShader, images["font1"], 15, 20);
	Controls3->setPosition(Vector3(-250, 0, 0));
	
	Controls4 = new Text(textShader, images["font1"], 15, 20);
	Controls4->setPosition(Vector3(-250, -60, 0));

	Controls5 = new Text(textShader, images["font1"], 15, 20);
	Controls5->setPosition(Vector3(-250, -120, 0));

	Controls6 = new Text(textShader, images["font1"], 15, 20);
	Controls6->setPosition(Vector3(-250, -180, 0));

	Loading = new Text(textShader, images["font1"], 15, 20);
	Loading->setPosition(Vector3(-60, 120, 0));

	for (int i = 0; i < 30; i++){
		enemies[i] = new Enemy(Vector3(0,-.5,0),enemyModel);
		enemies[i]->setScale(3,.5,3);
	}
}

void initialize()
{
	hasSavePoint = false;
	if (currLevel == 0){
		//glutSetCursor(GLUT_CURSOR_NONE);
		setMouse(win1.width / 2, win1.height / 2);
		updateProjection(win1);
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0.0, 0.0, 0.0, 1.0);											// specify clear values for the color buffers	
	loadImages();
	levels[currLevel] = new Level(currLevel + 1);
	Stamina = 100;
	makeEntites();
	levelEdges = levels[currLevel]->getEdges();
	
	camera1 = new Camera(Vector3(levels[currLevel]->startPoint->x, 2, levels[currLevel]->startPoint->y), findPosition(levels[currLevel]), Vector3(0, 1, 0));
	if (levels[currLevel]->startNode->left || levels[currLevel]->startNode->right){
		cylinder1.yaw(90);
	}
	startPlayer = cylinder1;
	resetEnemies();
	sound = new Sound();
	sound->Load();
	sound->Play(SOUND_MUSIC1);
	
}

int main(int argc, char **argv)
{
	// set window values
	win1.width = 640*1.5;
	win1.height = 480*1.5;
	win1.title = "Tutorial 4";
	win1.fovAngle = 45;
	win1.zNear = 0.1f;
	win1.zFar = 500.0f;

	//initialize and run program
	glutInit(&argc, argv);                                      // GLUT initialization
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);  // Display Mode
	glutInitWindowSize(win1.width, win1.height);					// set window size
	win1.id = glutCreateWindow(win1.title);					    // create Window

	glutDisplayFunc(display);									// register Display Function
	glutKeyboardUpFunc(keyboardUp);								    // register Keyboard Handler
	glutKeyboardFunc(keyboardDown);								    // register Keyboard Handler
	glutSpecialUpFunc(specialUp);
	glutSpecialFunc(specialDown);
	glutCloseFunc(close);

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	initShaders();
	initialize();
	glutMainLoop();												// run GLUT mainloop
	return 0;
}

//function to check if a player has completed the level
void checkNextLevel(){
	if (dist2(v2(camera1->getPosition().x, camera1->getPosition().z), v2(levels[currLevel]->endPoint->x, levels[currLevel]->endPoint->y)) < 3){
		if (currLevel != 5){
			currLevel++;
			State = "Loading";
			initialize();
		}
		else{
			State = "Game_Finished";
		}
	}
}

//function that returns a vector to make sure the player always faces the right way on level start
Vector3 findPosition(Level *l){
	if (l->startNode->up){
		return Vector3(l->startNode->getLocationX(), 2, l->startNode->getLocationY()+1);
	}
	else if (l->startNode->down){
		return Vector3(l->startNode->getLocationX(), 2, l->startNode->getLocationY()-1);
	}
	else if (l->startNode->left){
		return Vector3(l->startNode->getLocationX()-1, 2, l->startNode->getLocationY());
	}
	else if(l->startNode->right){
		return Vector3(l->startNode->getLocationX()+1, 2, l->startNode->getLocationY());
	}
}

float angleBetween(Vector3 a, Vector3 b){
	return(acos (a.dot(a,b)  ));	
}