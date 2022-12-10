#pragma once

#if !defined( WIN32_LEAN_AND_MEAN )
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#if !defined( NOMINMAX )
#define NOMINMAX
#endif // !NOMINMAX

#include <windows.h>
#include <winternl.h>

#include <intrin.h>

#pragma warning( push )
#pragma warning( disable : 4005 )
#include <ntstatus.h>
#pragma warning( pop )

#include <d3d9.h>