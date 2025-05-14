#ifndef SHARED_HEALTH_HH
#define SHARED_HEALTH_HH
#pragma once

class HealthComponent final {
public:
    float value;

public:
    static void fixedUpdate(void);
};

#endif // SHARED_HEALTH_HH
