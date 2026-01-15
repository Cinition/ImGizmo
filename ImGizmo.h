#pragma once

#include "imgui.h"

#ifndef IMGIZMO_API
#define IMGIZMO_API
#endif

//-----------------------------------------------------------------------------
// [SECTION] Forward declarations, flags and enums
//-----------------------------------------------------------------------------

struct ImGizmoContext;

typedef int ImGizmoAxis; //  -> enum ImGizmoAxis_ // Flags: for redefining axis to your engine

enum ImGizmoAxis_ {
    ImGizmoAxis_Forward = 0,  // Default
    ImGizmoAxis_Backward = 1, // Default
    ImGizmoAxis_Up = 2,       // Default
    ImGizmoAxis_Down = 3,     // Default
    ImGizmoAxis_Left = 4,     // Default
    ImGizmoAxis_Right = 5,    // Default
};

namespace ImGizmo {

//-----------------------------------------------------------------------------
// [SECTION] Context
//-----------------------------------------------------------------------------

    IMGIZMO_API ImGizmoContext* CreateContext();
    IMGIZMO_API void DestroyContext(ImGizmoContext* context = nullptr);
    IMGIZMO_API ImGizmoContext* GetCurrentContext();
    IMGIZMO_API void SetCurrentContext(ImGizmoContext* context);

//-----------------------------------------------------------------------------
// [SECTION] Begin/End Space
//-----------------------------------------------------------------------------

    bool Begin(const char* id, const float* viewMatrix, const float* projectionMatrix);
    void End();

//-----------------------------------------------------------------------------
// [SECTION] Gizmo variable functions
//-----------------------------------------------------------------------------

    void PushPositionOffset(const float x, const float y, const float z);
    void PopPositionOffset();

    void PushEulerRotationOffset(const float x, const float y, const float z);
    void PushQuatRotationOffset(const float x, const float y, const float z, const float w);
    void PopRotationOffset();

//-----------------------------------------------------------------------------
// [SECTION] Gizmo Functions
//-----------------------------------------------------------------------------

    void GizmoPoint(const char* id, const float* x, const float* y, const float* z);
    void GizmoTranslate(const char* id, float* x, float* y, float* z);
    void GizmoRotateEuler(const char* id, float* x, float* y, float* z);
    void GizmoRotateQuat(const char* id, float* x, float* y, float* z, float* w);
    void GizmoScale(const char* id, float* x, float* y, float* z);
}
