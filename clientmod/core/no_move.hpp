#pragma once

#include "../include/auto.hpp"
#include "../include/base.hpp"
#include "../include/win32.hpp"

namespace atom::core
{

class NoMove
{
public:
	NoMove() = default;

protected:
	NoMove( NoMove&& ) = delete;

protected:
	NoMove& operator=( NoMove&& ) = delete;
};

} // namespace atom::core