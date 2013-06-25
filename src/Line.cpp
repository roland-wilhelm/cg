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
int Line::ccw_max(Point &a_p, Point &a_q, Point &a_r){

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
bool Line::is_intersection_max(Line &a_line) {

	//DBG("Line %p", &a_line);

	//siehe Vorlesung Folie 2-19
	//both give 0 -> kollinear oder überlappend
	if ( ccw_max(m_start, m_end, a_line.m_start) == 0 && ccw_max(m_start, m_end, a_line.m_end) == 0) {

		// prüfung ob Ausgangsline ein Punkt ist -> vereinfachter überlappungstest, da Drehung der
		// Linie bei einem Punkt nicht funktioniert
		if ( m_start == m_end ) {

			//Punkt liegt auf Linie??
			if( ( m_start > a_line.m_start && m_start < a_line.m_end )
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
			if( ccw_max(m_start, a_line.m_start, p)*ccw_max(m_end,q,a_line.m_start) >= 0 ){
				return true;
			}
			//End-Punkt auf der Linie (inkl Ränder)
			else if(ccw_max(m_start, a_line.m_end, p)*ccw_max(m_end,q,a_line.m_end) >= 0){
				return true;
			}
			else
				return false;
		}
	}
	//one clockwise, the other anti-clockwise
	//Punkte liegen je links und rechts
	else if ( (ccw_max(m_start, m_end, a_line.m_start)*ccw_max(m_start, m_end, a_line.m_end)) <= 0
		  && ccw_max(a_line.m_start, a_line.m_end, m_start)*ccw_max(a_line.m_start, a_line.m_end, m_end) <= 0 ) {

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

bool Line::operator < (Line &a_line){
	double ywert1, ywert2;

	ywert1 = this->get_yvalue( this->getstart()->get_x() );
	ywert2 = a_line.get_yvalue( this->getstart()->get_x() );

	if( ywert1 < ywert2 )
		return true;
	else
		return false;

}

/*
 * Funktion gibt den y-Achsenabschnitt zurück.
 * Nach ihm werden die Lines in der Queue sortiert,
 * falls es keinen Schnittpunkt zwischen den Strecken gibt
 * bleibt die Reihenfolge der Strecken (in Y-Richtung)
 * gleich
 */

double Line::get_yvalue(double a_xwert) {
	DBG();

	double m,t;
	//y=mx+t -> t ist Achsenabschnitt

	m = ( m_start.get_y() - m_end.get_y() ) / ( m_start.get_x() - m_end.get_y() );
	t = m_start.get_y() - ( m * m_start.get_x() );

	return ((m*a_xwert)+t);
}

Point* Line::getstart(){
	if (m_start < m_end)
		return &m_start;
	else
		return &m_end;
}

Point* Line::getend(){
	if (m_start < m_end)
			return &m_end;
		else
			return &m_start;
}

/*
 * Koordinaten des Schnittpunktes berechnen
 */
Point Line::intersectionpoint(Line &a_line) {
	Point temp;
	Point *a_start( a_line.getstart() ), *a_end( a_line.getend() );
	double m_m, a_m, m_t, a_t;
	double xwert=0.0, ywert=0.0;

	//y=mx+t
	//m_m Steigung der Memberline
	//m_t Achsenabschnitt der Memberline
	m_m = ( m_start.get_y() - m_end.get_y() ) / ( m_start.get_x() - m_end.get_y() );
	m_t = m_start.get_y() - ( m_m * m_start.get_x() );

	//a_m Steigung der Argumentline
	//a_t Achsenabschnitt der Argumentline
	a_m = (a_start->get_y() - a_end->get_y() ) / ( a_start->get_x() - a_end->get_y() );
	a_t = a_start->get_y() - ( a_m * a_start->get_x() );

	xwert = (a_t-m_t) / (m_m-a_m);
	ywert = (m_m*xwert) + m_t;

	temp.set_point(xwert,ywert);
	return temp;
}
