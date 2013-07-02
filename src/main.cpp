/*
 * main.cpp
 *
 *  Created on: 09.04.2013
 *      Author: Roland
 */


#include <iostream>
#include "LineFile.h"

using namespace std;

#define MAX_FILES 1
#define STRECKEN_BASE "data/"
#define STRECKEN_SWEEP_TEST  STRECKEN_BASE "s_1000_test.dat"
#define STRECKEN_SWEEP STRECKEN_BASE "s_1000_1.dat"
#define STRECKEN_TEST STRECKEN_BASE "Strecken_test.dat"
#define STRECKEN_1000 STRECKEN_BASE "Strecken_1000.dat"
#define STRECKEN_10000 STRECKEN_BASE "Strecken_10000.dat"
#define STRECKEN_100000 STRECKEN_BASE "Strecken_100000.dat"


int main(int argc, char **argv) {

	const char *files[] = {
			STRECKEN_SWEEP_TEST,
			STRECKEN_SWEEP,
			STRECKEN_TEST,
			STRECKEN_1000,
			STRECKEN_10000,
			STRECKEN_100000,
			NULL
	};

	LineFile *lines = new LineFile[MAX_FILES];

	for(unsigned int i = 0; i < MAX_FILES; i++) {

		if(files[i] == NULL) {

			break;
		}

		//Aufgabe1 - Schnittpunkte n² berechnen
		lines[i].read_file(files[i]);
		lines[i].start_calculating_intersected_lines_max();
		lines[i].print_calculated_result();

		//Aufgabe3 - LineSweep
		lines[i].sweepiniteventqueue();
		lines[i].sweepstart();
		lines[i].sweepprint();
	}



	delete[] lines;

	return 0;
}
