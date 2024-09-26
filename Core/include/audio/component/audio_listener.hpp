#pragma once

#include "core.hpp"
#include "audio/audio_context.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE AudioListener : public Component
{
    REFLECTABLE_IMPL(AudioListener)
    
public:
    bool_t dopplerEffect = true;
    
    void Update() override;
    
    /// @brief Get the volume in the range [0, inf].
    [[nodiscard]]
    float_t GetVolume() const;
    
    /// @brief Set the volume in the range [0, inf]. Default value is 1.
    void SetVolume(float_t newVolume);

private:
    /// @brief Volume in the range [0, inf]. Default value is 1.
    float_t m_Volume = 1.f;

    Vector3 m_LastPosition;
};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::AudioListener, bases<XnorCore::Component>),
    field(dopplerEffect),
    field(
        m_Volume,
        XnorCore::Reflection::Range(0.f, 5.f),
        XnorCore::Reflection::ModifiedCallback<XnorCore::AudioListener>(
            [](XnorCore::AudioListener* listener)
            {
                listener->SetVolume(listener->m_Volume);
            }
        )
    )
)
