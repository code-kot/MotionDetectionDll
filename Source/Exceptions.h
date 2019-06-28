#pragma once
#include <stdexcept>

class NotImplementedException final : public std::logic_error
{
public:
	NotImplementedException() : std::logic_error("Not implemented exception") { };
};
