#ifndef SHARED_CLASS_HH
#define SHARED_CLASS_HH
#pragma once

enum class QF_Class : unsigned short {
    Unassigned = 0, ///< Matches with the QF_Team::Unassigned
    Spectator,      ///< Matches with the QF_Team::Spectator
    Default,        ///< Development
};

template<QF_Class class_value>
struct ClassFlagComponent final {};

using Class_Unassigned = ClassFlagComponent<QF_Class::Unassigned>;
using Class_Spectator = ClassFlagComponent<QF_Class::Spectator>;
using Class_Default = ClassFlagComponent<QF_Class::Default>;

class ClassComponent final {
public:
    const QF_Class value;

public:
    static void attach(entt::entity entity, QF_Class value);
    static void detach(entt::entity entity);
};

#endif // SHARED_CLASS_HH
