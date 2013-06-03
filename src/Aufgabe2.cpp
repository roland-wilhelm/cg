/*
 * Aufgabe2.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Roland
 */


#include "Aufgabe2.h"
#include "SvgFile.h"

#include <iostream>

using namespace std;

#define SVG_FILES_BASE "data/"

#define TEST_SVG SVG_FILES_BASE "Test.svg"
#define GERMANY_SVG SVG_FILES_BASE "DeutschlandMitStaedten.svg"



void start_aufgabe2() {


	/*
	 * Filling exact state ares according to
	 * http://www.statistik-portal.de/Statistik-Portal/de_jb01_jahrtab1.asp
	 *
	 */
	map<string, double> state_areas_exact;
	state_areas_exact["Baden-Wuerttemberg"] = 35751.65;
	state_areas_exact["Bayern"] = 70549.19;
	state_areas_exact["Berlin"] = 891.75;
	state_areas_exact["Brandenburg"] = 29477.16;
	state_areas_exact["Bremen"] = 404.23;
	state_areas_exact["Hamburg"] = 755.16;
	state_areas_exact["Hessen"] = 21114.72;
	state_areas_exact["Mecklenburg-Vorpommern"] = 23174.17;
	state_areas_exact["Niedersachsen"] = 47618.24;
	state_areas_exact["Nordrhein-Westfalen"] = 34083.52;
	state_areas_exact["Rheinland-Pfalz"] = 19847.39;
	state_areas_exact["Saarland"] = 2568.65;
	state_areas_exact["Sachsen"] = 18413.91;
	state_areas_exact["Sachsen-Anhalt"] = 20445.26;
	state_areas_exact["Schleswig-Holstein"] = 15763.18;
	state_areas_exact["Thueringen"] = 16172.14;



	SvgFile germany;
	if(germany.read_file(GERMANY_SVG) == true) {

		//germany.print_state_points("Bremen");
		germany.print_state_bounding_box();
		germany.start_calculation_area();
		germany.print_areas(state_areas_exact);
		germany.print_cities();
		bool result = germany.point_in_polygon(Point(477, 256), string("Brandenburg"));
		if(result == true)
			cout << "Schnittpunkte" << endl;
	}
	else {

		cerr << "Failure reading " << GERMANY_SVG << endl;
	}

}

void start_aufgabe2_test() {

	map<string, double> state_areas_exact;
	state_areas_exact["Quadrat"] = 9;
	state_areas_exact["Parallel"] = 39;
	state_areas_exact["QuadratInParallel"] = 12;
	state_areas_exact["ParallelOhneQuadrat"] = 39;
	state_areas_exact["Dreieck"] = 10.5;
	state_areas_exact["Rechteck"] = 1.5;



	SvgFile test;
	if(test.read_file(TEST_SVG) == true) {

//		test.print_state_points();
		test.print_state_bounding_box();
		test.start_calculation_area();
		test.print_areas(state_areas_exact);
		test.print_cities();
//		test.point_in_polygon(Point(5.0, 5.0), string("Parallel"));
	}
	else {

		cerr << "Failure reading " << TEST_SVG << endl;
	}

}

