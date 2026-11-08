#include "imguiCustom.h"
#include <algorithm>
#include "imgui.h"
#include "imgui_internal.h"

namespace ImGui
{
    /* pour le label ImGui fait :
    "Vitesse" : label visible et ID basé sur le label.
    "Vitesse##speed" : label visible Vitesse, ID basé sur Vitesse##speed.
    "##speed" : aucun label visible, ID basé sur ##speed.
    "Vitesse###speed" : label visible Vitesse, ID basé uniquement sur speed.
    */
    bool DragDouble(
        const char* label,
        double* value,
        float speed,
        double min,
        double max,
        const char* format,
        ImGuiSliderFlags flags,
        double step,
        double step_fast
    )
    {
        if (label == nullptr || value == nullptr)
            return false;

        // ============================================
        // Configuration
        // ============================================

        const bool useClamp = min != max;
        const bool useStep = step > 0.0;
        const bool useFastStep = step_fast > 0.0;

        if (useClamp && min > max)
        {
            IM_ASSERT(min <= max);
            return false;
        }

        bool valueChanged = false;

        // ============================================
        // Clamp
        // ============================================

        auto ClampValue = [&](double& v)
        {
            if (useClamp)
                v = std::clamp(v, min, max);
        };

        const double initialValue = *value;

        ClampValue(*value);

        if (*value != initialValue)
            valueChanged = true;

        // ============================================
        // Identification et label
        // ============================================

        const char* labelEnd = FindRenderedTextEnd(label);
        const bool hasVisibleLabel = label != labelEnd;

        PushID(label);

        BeginGroup();

        // ============================================
        // Dimensions
        // ============================================

        const float frameHeight = GetFrameHeight();
        const float spacing = GetStyle().ItemInnerSpacing.x;
        const float buttonWidth = frameHeight;

        const float totalWidth = CalcItemWidth();

        const float buttonsWidth = useStep
            ? buttonWidth * 2.0f + spacing * 2.0f
            : 0.0f;

        const float dragWidth = totalWidth - buttonsWidth;

        // ============================================
        // DragScalar
        // ============================================

        const double* pMin = useClamp ? &min : nullptr;
        const double* pMax = useClamp ? &max : nullptr;

        if (useClamp)
            flags |= ImGuiSliderFlags_AlwaysClamp;

        PushItemWidth(std::max(1.0f, dragWidth));

        if (DragScalar(
            "##value",
            ImGuiDataType_Double,
            value,
            speed,
            pMin,
            pMax,
            format,
            flags
        ))
        {
            valueChanged = true;
        }

        PopItemWidth();

        // ============================================
        // Boutons Step
        // ============================================

        if (useStep)
        {
            SameLine(0.0f, spacing);

            const double currentStep =
                (useFastStep && GetIO().KeyCtrl)
                ? step_fast
                : step;

            // Bouton -
            if (Button("-##step_minus", ImVec2(buttonWidth, 0.0f)))
            {
                const double oldValue = *value;

                *value -= currentStep;
                ClampValue(*value);

                if (*value != oldValue)
                    valueChanged = true;
            }

            SameLine(0.0f, spacing);

            // Bouton +
            if (Button("+##step_plus", ImVec2(buttonWidth, 0.0f)))
            {
                const double oldValue = *value;

                *value += currentStep;
                ClampValue(*value);

                if (*value != oldValue)
                    valueChanged = true;
            }
        }

        // ============================================
        // Label visible
        // ============================================

        if (hasVisibleLabel)
        {
            SameLine(0.0f, spacing);
            TextUnformatted(label, labelEnd);
        }

        EndGroup();

        PopID();

        return valueChanged;
    }
}
