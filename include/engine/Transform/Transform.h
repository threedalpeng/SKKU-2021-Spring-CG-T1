#pragma once
#include "engine/Component/Component.h"
#include "engine/Math.h"
#include "bullet/src/btBulletCollisionCommon.h"
#include "bullet/src/btBulletDynamicsCommon.h"

class Transform : public Component
{
public:
	Transform(std::shared_ptr<GameObject> obj);
	~Transform();

	vec3 position = vec3();
	Quaternion rotation = Quaternion(0.f, 0.f, 0.f, 1.f);
	vec3 scale = vec3(1.f, 1.f, 1.f);
	float mass = 1.0f;

	vec3 worldPosition = vec3();
	Quaternion worldRotation = Quaternion(0.f, 0.f, 0.f, 1.f);
	vec3 worldScale = vec3(1.f, 1.f, 1.f);

	vec3 _normal = vec3();
	btRigidBody* body = nullptr;

	void translate(const vec3& v);
	void translate(float x, float y, float z);
	void rotate(vec3 axis, float angle);
	void rotate(Quaternion q);
	void rotateAround(vec3 worldPoint, vec3 axis, float angle);

	void update();
	mat4 getModelMatrix();
	void setModelMatrix(mat4* m);

	vec3 localToWorldPoint(vec3 v);
	vec3 worldToLocalPoint(vec3 v);
	// void lookAt(Transform* target, vec3 up = vec3(0, 1, 0));
	void calWorlPositionBT();
	void setWorlPositionBT(btVector3 new_position);
	btVector3 getWorlPositionBT();
	btVector3 getVelocityBT();
	void setVelocityBT(btVector3 velocity);
	void addVelocityBT(btVector3 addVelocity);

private:
	void calculateModelMatrix();
	void calculateModelMatrix(mat4 parentMatrix);
	mat4 _modelMatrix = mat4();
};
