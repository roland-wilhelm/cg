/*
 * Event.cpp
 *
 *  Created on: 16.06.2013
 *      Author: Max
 */

#include "Event.h"
#include "log.h"

Event::Event(Point* a_punkt, Line* a_line, MyEventtype a_type): 	m_type(a_type),
																	m_punkt(a_punkt),
																	m_seg(a_line)

{

}

Event::~Event() {


}

bool Event::operator == (const Event& a_event){
	if(this->m_punkt->get_x() == a_event.m_punkt->get_x()
	   && this->m_punkt->get_y() == a_event.m_punkt->get_y())
		return true;
	else
		return false;

}

bool operator < (Event& a_event1, Event& a_event2){


	if(a_event1.get_x() < a_event2.get_x())
		return true;
	else if( ( a_event1.get_x() == a_event2.get_x() )
			&& ( a_event1.get_y() < a_event2.get_y() ) )
		return true;
	else
		return false;

}

