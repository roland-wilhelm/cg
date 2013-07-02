/*
 * Event.h
 *
 *  Created on: 16.06.2013
 *      Author: Max
 */

#ifndef _EVENT_H_
#define _EVENT_H_

using namespace std;

#include "Point.h"
#include "Line.h"

//enum um Eventyp festzulegen
enum MyEventtype { 	STARTPUNKT = 1,
					ENDPUNKT = 2,
					INTERSECTION = 3};

class Line;
class Point;

class Event {

private:
	MyEventtype m_type;
	Point m_punkt;
	Line* m_seg;
	Line* m_seg2;
public:
	Event(Point *a_punkt, Line *a_line, MyEventtype m_type);
	Event(Point &a_punkt, Line *a_line, MyEventtype m_type);
	Event(Point *a_punkt, Line *a_line, Line *a_line2);
	Event(Point &a_punkt, Line *a_line, Line *a_line2);
	virtual ~Event();

	//Funktionen zum Event
	int gettype() const { return m_type; }	//1 - Startpunkt, 2 - Endpunkt, 3 - Intersection, ELSE - FEHLER

	//Funktionen für List
	bool operator == (const Event &a_event);
	bool operator < (Event& a_event);

	//Funktionen zum Punkt
	Point get_point() const { 	return m_punkt; }
	double get_x() const { return m_punkt.get_x(); }
	double get_y() const { return m_punkt.get_y(); }

	//Funktionen zum Segment
	unsigned int get_linenr() const {return m_seg->get_lines_nr();}
	Line* get_line() const { return m_seg; }
	Line* get_line2() const { return m_seg2; }

};


#endif //_EVENT_H_
