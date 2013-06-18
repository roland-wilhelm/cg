/*
 * EventStart.h
 * Beschreibt den Beginn/Startpunkt eines Segments
 *
 *  Created on: 16.06.2013
 *      Author: Max
 */

#ifndef _EVENT_START_H_
#define _EVENT_START_H_

#include "Event.h"

using namespace std;

class EventStart : public Event {

private:

public:
	EventStart(Point* a_punkt, Line* a_line);
	~EventStart();
};


#endif //_EVENT_START_H_
