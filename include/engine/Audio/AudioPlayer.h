#pragma once
#include "engine/Component/Component.h"

class AudioPlayer : public Component {
public:
	AudioPlayer(std::shared_ptr<GameObject> obj) : Component(obj) {};

private:
	std::string
};
