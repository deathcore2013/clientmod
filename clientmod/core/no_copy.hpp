#pragma once

#include "../include/auto.hpp"
#include "../include/base.hpp"
#include "../include/win32.hpp"

namespace atom::core
{

class NoCopy
{
public:
	NoCopy() = default;

protected:
	NoCopy( const NoCopy& ) = delete;

protected:
	NoCopy& operator=( const NoCopy& ) = delete;
};

} // namespace atom::core