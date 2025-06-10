#pragma once

/*

Index of this file:
// [SECTION]

*/

// Configuration file with compile-time options
// (edit imgizmoconfig.h or '#define IMGIZMO_USER_CONFIG "myfilename.h" from your build system)
#ifdef IMGIZMO_USER_CONFIG
#include IMGIZMO_USER_CONFIG
#endif
#include "imgizmoconfig.h"

#include <imgui.h>

struct ImVec3 {
    float x, y, z;
    constexpr ImVec3()                             : x(0.f), y(0.f), z(0.f) {}
    constexpr ImVec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
#ifdef IM_VEC3_CLASS_EXTRA
    IM_VEC3_CLASS_EXTRA     // Define additional constructors and implicit cast operators in imgizmoconfig.h to convert back and forth between your math types and ImVec3.
#endif
};

struct ImMat44 {
    union {
        float m4x4[4][4];
        float m16[16];
    };

    ImMat44() {
        m4x4[0][0] = m4x4[0][1] = m4x4[0][2] = m4x4[0][3] =
        m4x4[1][0] = m4x4[1][1] = m4x4[1][2] = m4x4[1][3] =
        m4x4[2][0] = m4x4[2][1] = m4x4[2][2] = m4x4[2][3] =
        m4x4[3][0] = m4x4[3][1] = m4x4[3][2] = m4x4[3][3] = 0.f;
    };
#ifdef IM_MAT44_CLASS_EXTRA
    IM_MAT44_CLASS_EXTRA     // Define additional constructors and implicit cast operators in imgizmoconfig.h to convert back and forth between your matrices and ImMat44.
#endif
};

struct ImGizmoContext;

namespace ImGizmo {

//-----------------------------------------------------------------------------
// [SECTION] Context functions
//-----------------------------------------------------------------------------

    ImGizmoContext* CreateContext();
    void DestroyContext(ImGizmoContext* context = nullptr);
    ImGizmoContext* GetCurrentContext();
    void SetCurrentContext(ImGizmoContext* context);

//-----------------------------------------------------------------------------
// [SECTION] ImGizmo base functions
//-----------------------------------------------------------------------------

    bool Begin(const char* _id, ImMat44 _view, ImMat44 _proj, const ImVec2& _size = ImVec2(0,0));
    void End();
    bool IsOver();
    bool IsUsing();
    ImVec2 GetActivePos();
    ImVec2 ConvertTo2DCoords(const ImVec3& pos);

//-----------------------------------------------------------------------------
// [SECTION] Drawing API
//-----------------------------------------------------------------------------

    bool DrawPoint(const char* _id, const ImVec3& _point, float _radius, ImU32 color = ImGui::GetColorU32({1.f, 1.f, 1.f, 1.f}));
    bool DrawLine(const char* _id, const ImVec3& _point1, const ImVec3& _point2, ImU32 color = ImGui::GetColorU32({1.f, 1.f, 1.f, 1.f}));
    bool DrawTriangle(const char* _id, const ImVec3& _point1, const ImVec3& _point2, const ImVec3& _point3, ImU32 color = ImGui::GetColorU32({1.f, 1.f, 1.f, 1.f}));
    bool DrawSquare(const char* _id, const ImVec3& _point1, const ImVec3& _point2, const ImVec3& _point3, const ImVec3& _point5, ImU32 color = ImGui::GetColorU32({1.f, 1.f, 1.f, 1.f}));

//-----------------------------------------------------------------------------
// [SECTION] 3D Space helper function
//-----------------------------------------------------------------------------

    void SetNextItemRotation(ImVec3* _left, ImVec3* _up, ImVec3* _at);
    void SetNextItemRotation(ImVec3* _euler);
    void SetNextItemRotation(ImVec4* _quat);

//-----------------------------------------------------------------------------
// [SECTION] Predefined Gizmos
//-----------------------------------------------------------------------------

    bool Translate(const char* _id, ImVec3* _position);
}

// Helpers: ImVec3 operators
// - These helper operators are implemented in the same way Dear ImGui implements their Vector operators.
// - Add '#define IMGUI_DEFINE_MATH_OPERATORS' before including this file (or in imconfig.h) to access courtesy maths operators for ImVec2/ImVec3/ImVec4.
#ifdef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS_IMPLEMENTED
IM_MSVC_RUNTIME_CHECKS_OFF
static inline ImVec3  operator*(const ImVec3& lhs, const float rhs)     { return ImVec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs); }
static inline ImVec3  operator/(const ImVec3& lhs, const float rhs)     { return ImVec3(lhs.x / rhs, lhs.y / rhs, lhs.y / rhs); }
static inline ImVec3  operator+(const ImVec3& lhs, const ImVec3& rhs)   { return ImVec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
static inline ImVec3  operator-(const ImVec3& lhs, const ImVec3& rhs)   { return ImVec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
static inline ImVec3  operator*(const ImVec3& lhs, const ImVec3& rhs)   { return ImVec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z); }
static inline ImVec3  operator/(const ImVec3& lhs, const ImVec3& rhs)   { return ImVec3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z); }
static inline ImVec3  operator-(const ImVec3& lhs)                      { return ImVec3(-lhs.x, -lhs.y, -lhs.z); }
static inline ImVec3& operator*=(ImVec3& lhs, const float rhs)          { lhs.x *= rhs; lhs.y *= rhs; lhs.z *= rhs; return lhs; }
static inline ImVec3& operator/=(ImVec3& lhs, const float rhs)          { lhs.x /= rhs; lhs.y /= rhs; lhs.z /= rhs; return lhs; }
static inline ImVec3& operator+=(ImVec3& lhs, const ImVec3& rhs)        { lhs.x += rhs.x; lhs.y += rhs.y; lhs.z += rhs.z;return lhs; }
static inline ImVec3& operator-=(ImVec3& lhs, const ImVec3& rhs)        { lhs.x -= rhs.x; lhs.y -= rhs.y; lhs.z -= rhs.z;return lhs; }
static inline ImVec3& operator*=(ImVec3& lhs, const ImVec3& rhs)        { lhs.x *= rhs.x; lhs.y *= rhs.y; lhs.z *= rhs.z;return lhs; }
static inline ImVec3& operator/=(ImVec3& lhs, const ImVec3& rhs)        { lhs.x /= rhs.x; lhs.y /= rhs.y; lhs.z /= rhs.z;return lhs; }
static inline bool    operator==(const ImVec3& lhs, const ImVec3& rhs)  { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
static inline bool    operator!=(const ImVec3& lhs, const ImVec3& rhs)  { return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z; }
IM_MSVC_RUNTIME_CHECKS_RESTORE
#endif
