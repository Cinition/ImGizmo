/*

    // [SECTION] Forward declarations, flags and enums
    // [SECTION] Context
    // [SECTION] Begin/End Space
    
    // [SECTION] ImGizmoPoint3D
    // [SECTION] ImGizmoMatrix
    // [SECTION] ImGizmoBox3D

*/

#pragma once

#include <imgui.h>

#ifndef IMGIZMO_API
#define IMGIZMO_API
#endif

//-----------------------------------------------------------------------------
// [SECTION] Forward declarations, flags and enums
//-----------------------------------------------------------------------------

struct ImGizmoContext;
struct ImGizmoSpace;
struct ImGizmoVec3;
struct ImGizmoMatrix33;
struct ImGizmoMatrix44;
struct ImGizmoFrustum;

struct ImRect;

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

    // Starts a 3D Context 'Space' everything in the ImGizmo API should be used between Begin() and End(), 
    // as they depend on the space context created inbetween these function for rendering of the 3D.
    // If this function returns true, End() MUST be called!
    //
    // Important notes:
    // - handles the same as an ImGui, Begin(). Meaning that Window Manipulation functions() work here aswell.
    // - we calculate all manipulation and rendering in the End() call.
    bool Begin(const char* id, const float* viewMatrix, const float* projectionMatrix);
    void End();

    void PushPosition(const float x, const float y, const float z);

    void Translate(float* x, float* y, float* z);
    void Rotate(float eulerX, float eulerY, float eulerZ);
    void Scale(float x, float y, float z);
}

//-----------------------------------------------------------------------------
// [SECTION] ImGizmoPoint3D
//-----------------------------------------------------------------------------

struct ImGizmoVec3 {
    float x, y, z;

    ImGizmoVec3() : x(0.f), y(0.f), z(0.f) {}
    ImGizmoVec3(float _x,float _y,float _z) : x(_x), y(_y), z(_z) {}

    float& operator[](size_t index) {
        return ((float*)(void*)(char*)this)[index];
    }

    const float& operator[](size_t index) const {
        return ((const float*)(const void*)(const char*)this)[index];
    }

    IMGIZMO_API ImGizmoVec3 operator*(float rhs) const;
    IMGIZMO_API ImGizmoVec3 operator/(float rhs) const;
    IMGIZMO_API ImGizmoVec3 operator*(const ImGizmoVec3& rhs) const;
    IMGIZMO_API ImGizmoVec3 operator/(const ImGizmoVec3& rhs) const;
    IMGIZMO_API ImGizmoVec3 operator+(const ImGizmoVec3& rhs) const;
    IMGIZMO_API ImGizmoVec3 operator-(const ImGizmoVec3& rhs) const;

    IMGIZMO_API ImGizmoVec3 operator-() const;

    IMGIZMO_API ImGizmoVec3 operator*(float rhs);
    IMGIZMO_API ImGizmoVec3 operator/(float rhs);
    IMGIZMO_API ImGizmoVec3 operator*=(const ImGizmoVec3& rhs);
    IMGIZMO_API ImGizmoVec3 operator/=(const ImGizmoVec3& rhs);
    IMGIZMO_API ImGizmoVec3 operator+=(const ImGizmoVec3& rhs);
    IMGIZMO_API ImGizmoVec3 operator-=(const ImGizmoVec3& rhs);

    IMGIZMO_API ImGizmoVec3 operator==(const ImGizmoVec3& rhs) const;

    IMGIZMO_API float Dot(const ImGizmoVec3& rhs) const;
    IMGIZMO_API ImGizmoVec3 Cross(const ImGizmoVec3& rhs) const;
    IMGIZMO_API void Normalize() const;
    IMGIZMO_API ImGizmoVec3 Normalized() const;
};

//-----------------------------------------------------------------------------
// [SECTION] ImGizmoMatrix
//-----------------------------------------------------------------------------

struct ImGizmoMatrix {
    // Row major
    // [ 11 12 13 14 ] 0 => 3
    // [ 21 22 23 24 ] 4 => 7
    // [ 31 32 33 34 ] 8 => 11
    // [ 41 42 43 44 ] 12 => 15

    float m[16];

    ImGizmoMatrix() {
        m[0] = 1.f;
        m[1] = 0.f;
        m[2] = 0.f;
        m[3] = 0.f;
        m[4] = 0.f;
        m[5] = 1.f;
        m[6] = 0.f;
        m[7] = 0.f;
        m[8] = 0.f;
        m[9] = 0.f;
        m[10] = 1.f;
        m[11] = 0.f;
        m[12] = 0.f;
        m[13] = 0.f;
        m[14] = 0.f;
        m[15] = 1.f;
    }

    ImGizmoMatrix(const float* rhs) {
        m[0] = rhs[0];
        m[1] = rhs[1];
        m[2] = rhs[2];
        m[3] = rhs[3];
        m[4] = rhs[4];
        m[5] = rhs[5];
        m[6] = rhs[6];
        m[7] = rhs[7];
        m[8] = rhs[8];
        m[9] = rhs[9];
        m[10] = rhs[10];
        m[11] = rhs[11];
        m[12] = rhs[12];
        m[13] = rhs[13];
        m[14] = rhs[14];
        m[15] = rhs[15];
    }

    IMGIZMO_API ImGizmoVec3 operator*(const ImGizmoVec3& rhs) const;

    IMGIZMO_API ImGizmoVec3 Transform(const ImGizmoVec3& rhs, float w) const;
    IMGIZMO_API ImGizmoMatrix Invert() const;
};

//-----------------------------------------------------------------------------
// [SECTION] ImGizmoBox3D
//-----------------------------------------------------------------------------

struct ImGizmoFrustum {
    ImGizmoVec3 min;
    ImGizmoVec3 max;
};
