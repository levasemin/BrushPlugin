#include "Ring.hpp"

Ring::Ring(int radius, booba::Color color):
    radius_(radius),
    color_(color)
{
    create(radius_ * 2, radius_ * 2, booba::Color(0, 0, 0, 0));

    updateTexture();
}

void Ring::setRadius(int radius)
{
    if (radius_ != radius)
    {
        radius_ = radius;

        create(radius_ * 2, radius_ * 2, booba::Color(0, 0, 0, 0));

        updateTexture();
    }
}

void Ring::setColor(booba::Color color)
{
    if (color_ != color)
    {
        color_ = color;

        updateTexture();
    }
}

void Ring::updateTexture()
{
    float frame_width = pow(radius_, 2) / 4;

    for (int y = 0; y < radius_ * 2; y++)
    {
        for (int x = 0; x < radius_ * 2; x++)
        {
            float value = float(pow(radius_, 2) - (pow(x - radius_, 2) + pow(y - radius_, 2)));

            if (0 < value && value < frame_width)
            {
                setPixel(x, y, color_);
            }
        }
    }
}