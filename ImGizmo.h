#pragma once

#include <imgui.h>
#include <cstdint>

#ifndef IMGIZMO_NAMESPACE
#define IMGIZMO_NAMESPACE ImGizmo
#endif

struct ImGizmoContext;

namespace IMGIZMO_NAMESPACE {
    ImGizmoContext* CreateContext();
    void DestroyContext(ImGizmoContext* context = nullptr);
    ImGizmoContext* GetCurrentContext();
    void SetCurrentContext(ImGizmoContext* context);

    bool Begin(const char* _id, float* _view, float* _proj, const ImVec2& _size = ImVec2(0,0));
    void End();
}
