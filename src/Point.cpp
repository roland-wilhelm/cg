/*
 * Point.cpp
 *
 *  Created on: 09.04.2013
 *      Author: Roland
 */

#include "Point.h"
#include "log.h"

#include <iostream>
#include <cassert>

using namespace std;


ostream& operator <<(ostream &os, const Point &a_point) {

	DBG();

	os << "X: " << a_point.m_x << "\t Y: " << a_point.m_y;

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

	//DBG();

}

void Point::set_point(double a_x, double a_y) {

	DBG();

	m_x = a_x;
	m_y = a_y;

}

void Point::set_point(const Point &a_point) {

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

Point Point::operator -(const Point &a_point) {

	Point point;

	DBG();

	assert(&a_point);

	if(&a_point == NULL)
		return *this;

	point.m_x = m_x - a_point.m_x;
	point.m_y = m_y - a_point.m_y;

	return point;
}

Point Point::operator +(const Point &a_point) {

	Point point;

	DBG();

	assert(&a_point);

	if(&a_point == NULL)
		return *this;


	point.m_x = m_x + a_point.m_x;
	point.m_y = m_y + a_point.m_y;

	return point;
}

/*
 * Punkt ist kleiner oder gleich, wenn
 * 1. Bei einer Schräge X und Y eines Punktes <=
 * 2. Horizontale oder Vertiale Stecke X oder entsprechend Y <=
 */
bool Point::operator <=(const Point &a_point) {

	bool ret = false;

	if(m_y == a_point.m_y) {

		if(m_x <= a_point.m_x) {

			ret = true;
		}
		else {

			ret = false;
		}
	}
	else if(m_x == a_point.m_x) {

		if(m_y <= a_point.m_y) {

			ret = true;
		}
		else {

			ret = false;
		}
	}
	else {

		if((m_y <= a_point.m_y) && (m_x <= a_point.m_x)) {

			ret = true;
		}
		else {

			ret = false;
		}

	}

	return ret;

}
