#pragma once

#include <queue>
#include "imgui_internal.h"

struct ImGizmoMatrix {
    union {
        float m4x4[4][4];
        float m16[16];
    };

    ImGizmoMatrix() {
        // setup identity matrix
        m4x4[0][1] = m4x4[0][2] = m4x4[0][3] =
        m4x4[1][0] = m4x4[1][2] = m4x4[1][3] =
        m4x4[2][0] = m4x4[2][1] = m4x4[2][3] =
        m4x4[3][0] = m4x4[3][1] = m4x4[3][2] = 0.f;

        m4x4[0][0] = m4x4[1][1] = m4x4[2][2] = m4x4[3][3] = 1.f;
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
};

struct ImGizmoSpace {
    ImGuiID ID;
    ImRect frameRect;
    ImGizmoMatrix viewMatrix;
    ImGizmoMatrix projMatrix;
};

struct ImGizmoContext {
    std::queue<ImGizmoSpace> gizmoSpaces;
    // add context wide styling variables (like imgui)
};
