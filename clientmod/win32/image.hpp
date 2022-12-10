#pragma once

#include "../include/auto.hpp"
#include "../include/base.hpp"
#include "../include/win32.hpp"

namespace atom::win32
{

std::uintptr_t GetImageBase( const wchar_t* name );

IMAGE_DOS_HEADER* GetImageDosHeader( std::uintptr_t image_base );
IMAGE_NT_HEADERS* GetImageNtHeaders( std::uintptr_t image_base );

IMAGE_SECTION_HEADER* GetImageSectionHeader( std::uintptr_t image_base, const char* name );

std::uintptr_t GetImageExport( std::uintptr_t image_base, const char* name );

} // namespace atom::win32