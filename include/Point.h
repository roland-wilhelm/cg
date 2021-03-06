/*
 * Point.h
 *
 *  Created on: 09.04.2013
 *      Author: Roland
 */

#ifndef POINT_H_
#define POINT_H_

#include <ostream>


class Point {

	private:

		double m_x;
		double m_y;

	public:

		Point();
		Point(double a_x, double a_y);
		Point(const Point &a_point);

		~Point();

		void set_point(double a_x, double a_y);
		void set_point(const Point &a_point);
		void set_x(double a_x) { m_x = a_x; }
		void set_y(double a_y) { m_y = a_y; }
		double get_x() const { return m_x; }
		double get_y() const { return m_y; }

		bool operator ==(const Point &a_point);
		bool operator <=(const Point &a_point);
		Point operator -(const Point &a_point);
		Point operator +(const Point &a_point);

		friend std::ostream& operator <<(std::ostream &os, const Point &a_point);



};

#endif /* POINT_H_ */
