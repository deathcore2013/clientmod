#pragma once

#include "../include/auto.hpp"
#include "../include/base.hpp"
#include "../include/win32.hpp"

#include "no_copy.hpp"
#include "no_move.hpp"

namespace atom::core
{

template< typename InstanceType >
class Singleton : public NoCopy, public NoMove
{
public:
	static InstanceType& Instance()
	{
		static InstanceType instance = { };
		return instance;
	}
};

} // namespace atom::core