#pragma once

#define API_CDECL																			__cdecl
#define API_STDCALL																		__stdcall
#define API_THISCALL																	__thiscall
#define API_FASTCALL																	__fastcall
#define API_VECTORCALL																__vectorcall

#define JOIN_IMPL( A, B )															A ## B
#define JOIN( A, B )																	JOIN_IMPL( A, B )

#define SEH2_TRY()																		__try
#define SEH2_EXCEPT( Expression )											__except( Expression )

using int8 = signed __int8;
using int16 = signed __int16;
using int32 = signed __int32;
using int64 = signed __int64;

using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

#pragma warning( disable : 6001 )
#pragma warning( disable : 26812 )
#pragma warning( disable : 26454 )