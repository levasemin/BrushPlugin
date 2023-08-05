#include "Interpolator.hpp"

std::pair<int, int> Interpolator::operator()(float t, std::pair<int, int> point_0, std::pair<int, int> point_1, std::pair<int, int> point_2, std::pair<int, int> point_3)
{
    std::pair<int, int> new_point(0.f, 0.f);

    if (type_ == CATMULL_ROM)
    {
        float coeff_0 = -t * pow(1.f - t, 2.f);
        float coeff_1 = (2.f - 5.f*pow(t, 2.f) + 3.f*pow(t, 3.f));
        float coeff_2 = t * (1.f + 4.f*t - 3.f*pow(t, 2.f));
        float coeff_3 = pow(t, 2.f) * (1.f - t);

        new_point.first  = 0.5f * (coeff_0 * point_0.first  + coeff_1 * point_1.first  + coeff_2 * point_2.first  - coeff_3 * point_3.first);
        new_point.second = 0.5f * (coeff_0 * point_0.second + coeff_1 * point_1.second + coeff_2 * point_2.second - coeff_3 * point_3.second);
    }

    return new_point;
}