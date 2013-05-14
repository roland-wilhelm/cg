/*
 * Line.cpp
 *
 *  Created on: 09.04.2013
 *      Author: Roland
 */

#include "Line.h"
#include "log.h"

using namespace std;

unsigned int Line::m_lines_nr = 0;

ostream& operator <<(ostream &os, Line &a_line) {

	DBG();

	os << "Start: " << a_line.m_start << "\t End: " << a_line.m_end;

	return os;
}


Line::Line() : m_start(Point()), m_end(Point()), is_line(false) {

	m_lines_nr++;
	DBG("Line number %d", m_lines_nr);
	check_is_line();
}

Line::Line(const Point &a_start, const Point &a_end) :
			m_start(Point(a_start)),
			m_end(Point(a_end)),
			is_line(false) {

	m_lines_nr++;
	DBG("Line number %d", m_lines_nr);
	check_is_line();
}

Line::Line(double a_start_x, double a_start_y, double a_end_x, double a_end_y) :
			m_start(Point(a_start_x, a_start_y)),
			m_end(Point(a_end_x, a_end_y)),
			is_line(false){

	m_lines_nr++;
	DBG("Line number %d", m_lines_nr);
	check_is_line();
}

Line::~Line() {

	m_lines_nr--;
	//DBG("Line number %d", m_lines_nr);
}

/*
 * Zur Ermittlung, ob Strecke oder Punkt
 *
 */
bool Line::check_is_line() {

	DBG();

	if(m_start == m_end) {

		is_line = false;
	}
	else {

		is_line = true;
	}

	return is_line;
}

/*
 *
 */
bool Line::get_is_line() {

	DBG();

	return is_line;
}

/*
 *
 */
unsigned int Line::get_lines_nr() {

	DBG();

	return m_lines_nr;
}

/*
 * @brief
 * @param
 * @retval 1 Punkt r liegt links von [p q}
 * @retval 0 Punkt r liegt auf pq
 * @retval -1 Punkt r liegt rechts von [p q}
 * @retval -2 On failure
 *
 */
int Line::ccw(const Point &a_p, const Point &a_q, const Point &a_r) {

	double result = 0.0;
	int ret = -2;

	result = a_p.get_x() * a_q.get_y() - a_p.get_y() * a_q.get_x();
	result += a_q.get_x() * a_r.get_y() - a_q.get_y() *a_r.get_x();
	result += a_p.get_y() * a_r.get_x() - a_p.get_x() * a_r.get_y();

	DBG("CCW: ", result);

	if(result > 0)
		ret = 1;
	else if(result == 0)
		ret = 0;
	else if(result < 0)
		ret = -1;


	return ret;
}

/*
 * Ermittelt ob Schnittpunkt existiert
 *
 */
bool Line::is_intersection(Line &a_line) {

	//DBG("Line %p", &a_line);

	bool intersected = false;

	// Prüfe ob beide Strecken nur Punkte sind
	if((is_line == false) && (a_line.is_line == false)) {

		// Liegen beide Punkte aufeinander --> Schnittpunkt
		if(m_start == a_line.m_start) {

			return true;
		}
		else {

			return false;
		}

	}

	int p1p2q1 = ccw(m_start, m_end, a_line.m_start);
	int p1p2q2 = ccw(m_start, m_end, a_line.m_end);

	if((p1p2q1 == 0) && (p1p2q2 == 0)) {

		/*
		 * Die Strecken sind kollinear oder überlappen sich. Hier soll
		 * untersucht werden, ob ein oder beide Punkte auf der Strecke der anderen liegt.
		 * Exakt:
		 * 1.) q1 = p1 + lamda1(p2 - p1) --> Wenn für ein Lamda =1 der Punkt q1 sich noch auf der Strecke liegt.
		 * 2.) q2 = p1 + lamda2(p2 - p1) --> Wenn für ein Lamda =1 der Punkt q2 sich noch auf der Strecke liegt.
		 * Ergebnis:
		 * Wenn 1. und 2. gültig, dann befindet sich die Linie komplett in der anderen Linie --> Schnittpunkt
		 * Wenn 1. oder 2. gültig, dann befindet sich mind. 1 Punkt der anderen Strecke --> Schnittpunkt
		 * Wenn 1. und 2. nicht gültig, dann schneiden sich die Linien nicht
		 */

		if(a_line.m_start <= m_end) {

			intersected = true;
		}
		else if(a_line.m_start <= m_start) {

			intersected = true;
		}
		else if(a_line.m_end <= m_end) {

			intersected = true;
		}
		else if(a_line.m_end <= m_start) {

			intersected = true;
		}
		else {

			intersected = false;
		}


	}
	else {

		/*
		 * Die beiden Strecken liegen nicht auf einer Linie, ob ein
		 * Schnittpunkt vorliegt soll hier untersucht werden.
		 */
		int q1q2p1 = ccw(a_line.m_start, a_line.m_end, m_start);
		int q1q2p2 = ccw(a_line.m_start, a_line.m_end, m_end);

		if(((p1p2q1*p1p2q2) <= 0) && ((q1q2p1*q1q2p2) <= 0)) {

			// Beide Stecken schneiden sich
			intersected = true;

		}
		else {

			/*
			 * Beide Punkte einer Strecken liegen jeweils rechts und
			 * links einer anderen Strecken, ohne sich zu schneiden.
			 */

			intersected = false;
		}

	}

	return intersected;
}

/*
 * Equal Operator zur Ermittlung ob 2 Strecken
 * identisch sind. Identische Strecken = Schnittpunkt
 *
 */
bool Line::operator ==(const Line &a_line) {

	DBG("Line %p", &a_line);

	if((m_start == a_line.m_start) && (m_end == a_line.m_end)) {

		return true;
	}
	else {

		return false;
	}
}
