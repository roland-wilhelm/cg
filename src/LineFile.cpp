/*
 * LineFile.cpp
 *
 *  Created on: 11.04.2013
 *      Author: Roland
 */


#include "LineFile.h"
#include "log.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>
#include <algorithm>


using namespace std;

/*
 * Init member files
 */
LineFile::LineFile() :
		m_intersected_lines_nr(0),
		m_compared_lines_nr(0),
		m_valid_lines_nr(0),
		m_invalid_lines_nr(0),
		m_file_name(string()),
		m_timeDiff(0.0),
		m_timeMax(0.0),
		m_timeMin(0.0),
		m_iterations(0),
		m_start_time(0),
		m_stop_time(0),
		m_firstIteration(true) {

	DBG();

}

/*
 * Open the file and read all lines
 */
LineFile::LineFile(const char *a_file) :
				m_intersected_lines_nr(0),
				m_compared_lines_nr(0),
				m_valid_lines_nr(0),
				m_invalid_lines_nr(0),
				m_file_name(a_file),
				m_timeDiff(0.0),
				m_timeMax(0.0),
				m_timeMin(0.0),
				m_iterations(0),
				m_start_time(0),
				m_stop_time(0),
				m_firstIteration(true) {

	DBG();

	read_file(a_file);
}

static void delete_line(Line *a_line) {

	delete a_line;
}

LineFile::~LineFile() {

	DBG();

	for_each(m_lines.begin(), m_lines.end(), delete_line);
	m_lines.clear();
}

bool LineFile::read_file(const char *a_file) {

	DBG("File %s", a_file);

	assert(a_file);

	if(!a_file) {

		cerr << "File " <<  a_file << " not valid." << endl;
		return false;
	}

	if(m_lines.empty() == false) {

		DBG("Lines already stored. Remove them and reading once again.");
		for_each(m_lines.begin(), m_lines.end(), delete_line);
		m_lines.clear();
	}

	ifstream ifile;

	double start_x, start_y, end_x, end_y;
	char buffer[100];
	bool result = false;


	m_file_name = string(a_file);
	m_invalid_lines_nr = 0;
	m_valid_lines_nr = 0;

	ifile.open(a_file, ios::in);
	if(ifile.fail() == true) {

		cerr << "File " <<  a_file << " not opened." << endl;
		return false;
	}

	cout << "Start loading file " << m_file_name << endl;
	while(ifile.getline(buffer, sizeof(buffer))) {

		if(ifile.fail()) {

			cerr << "File " <<  a_file << " failure during reading." << endl;
			break;
		}

		/*
		 * Check valid line
		 * Line is valid only consists of figure no strings allowed.
		 * Exactly 4 figure allowed, conversion error if less than 4 or more than 4 figures appears
		 * EOF and fail bit in the string once set keep them until clear() or new string created.
		 */
		bool conversion = true;
		istringstream istream;
		istream.str(string(buffer));

		/*
		 * Check with fail whether a conversion error occurs.
		 * Check whether EOF occurs --> less than 4 figures.
		 */
		istream >> start_x;
		if(istream.eof() || istream.fail())
			conversion = false;

		istream >> start_y;
		if(istream.eof() || istream.fail())
			conversion = false;

		istream >> end_x;
		if(istream.eof() || istream.fail())
			conversion = false;

		/*
		 * Check with fail whether a conversion error occurs.
		 * Check whether EOF not occurs --> more than 4 figures.
		 */
		istream >> end_y;
		if((istream.eof() != true) || istream.fail())
			conversion = false;


		if(conversion == true) {

			/* valid conversion create new line */
			Line *line = new Line(start_x, start_y, end_x, end_y);
			if(!line) {

				cerr << "File " <<  a_file << " failure during reading." << endl;
				break;
			}
			else {

				m_lines.push_back(line);
				m_valid_lines_nr++;
				result = true;
			}

		}
		else {

			/* invalid conversion, check next line */
			m_invalid_lines_nr++;
			continue;
		}

	}


	ifile.close();

	return result;
}

bool LineFile::start_calculating_intersected_lines(unsigned int a_iterations) {

	unsigned int intersections;

	m_iterations = a_iterations;


#ifdef DEBUG_TEST
	ostringstream ostream;
#endif

	DBG();

	cout << "Start calculating file " << m_file_name << endl;
	cout << "Max. lines to compare: " << m_valid_lines_nr/2*(m_valid_lines_nr-1) << endl;
	cout << "Iterations: " << m_iterations <<  endl;

	for(unsigned int u = 0; u < m_iterations; u++) {

		m_intersected_lines_nr = 0;
		m_compared_lines_nr = 0;

		start_timer();

		for(unsigned int i = 0; i < m_lines.size(); i++) {

			intersections = 0;

#ifdef DEBUG_TEST
		ostream.str("");
#endif

			for(unsigned int j = i+1; j < m_lines.size(); j++) {

				m_compared_lines_nr++;

				if(m_lines[i]->is_intersection(*m_lines[j]) == true) {

					m_intersected_lines_nr++;
					intersections++;

#ifdef DEBUG_TEST
				ostream << j+1 << "; ";
#endif

				}
			}

#ifdef DEBUG_TEST
		cout << "Line " << i+1 << " intersect " << intersections << " lines { " << ostream.str() << "}" << endl;
#endif

		}

		stop_timer();

	}

	return true;
}

void LineFile::reset_timer() {

	DBG();

	m_start_time = 0;
	m_stop_time = 0;

}

bool  LineFile::start_timer() {

	DBG();

	reset_timer();
	m_start_time = clock();

	return true;
}

bool  LineFile::stop_timer() {

	double timeDiff = 0.0;

	DBG();

	m_stop_time = clock();
	timeDiff = ((double)(m_stop_time-m_start_time))/ CLOCKS_PER_SEC;
	m_timeDiff += timeDiff;

	if(m_firstIteration) {

		m_firstIteration = false;
		m_timeMin = m_timeMax = timeDiff;
	}
	else {

		if(timeDiff < m_timeMin) {

			m_timeMin = timeDiff;
		}

		if(timeDiff > m_timeMax) {

			m_timeMax = timeDiff;
		}
	}

	return true;
}

void LineFile::print_delta_time() {

	DBG();

	if(m_iterations > 0)
		cout << "Average time: " << m_timeDiff/ m_iterations << " seconds" << endl;
	else
		cout << "Average time: " << "Division by zero" << " seconds" << endl;

	cout << "Min. time: " << m_timeMin << " seconds" << endl;
	cout << "Max. time: " << m_timeMax << " seconds" << endl;

}

void LineFile::print_calculated_result() {

	DBG();

	cout << "-------------------------------------" << endl;
	cout << "File name: " << m_file_name << endl;
	cout << "Valid lines: " << m_valid_lines_nr << endl;
	cout << "Invalid lines: " << m_invalid_lines_nr << endl;
	cout << "Compared lines: " << m_compared_lines_nr << endl;
	cout << "Intersected lines: " << m_intersected_lines_nr << endl;
	print_delta_time();
	cout << "-------------------------------------" << endl;
	cout << "-------------------------------------" << endl;

}
