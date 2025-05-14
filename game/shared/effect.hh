#ifndef SHARED_EFFECT_HH
#define SHARED_EFFECT_HH
#pragma once

enum class QF_Effect : unsigned short {
    Regeneration, ///< Being healed by something like a dispenser, health will not exceed base value
    OverHealing,  ///< Being over-healed by a friendly medic, health will not exceed 1.5x base value
    SuperHealing, ///< Being super-healed by a friendly medic, health will not exceed 2.5x base value
    COUNT,
};

constexpr static std::size_t QF_EFFECT_COUNT = static_cast<std::size_t>(QF_Effect::COUNT);

template<QF_Effect effect_value>
struct EffectFlagComponent final {};

using Effect_Regeneration = EffectFlagComponent<QF_Effect::Regeneration>;
using Effect_OverHealing = EffectFlagComponent<QF_Effect::OverHealing>;
using Effect_SuperHealing = EffectFlagComponent<QF_Effect::SuperHealing>;

class EffectComponent final {
public:
    std::array<std::pair<bool, std::uint64_t>, QF_EFFECT_COUNT> effects;

public:
    static void attach(entt::entity entity, QF_Effect effect, std::uint64_t duration = UINT64_MAX);
    static void detach(entt::entity entity, QF_Effect effect);
    static void fixedUpdate(void);
};

#endif // SHARED_EFFECT_HH
