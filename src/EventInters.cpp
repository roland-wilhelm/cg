/*
 * EventInters.cpp
 * Event, das den Schnittpunkt zweier Segmente beschreibt
 *
 *  Created on: 16.06.2013
 *      Author: Max
 */

#include "EventInters.h"
#include "log.h"

EventInters::EventInters(Point* a_punkt, Line* a_line1, Line* a_line2):	Event(a_punkt, a_line1, INTERSECTION),
																		m_seg2(a_line2)
{

}

EventInters::~EventInters() {


}

void EventInters::handle(){

}
