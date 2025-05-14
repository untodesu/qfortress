#include "shared/pch.hh"

#include "shared/effect.hh"

#include "shared/globals.hh"

void EffectComponent::attach(entt::entity entity, QF_Effect effect, std::uint64_t duration)
{
    auto& component = globals::registry.get_or_emplace<EffectComponent>(entity);

    component.effects[static_cast<unsigned int>(effect)].first = true;
    component.effects[static_cast<unsigned int>(effect)].second = globals::curtime + duration;

    switch(effect) {
        case QF_Effect::Regeneration:
            globals::registry.emplace_or_replace<Effect_Regeneration>(entity);
            break;

        case QF_Effect::OverHealing:
            globals::registry.emplace_or_replace<Effect_OverHealing>(entity);
            break;

        case QF_Effect::SuperHealing:
            globals::registry.emplace_or_replace<Effect_SuperHealing>(entity);
            break;
    }
}

void EffectComponent::detach(entt::entity entity, QF_Effect effect)
{
    auto& component = globals::registry.get_or_emplace<EffectComponent>(entity);

    component.effects[static_cast<unsigned int>(effect)].first = false;
    component.effects[static_cast<unsigned int>(effect)].second = UINT64_MAX;

    switch(effect) {
        case QF_Effect::Regeneration:
            globals::registry.remove<Effect_Regeneration>(entity);
            break;

        case QF_Effect::OverHealing:
            globals::registry.remove<Effect_OverHealing>(entity);
            break;

        case QF_Effect::SuperHealing:
            globals::registry.remove<Effect_SuperHealing>(entity);
            break;
    }
}

void EffectComponent::fixedUpdate(void)
{
    auto view = globals::registry.view<EffectComponent>();

    for(auto [entity, component] : view.each()) {
        for(std::size_t effect_id = 0; effect_id < QF_EFFECT_COUNT; effect_id += 1) {
            const auto& effect = component.effects[effect_id];

            if(effect.first && globals::curtime > effect.second) {
                EffectComponent::detach(entity, static_cast<QF_Effect>(effect_id));
            }
        }
    }
}
