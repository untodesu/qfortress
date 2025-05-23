#ifndef SHARED_TRANSFORM_HH
#define SHARED_TRANSFORM_HH
#pragma once

class TransformComponent {
public:
    Vector3f position;
    Vector3f angles;

public:
    static void interpolate(TransformComponent& out, const TransformComponent& a, const TransformComponent& b, float alpha);
};

class TransformComponent_Prev final : public TransformComponent {};
class TransformComponent_Intr final : public TransformComponent {};

#endif // SHARED_TRANSFORM_HH
