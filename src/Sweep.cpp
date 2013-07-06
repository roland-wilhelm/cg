/*
 * Sweep.cpp
 *
 *  Created on: 10.06.2013
 *      Author: Max
 */
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <iterator>

#include "Sweep.h"
#include "Event.h"
#include "log.h"

Sweep::Sweep():	eventqueue(),
				segmentqueue(),
				ereignis(NULL),
				output() {
	DBG();
	eventqueue.clear();
	segmentqueue.clear();
	output.clear();
}

Sweep::~Sweep(){
	eventqueue.clear();
	segmentqueue.clear();
	output.clear();
}

/*
 * Behandlung von Startpunkten
 */
void Sweep::leftendpoint(){
	DBG();
	Line *segA=NULL, *segB=NULL;
	Line *aktseg=NULL;
	Point interp1, interp2;

	//Linie in Segmentqueue an richtiger stelle einordnen
	//Test ob Schnittpunkt mit den Nachbarsegmenten
	addseg(aktseg=ereignis->get_line());

	segA = getneighbour_high(aktseg);
	segB = getneighbour_low(aktseg);

	if ( segA != NULL ) {
		if( aktseg->is_intersection_max(segA) == true ) {
			//Koordinaten vom Schnittpunkt berechnen
			//und in Queue einsortieren (Event und Output)
			//nur in Outputqueue falls Schnittpunkt hinter/auf sweep Position liegt!
			interp1 = aktseg->intersectionpoint(segA);
			Event Inter1(interp1,aktseg,segA);
			if(Inter1.get_x() > getxposition()) {
				addinter( Inter1 );
			}
			addevent( Inter1 );
		}
	}

	if ( segB != NULL ) {
		if(aktseg->is_intersection_max(segB) == true) {
			//Koordinaten vom Schnittpunkt berechnen
			//und in Queue einsortieren (Event und Output)
			//nur in Outputqueue falls Schnittpunkt hinter/auf sweep Position liegt!
			interp2 = aktseg->intersectionpoint(segB);
			Event Inter2(interp2, aktseg, segB);
			if(Inter2.get_x() > getxposition()) {
				addevent( Inter2 );
			}
			addinter( Inter2 );

		}
	}


}

/*
 * Behandlung von Endpunkten
 */
void Sweep::rightendpoint(){
	Line *segA=NULL, *segB=NULL;
	Point interp1;

	DBG();

	//Segment vom Event in Queue finden und Nachbarn merken
	//Dann Segment aus Queue entfernen und Nachbarn auf
	//Schnittpunkt prüfen, falls nicht vorher erledigt

	segA = getneighbour_high(ereignis->get_line());
	segB = getneighbour_low(ereignis->get_line());

	delseg(ereignis->get_line());
	if ( segA != NULL && segB != NULL) {
		if ( segA->is_intersection_max(segB) == true ) {
			//Koordinaten vom Schnittpunkt berechnen
			//und in Queue einsortieren (Event und Output)
			//nur in Outputqueue falls Schnittpunkt vor sweep Position liegt!
			interp1 = segA->intersectionpoint(segB);
			Event Inter1(interp1, segB, segA);
			if(Inter1.get_x() > getxposition()) {
				addevent( Inter1 );
			}
			addinter( Inter1 );
		}
	}
}

/*
 * Behandlung von Schnittpunkten
 */
void Sweep::treatintersection(){
	Line *segA=ereignis->get_line(), *segB=ereignis->get_line2(), *neighbourA=NULL, *neighbourB=NULL;
	Point interp1, interp2;
	list<Line>::iterator it_A = find(segmentqueue.begin(),segmentqueue.end(), *segA);
	list<Line>::iterator it_B = find(segmentqueue.begin(),segmentqueue.end(),*segB);

	DBG();

	/*
	 * NeighbourA ist der Nachbar von A nach dem Swappen
	 */
	if ( *it_B == *(++it_A) ){
		neighbourA = getneighbour_high(segB);
		neighbourB = getneighbour_low(segA);
		//Swap Elements
		//segB vor segA einordnen und altes Element segB löschen
		iter_swap(--it_A, it_B);

	}
	else if ( *it_B == *(--(--it_A)) ) {
		neighbourA = getneighbour_low(segB);
		neighbourB = getneighbour_high(segA);
		//Swap Elements
		//segA vor segB einordnen und altes Element segA löschen
		iter_swap(++it_A, it_B);

	}

	//Nun mit den Nachbarsegmenten auf Schnittpunkte prüfen,
	//falls noch nicht erledigt
	if ( neighbourA != NULL ) {
			if ( segA->is_intersection_max(neighbourA) == true ) {
				//Koordinaten vom Schnittpunkt berechnen
				//und in Queue einsortieren (Event und Output)
				//nur in Outputqueue falls Schnittpunkt vor sweep Position liegt!
				interp1 = segA->intersectionpoint(neighbourA);
				Event Inter1(interp1, neighbourA, segA);
				if(Inter1.get_x() > getxposition()) {
					addevent( Inter1 );
				}
				addinter( Inter1 );
			}
		}
	if ( neighbourB != NULL ) {
			if ( segB->is_intersection_max(neighbourB) == true ) {
				//Koordinaten vom Schnittpunkt berechnen
				//und in Queue einsortieren (Event und Output)
				//nur in Outputqueue falls Schnittpunkt vor sweep Position liegt!
				interp2 = segB->intersectionpoint(neighbourB);
				Event Inter2(interp2, neighbourB, segB);
				if(Inter2.get_x() > getxposition()) {
					addevent( Inter2 );
				}
				addinter( Inter2 );
			}
		}
}

/*
 * findet oberen Nachbar in der Liste
 * ist er nicht vorhanden wird NULL übergeben
 */
Line* Sweep::getneighbour_high(Line *a_seg){
	list<Line>::iterator it;

	it = find(segmentqueue.begin(), segmentqueue.end(), *a_seg);
	if(it != segmentqueue.end() && it != segmentqueue.begin() )
		return &(*(++it));
	else
		return NULL;
}

/*
 * findet unteren Nachbar in der Liste
 * ist er nicht vorhanden wird NULL übergeben
 */
Line* Sweep::getneighbour_low(Line *a_seg){
	list<Line>::iterator it;

	it = find(segmentqueue.begin(), segmentqueue.end(), *a_seg);

	if (it != segmentqueue.begin() && it != segmentqueue.begin())
		return &(*(--it));
	else
		return NULL;
}

/*
 * findet Segment in der Liste
 * ist es nicht vorhanden wird NULL übergeben
 */
Line* Sweep::getseg(Line* a_seg){
	list<Line>::iterator it;

	it = find(segmentqueue.begin(), segmentqueue.end(), *a_seg);
	if ( it != segmentqueue.end() )
		return &(*it);
	else
		return NULL;
}

/*
 * Funktion soll ausgeben wo sich der Sweep gerade befindet
 */
double Sweep::getxposition() {
	double aktpos = -1.0;

	if( eventqueue.empty() != true )
		aktpos = ereignis->get_x();

	return aktpos;
}

/*
 * Funktion um die Eventqueue mit Start- und Endpunkten zu füllen
 */
void Sweep::addevent(Point* a_point, Line* a_line){
	//Eventqueue vor hinzufügen
	DBG("Eventqueue vor hinzufügen eines Event")
	print_eventqueue();

	//a_point ist startpunkt
	if ( a_point == a_line->getstart() ){
		eventqueue.push_front( Event(a_point, a_line, STARTPUNKT) );
	}
	//a_point ist endpunkt
	else {
		eventqueue.push_front( Event(a_point, a_line, ENDPUNKT) );
	}

	DBG("Eventqueue vor hinzufügen eines Event")
	print_eventqueue();
}

void Sweep::addevent(Event a_event) {
	//Eventqueue vor hinzufügen
	DBG("Eventqueue vor hinzufügen einer Intersection")
	print_eventqueue();
	eventqueue.push_front(a_event);


	eventqueue.sort();
	DBG("Eventqueue nach hinzufügen einer Intersection")
	print_eventqueue();
}

/*
 * Einfügen eines Segments an der richtigen Stelle oder am Ende
 */
Line* Sweep::addseg(Line* a_seg) {
	//segmentqueue.push_front(*a_seg);
	//sortseg();
	list<Line>::iterator newseg;

	//Segment vor hinzufügen
	DBG("Segment vor hinzufügen eines neuen")
	print_segmentqueue();

	if ( segmentqueue.empty() ) {
		segmentqueue.push_front(*a_seg);
		return a_seg;
	}
	else {

		for ( newseg = segmentqueue.begin(); newseg != segmentqueue.end(); ++newseg ) {
		//Iterator solange y-achsenabschnitt > ist, dann an dieser Position einfügen
		//und die Referenz auf das Element zurückgeben
			if ( newseg->get_yvalue(getxposition()) > a_seg->get_yvalue(getxposition()) ) {
				segmentqueue.insert (newseg, *a_seg);
				return a_seg;
			}
		}
	}
	segmentqueue.insert (newseg, *a_seg);
	segmentqueue.unique();

	//Segment vor hinzufügen
	DBG("Segment nach hinzufügen eines neuen")
	print_segmentqueue();
	return a_seg;

}

void Sweep::print_eventqueue(){
	list<Event>::iterator it;

	for(it = eventqueue.begin(); it != eventqueue.end(); ++it)
		cout << "Event[P(" << it->get_x() <<"," << it->get_y() << ")-T" << it->gettype() <<"], ";

	cout << endl;
}


void Sweep::print_segmentqueue(){
	list<Line>::iterator it;

	for(it = segmentqueue.begin(); it != segmentqueue.end(); ++it)
		cout << "Segment[P1(" << it->getstart()->get_x() << "/" << it->getstart()->get_y() << "), P2("
		     << it->getend()->get_x() << "/"  << it->getend()->get_y() << ")], ";

	cout << endl;
}

void Sweep::print_outputqueue(){
	list<Event>::iterator it;

	for(it = output.begin(); it != output.end(); ++it)
		cout << "Schnitt(" << it->get_x() << "," << it->get_y() << "), ";

	cout << endl;
}

void Sweep::calcinters(){
bool test = false;
list<Event>::iterator neweve;

	//Eventqueue nach x sortieren
	eventqueue.sort();
	while( (test = eventqueue.empty()) == false) {
		//zwischenergebnisse
		print_eventqueue();
		print_segmentqueue();
		print_outputqueue();

		neweve = eventqueue.begin();
		ereignis = &(*neweve);

		switch(ereignis->gettype()){
			case STARTPUNKT:
				leftendpoint( );
				break;

			case ENDPUNKT:
				rightendpoint( );
				break;

			case INTERSECTION:
				treatintersection( );
				break;

			default:
				exit(1);
				break;
		}
		//Erstes Event aus Queue entfernen
		delevent(ereignis);
	} //end while


}

