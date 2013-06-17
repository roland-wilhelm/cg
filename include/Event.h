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

class Event {

private:
	MyEventtype m_type;
	Point* m_punkt;
	Line* m_seg;
public:
	Event();
	Event(Point* a_punkt, Line* a_line, MyEventtype m_type);
	virtual ~Event();

	//Funktionen zum Event
	int gettype(){ return m_type; }	//1 - Startpunkt, 2 - Endpunkt, 3 - Intersection, ELSE - FEHLER
	virtual void handle() = 0; //handle() ist je nach Eventtyp die entsprechende Aktion

	//Funktionen f�r List
	bool operator == (const Event& a_event);
	bool operator < (Event& a_event);

	//Funktionen zum Punkt
	double get_x(){ return m_punkt->get_x(); }
	double get_y(){ return m_punkt->get_y(); }

	//Funktionen zum Segment
	const unsigned int get_linenr(){return m_seg->get_lines_nr();}
	const Line* get_line() { return m_seg; }

};


#endif //_EVENT_H_
