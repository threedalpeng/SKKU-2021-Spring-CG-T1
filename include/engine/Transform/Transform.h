#pragma once
#include "engine/Component/Component.h"
#include "engine/Math.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

class Transform : public Component
{
public:
	Transform(std::shared_ptr<GameObject> obj);

	vec3 position = vec3();
	Quaternion rotation = Quaternion(0.f, 0.f, 0.f, 1.f);
	vec3 scale = vec3(1.f, 1.f, 1.f);
	vec3 worldPosition = vec3();
	Quaternion worldRotation = Quaternion(0.f, 0.f, 0.f, 1.f);
	vec3 worldScale = vec3(1.f, 1.f, 1.f);
	vec3 _normal = vec3();

	void translate(const vec3& v);
	void translate(float x, float y, float z);
	void rotate(const vec3& axis, float angle);
	void rotateAround(const vec3& worldPoint, const vec3& axis, float angle);

	void update();
	mat4 getModelMatrix();
	void setModelMatrix(mat4* m);

	vec3 localToWorldPoint(vec3 v);
	vec3 worldToLocalPoint(vec3 v);

	Quaternion worldToLocalRotation(Quaternion q);

	// void lookAt(Transform* target, vec3 up = vec3(0, 1, 0));

	btTransform toBtTransform();
	void setByBtTransform(const btTransform& btTrans);

private:
	void calculateModelMatrix();
	void calculateModelMatrix(const mat4& parentMatrix);
	mat4 _modelMatrix = mat4();
};
