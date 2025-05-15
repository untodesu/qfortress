#include "shared/pch.hh"

#include "shared/health.hh"

#include "core/config.hh"
#include "core/cvar.hh"
#include "shared/class.hh"
#include "shared/effect.hh"
#include "shared/globals.hh"
#include "shared/team.hh"

constexpr static float HEALTH_DECREASE_RATE = 2.0f; ///< Health decrease rate per second when above base value

constexpr static float REGENERATION_RATE = 10.0f; ///< Regeneration rate of health points per second
constexpr static float OVERHEAL_RATE = 25.0f;     ///< Overheal rate of health points per second
constexpr static float SUPERHEAL_RATE = 100.0f;   ///< Superheal rate of health points per second

constexpr static float OVERHEAL_BASE_FACTOR = 1.5f;  ///< Overheal to base health increase factor
constexpr static float SUPERHEAL_BASE_FACTOR = 2.5f; ///< Superheal to base health increase factor

template<QF_Class class_value>
constexpr static float BASE_HEALTH;
template<>
constexpr static float BASE_HEALTH<QF_Class::Unassigned> = 0.0f;
template<>
constexpr static float BASE_HEALTH<QF_Class::Spectator> = 0.0f;
template<>
constexpr static float BASE_HEALTH<QF_Class::Default> = 100.0f;

static inline float baseHealthForClass(QF_Class class_value)
{
    switch(class_value) {
        case QF_Class::Unassigned:
            return BASE_HEALTH<QF_Class::Unassigned>;

        case QF_Class::Spectator:
            return BASE_HEALTH<QF_Class::Spectator>;

        case QF_Class::Default:
            return BASE_HEALTH<QF_Class::Default>;

        default:
            return std::numeric_limits<float>::quiet_NaN();
    }
}

void HealthComponent::fixedUpdate(void)
{
    auto group = globals::registry.group<HealthComponent>(entt::get<ClassComponent>);

    for(auto [entity, health, qf_class] : group.each()) {
        auto base_health = baseHealthForClass(qf_class.value);

        // Apply health decrease whenever the health
        // is above the base health for the class
        if(health.value > base_health) {
            health.value -= HEALTH_DECREASE_RATE * globals::fixed_frametime;
        }

        // Apply regeneration if possible
        if(globals::registry.all_of<Effect_Regeneration>(entity)) {
            health.value += REGENERATION_RATE * globals::fixed_frametime;
            health.value = std::min(health.value, base_health);
        }

        // Apply overhealing if possible
        if(globals::registry.all_of<Effect_OverHealing>(entity)) {
            health.value += OVERHEAL_RATE * globals::fixed_frametime;
            health.value = std::min(health.value, base_health * OVERHEAL_BASE_FACTOR);
        }

        // Apply superhealing if possible
        if(globals::registry.all_of<Effect_SuperHealing>(entity)) {
            health.value += SUPERHEAL_RATE * globals::fixed_frametime;
            health.value = std::min(health.value, base_health * SUPERHEAL_BASE_FACTOR);
        }

        // Round up to nearest integer
        health.value = std::ceilf(health.value);
    }
}
