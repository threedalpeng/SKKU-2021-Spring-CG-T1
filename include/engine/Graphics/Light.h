#pragma once
#include "engine/Component/Component.h"
#include "engine/Transform/Transform.h"
#include "engine/Graphics/Shader.h"
#include <iostream>
class Light : public Component
{
public:
	enum class Type {
		Directional,
		Point
	};

	/* Light */
	float lightSource = 0.f;
	vec4 ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	vec4 diffuse = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	vec4 specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	/* Depth Camera */
	vec3 eye = vec3(3, 0, 100);
	vec3 at = vec3(0);
	vec3 up = vec3(0.f, 1.f, 0.f);
	mat4	view_matrix = mat4::look_at(eye, at, up);

	float	fovy = PI / 3.0f; // must be in radian
	float	aspect_ratio;
	float	dNear = 1.0f;
	float	dFar = 500.0f;
	mat4	projection_matrix;

	static uint shadowWidth;
	static uint shadowHeight;

	GLuint 	depthMapFBO = 0;
	GLuint 	depthMap = 0;
	bool 	hasDepthMap = false;

	Light(std::shared_ptr<GameObject> obj) : Component(obj) {}

	void init() {
		/* Depth Map FBO */
		glGenFramebuffers(1, &depthMapFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			printf("Error: Light depthMapFBO\n");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void setType(Type type) {
		_type = type;
		switch (type) {
		case Type::Directional:
			lightSource = 0.f;
			break;
		case Type::Point:
			lightSource = 1.f;
			break;
		}
	}

	void loadShader(Shader* shader) {
		_shader = shader;
	}

	void loadShaderDepth(Shader* shader) {
		_shaderDepth = shader;
	}

	void update() {
		/* Depth camera update */
		eye = getComponent<Transform>()->worldPosition;
		aspect_ratio = shadowWidth / float(shadowHeight);
		if (GameObject* parent = getCurrentObject()->getParent()) {
			at = parent->getComponent<Transform>()->worldPosition;
		}
		projection_matrix = mat4::perspective(fovy, aspect_ratio, dNear, dFar);
		view_matrix = mat4::look_at(eye, at, up);
		hasDepthMap = false;	// initialize hasDepthMap
	}

	void renderDepth() {
		if (!_shaderDepth) return;

		glViewport(0, 0, shadowWidth, shadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glEnable(GL_CULL_FACE);								// turn on backface culling
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearDepth(1.0f);
		glDepthFunc(GL_LESS);
		glClear(GL_DEPTH_BUFFER_BIT);

		mat4 lightSpaceMatrix = projection_matrix * view_matrix;

		glUseProgram(_shaderDepth->getProgram());
		glUniformMatrix4fv(_shaderDepth->getUniformLocation("lightSpaceMatrix"), 1, GL_TRUE, lightSpaceMatrix);
		hasDepthMap = true;
	}

	void render() {
		if (!_shader) { printf("The light object does not have _shader.\n");	return; }
		glUseProgram(_shader->getProgram());

		vec4 position = vec4(getComponent<Transform>()->worldPosition, lightSource);
		// std::cout << position.x << " " << position.y << " " << position.z << " " <<  position.w << std::endl;
		glUniform4fv(_shader->getUniformLocation("light_position"), 1, position);
		glUniform4fv(_shader->getUniformLocation("Ia"), 1, ambient);
		glUniform4fv(_shader->getUniformLocation("Id"), 1, diffuse);
		glUniform4fv(_shader->getUniformLocation("Is"), 1, specular);

		// for shadow
		mat4 lightSpaceMatrix = projection_matrix * view_matrix;
		glUniformMatrix4fv(_shader->getUniformLocation("lightSpaceMatrix"), 1, GL_TRUE, lightSpaceMatrix);
		glUniform1i(_shader->getUniformLocation("b_shadow"), hasDepthMap);
		if (hasDepthMap)
		{
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			glUniform1i(_shader->getUniformLocation("shadowMap"), 3);
		}
		// glUniform3fv(_shader->getUniformLocation("lightPos"), 1, position); is same with light_position
	}

private:
	Type _type = Type::Directional;
	Shader* _shader = nullptr;
	Shader* _shaderDepth = nullptr;
};