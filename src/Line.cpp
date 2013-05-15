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


Line::Line() : m_start(Point()), m_end(Point()) {
	m_lines_nr++;
	DBG("Line number %d", m_lines_nr)
}

Line::Line(const Point &a_start, const Point &a_end) :
			m_start(Point(a_start)),
			m_end(Point(a_end)){

	m_lines_nr++;
	DBG("Line number %d", m_lines_nr);
}

Line::Line(double a_start_x, double a_start_y, double a_end_x, double a_end_y) :
			m_start(Point(a_start_x, a_start_y)),
			m_end(Point(a_end_x, a_end_y)){

	m_lines_nr++;
	DBG("Line number %d", m_lines_nr);
}

Line::~Line() {

	m_lines_nr--;
	//DBG("Line number %d", m_lines_nr);
}

/*
 * Ermittelt ob r rechts oder links der Strecke liegt. Liegt er links,
 * ist das Dreieck gegen den Uhrzeigersinn (anticlockwise-> kleiner 0) aufgespannt.
 * Liegt es rechts (-> >0) wird es im Urzeigersinn aufgespannt, liegt der Punkt
 * auf der Strecke ( -> 0 ) ergibt sich ein Schnittpunkt.
 */

//-> umbenennen in ccw()!!
int Line::ccw(Point &a_p, Point &a_q, Point &a_r){

	//x und y koordinaten der Punkte
	double result;

	result = (a_p.get_y()*a_r.get_x())-(a_q.get_y()*a_r.get_x());
	result += (a_q.get_x()*a_r.get_y())-(a_p.get_x()*a_r.get_y());
	result += (a_p.get_x()*a_q.get_y())-(a_p.get_y()*a_q.get_x());

	if (result < 0.0)
		return -1;
	else if(result == 0.0)
		return 0;
	else
		return +1;
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
	//both give 0 -> kollinear oder überlappend
	if ( ccw(m_start, m_end, a_line.m_start) == 0 && ccw(m_start, m_end, a_line.m_end) == 0) {

		// prüfung ob Ausgangsline ein Punkt ist -> vereinfachter überlappungstest, da Drehung der
		// Linie bei einem Punkt nicht funktioniert
		if ( m_start == m_end ) {

			//Punkt liegt auf Linie??
			if( (m_start > a_line.m_start && m_start < a_line.m_end)
				|| (m_start < a_line.m_start && m_start > a_line.m_end)){
				return true;
			}
			else
				return false;

		}//ende if (m_start==m_end)

		else { //überlappungstest (line <-> line oder line <-> punkt)
			//p über m_start - drehung um -90°, q über m_end - drehung um 90° des gegengesetzten Vektor
			Point p(m_end.get_y()-m_start.get_y(), m_end.get_x()-m_start.get_x()),
				  q(m_start.get_y()-m_end.get_y(), m_end.get_x()-m_start.get_x());

			//Start-Punkt auf der Linie (inkl Ränder)
			if( ccw(m_start, a_line.m_start, p)*ccw(m_end,q,a_line.m_start) >= 0 ){
				return true;
			}
			//End-Punkt auf der Linie (inkl Ränder)
			else if(ccw(m_start, a_line.m_end, p)*ccw(m_end,q,a_line.m_end) >= 0){
				return true;
			}
			else
				return false;
		}
	}
	//one clockwise, the other anti-clockwise
	//Punkte liegen je links und rechts
	else if ( (ccw(m_start, m_end, a_line.m_start)*ccw(m_start, m_end, a_line.m_end)) <= 0
		  && ccw(a_line.m_start, a_line.m_end, m_start)*ccw(a_line.m_start, a_line.m_end, m_end) <= 0 ) {

		//->Schnittpunkt!!
		return true;
	}

	//both clockwise / anticlockwise -> on the same side
	return false;
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
