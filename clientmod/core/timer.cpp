#include "timer.hpp"

namespace atom::core
{

Timer::Timer( bool reset /*= false*/ )
	: m_frequency()
	, m_begin()
	, m_end()
{
	QueryPerformanceFrequency( &m_frequency );

	if( reset )
	{
		Reset();
	}
}

void Timer::Reset()
{
	QueryPerformanceCounter( &m_begin );
}

std::int64_t Timer::Elapsed()
{
	QueryPerformanceCounter( &m_end );

	const auto count = ( m_end.QuadPart - m_begin.QuadPart );

	if( count > 0 )
	{
		return ( ( count * 1000LL ) / m_frequency.QuadPart );
	}

	return 0;
}

} // namespace atom::core