﻿#pragma once

struct CameraComponent
{

    float heightOffset;
    float focusHeightOffset;

    float xDelta = 0.0f;
    float yDelta = 0.0f;
    float radius = 5.0f;

    float currentRadius = 0.0f;

    float xSpeed = 80.0f;
    float ySpeed = 80.0f;
    float yMinLimit = -90.0f;
    float yMaxLimit = 90.0f;
    float m_pitchLimit = -35.f;
};