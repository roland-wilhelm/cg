/*
 * EventEnd.cpp
 * Event, das das Ende eines Segments beschreibt
 *
 *  Created on: 16.06.2013
 *      Author: Max
 */

#include "EventEnd.h"
#include "log.h"

EventEnd::EventEnd(Point* a_punkt, Line* a_line): 	Event(a_punkt, a_line, ENDPUNKT)
{

}
