#pragma once

#include <imgui.h>

struct ImVec3;
struct ImMat44;
struct ImGizmoContext;

namespace ImGizmo {

    // Context functions
    ImGizmoContext* CreateContext();
    void DestroyContext(ImGizmoContext* context = nullptr);
    ImGizmoContext* GetCurrentContext();
    void SetCurrentContext(ImGizmoContext* context);

    // Spaces
    // Begin() = pushes a new Space/Window to context. Call End() to close the current Space/Window.
    // - handles the same as an ImGui, Begin(). Meaning that Window Manipulation functions() work here aswell.
    // - we calculate all manipulation and rendering in the End() call.
    bool Begin(const char* id, const ImMat44& view, const ImMat44& proj, const ImVec3& cameraPosition);
    void End();

    // Space utilities
    bool IsOver();
    bool IsUsing();
    const char* GetHoveredID();
    const char* GetActiveID();
    ImVec3 GetHoveredPos();
    ImVec2 GetUsingStartPos();
    ImVec2 GetLastMousePos();

    // Space conversions
    ImVec2 Convert3DTo2D(const ImVec3& pos);
    ImVec3 Convert2DTo3D(const ImVec3& pos);

    // Drawing Primitives
    bool DrawPoint(
        const char* id,
        const ImVec3& point,
        float radius, float border,
        ImU32 flags = 0, ImU32 color = 0xFFFFFFFF,
        ImU32 borderColor = 0x000000FF
    );
    bool DrawLine(
        const char* id,
        const ImVec3& point1,
        const ImVec3& point2,
        float border,
        ImU32 flags = 0,
        ImU32 color = 0xFFFFFFFF,
        ImU32 borderColor = 0x000000FF
    );
    bool DrawTriangle(
        const char* id,
        const ImVec3& point1,
        const ImVec3& point2,
        const ImVec3& point3,
        float border,
        ImU32 flags = 0,
        ImU32 color = 0xFFFFFFFF,
        ImU32 borderColor = 0x000000FF
    );
    bool DrawQuad(
        const char* id,
        const ImVec3& point1,
        const ImVec3& point2,
        const ImVec3& point3,
        const ImVec3& point4,
        float border,
        ImU32 flags = 0,
        ImU32 color = 0xFFFFFFFF,
        ImU32 borderColor = 0x000000F
    );

    // Predefined Gizmos
    bool Translate(
        const char* id,
        ImVec3* value,
        ImVec3* position = nullptr,
        ImVec3* rotation = nullptr
    );
    bool Rotate(
        const char* id,
        ImVec3* value,
        ImVec3* position = nullptr,
        ImVec3* rotation = nullptr
    );
    bool Scale(
        const char* id,
        ImVec3* value,
        ImVec3* position = nullptr,
        ImVec3* rotation = nullptr
    );
}
