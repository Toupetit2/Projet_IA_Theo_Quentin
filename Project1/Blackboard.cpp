#include "Blackboard.hpp"

void Blackboard::SetValue(const std::string& key, int value)
{
	data[key] = value;
}

int Blackboard::GetValue(const std::string& key)
{
	return data[key];
}