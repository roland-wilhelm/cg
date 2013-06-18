/*
 * Sweep.h
 *
 *  Created on: 10.06.2013
 *      Author: Max
 */

#ifndef _SWEEP_H_
#define _SWEEP_H_

#include <list>
#include "Event.h"
#include "EventInters.h"
#include "EventStart.h"
#include "EventEnd.h"


using namespace std;

class Sweep {

private:

	//Point** Eventqueue;
	list<Event> eventqueue;
	//Line** Segmentqueue;
	list<Line> segmentqueue;
	Event* ereignis;
	//Point** Intersectionqueue;
	list<EventInters> output;

public:
	Sweep();
	~Sweep();

	void leftendpoint();
	void rightendpoint();
	void treatintersection();

	void addevent(const Event *a_event) { eventqueue.push_front(*a_event); }
	//void addseg(const Line *a_seg) { segmentqueue.push_front(*a_seg); }
	Line* addseg(Line const *a_seg);
	// void addinter(const Event *a_event) { output.push_front((EventInters)*a_event); }
	void delevent() { eventqueue.pop_front(); }
	void delseg() { segmentqueue.pop_front(); }
	void sortevent() { eventqueue.sort(); }
	void sortseg() { segmentqueue.sort(); }
	double getxposition();


};


#endif //_SWEEP_H_
