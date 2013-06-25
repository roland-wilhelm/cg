/*
 * Aufgabe1.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Roland
 */


#include "Aufgabe1.h"

#include "LineFile.h"

#include <iostream>

using namespace std;

#define MAX_FILES 3
#define STRECKEN_BASE "data/"
#define STRECKEN_TEST STRECKEN_BASE "Strecken_test.dat"
#define STRECKEN_1000 STRECKEN_BASE "Strecken_1000.dat"
#define STRECKEN_10000 STRECKEN_BASE "Strecken_10000.dat"
#define STRECKEN_100000 STRECKEN_BASE "Strecken_100000.dat"


void start_aufgabe1(unsigned int a_iterations) {

	const char *files[] = {

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
		else {

			if(lines[i].read_file(files[i]) == false) {

				cerr << "Failure reading " << files[i] << endl;
				continue;
			}
			else {

				lines[i].start_calculating_intersected_lines(a_iterations);
				//lines[i].start_calculating_intersected_lines_openmp();
				lines[i].print_calculated_result();
			}

		}

	}

	delete[] lines;
}

void start_aufgabe1_test(unsigned int a_iterations) {

	LineFile line;

	if(line.read_file(STRECKEN_TEST) == true) {

		line.start_calculating_intersected_lines(a_iterations);
		line.print_calculated_result();
	}
	else {

		cerr << "Failure reading " << STRECKEN_TEST << endl;
	}

}

