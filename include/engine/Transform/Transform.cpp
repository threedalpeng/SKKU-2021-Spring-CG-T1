#include "Transform.h"
#include <iostream>

Transform::Transform(std::shared_ptr<GameObject> obj) : Component(obj) {}

void Transform::translate(const vec3& v) {
	position += v;
}
void Transform::translate(float x, float y, float z) {
	vec3 v = vec3(x, y, z);
	position += v;
}

void Transform::rotate(vec3 axis, float angle) {
	Quaternion q = Quaternion::axisAngle(axis, angle);
	rotation = rotation * q;
}

void Transform::rotateAround(vec3 worldPoint, vec3 axis, float angle)
{
	Quaternion q = Quaternion::axisAngle(axis, angle);
	position = q * (position - worldPoint) + worldPoint;
	rotation = q * rotation;
}

void Transform::update() {
	if(body) wolrPositionBullet();
	GameObject* parent = getCurrentObject()->getParent();
	if (parent) {
		Transform* parentTransform = parent->getComponent<Transform>();
		calculateModelMatrix(parentTransform->getModelMatrix());
		worldPosition = vec3(_modelMatrix._14, _modelMatrix._24, _modelMatrix._34);
		worldRotation = parentTransform->worldRotation * worldRotation;
		worldScale = parentTransform->worldScale * scale;
	}
	else {
		worldPosition = position;
		worldRotation = rotation;
		worldScale = scale;
		calculateModelMatrix();
	}
}
void Transform::calculateModelMatrix() {
	_modelMatrix = mat4::translate(position)
		* rotation.toMatrix()
		* mat4::scale(scale);
}
void Transform::calculateModelMatrix(mat4 parentMatrix) {
	_modelMatrix = mat4::translate(position)
		* rotation.toMatrix()
		* mat4::scale(scale);
	_modelMatrix = parentMatrix * _modelMatrix;
}

mat4 Transform::getModelMatrix() {
	return _modelMatrix;
}

void Transform::setModelMatrix(mat4* m) {
	for(int i = 0; i < 16; i++) _modelMatrix[i] = m->a[i];
}

vec3 Transform::localToWorldPoint(vec3 v)
{
	return mat3(_modelMatrix) * v;
}

vec3 Transform::worldToLocalPoint(vec3 v)
{
	return mat3(_modelMatrix).transpose() * v;
}

void Transform::wolrPositionBullet()
{
	btTransform trans;
	body->getMotionState()->getWorldTransform(trans);
	// float tmp_1[16];
	// trans.getOpenGLMatrix(tmp_1);
	// mat4 tmp_2 = mat4(tmp_1[0], tmp_1[1], tmp_1[2], tmp_1[3], 
	// 					tmp_1[4], tmp_1[5], tmp_1[6], tmp_1[7], 
	// 					tmp_1[8], tmp_1[9], tmp_1[10], tmp_1[11], 
	// 					tmp_1[12], tmp_1[13], tmp_1[14], tmp_1[15] );
	// tmp_2 = tmp_2.transpose();
	btVector3 tmp = trans.getOrigin();
	position = vec3(tmp.getX(), tmp.getY(), tmp.getZ());
	

}