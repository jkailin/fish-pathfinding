#ifndef PATH_UTIL
#define PATH_UTIL

#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include <vector>
#include "math.h"


class PathUtil {
public:
    static sf::Vector2f isLinesIntersect(sf::Vector2f line1_start_point, sf::Vector2f line1_end_point, sf::Vector2f line2_start_point, sf::Vector2f line2_end_point);
    static bool isPolygonLineIntersect(sf::Vector2f line_start_point, sf::Vector2f line_end_point, std::vector<sf::Vector2f> polygon);
    static sf::Vector2f getPath(sf::Vector2f target_point, sf::Vector2f source_point, std::vector<std::vector<sf::Vector2f>> polygons, double travelDistance);
    static std::vector<sf::Vector2f> expandPolygon(std::vector<sf::Vector2f> polygon, double expand_size);
    static sf::Vector2f nextTargetPoint(sf::Vector2f currPoint, sf::Vector2f targetPoint, double deltaTime);

};
#endif
