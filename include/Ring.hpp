#pragma once

#include "PluginStandart.hpp"
#include <cmath>

class Ring : public booba::Picture
{
public:
    Ring(int radius, booba::Color color = booba::Color::BLACK);

    void setRadius(int radius);
    void setColor(booba::Color color);
    const booba::Picture &getPicture();

private:
    booba::Color color_;
    int radius_;

    void updateTexture();
};