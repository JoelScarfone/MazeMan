#include "Entity.h"

Entity::Entity()
{
	
}

Entity::Entity(Vector3 location, Renderable* renderable){
	position = location;
	model = renderable;
}

Entity::~Entity(){
	model = 0;
	while (!children.empty())
	{
		Entity *temp = children.back();
		children.pop_back();
		delete temp;
	}
}

void Entity::setRenderable(Renderable* renderable){
	model = renderable;
}

void Entity::setScale(float scaleX, float scaleY, float scaleZ){
	scale = Vector3(scaleX, scaleY, scaleZ);
}

void Entity::setPosition(Vector3 pos){
	position = pos;
}

void Entity::addChild(Entity *child){
	children.push_back(child);
}

void Entity::pitch(float angleDeg){


	Vector3 axis = Vector3::cross(orientation.forward, orientation.up);
	rotate(axis, angleDeg);


}

void Entity::yaw(float angleDeg){


	rotate(orientation.up, angleDeg);


}

void Entity::roll(float angleDeg){

	rotate(orientation.forward, angleDeg);

}

void Entity::rotate(Vector3 axis, float angleDeg){
	
	Matrix4 rotMatrix = Matrix4::IDENTITY;
	rotMatrix.rotate(axis, angleDeg);

	orientation.up = orientation.up * rotMatrix;
	orientation.forward = orientation.forward * rotMatrix;

	orientation.up.normalize();
	orientation.forward.normalize();

}

Matrix4 Entity::getModelMatrix(){

	Matrix4 modelMatrix;
	Matrix4 t = translationMatrix(position);
	Matrix4 o = orientationMatrix(orientation.forward, orientation.up);
	Matrix4 s = scaleMatrix(scale);
	modelMatrix = t*o*s;
	return modelMatrix;
}

void Entity::render(Matrix4 &parent){
	Matrix4 mvpMatrix;
	mvpMatrix = parent * getModelMatrix();
	model->renderSelf(mvpMatrix);
	renderChildren(mvpMatrix);
}

void Entity::renderChildren(Matrix4 self){
	for (auto it = children.begin(); it != children.end(); ++it) {
		(*it)->render(self);
	}
}
void Entity::resetOrientation(){
	Orientation temp;
	orientation = temp;
}

