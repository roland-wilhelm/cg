/*
 * Line.cpp
 *
 *  Created on: 09.04.2013
 *      Author: Roland
 */

#include "Line.h"
#include "LineFile.h"
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
 * Ermittelt ob r rechts oder links der Strecke liegt. Liegt er links,
 * ist das Dreieck gegen den Uhrzeigersinn (anticlockwise-> kleiner 0) aufgespannt.
 * Liegt es rechts (-> >0) wird es im Urzeigersinn aufgespannt, liegt der Punkt
 * auf der Strecke ( -> 0 ) ergibt sich ein Schnittpunkt.
 */
int Line::clockwise(Point &a_p, Point &a_q, Point &a_r){

	//x und y koordinaten der Punkte
	double p1,p2,q1,q2,r1,r2, result;

	p1=a_p.get_x();
	p2=a_p.get_y();
	q1=a_q.get_x();
	q2=a_q.get_y();
	r1=a_r.get_x();
	r2=a_r.get_y();


	result=(p2*r1)-(q2*r1)+(q1*r2)-(p1*r2)-(p2*q1)+(p1*q2);

	return result;
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
 * Ermittelt ob Schnittpunkt existiert
 *
 */
bool Line::is_intersection(Line &a_line) {

	//DBG("Line %p", &a_line);

	//siehe Vorlesung Folie 2-19
	//both give 0 -> kollinear und überlappend
	if ( clockwise(m_start, m_end, a_line.m_start) == 0 && clockwise(m_start, m_end, a_line.m_end) == 0) {
		//Test ob Koliniear oder Überlappend

	}
	//one clockwise, the other anti-clockwise
	else if ( (clockwise(m_start, m_end, a_line.m_start)*clockwise(m_start, m_end, a_line.m_end)) <= 0
		  && clockwise(a_line.m_start, a_line.m_end, m_start)*clockwise(a_line.m_start, a_line.m_end, m_end) <= 0 ) {

		//->Schnittpunkt!!
	}

	//both clockwise / anticlockwise -> on the same side
	else {
		//kein Schnittpunkt!
	}

	return true;
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
