/*
 * Point.cpp
 *
 *  Created on: 09.04.2013
 *      Author: Roland
 */

#include "Point.h"
#include "log.h"


using namespace std;


ostream& operator <<(ostream &os, Point &a_point) {

	DBG();

	os << "X: " << a_point.get_x() << "\t Y: " << a_point.get_y();

	return os;
}


Point::Point() : m_x(0.0), m_y(0.0) {

	DBG();

}

Point::Point(double a_x, double a_y) : m_x(a_x), m_y(a_y) {

	DBG();

}

Point::Point(const Point &a_point) : m_x(a_point.m_x), m_y(a_point.m_y) {

	DBG();
}

Point::~Point() {

	DBG();

}

void Point::set_point(double a_x, double a_y) {

	DBG();

	m_x = a_x;
	m_y = a_y;

}

void Point::set_point(Point &a_point) {

	DBG();

	m_x = a_point.m_x;
	m_y = a_point.m_y;

}

bool Point::operator ==(const Point &a_point) {

	DBG();

	if((a_point.m_x == m_x) && (a_point.m_y == m_y)) {

		return true;
	}
	else {

		return false;
	}
}

bool Point::operator < (Point &a_point) {
	if ((m_x < a_point.m_x) && (m_y < a_point.m_y)){
		return true;
	}
	else {
		return false;
	}

}

bool Point::operator > (Point &a_point) {
	if ((m_x > a_point.m_x) && (m_y > a_point.m_y)){
		return true;
	}
	else {
		return false;
	}
}
