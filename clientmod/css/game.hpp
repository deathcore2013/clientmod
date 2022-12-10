#pragma once

#include "../source/sdk.hpp"

#include "../source/client/base_entity.hpp"
#include "../source/client/player.hpp"

#include "../configuration/storage.hpp"

namespace css
{

extern std::atomic< bool > m_execute;

extern std::uintptr_t m_image_process;
extern std::uintptr_t m_image_client;
extern std::uintptr_t m_image_engine;
extern std::uintptr_t m_image_shaderapidx9;

extern CHLClient* m_client;
extern CClientEntityList* m_client_entity_list;
extern CEngineClient* m_engine_client;
extern CEngineTraceClient* m_engine_trace_client;
extern CRender* m_render;

extern IDirect3DDevice9* m_device;

extern PresentFn m_present;
extern ResetFn m_reset;

extern WNDPROC m_process_message;

bool Create();
void Destroy();

bool WorldToScreen( const Vector& point, ImVec2& screen );

double GetCurtime();

} // namespace css