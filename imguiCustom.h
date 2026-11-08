#pragma once

#include "imgui.h"

namespace ImGui
{
    bool DragDouble(
        const char* label,
        double* value,
        float speed = 1.0f,
        double min = 0.0,
        double max = 0.0,
        const char* format = "%.3f",
        ImGuiSliderFlags flags = 0,
        double step = 0.0,
        double step_fast = 0.0
    );
}
