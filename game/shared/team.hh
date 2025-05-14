#ifndef SHARED_TEAM_HH
#define SHARED_TEAM_HH 1
#pragma once

enum class QF_Team : unsigned short {
    Unassigned = 0, ///< Just joined a server and haven't decided yet
    Spectator,      ///< Noclips around but can't interact with the world
    TF_Blue,        ///< Methinks in TF lore BLU is Builders League United
    TF_Red,         ///< Methinks in TF lore RED is Reliable Excavation Demolition
};

template<QF_Team team_type>
struct TeamFlagComponent final {};

using Team_Unassigned = TeamFlagComponent<QF_Team::Unassigned>;
using Team_Spectator = TeamFlagComponent<QF_Team::Spectator>;
using Team_TF_Blue = TeamFlagComponent<QF_Team::TF_Blue>;
using Team_TF_Red = TeamFlagComponent<QF_Team::TF_Red>;

class TeamComponent final {
public:
    const QF_Team value;

public:
    static void attach(entt::entity entity, QF_Team value);
    static void detach(entt::entity entity);
};

#endif // SHARED_TEAM_HH
