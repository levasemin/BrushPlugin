#pragma once
#include "PluginStandart.hpp"
#include <cmath>

class Circle : public booba::Picture
{
public:

    Circle(int radius, booba::Color color = booba::Color::BLACK);

    void setRadius(int radius);

    void setColor(const booba::Color &color);

private:
    int radius_ = 0;
    booba::Color color_;

    void updateTexture();
};