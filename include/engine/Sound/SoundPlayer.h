#pragma once

#include <iostream>

#include "irrKlang/irrKlang.h"
#pragma comment(lib, "irrKlang.lib")

#include "engine/Component/Component.h"

/* Container Wrapper for IrrKlang Sound Engine*/
class SoundPlayer : public Component {
public:
	enum class Type {
		Background,
		Point,
		Event2D
	};

	SoundPlayer(std::shared_ptr<GameObject> obj);
	~SoundPlayer();

	void loadSoundFrom(std::string soundPath);
	void setType(Type type);

	void play();
	void pause();
	void stop();

	void setVolume(float volume);

	void update();

	void clear();
	static void terminate();
private:
	irrklang::ISound* _sound = nullptr;
	irrklang::ISoundSource* _soundSource = nullptr;
	Type _type = Type::Background;

	static irrklang::ISoundEngine* _engine;
};
