/*
 * Sweep.cpp
 *
 *  Created on: 10.06.2013
 *      Author: Max
 */

#include "Sweep.h"
#include "Event.h"
#include "log.h"

Sweep::Sweep():	eventqueue(),
				segmentqueue(),
				ereignis(eventqueue.begin()),
				output() {
	DBG();
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
	Line& segA=NULL, segB=NULL;
	Line *aktseg;

	//Linie in Segmentqueue an richtiger stelle einordnen
	//Test ob Schnittpunkt mit den Nachbarsegmenten
	addseg(aktseg=ereignis->get_line());

	segA = getneighbour_high(ereignis->get_line());
	segB = getneighbour_low(ereignis->get_line());

	if ( segA != NULL ) {
		if(aktseg->is_intersection_max(segA) == true) {
			//Koordinaten vom Schnittpunkt berechnen
			//und in Queue einsortieren (Event und Output)
			//nur in Outputqueue falls Schnittpunkt hinter/auf sweep Position liegt!

			Event Inter1(aktseg->intersectionpoint(segA),aktseg,segA);
			if(Inter1.get_x() > getxposition()) {
				addinter( &Inter1 );
			}
			addevent( &Inter1 );
		}
	}

	if ( segB != NULL ) {
		if(aktseg->is_intersection_max(segB) == true) {
			//Koordinaten vom Schnittpunkt berechnen
			//und in Queue einsortieren (Event und Output)
			//nur in Outputqueue falls Schnittpunkt hinter/auf sweep Position liegt!
			Event Inter1(aktseg->intersectionpoint(segA),aktseg,segA);
			if(Inter1.get_x() > getxposition()) {
				addinter( &Inter1 );
			}
			addevent( &Inter1 );
		}
	}


}

/*
 * Behandlung von Endpunkten
 */
void Sweep::rightendpoint(){
	Line &segA, &segB;

	DBG();

	//Segment vom Event in Queue finden und Nachbarn merken
	//Dann Segment aus Queue entfernen und Nachbarn auf
	//Schnittpunkt prüfen, falls nicht vorher erledigt

	segA = getneighbour_high(ereignis->get_line());
	segB = getneighbour_low(ereignis->get_line());

	delevent();
	if ( segA != NULL ) {
		if ( segA.is_intersection_max(segB) == true ) {
			//Koordinaten vom Schnittpunkt berechnen
			//und in Queue einsortieren (Event und Output)
			//nur in Outputqueue falls Schnittpunkt vor sweep Position liegt!
			Event Inter1(segA.intersectionpoint(segB),segB,segA);
			if(Inter1.get_x() < getxposition()) {
				addinter( &Inter1 );
			}
			addevent( &Inter1 );
		}
	}
}

/*
 * Behandlung von Schnittpunkten
 */
void Sweep::treatintersection(){
	Line& segA, segB, neighbourA, neighbourB;
	list<Line>::iterator it;

	DBG();
	segA = ereignis->get_line();
	segB = ereignis->get_line2();

	//die beiden Segmente des Schnittpunkts in der Seqmentqueue
	//finden und vertauschen (swappen)
	for (it = segmentqueue.begin(); it != segmentqueue.end(); ++it) {
		if ( segA == it)
			segA == it;
			break;
	}

	/*
	 * NeighbourA ist der Nachbar von A nach dem Swappen
	 */
	if ( segB == it++){
		segB = ++it;
		neighbourA = getneighbour_high(&segB);
		neighbourB = getneighbour_low(&segA);
		//Swap Elements
		it = segB;
		++it = segA;
	}
	else if (segB == it--) {
		segB = --it;
		neighbourA = getneighbour_low(&segB);
		neighbourB = getneighbour_high(&segA);
		//Swap Elements
		it = segB;
		--it = segA;
	}

	//Nun mit den Nachbarsegmenten auf Schnittpunkte prüfen,
	//falls noch nicht erledigt
	if ( neighbourA != NULL ) {
			if ( segA.is_intersection_max(neighbourA) == true ) {
				//Koordinaten vom Schnittpunkt berechnen
				//und in Queue einsortieren (Event und Output)
				//nur in Outputqueue falls Schnittpunkt vor sweep Position liegt!
				Event Inter1(segA.intersectionpoint(neighbourA),neighbourA,segA);
				if(Inter1.get_x() < getxposition()) {
					addinter( &Inter1 );
				}
				addevent( &Inter1 );
			}
		}
	if ( neighbourB != NULL ) {
			if ( segB.is_intersection_max(neighbourB) == true ) {
				//Koordinaten vom Schnittpunkt berechnen
				//und in Queue einsortieren (Event und Output)
				//nur in Outputqueue falls Schnittpunkt vor sweep Position liegt!
				Event Inter1(segB.intersectionpoint(neighbourB),neighbourB,segB);
				if(Inter1.get_x() < getxposition()) {
					addinter( &Inter1 );
				}
				addevent( &Inter1 );
			}
		}
}

/*
 * findet oberen Nachbar in der Liste
 * ist er nicht vorhanden wird NULL übergeben
 */
Line& Sweep::getneighbour_high(Line* a_seg){
	list<Line>::iterator it;
	for ( it=segmentqueue.begin(); it != segmentqueue.end(); ++it) {
		if (*it == *a_seg) {
			return ++it;
		}
	}
	return NULL;
}

/*
 * findet unteren Nachbar in der Liste
 * ist er nicht vorhanden wird NULL übergeben
 */
Line& Sweep::getneighbour_low(Line* a_seg){
	list<Line>::iterator it;
	for ( it=segmentqueue.begin(); it != segmentqueue.end(); ++it) {
		if (*it == *a_seg) {
			return --it;
		}
	}
	return NULL;
}

/*
 * findet Segment in der Liste
 * ist es nicht vorhanden wird NULL übergeben
 */
Line& Sweep::getseg(Line* a_seg){
	list<Line>::iterator it;
	for ( it=segmentqueue.begin(); it != segmentqueue.end(); ++it) {
		if (*it == *a_seg) {
			return it;
		}
	}
	return NULL;
}

/*
 * Funktion soll ausgeben wo sich der Sweep gerade befindet
 */
double Sweep::getxposition() {
	double aktpos = -1.0;

	if(ereignis != NULL)
		aktpos = ereignis->get_x();

	return aktpos;
}

/*
 * Funktion um die Eventqueue mit Start- und Endpunkten zu füllen
 */
void Sweep::addevent(Point* a_point, Line* a_line){
	//a_point ist startpunkt
	if (a_point == a_line->getstart){
		eventqueue.push_front( Event(a_point, a_line, STARTPUNKT) );
	}
	//a_point ist endpunkt
	else {
		eventqueue.push_front( Event(a_point, a_line, ENDPUNKT) );
	}

}

void Sweep::addevent(Event* a_event) {
	eventqueue.push_front(*a_event);
	eventqueue.sort();
}

/*
 * Einfügen eines Segments an der richtigen Stelle oder am Ende
 */
Line* Sweep::addseg(Line* a_seg) {
	//segmentqueue.push_front(*a_seg);
	//sortseg();

	list<Line>::iterator newseg;

	for ( newseg = segmentqueue.begin(); newseg != segmentqueue.end(); ++newseg ) {
	//Iterator solange y-achsenabschnitt > ist
	//dann an dieser Position einfügen
	//und die Referenz auf das Element zurückgeben
		if ( newseg->get_yvalue(getxposition()) > a_seg->get_yvalue(getxposition()) ) {
			segmentqueue.insert (newseg, *a_seg);
			return newseg;
		}
	}

	segmentqueue.insert (newseg, *a_seg);
	return newseg;

}

void Sweep::calcinters(){


	//Eventqueue nach x sortieren
	eventqueue.sort();

	while(eventqueue.empty() == false) {
		ereignis = eventqueue.begin();

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
		delevent();
	} //end while


}

