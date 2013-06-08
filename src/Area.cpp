/*
 * Area.cpp
 *
 *  Created on: 07.06.2013
 *      Author: Roland
 */


#include "Area.h"
#include <string>
#include "Point.h"
#include "Line.h"
#include <iostream>
#include <iomanip>
#include <math.h>


using namespace std;



Area::Area() : m_area(0.0),
			   m_within_state(false)  {


}

Area::Area(std::vector<Point> &a_points) : m_area(0.0),
										   m_within_state(false),
										   m_points(a_points) {

	calculate_bounding_box();
	calculate_area();

}

Area::~Area() {

	m_points.clear();
}

bool Area::calculate_area() {

	vector<Point>::iterator iterPoints = m_points.begin();
	if(iterPoints == m_points.end()) {

		std::cerr << "No points available." << std::endl;
		return false;
	}

	/*
	 * handle last point of the state, because
	 * last point is the start. IMPORTANT: iterOuter->end() -1
	 */
	double area = 0.0;
	for(; iterPoints != (m_points.end() - 1);	iterPoints++) {


		double x1 = iterPoints->get_x();
		double y1 = iterPoints->get_y();

		double x2 = (iterPoints+1)->get_x();
		double y2 = (iterPoints+1)->get_y();

		area += (( y1 + y2 ) * ( x1 - x2 ));
	}

	m_area = fabs(area) / 2;

	return true;

}

bool Area::calculate_bounding_box() {

	vector<Point>::iterator iterPoints = m_points.begin();
	if(iterPoints == m_points.end()) {

		cerr << "No points available." << endl;
		return false;
	}

	m_bounding_box_min = m_bounding_box_max = *iterPoints;
	iterPoints++;
	for(; iterPoints != (m_points.end() - 1); iterPoints++) {

		if(iterPoints->get_x() < m_bounding_box_min.get_x() ) {

			m_bounding_box_min.set_x(iterPoints->get_x());
		}

		if(iterPoints->get_x() > m_bounding_box_max.get_x() ) {

			m_bounding_box_max.set_x(iterPoints->get_x());
		}

		if(iterPoints->get_y() < m_bounding_box_min.get_y() ) {

			m_bounding_box_min.set_y(iterPoints->get_y());
		}

		if(iterPoints->get_y() > m_bounding_box_max.get_y() ) {

			m_bounding_box_max.set_y(iterPoints->get_y());
		}

	}

	return true;
}

bool Area::point_in_area(const Point &a_point, int *a_intersections) {

	vector<Point>::iterator iterPoints = m_points.begin();
	if(iterPoints == m_points.end()) {

		cerr << "No points available." << endl;
		return false;
	}


	// First point is out of the state, second the point to be found
	Line line(Point(m_bounding_box_min.get_x() - 1, m_bounding_box_min.get_y()), a_point);
	cout << setw(20) << "    Compared line (p | q): " << setw(45) << line << endl;

	(*a_intersections) = 0;

	/*
	 * Wenn eine Linie sich genau am Schnittpunkt zweier Segmente schneidet,
	 * werden zwei intersections erkannt, ist soweit ja richtig,
	 * aber es darf nur als eine intrsection behandelt werden , da sonst
	 * der Punkt außerhalb und nicht innerhalb liegt.
	 */
	while(Line::ccw(line.get_start_point(), line.get_end_point(), *iterPoints) == 0)
		iterPoints++;

	for(; iterPoints != (m_points.end() - 1); iterPoints++) {

		double x1 = iterPoints->get_x();
		double y1 = iterPoints->get_y();
		double x2 = (iterPoints+1)->get_x();
		double y2 = (iterPoints+1)->get_y();

		Line line2(x1, y1, x2, y2);

		if(line.is_intersection(line2) == true) {

			cout << setw(20) << "    Intersected line: " << setw(50) << line2 << endl;
			(*a_intersections)++;

		}

	}

	if((*a_intersections) % 2 == 1) {

		return true;
	}

	return false;
}

void Area::set_points(std::vector<Point> &a_points) {

	m_points = a_points;
	calculate_bounding_box();
	calculate_area();
}

void Area::set_within_state(bool a_within_state) {

	m_within_state = a_within_state;
}

bool Area::operator <(const Area &a_area) {

	if(m_area < a_area.m_area)
		return true;
	else
		return false;

}

ostream& operator <<(ostream &os, Area &a_area) {

	os 	<< "  Bounding Box:"
		<< "  Min: " << a_area.m_bounding_box_min
		<< "  Max: " << a_area.m_bounding_box_max << endl;
	os	<< "  Area: "
		<< setw(10)
		<< a_area.m_area
		<< endl;

	return os;
}
