#pragma once
#include "Vector.h"

class Camera
{

public:
    vec3 m_pos;
    vec3 m_dir;
    vec3 m_left;
    vec3 m_up;
    vec3 m_veloc;

    float m_PI = 3.14159265;
    float m_scl = 1.0;
    float m_yaw;
    float m_pitch;

    Camera(vec3 pos);
    ~Camera();

    void activate();

    void forward();
    void back();
    void left();
    void right();

    void updateYaw(float dYaw);
    void updatePitch(float dPitch);
    void update();

private:
    float cvtToRad(float ang);
};