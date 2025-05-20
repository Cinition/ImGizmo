#pragma once

#include <imgui.h>

#ifndef IMGIZMO_NAMESPACE
#define IMGIZMO_NAMESPACE ImGizmo
#endif

struct ImGizmoMatrix;

struct ImGizmoVec3 {
    float x;
    float y;
    float z;

    ImGizmoVec3() {
        x = 0.f;
        y = 0.f;
        z = 0.f;
    }

    ImGizmoVec3(float _x, float _y, float _z) {
        x = _x;
        y = _y;
        z = _z;
    }

    ImGizmoVec3 Transform(const ImGizmoMatrix& _matrix);
};

struct ImGizmoVec4 {
    float x;
    float y;
    float z;
    float w;

    ImGizmoVec4() {
        x = 0.f;
        y = 0.f;
        z = 0.f;
        w = 0.f;
    }

    ImGizmoVec4(float _x, float _y, float _z, float _w) {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    ImGizmoVec4 Transform(const ImGizmoMatrix& _matrix);
};

struct ImGizmoMatrix {
    union {
        float m4x4[4][4];
        float m16[16];
    };

    ImGizmoMatrix() {
        m4x4[0][0] = m4x4[0][1] = m4x4[0][2] = m4x4[0][3] =
        m4x4[1][0] = m4x4[1][1] = m4x4[1][2] = m4x4[1][3] =
        m4x4[2][0] = m4x4[2][1] = m4x4[2][2] = m4x4[2][3] =
        m4x4[3][0] = m4x4[3][1] = m4x4[3][2] = m4x4[3][3] = 0.f;
    };

    ImGizmoMatrix(float* _matrix) {
        m16[0] = _matrix[0];
        m16[1] = _matrix[1];
        m16[2] = _matrix[2];
        m16[3] = _matrix[3];
        m16[4] = _matrix[4];
        m16[5] = _matrix[5];
        m16[6] = _matrix[6];
        m16[7] = _matrix[7];
        m16[8] = _matrix[8];
        m16[9] = _matrix[9];
        m16[10] = _matrix[10];
        m16[11] = _matrix[11];
        m16[12] = _matrix[12];
        m16[13] = _matrix[13];
        m16[14] = _matrix[14];
        m16[15] = _matrix[15];
    }

    static ImGizmoMatrix Indentity() {
        ImGizmoMatrix m;

        m.m4x4[0][1] = m.m4x4[0][2] = m.m4x4[0][3] =
            m.m4x4[1][0] = m.m4x4[1][2] = m.m4x4[1][3] =
            m.m4x4[2][0] = m.m4x4[2][1] = m.m4x4[2][3] =
            m.m4x4[3][0] = m.m4x4[3][1] = m.m4x4[3][2] = 0.f;

        m.m4x4[0][0] = m.m4x4[1][1] = m.m4x4[2][2] = m.m4x4[3][3] = 1.f;

        return m;
    };

    ImGizmoVec3 Pos() const;
    ImGizmoMatrix operator* (const ImGizmoMatrix& _matrix) const;
};

struct ImGizmoContext;

namespace IMGIZMO_NAMESPACE {
    ImGizmoContext* CreateContext();
    void DestroyContext(ImGizmoContext* context = nullptr);
    ImGizmoContext* GetCurrentContext();
    void SetCurrentContext(ImGizmoContext* context);

    bool Begin(const char* _id, float* _view, float* _proj, const ImVec2& _size = ImVec2(0,0));
    void End();

    void AttachNextSpaceToViewport(ImGuiViewport* _viewport);

    bool DrawPoint(const char* _id, ImGizmoVec3 point);
    bool DrawLine(const char* _id, ImGizmoVec3 point1, ImGizmoVec3 point2);
}
