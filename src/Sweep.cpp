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
				ereignis(NULL),
				output() {
	DBG();
}

void Sweep::leftendpoint(){
	DBG();
	Line *segA, *segB;

	//Linie in Segmentqueue an richtiger stelle einordnen

	addseg( ereignis->get_line() );
	sortseg();

	//Test ob Schnittpunkt mit den Nachbarsegmenten

}

void Sweep::rightendpoint(){
	DBG();

	//Segment vom Event in Queue finden und Nachbarn merken
	//Dann Segment aus Queue entfernen und Nachbarn auf
	//Schnittpunkt prüfen, falls noch vorher erledigt

}

void Sweep::treatintersection(){
	DBG();

	//addinter(ereignis);

	//die beiden Segmente des Schnittpunkts in der Seqmentqueue
	//finden und vertauschen (swappen)

	//Nun mit den Nachbarsegmenten auf Schnittpunkte prüfen,
	//falls noch nicht erledigt
}



//Funktion soll ausgeben wo sich der Sweep gerade befindet
double Sweep::getxposition() {
	double aktpos = -1.0;

	if(ereignis != NULL)
		aktpos = ereignis->get_x();

	return aktpos;
}

Line* addseg(const Line *a_seg) {
	Line* newseg;

	//Iterator solange y-achsenabschnitt > ist

	//dann an dieser Position einfügen

	//und die Referenz auf das Element zurückgeben

	return newseg;
}

