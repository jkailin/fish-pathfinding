#ifndef PATH_UTIL
#define PATH_UTIL

#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include <vector>

// Finds the point where two lines intersect
static sf::Vector2f line_intersection(sf::Vector2f line1_start_point, sf::Vector2f line1_end_point, sf::Vector2f line2_start_point, sf::Vector2f line2_end_point)
{
    sf::Vector2f intersection_point(-10000, -10000);

    double dy1 = line1_end_point.y - line1_start_point.y;
    double dx1 = line1_end_point.x - line1_start_point.x;
    double dy2 = line2_end_point.y - line2_start_point.y;
    double dx2 = line2_end_point.x - line2_start_point.x;

    if (dy1 * dx2 != dy2 * dx1)
    {
        double x, y;

        x = ((line2_start_point.y - line1_start_point.y) * dx1 * dx2 + dy1 * dx2 * line1_start_point.x - dy2 * dx1 * line2_start_point.x) / (dy1 * dx2 - dy2 * dx1);

        if (dx1 != 0)
            y = line1_start_point.y + (dy1 / dx1) * (x - line1_start_point.x);
        else
            y = line2_start_point.y + (dy2 / dx2) * (x - line2_start_point.x);

        intersection_point.x = x;
        intersection_point.y = y;
    }

    return intersection_point;
    }


#endif
