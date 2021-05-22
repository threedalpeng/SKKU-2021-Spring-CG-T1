#pragma once

// Standard Library
#include <iostream>

// Engine
#include "engine/Core.h"
#include "cg/cgmath.h"
#include "../Manager/GameManager.h"
#include "../Manager/ResourceManager.h"

#include "MeshMaker.h"
#include "ParticleScript.h"

class ParticleMaker
{
public:
	static int maxForOne;
	static void  makeExplodeParticle(vec3 parent_position, float scale = 1.0f);
};

int ParticleMaker::maxForOne = 80;

void ParticleMaker::makeExplodeParticle(vec3 parent_position, float scale)
{
	MeshRenderer* meshRenderer;
	Mesh* sphereMesh = MeshMaker::makeSphere();
	Texture* fireTexture = ResourceManager::getTexture("fire");
	Material* material = ResourceManager::getMaterial("Basic");

	Transform* transform;
	GameObject* particle;

	for (int i = 0; i < maxForOne && GameManager::particleFull(); i++)
	{
		particle = GameObject::create("particle");

		meshRenderer = particle->addComponent<MeshRenderer>();
		meshRenderer->loadMesh(sphereMesh);
		meshRenderer->loadTexture(fireTexture);
		meshRenderer->loadMaterial(material);
		meshRenderer->loadShader(GameManager::basicShader);

		meshRenderer->isShaded = false;
		meshRenderer->isColored = true;
		meshRenderer->hasTexture = true;
		meshRenderer->color = vec4(1.0f, 0.5f, 0.0f, 1.0f);

		transform = particle->getComponent<Transform>();
		transform->position = parent_position;
		transform->rotation = Quaternion(0.f, 0.f, 0.f, 1.f);
		transform->scale = vec3(0.05f, 0.05f, 0.05f) * scale;
		transform->mass = 0.1f;

		ParticleScript* particleScript = new ParticleScript(transform->position, scale);
		particle->addComponent<ScriptLoader>()->addScript(particleScript);
		particleScript->meshRenderer = meshRenderer;
		particleScript->transform = transform;
		particleScript->init();

		SceneManager::scene()->addObject(particle);
		GameManager::increaseParticle();
	}
}