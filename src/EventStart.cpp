/*
 * EventStart.cpp
 * Event, das den Beginn eines Segments beschreibt
 *
 *  Created on: 16.06.2013
 *      Author: Max
 */

#include "EventStart.h"
#include "Event.h"
#include "log.h"

EventStart::EventStart(Point* a_punkt, Line* a_line): 	Event(a_punkt,a_line,STARTPUNKT)
{

}

EventStart::~EventStart() {


}
