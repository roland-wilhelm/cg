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
		static unsigned int m_lines_nr;
		int ccw_max(Point &a_p, Point &a_q, Point &a_r);

	public:

		Line();
		Line(const Point &a_start, const Point &a_end);
		Line(double a_start_x, double a_start_y, double a_end_x, double a_end_y);

		virtual ~Line();

		static unsigned int get_lines_nr();
		bool is_intersection_max(Line &a_line);

		bool operator ==(const Line &a_line);
		friend std::ostream& operator <<(std::ostream &os, Line &a_line);
};

#endif /* LINE_H_ */
