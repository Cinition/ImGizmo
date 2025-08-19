#pragma once

#include <imgui.h>

struct ImVec3 {
    float x, y, z;
    constexpr ImVec3()                          : x(0.f), y(0.f), z(0.f) {}
    constexpr ImVec3(float x, float y, float z) : x(x), y(y), z(z) {}
    inline ImVec2 xy() const { return ImVec2(x, y); }
#ifdef IM_VEC3_CLASS_EXTRA
    IM_VEC3_CLASS_EXTRA     // Define additional constructors and implicit cast operators in imgizmoconfig.h to convert back and forth between your math types and ImVec3.
#endif
};

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
    bool Begin(const char* id, float* viewMatrix, float* projectionMatrix, const ImVec3& cameraPosition);
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
        float radius,
        ImU32 color = 0xFFFFFFFF,
        float borderThickness = 1.f,
        ImU32 borderColor = 0x000000FF,
        ImU32 flags = 0
    );
    bool DrawLine(
        const char* id,
        const ImVec3& point1,
        const ImVec3& point2,
        ImU32 color = 0xFFFFFFFF,
        float borderThickness = 1.f,
        ImU32 borderColor = 0x000000FF,
        ImU32 flags = 0
    );
    bool DrawTriangle(
        const char* id,
        const ImVec3& point1,
        const ImVec3& point2,
        const ImVec3& point3,
        ImU32 color = 0xFFFFFFFF,
        float borderThickness = 1.f,
        ImU32 borderColor = 0x000000FF,
        ImU32 flags = 0
    );
    bool DrawQuad(
        const char* id,
        const ImVec3& point1,
        const ImVec3& point2,
        const ImVec3& point3,
        const ImVec3& point4,
        ImU32 color = 0xffffffff,
        float borderThickness = 1.f,
        ImU32 borderColor = 0x000000ff,
        ImU32 flags = 0
    );

    // Predefined Gizmos
    bool Translate(
        const char* id,
        ImVec3* value,
        const ImVec3* position = NULL,
        const ImVec3* rotation = NULL
    );
    bool Rotate(
        const char* id,
        ImVec3* value,
        const ImVec3* position = NULL,
        const ImVec3* rotation = NULL
    );
    bool Scale(
        const char* id,
        ImVec3* value,
        const ImVec3* position = NULL,
        const ImVec3* rotation = NULL
    );
}
