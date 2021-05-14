#include "ServiceLocator.h"

ServiceLocator::ServiceLocator()
{
}

std::unordered_map<std::type_index, std::any> ServiceLocator::services = std::unordered_map<std::type_index, std::any>();