#include "shared/pch.hh"

#include "shared/team.hh"

#include "shared/class.hh"
#include "shared/globals.hh"

void TeamComponent::attach(entt::entity entity, QF_Team value)
{
    globals::registry.emplace_or_replace<TeamComponent>(entity, TeamComponent { value });

    globals::registry.remove<Team_Unassigned>(entity);
    globals::registry.remove<Team_Spectator>(entity);
    globals::registry.remove<Team_TF_Blue>(entity);
    globals::registry.remove<Team_TF_Red>(entity);

    switch(value) {
        case QF_Team::Unassigned:
            globals::registry.emplace_or_replace<Team_Unassigned>(entity);
            break;

        case QF_Team::Spectator:
            globals::registry.emplace_or_replace<Team_Spectator>(entity);
            ClassComponent::attach(entity, QF_Class::Spectator);
            break;

        case QF_Team::TF_Blue:
            globals::registry.emplace_or_replace<Team_TF_Blue>(entity);
            break;

        case QF_Team::TF_Red:
            globals::registry.emplace_or_replace<Team_TF_Red>(entity);
            break;
    }
}

void TeamComponent::detach(entt::entity entity)
{
    globals::registry.remove<Team_Unassigned>(entity);
    globals::registry.remove<Team_Spectator>(entity);
    globals::registry.remove<Team_TF_Blue>(entity);
    globals::registry.remove<Team_TF_Red>(entity);

    globals::registry.remove<TeamComponent>(entity);
}
