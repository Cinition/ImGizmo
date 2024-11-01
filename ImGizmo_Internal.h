#pragma once

#include "imgui_internal.h"


typedef struct {
    float m[4][4];
} ImGizmoMatrix;

ImGizmoMatrix convertToMatrix(float* _matrix) {
    ImGizmoMatrix matrix;

    matrix.m[0][0] = _matrix[0];
    matrix.m[0][1] = _matrix[1];
    matrix.m[0][2] = _matrix[2];
    matrix.m[0][3] = _matrix[3];
    matrix.m[1][0] = _matrix[4];
    matrix.m[1][1] = _matrix[5];
    matrix.m[1][2] = _matrix[6];
    matrix.m[1][3] = _matrix[7];
    matrix.m[2][0] = _matrix[8];
    matrix.m[2][1] = _matrix[9];
    matrix.m[2][2] = _matrix[10];
    matrix.m[2][3] = _matrix[11];
    matrix.m[3][0] = _matrix[12];
    matrix.m[3][1] = _matrix[13];
    matrix.m[3][2] = _matrix[14];
    matrix.m[3][3] = _matrix[15];
    
    return matrix;
}

struct ImGizmoSpace {
    ImGuiID ID;
    ImRect frameRect;
    ImGizmoMatrix viewMatrix;
    ImGizmoMatrix projMatrix;
};

struct ImGizmoContext {
    ImPool<ImGizmoSpace> gizmoSpaces;
    ImGizmoSpace* currentGizmoSpace;
};
