#pragma once

#include "imgui_internal.h"
#include "ImGizmo_Math.h"

struct ImGizmoSpace {
    ImGuiID ID;
    ImGizmoMatrix viewMatrix;
    ImGizmoMatrix projMatrix;
    ImDrawList* drawList;
    ImRect frameRect;

    bool initialized;
};

struct ImGizmoContext {
    ImGizmoSpace currentSpace;
    // TODO: add global styling options, like in imgui
};
