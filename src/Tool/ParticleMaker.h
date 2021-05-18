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
	static void  makeExplodeParticle();
};

int ParticleMaker::maxForOne = 5;

void ParticleMaker::makeExplodeParticle()
{
	MeshRenderer* meshRenderer;
	Mesh* sphereMesh = MeshMaker::makeSphere();
	Texture* fireTexture = ResourceManager::getTexture("fire");
	Material* material = new Material();
	// Shader* basicShader = new Shader("shaders/solar-system.vert", "shaders/solar-system.frag");
	Transform* transform;
	GameObject* particle;

	for(int i = 0; i < maxForOne && GameManager::particleFull(); i++)
	{
		particle = GameObject::create("particle");
		
		meshRenderer = particle->addComponent<MeshRenderer>();
		meshRenderer->loadMesh(sphereMesh);
		meshRenderer->loadTexture(fireTexture);
		meshRenderer->loadMaterial(material);
		meshRenderer->loadShader(basicShader);

		meshRenderer->isShaded = false;
		meshRenderer->isColored = true;
		meshRenderer->hasTexture = false;
		meshRenderer->color = vec4(1.0f, 0.5f, 0.0f, 1.0f);

		transform = particle->getComponent<Transform>();
		transform->position = vec3(0.0f, 0.0f, 0.0f);
		transform->rotation= Quaternion(0.f, 0.f, 0.f, 1.f);
		transform->scale = vec3(0.1f, 0.1f, 0.1f);
		transform->mass = 0.1f;

		ParticleScript* particleScript = new ParticleScript(transform->position);
		particle->addComponent<ScriptLoader>()->addScript(particleScript);
		particleScript->meshRenderer = meshRenderer;
		particleScript->transform = transform;
		SceneManager::scene()->addObject(particle);
	}
}