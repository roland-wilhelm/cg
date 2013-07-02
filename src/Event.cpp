/*
 * Event.cpp
 *
 *  Created on: 16.06.2013
 *      Author: Max
 */

#include "Event.h"
#include "log.h"

Event::Event(Point* a_punkt, Line* a_line, MyEventtype a_type):	m_type(a_type),
																m_punkt(*a_punkt),
																m_seg(a_line),
																m_seg2(NULL)
{

}

Event::Event(Point &a_punkt, Line* a_line, MyEventtype a_type):	m_type(a_type),
																m_punkt(a_punkt),
																m_seg(a_line),
																m_seg2(NULL)
{

}

Event::Event(Point *a_punkt, Line* a_line, Line* a_line2):	m_type(INTERSECTION),
															m_punkt(*a_punkt),
															m_seg(a_line),
															m_seg2(a_line2)
{

}

Event::Event(Point &a_punkt, Line* a_line, Line* a_line2):	m_type(INTERSECTION),
															m_punkt(a_punkt),
															m_seg(a_line),
															m_seg2(a_line2)
{

}


Event::~Event() {


}

bool Event::operator == (const Event &a_event){

	if( ( m_punkt == a_event.get_point() ) && ( m_type == a_event.m_type ) )
		return true;

		return false;
}

bool Event::operator < (Event& a_event){

	if(this->get_x() < a_event.get_x())
		return true;
	else if( this->get_x() == a_event.get_x() ) { //Behandlung von gleichen x-Werten
		//Startpunkt immer zuerst
		if(this->gettype() == STARTPUNKT)
			return true;
		//Endpunkt immer als letztes
		else if(this->gettype() == ENDPUNKT)
			return false;
		//Schnittpunkt vor ENDPUNKT
		else if(this->gettype() == INTERSECTION && a_event.gettype() == ENDPUNKT)
			return true;
		//aber hinter STARTPUNKT
		else
			return false;
	}
	else
		return false;

}

