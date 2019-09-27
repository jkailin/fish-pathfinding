#include "PathUtil.h"

sf::Vector2f PathUtil::isLinesIntersect(sf::Vector2f line1_start_point, sf::Vector2f line1_end_point, sf::Vector2f line2_start_point, sf::Vector2f line2_end_point) {

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

bool PathUtil::isPolygonLineIntersect(sf::Vector2f line_start_point, sf::Vector2f line_end_point, std::vector<sf::Vector2f> polygon)  {
    sf::Vector2f current_point, next_point;
	sf::Vector2f intersect_point;

	double line_min_x, line_max_x, line_min_y, line_max_y;
	bool isIntersect = false;

	// find max x of line.
	if (line_start_point.x < line_end_point.x)
	{
		line_min_x = line_start_point.x;
		line_max_x = line_end_point.x;
	}
	else
	{
		line_min_x = line_end_point.x;
		line_max_x = line_start_point.x;
	}

	// find max y of line
	if (line_start_point.y < line_end_point.y)
	{
		line_min_y = line_start_point.y;
		line_max_y = line_end_point.y;
	}
	else
	{
		line_min_y = line_end_point.y;
		line_max_y = line_start_point.y;
	}

	// iterate through the polygons points
	for (int i = 0; i < polygon.size(); i++)
	{

		current_point = polygon[i];

		// assumes points are in clockwise/counter-clockwise order
		if (i < polygon.size() - 1)
			next_point = polygon[i + 1];
		else
			next_point = polygon[0];

		// if both curr and next are less/greater than min/max, can't intersect that line
		if ((current_point.x <= line_min_x) && (next_point.x <= line_min_x))
			continue;

		if ((current_point.x >= line_max_x) && (next_point.x >= line_max_x))
			continue;

		if ((current_point.y <= line_min_y) && (next_point.y <= line_min_y))
			continue;

		if ((current_point.y >= line_max_y) && (next_point.y >= line_max_y))
			continue;

		// check if line intersects polygon edge
		intersect_point = PathUtil::isLinesIntersect(line_start_point, line_end_point, current_point, next_point);
		
		// if intersection was found
		if (intersect_point.x > -10000)
		{
			if (line_min_x != line_max_x)
			{
				if ((intersect_point.x > line_min_x) && (intersect_point.x < line_max_x))
				{
					// intersection was found
					// rc = 1;
					isIntersect = true;
					break;
				}
			}
			else
			{
				if ((intersect_point.y > line_min_y) && (intersect_point.y < line_max_y))
				{
					// intersection was found
					// rc = 1;
					isIntersect = true;
					break;
				}
			}
		}
	}
	return isIntersect;
}


std::vector<sf::Vector2f> PathUtil::expandPolygon(std::vector<sf::Vector2f> polygon, double expand_size) {
    double x_offset, y_offset;
	double x_size, y_size;
	double x_ratio, y_ratio;

	// initialize
	x_offset = polygon[0].x;
	y_offset = polygon[0].y;
	x_size = polygon[0].x;
	y_size = polygon[0].y;

	for (int i = 1; i < polygon.size(); i++)
	{
		if ( polygon[i].x < x_offset )
			x_offset = polygon[i].x;

		if (polygon[i].y < y_offset)
			y_offset = polygon[i].y;

		if (polygon[i].x > x_size)
			x_size = polygon[i].x;

		if (polygon[i].y > y_size)
			y_size = polygon[i].y;
	}

	std::vector<sf::Vector2f> expandedPolygon;
	for (int i = 0; i < polygon.size(); i++)
	{

		expandedPolygon.push_back(sf::Vector2f(polygon[i].x - x_offset, polygon[i].y - y_offset));
	}

	x_size -= x_offset;
	y_size -= y_offset;

	x_ratio = (x_size + 2 * expand_size) / x_size;
	y_ratio = (y_size + 2 * expand_size) / y_size;

	for (int i = 0; i < expandedPolygon.size(); i++)
	{
		expandedPolygon[i].x = expandedPolygon[i].x * x_ratio;
		expandedPolygon[i].y = expandedPolygon[i].y * y_ratio;
	}

	for (int i = 0; i < expandedPolygon.size(); i++)
	{
		expandedPolygon[i].x = expandedPolygon[i].x + x_offset - expand_size;
		expandedPolygon[i].y = expandedPolygon[i].y + y_offset - expand_size;
	}

	return expandedPolygon;
}

sf::Vector2f PathUtil::getPath(sf::Vector2f target_point, sf::Vector2f source_point, std::vector<std::vector<sf::Vector2f>> polygons, double travelDistance) {
    sf::Vector2f output_point;
	sf::Vector2f new_point;

	double distancePlayerToPolygon, source_distance, target_distance, totalDistance, distanceEnemyToPolygon;
	int vertex_index;
	// int rc;
	bool isIntersect;

	output_point.x = target_point.x;
	output_point.y = target_point.y;

	// Distance from player fish to enemy fish (default)
	source_distance = sqrt((source_point.x - target_point.x) * (source_point.x - target_point.x) + (source_point.y - target_point.y) * (source_point.y - target_point.y));
	target_distance = 2048;

	// Iterate through all the shapes on the board. They are all convex.
	for (auto const& polygon : polygons)
	{
		// check if the line between enemy fish and player fish intersects a polygon
		isIntersect = isPolygonLineIntersect(source_point, target_point, polygon);
		// if (rc == 1)
		if (isIntersect)

		{	
            // Find the points of the polygon that can be reached without hitting the polygon on the way.
			for (auto const& vertex : polygon)
			{
				for (auto const& polygon1 : polygons) {
                    // if polygon intersected, ignore that point. 
					isIntersect = isPolygonLineIntersect(source_point, vertex, polygon1);
					if(isIntersect)
						break;

				}
				if ( !isIntersect)
				{
                    // calculate distance from minion to that point to the target
					distancePlayerToPolygon = sqrt((target_point.x - vertex.x) * (target_point.x - vertex.x) + (target_point.y - vertex.y) * (target_point.y - vertex.y));
					distanceEnemyToPolygon = sqrt((source_point.x - vertex.x) * (source_point.x - vertex.x) + (source_point.y - vertex.y) * (source_point.y - vertex.y));
					totalDistance = distanceEnemyToPolygon + distancePlayerToPolygon;

                    // if enemy is too close to point it could go into shape (temporary fix).
					if (totalDistance > 0 && distancePlayerToPolygon > 0)
					{
						if (distancePlayerToPolygon < target_distance)
						{
							
							target_distance = distancePlayerToPolygon;
							output_point.x = vertex.x;
							output_point.y = vertex.y;
							
						}
					}

				}
			}
		}
	}
	return output_point;
}

