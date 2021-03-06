/*
 * Line.h
 *
 *  Created on: 09.04.2013
 *      Author: Roland
 */

#ifndef LINE_H_
#define LINE_H_

#include "Point.h"

#include <ostream>

class Line {

	private:

		Point m_start;
		Point m_end;
		bool is_line;
		static unsigned int m_lines_nr;

		bool check_is_line();

	public:

		Line();
		Line(const Point &a_start, const Point &a_end);
		Line(double a_start_x, double a_start_y, double a_end_x, double a_end_y);

		~Line();

		Point& get_start_point() { return m_start; }
		Point& get_end_point() { return m_end; }
		bool get_is_line();
		static int ccw(const Point &a_p, const Point &a_q, const Point &a_r);
		static unsigned int get_lines_nr();
		bool is_intersection(Line &a_line);

		bool operator ==(const Line &a_line);
		friend std::ostream& operator <<(std::ostream &os, Line &a_line);
};

#endif /* LINE_H_ */
