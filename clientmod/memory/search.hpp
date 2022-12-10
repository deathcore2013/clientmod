#pragma once

#include "../include/auto.hpp"
#include "../include/base.hpp"
#include "../include/win32.hpp"

namespace atom::memory
{

std::size_t Search( std::uintptr_t begin, std::uintptr_t end, const char* signature, std::vector< std::uintptr_t >& output );
std::size_t SearchSection( std::uintptr_t image_base, const char* name, const char* signature, std::vector< std::uintptr_t >& output );

} // namespace atom::memory