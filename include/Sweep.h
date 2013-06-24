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

/*
#include "EventInters.h"
#include "EventStart.h"
#include "EventEnd.h"
*/

using namespace std;

class Sweep {

private:

	//Point** Eventqueue;
	list<Event> eventqueue;
	//Line** Segmentqueue;
	list<Line> segmentqueue;
	list<Event>::iterator ereignis;
	//Point** Intersectionqueue;
	list<Event> output;

public:
	Sweep();
	~Sweep();

	void leftendpoint();
	void rightendpoint();
	void treatintersection();

	void addevent(Point *a_point, Line *a_line);
	void addevent(Event *a_event);
	void sortevent() { eventqueue.sort(); }
	void delevent() { eventqueue.pop_front(); }

	//void addseg(const Line *a_seg) { segmentqueue.push_front(*a_seg); }
	Line* addseg(Line *a_seg);
	void sortseg() { segmentqueue.sort(); }
	Line& getneighbour_high(Line* a_seg);
	Line& getneighbour_low(Line* a_seg);
	Line& getseg(Line* a_seg);
	void delseg() { segmentqueue.pop_front(); }

	void addinter(Event *a_event) { output.push_front(*a_event);
									output.unique(); }

	double getxposition();
	void calcinters();
	unsigned int getinters() { return output.size(); }


};


#endif //_SWEEP_H_
