#pragma once
#include "cg/cgmath.h"
#include "engine/Object/GameObject.h"
#include "engine/Graphics/Shader.h"
#include "engine/Transform/Transform.h"
#include "engine/Screen.h"

class Camera : public Component
{
public:
	Camera(std::shared_ptr<GameObject> obj) : Component(obj) {};

	void update() {
		Transform* transform = getComponent<Transform>();
		eye = transform->worldPosition;
		//at = transform->localToWorldPos(vec3(0.f, 0.f, 1.f));
		up = transform->localToWorldPoint(vec3(0.f, 1.f, 0.f));
		if (trackingMode) {
			at = target->worldPosition;
		}
		view_matrix = mat4::look_at(eye, at, up);

		aspect_ratio = Screen::width() / static_cast<float>(Screen::height());
		projection_matrix = mat4::perspective(fovy, aspect_ratio, dNear, dFar);
	}

	void render() {
		for (auto shader : _shaders) {
			glUseProgram(shader->getProgram());
			glUniformMatrix4fv(shader->getUniformLocation("view_matrix"), 1, GL_TRUE, view_matrix);
			glUniformMatrix4fv(shader->getUniformLocation("projection_matrix"), 1, GL_TRUE, projection_matrix);
		}
	}

	void setCameraUp(vec3 _up) {
		up = _up;
	}

	void activateTracking(Transform* _target) {
		trackingMode = true;
		target = _target;
		at = target->worldPosition;
	}
	void activateTracking(Transform* _target, vec3 _up) {
		trackingMode = true;
		target = _target;
		up = _up;
		at = target->worldPosition;
	}
	void deactivateTracking() {
		trackingMode = false;
		target = nullptr;
	}

	void addShader(Shader* shader) {
		_shaders.push_back(shader);
	}

	void setThisMainCamera() {
		main = this;
	}

	vec3 viewportToWorldPoint(vec3 v) {
		return mat3(view_matrix).transpose() * v;
	}

	vec3 worldToViewportPoint(vec3 v) {
		return mat3(view_matrix) * v;
	}

	float fovy = PI / 4.0f; // must be in radian
	float aspect_ratio = 1.f;
	float dNear = 1.0f;
	float dFar = 1000.0f;
	mat4 projection_matrix;

	static Camera* main;

	vec3 eye = vec3(0, 30, 300);
	vec3 at = vec3(0, 0, 0);
	vec3 up = vec3(0, 1, 0);
	mat4 view_matrix = mat4::look_at(eye, at, up);
private:
	bool trackingMode = false;
	Transform* target = nullptr;
	std::vector<Shader*> _shaders = std::vector<Shader*>();
};
