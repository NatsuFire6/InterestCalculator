
#pragma once

#include "imgui.h"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <type_traits>

/* pour le label ImGui fait :
    "Vitesse" : label visible et ID basé sur le label.
    "Vitesse##speed" : label visible Vitesse, ID basé sur Vitesse##speed.
    "##speed" : aucun label visible, ID basé sur ##speed.
    "Vitesse###speed" : label visible Vitesse, ID basé uniquement sur speed.
*/
/* Type pris en charges :
    int8_t    / signed char	        ImGuiDataType_S8
    uint8_t   / unsigned char	    ImGuiDataType_U8
    int16_t   / short	            ImGuiDataType_S16
    uint16_t  / unsigned short	    ImGuiDataType_U16
    int32_t   / int	                ImGuiDataType_S32
    uint32_t  / unsigned int	    ImGuiDataType_U32
    int64_t   / long long	        ImGuiDataType_S64
    uint64_t  / unsigned long long	ImGuiDataType_U64
    float	                        ImGuiDataType_Float
    double	                        ImGuiDataType_Double
*/
namespace ImGui
{
    namespace CustomDetail
    {
        // ============================================
        // Détermination du type ImGui
        // ============================================

        template<typename T>
        constexpr ImGuiDataType GetDataType()
        {
            using Type = std::remove_cv_t<T>;

            if constexpr (std::is_same_v<Type, float>)
                return ImGuiDataType_Float;

            else if constexpr (std::is_same_v<Type, double>)
                return ImGuiDataType_Double;

            else if constexpr (std::is_integral_v<Type>
                               && !std::is_same_v<Type, bool>)
            {
                if constexpr (sizeof(Type) == 1)
                    return std::is_signed_v<Type>
                        ? ImGuiDataType_S8 : ImGuiDataType_U8;

                else if constexpr (sizeof(Type) == 2)
                    return std::is_signed_v<Type>
                        ? ImGuiDataType_S16 : ImGuiDataType_U16;

                else if constexpr (sizeof(Type) == 4)
                    return std::is_signed_v<Type>
                        ? ImGuiDataType_S32 : ImGuiDataType_U32;

                else if constexpr (sizeof(Type) == 8)
                    return std::is_signed_v<Type>
                        ? ImGuiDataType_S64 : ImGuiDataType_U64;
            }

            static_assert(
                std::is_arithmetic_v<Type>
                && !std::is_same_v<Type, bool>,
                "DragDouble: unsupported numeric type"
            );

            return ImGuiDataType_Double;
        }

        // ============================================
        // Format par défaut
        // ============================================

        template<typename T>
        constexpr const char* GetDefaultFormat()
        {
            using Type = std::remove_cv_t<T>;

            if constexpr (std::is_same_v<Type, float>)
                return "%.3f";

            else if constexpr (std::is_same_v<Type, double>)
                return "%.6f";

            else if constexpr (std::is_integral_v<Type>
                               && std::is_signed_v<Type>)
            {
                if constexpr (sizeof(Type) <= 4)
                    return "%d";
                else
                    return "%lld";
            }
            else
            {
                if constexpr (sizeof(Type) <= 4)
                    return "%u";
                else
                    return "%llu";
            }
        }

        // ============================================
        // Fin du texte visible
        // ============================================

        inline const char* FindLabelEnd(const char* label)
        {
            const char* end = std::strstr(label, "##");
            return end ? end : label + std::strlen(label);
        }

        // ============================================
        // Modification sécurisée par Step
        // ============================================

        template<typename T>
        T ApplyStep(T value, T step, bool increase,
                    bool useClamp, T min, T max)
        {
            long double result = static_cast<long double>(value);
            const long double delta = static_cast<long double>(step);

            result += increase ? delta : -delta;

            long double lower =
                static_cast<long double>(std::numeric_limits<T>::lowest());

            long double upper =
                static_cast<long double>(std::numeric_limits<T>::max());

            if (useClamp)
            {
                lower = static_cast<long double>(min);
                upper = static_cast<long double>(max);
            }

            result = std::clamp(result, lower, upper);

            return static_cast<T>(result);
        }
    }

    // ================================================
    // DragDouble générique
    // ================================================

    template<typename T>
    bool DragDouble(
        const char* label,
        T* value,
        float speed = 1.0f,
        T min = T{},
        T max = T{},
        const char* format = nullptr,
        ImGuiSliderFlags flags = 0,
        T step = T{},
        T step_fast = T{}
    )
    {
        static_assert(
            std::is_arithmetic_v<T> && !std::is_same_v<T, bool>,
            "DragDouble requires a supported numeric type"
        );

        if (label == nullptr || value == nullptr)
            return false;

        // ============================================
        // Configuration
        // ============================================

        const bool useClamp = min != max;
        const bool useStep = step > T{};
        const bool useFastStep = step_fast > T{};

        if (useClamp && min > max)
        {
            IM_ASSERT(min <= max);
            return false;
        }

        bool valueChanged = false;

        // ============================================
        // Clamp
        // ============================================

        auto ClampValue = [&](T& v)
        {
            if (useClamp)
                v = std::clamp(v, min, max);
        };

        const T initialValue = *value;

        ClampValue(*value);

        if (*value != initialValue)
            valueChanged = true;

        // ============================================
        // Format et type
        // ============================================

        constexpr ImGuiDataType dataType =
            CustomDetail::GetDataType<T>();

        if (format == nullptr)
            format = CustomDetail::GetDefaultFormat<T>();

        const T* pMin = useClamp ? &min : nullptr;
        const T* pMax = useClamp ? &max : nullptr;

        if (useClamp)
            flags |= ImGuiSliderFlags_AlwaysClamp;

        // ============================================
        // Identification et label
        // ============================================

        const char* labelEnd = CustomDetail::FindLabelEnd(label);
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

        PushItemWidth(std::max(1.0f, dragWidth));

        if (DragScalar(
            "##value",
            dataType,
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

            const T currentStep =
                (useFastStep && GetIO().KeyCtrl)
                ? step_fast
                : step;

            // Bouton -
            if (Button("-##step_minus",
                       ImVec2(buttonWidth, 0.0f)))
            {
                const T oldValue = *value;

                *value = CustomDetail::ApplyStep(
                    *value, currentStep, false,
                    useClamp, min, max
                );

                if (*value != oldValue)
                    valueChanged = true;
            }

            SameLine(0.0f, spacing);

            // Bouton +
            if (Button("+##step_plus",
                       ImVec2(buttonWidth, 0.0f)))
            {
                const T oldValue = *value;

                *value = CustomDetail::ApplyStep(
                    *value, currentStep, true,
                    useClamp, min, max
                );

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
