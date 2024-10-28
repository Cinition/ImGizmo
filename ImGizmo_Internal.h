#pragma once

#include "imgui_internal.h"

typedef union {
    float* ptr;
    float matrix[4][4];
} ImGizmoMatrix;

struct ImGizmoSpace {
    ImGizmoMatrix viewMatrix;
    ImGizmoMatrix projMatrix;
};

struct ImGizmoContext {
    ImPool<ImGizmoSpace> gizmoSpaces;
    ImGizmoSpace* currentGizmoSpace;
};
