#pragma once
#include <functional>
#include <unordered_map>
#include <string>

struct GuiEvent
{
	int guiId;
};

struct HpChangedEvent
{
	int hp;
};

template <typename T>
class EventManager
{
public:
	static unsigned addListener(std::function<bool(const T&)> listener)
	{
		auto id = getNextId();
		getHandlers()[id] = listener;
		return id;
	}

	static void removeListener(unsigned handle)
	{
		getHandlers().erase(handle);
	}

	static void tringgerEvent(T e)
	{
		for (const auto& h : getHandlers())
		{
			h.second(e);
		}
	}

private:
	static unsigned getNextId()
	{
		static unsigned id = 0;
		return ++id;
	}

	static std::unordered_map<unsigned, std::function<bool(const T&)>>& getHandlers()
	{
		static std::unordered_map<unsigned, std::function<bool(const T&)>> handlers;
		return handlers;
	}
};