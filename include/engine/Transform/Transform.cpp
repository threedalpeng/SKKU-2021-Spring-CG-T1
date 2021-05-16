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

void Transform::rotate(const vec3& axis, float angle) {
	Quaternion q = Quaternion::axisAngle(axis, angle);
	rotation = rotation * q;
}

void Transform::rotateAround(const vec3& worldPoint, const vec3& axis, float angle)
{
	Quaternion q = Quaternion::axisAngle(axis, angle);
	position = q * (position - worldPoint) + worldPoint;
	rotation = q * rotation;
}

void Transform::update() {
	if (body) calWolrPositionBT();
	GameObject* parent = getCurrentObject()->getParent();
	if (parent) {
		Transform* parentTransform = parent->getComponent<Transform>();
		calculateModelMatrix(parentTransform->getModelMatrix());
		worldPosition = vec3(_modelMatrix._14, _modelMatrix._24, _modelMatrix._34);
		worldRotation = parentTransform->worldRotation * rotation;
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
void Transform::calculateModelMatrix(const mat4& parentMatrix) {
	_modelMatrix = mat4::translate(position)
		* rotation.toMatrix()
		* mat4::scale(scale);
	_modelMatrix = parentMatrix * _modelMatrix;
}

mat4 Transform::getModelMatrix() {
	return _modelMatrix;
}

void Transform::setModelMatrix(mat4* m) {
	for (int i = 0; i < 16; i++) _modelMatrix[i] = m->a[i];
}

vec3 Transform::localToWorldPoint(vec3 v)
{
	vec4 r = _modelMatrix * vec4(v, 1.f);
	return vec3(r.x, r.y, r.z);
}

vec3 Transform::worldToLocalPoint(vec3 v)
{
	vec4 r = _modelMatrix.transpose() * vec4(v, 1.f);
	return vec3(r.x, r.y, r.z);
}

btTransform Transform::toBtTransform() {
	btQuaternion q(worldRotation.x, worldRotation.y, worldRotation.z, worldRotation.w);
	btVector3 v(worldPosition.x, worldPosition.y, worldPosition.z);
	return btTransform(q, v);
}

void Transform::setByBtTransform(const btTransform& btTrans) {
	btVector3 v = btTrans.getOrigin();
	btQuaternion q = btTrans.getRotation();
	position = worldToLocalPoint(vec3(v.x(), v.y(), v.z()));
	rotation = worldToLocalRotation(Quaternion(q.x(), q.y(), q.z(), q.w()));
}

void Transform::calWolrPositionBT()
{
	btTransform trans;	body->getMotionState()->getWorldTransform(trans);
	btVector3 tmp = trans.getOrigin();
	position = vec3(tmp.getX(), tmp.getY(), tmp.getZ());
}

void Transform::setWorlPositionBT(btVector3 new_position)
{
	btTransform trans;	body->getMotionState()->getWorldTransform(trans);
	trans.setOrigin(new_position);
}

btVector3 Transform::getVelocityBT()
{
	btTransform trans;	body->getMotionState()->getWorldTransform(trans);
	return body->getLinearVelocity();
}

void Transform::setVelocityBT(btVector3 velocity)
{
	btTransform trans;	body->getMotionState()->getWorldTransform(trans);
	body->setLinearVelocity(velocity);
}

void Transform::addVelocityBT(btVector3 addVelocity)
{
	btTransform trans;	body->getMotionState()->getWorldTransform(trans);
	btVector3 previouse_velocity = body->getLinearVelocity();
	body->setLinearVelocity(previouse_velocity + addVelocity);
}