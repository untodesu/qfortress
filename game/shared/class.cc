#include "shared/pch.hh"

#include "shared/class.hh"

#include "shared/globals.hh"

void ClassComponent::attach(entt::entity entity, QF_Class value)
{
    globals::registry.emplace_or_replace<ClassComponent>(entity, ClassComponent { value });

    globals::registry.remove<Class_Unassigned>(entity);
    globals::registry.remove<Class_Spectator>(entity);
    globals::registry.remove<Class_Default>(entity);

    switch(value) {
        case QF_Class::Unassigned:
            globals::registry.emplace_or_replace<Class_Unassigned>(entity);
            break;

        case QF_Class::Spectator:
            globals::registry.emplace_or_replace<Class_Spectator>(entity);
            break;

        case QF_Class::Default:
            globals::registry.emplace_or_replace<Class_Default>(entity);
            break;
    }
}

void ClassComponent::detach(entt::entity entity)
{
    globals::registry.remove<Class_Unassigned>(entity);
    globals::registry.remove<Class_Spectator>(entity);
    globals::registry.remove<Class_Default>(entity);

    globals::registry.remove<ClassComponent>(entity);
}
