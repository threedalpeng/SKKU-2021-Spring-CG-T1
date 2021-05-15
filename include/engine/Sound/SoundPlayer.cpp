#include "SoundPlayer.h"
#include "engine/Transform/Transform.h"

SoundPlayer::SoundPlayer(std::shared_ptr<GameObject> obj) : Component(obj) {}

SoundPlayer::~SoundPlayer()
{
	clear();
}

void SoundPlayer::loadSoundFrom(std::string soundPath) {
	_soundSource = _engine->addSoundSourceFromFile(soundPath.c_str());
}

void SoundPlayer::setType(Type type) {
	_type = type;
}

void SoundPlayer::play() {
	if (!_soundSource) {
		return;
	}
	if (_sound) {
		_sound->setIsPaused(false);
		return;
	}

	switch (_type) {
	case Type::Background:
		_sound = _engine->play2D(_soundSource, true, false, true);
		break;
	case Type::Point:
		Transform* transform = getComponent<Transform>();
		irrklang::vec3df soundPosition = reinterpret_cast<irrklang::vec3df&>(transform->worldPosition);
		_sound = _engine->play3D(_soundSource, soundPosition, true, false, true);
		break;
	}
}

void SoundPlayer::pause() {
	if (_sound) {
		_sound->setIsPaused();
	}
}

void SoundPlayer::stop() {
	if (_sound) {
		_sound->stop();
		_sound->drop();
		_sound = nullptr;
	}
}

void SoundPlayer::setVolume(float volume) {
	if (_sound) {
		_sound->setVolume(volume);
	}
	if (_soundSource) {
		_sound->setVolume(volume);
	}
}

void SoundPlayer::update() {
	if (_type == Type::Point && _sound) {
		Transform* transform = getComponent<Transform>();
		_sound->setPosition(reinterpret_cast<irrklang::vec3df&>(transform->worldPosition));
		//_sound->setVelocity();
	}
}

void SoundPlayer::clear() {
	stop();
	if (_soundSource) {
		_soundSource = nullptr;
	}
}

void SoundPlayer::terminate() {
	if (_engine) {
		_engine->drop();
	}
}

irrklang::ISoundEngine* SoundPlayer::_engine = irrklang::createIrrKlangDevice();