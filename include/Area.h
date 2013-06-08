/*
 * Area.h
 *
 *  Created on: 06.06.2013
 *      Author: Roland
 */

#ifndef AREA_H_
#define AREA_H_

#include <string>
#include "Point.h"
#include <iostream>
#include <vector>

class Point;

class Area {

private:

	double m_area;
	Point m_bounding_box_min;
	Point m_bounding_box_max;
	bool m_within_state;
	std::vector<Point> m_points;


	bool calculate_area();
	bool calculate_bounding_box();

public:

	Area();
	Area(std::vector<Point> &a_points);
	~Area();

	bool is_within_state() const { return m_within_state; }
	bool point_in_area(const Point &a_point,  int *a_intersections);
	double get_area() const { return m_area; }
	const Point& get_box_min() const { return m_bounding_box_min; }
	const Point& get_box_max() const { return m_bounding_box_max; }
	const std::vector<Point>& get_points() const { return m_points; }

	void set_points(std::vector<Point> &a_points);
	void set_within_state(bool a_within_state);

	bool operator <(const Area &a_area);
	friend std::ostream& operator <<(std::ostream &os, Area &a_area);

};


#endif /* AREA_H_ */
