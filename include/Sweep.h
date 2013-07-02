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
	list<Event> eventqueue;
	list<Line> segmentqueue;
	Event *ereignis;
	list<Event> output;

public:
	Sweep();
	~Sweep();

	void leftendpoint();
	void rightendpoint();
	void treatintersection();

	void addevent(Point *a_point, Line *a_line);
	void addevent(Event a_event);
	void sortevent() { eventqueue.sort(); }
	void delevent(Event *a_event) { eventqueue.remove(*a_event); }
	void print_eventqueue();

	Line* addseg(Line *a_seg);
	void sortseg() { segmentqueue.sort(); }
	Line* getneighbour_high(Line *a_seg);
	Line* getneighbour_low(Line *a_seg);
	Line* getseg(Line* a_seg);
	void delseg(Line* a_seg) { segmentqueue.remove(*a_seg); }
	void print_segmentqueue();

	void addinter(Event a_event) { output.push_front(a_event);
									output.unique(); }
	void print_outputqueue();

	double getxposition();
	void calcinters();
	unsigned int getinters() { return output.size(); }


};


#endif //_SWEEP_H_
