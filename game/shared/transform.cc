#include "shared/pch.hh"

#include "shared/transform.hh"

#include "shared/globals.hh"

void TransformComponent::interpolate(TransformComponent& out, const TransformComponent& a, const TransformComponent& b, float alpha)
{
    out.position = glm::mix(a.position, b.position, glm::clamp(alpha, 0.0f, 1.0f));
    out.angles = glm::mix(a.angles, b.angles, glm::clamp(alpha, 0.0f, 1.0f));
}
