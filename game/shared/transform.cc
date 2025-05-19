#include "shared/pch.hh"

#include "shared/transform.hh"

#include "shared/globals.hh"

void TransformComponent::interpolate(TransformComponent& out, const TransformComponent& a, const TransformComponent& b, float alpha)
{
    auto alpha_clamped = std::clamp(alpha, 0.0f, 1.0f);

    out.position.x() = std::lerp(a.position.x(), b.position.x(), alpha_clamped);
    out.position.y() = std::lerp(a.position.y(), b.position.y(), alpha_clamped);
    out.position.z() = std::lerp(a.position.z(), b.position.z(), alpha_clamped);

    out.angles.x() = std::lerp(a.angles.x(), b.angles.x(), alpha_clamped);
    out.angles.y() = std::lerp(a.angles.y(), b.angles.y(), alpha_clamped);
    out.angles.z() = std::lerp(a.angles.z(), b.angles.z(), alpha_clamped);
}
