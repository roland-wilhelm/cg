/*
 * EventEnd.h
 * Beschreibt das Ende eines Segments
 *
 *  Created on: 16.06.2013
 *      Author: Max
 */

#ifndef _EVENT_END_H_
#define _EVENT_END_H_

#include "Event.h"

using namespace std;

class EventEnd: public Event {

private:

public:
	EventEnd(Point* a_punkt, Line* a_line);
	~EventEnd() {}

};


#endif //_EVENT_END_H_
