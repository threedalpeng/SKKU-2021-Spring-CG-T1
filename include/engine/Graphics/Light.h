#pragma once
#include "engine/Component/Component.h"
#include "engine/Transform/Transform.h"
#include "engine/Graphics/Shader.h"

class Light : public Component
{
public:
	enum class Type {
		Directional,
		Point
	};

	float lightSource = 0.f;
	vec4 ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	vec4 diffuse = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	vec4 specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	Light(std::shared_ptr<GameObject> obj) : Component(obj) {}

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

	void render() {
		glUseProgram(_shader->getProgram());

		vec4 position = vec4(getComponent<Transform>()->worldPosition, lightSource);
		glUniform4fv(_shader->getUniformLocation("light_position"), 1, position);
		glUniform4fv(_shader->getUniformLocation("Ia"), 1, ambient);
		glUniform4fv(_shader->getUniformLocation("Id"), 1, diffuse);
		glUniform4fv(_shader->getUniformLocation("Is"), 1, specular);
	}

private:
	Type _type = Type::Directional;
	Shader* _shader = nullptr;
};
