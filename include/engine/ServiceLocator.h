#pragma once
#include <unordered_map>
#include <typeindex>
#include <any>

class ServiceLocator
{
public:
	ServiceLocator();

	template<typename T>
	static T* getService();

	template<typename T>
	static void provide(T* service);
private:
	static std::unordered_map<std::type_index, std::any> services;
};

template<typename T>
T* ServiceLocator::getService()
{
	auto it = services.find(typeid(T));
	if (it == services.cend()) {
		return nullptr;
	}
	return std::any_cast<T*>(it->second);
}

template<typename T>
void ServiceLocator::provide(T* service)
{
	auto result = services.emplace(typeid(T), nullptr);
	auto it = result.first;
	it->second = service;
}