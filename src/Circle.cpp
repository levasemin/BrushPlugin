#include "Circle.hpp"

Circle::Circle(int radius, booba::Color color): Picture(),
    radius_(radius),
    color_(color)
{
    create(radius_ * 2, radius_ * 2, booba::Color(0, 0, 0, 0));

    updateTexture();
}

void Circle::setRadius(int radius)
{
    if (radius_ != radius)
    {
        radius_ = radius;

        create(radius_ * 2, radius_ * 2, booba::Color(0, 0, 0, 0));

        updateTexture();
    }
}

void Circle::setColor(const booba::Color &color)
{
    if (color_ != color)
    {
        color_ = color;

        updateTexture();
    }
}

void Circle::updateTexture()
{
    for (int y = 0; y < radius_ * 2; y++)
    {
        for (int x = 0; x < radius_ * 2; x++)
        {
            float value = float(pow(radius_, 2) - (pow(x - radius_, 2) + pow(y - radius_, 2)));
            
            if (value > 0)
            {
                setPixel(x, y, color_);
            }
        }
    }
}