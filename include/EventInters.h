/*
 * EventEnd.h
 * Beschreibt den Schnittpunkt zweier Segmente
 *
 *  Created on: 16.06.2013
 *      Author: Max
 */

#ifndef _EVENT_INTERS_H_
#define _EVENT_INTERS_H_

using namespace std;

#include "Event.h"
#include "Line.h"

class EventInters: public Event {

private:
	Line* m_seg2;
public:
	EventInters(Point* a_punkt, Line* a_line1, Line* a_line2);
	~EventInters();

	void handle();
};


#endif //_EVENT_INTERS_H_
