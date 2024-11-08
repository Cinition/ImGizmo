#pragma once

#include <imgui.h>

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

    bool DrawPosition(float* _matrix);
    bool DrawPosition(float* _x, float* _y, float* _z);
    bool DrawRotation(float* _matrix);
    bool DrawRotation(float* _x, float* _y, float* _z);
    bool DrawScaling(float* _matrix);
    bool DrawScaling(float* _x, float* _y, float* _z);
}
