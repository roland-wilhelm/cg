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
#define STRECKEN_TEST STRECKEN_BASE "Strecken_test.dat"
#define STRECKEN_1000 STRECKEN_BASE "Strecken_1000.dat"
#define STRECKEN_10000 STRECKEN_BASE "Strecken_10000.dat"
#define STRECKEN_100000 STRECKEN_BASE "Strecken_100000.dat"


int main(int argc, char **argv) {

	const char *files[] = {

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
		else {

			if(lines[i].read_file(files[i]) == false)
				continue;

			lines[i].start_calculating_intersected_lines();
			lines[i].print_calculated_result();

		}

	}



	delete[] lines;

	return 0;
}
