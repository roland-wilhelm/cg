/*
 * Event.cpp
 *
 *  Created on: 16.06.2013
 *      Author: Max
 */

#include "Event.h"
#include "log.h"

Event::Event(Point* a_punkt, Line* a_line, MyEventtype a_type):	m_type(a_type),
																m_punkt(a_punkt),
																m_seg(a_line),
																m_seg2(NULL)
{

}

Event::Event(Point* a_punkt, Line* a_line, Line* a_line2):	m_type(INTERSECTION),
															m_punkt(a_punkt),
															m_seg(a_line),
															m_seg2(a_line2)
{

}

Event::~Event() {


}

bool Event::operator == (const Event& a_event){
	if(this->m_punkt->get_x() == a_event.m_punkt->get_x()
	   && this->m_punkt->get_y() == a_event.m_punkt->get_y()
	   && this->m_type == a_event.m_type)
		return true;
	else
		return false;

}

bool operator < (Event& a_event1, Event& a_event2){

	if(a_event1.get_x() < a_event2.get_x())
		return true;
	else if( a_event1.get_x() == a_event2.get_x() ) { //Behandlung von gleichen x-Werten
		//Startpunkt immer zuerst
		if(a_event1.gettype() == STARTPUNKT)
			return true;
		//Endpunkt immer als letztes
		else if(a_event1.gettype() == ENDPUNKT)
			return false;
		//Schnittpunkt vor ENDPUNKT
		else if(a_event1.gettype() == INTERSECTION && a_event2.gettype() == ENDPUNKT)
			return true;
		//aber hinter STARTPUNKT
		else
			return false;
	}
	else
		return false;

}

