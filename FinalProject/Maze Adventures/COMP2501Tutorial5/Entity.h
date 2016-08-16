#include "Renderable.h"

struct Orientation{
	Vector3 forward;
	Vector3 up;

	Orientation() {
		forward = Vector3(0, 0, -1);
		up = Vector3(0, 1, 0);
	}

	Orientation(Vector3 _forward, Vector3 _up) {
		forward = _forward;
		up = _up;
	}

	Orientation(Vector3 _position, Vector3 _lookat, Vector3 _up) {
		forward = _lookat - _position;
		up = _up;
	}
};

class Entity {
public:
	Entity();
	Entity(Vector3 location, Renderable* renderable);
	~Entity();
	void setRenderable(Renderable* renderable);
	void render(Matrix4 &parent);
	void addChild(Entity *child);
	void pitch(float angleDeg);
	void yaw(float angleDeg);
	void roll(float angleDeg);
	void rotate(Vector3 axis, float angleDeg);
	void setScale(float scaleX, float scaleY, float scaleZ);
	void setPosition(Vector3 pos);
	void resetOrientation();
	

protected:
	void renderChildren(Matrix4 self);
	Matrix4 getModelMatrix();

	std::vector<Entity*> children = std::vector<Entity*>(); //children entities
	Renderable* model;                                      //the actual model
	Orientation orientation;
	Vector3 scale = Vector3(1, 1, 1);
	Vector3 position = Vector3(0, 0, 0);
};
