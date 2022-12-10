#pragma once

#include "../include/auto.hpp"
#include "../include/base.hpp"
#include "../include/win32.hpp"

#include "no_copy.hpp"
#include "no_move.hpp"

namespace atom::core
{

class Timer : public NoCopy, public NoMove
{
public:
	Timer( bool reset = false );

public:
	void Reset();
	std::int64_t Elapsed();

protected:
	LARGE_INTEGER m_frequency = { };
	LARGE_INTEGER m_begin = { };
	LARGE_INTEGER m_end = { };
};

} // namespace atom::core