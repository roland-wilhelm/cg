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
		m_valid_lines_nr(0),
		m_invalid_lines_nr(0),
		m_file_name(string()),
		m_timediff(0.0),
		m_start_time(0),
		m_stop_time(0) {

	DBG();

}

/*
 * Open the file and read all lines
 */
LineFile::LineFile(const char *a_file) :
				m_intersected_lines_nr(0),
				m_valid_lines_nr(0),
				m_invalid_lines_nr(0),
				m_file_name(a_file),
				m_timediff(0.0),
				m_start_time(0),
				m_stop_time(0) {

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

bool LineFile::start_calculating_intersected_lines() {

	DBG();

	m_intersected_lines_nr = 0;
	start_timer();

	cout << "Start calculating file " << m_file_name << endl;
	for(unsigned int i = 0; i < m_lines.size(); i++) {

		for(unsigned int j = i+1; j < m_lines.size(); j++) {

			if(m_lines[j]->is_intersection(*m_lines[i]) == true) {

				m_intersected_lines_nr++;
			}
		}
	}

	stop_timer();

	return true;
}

void LineFile::reset_timer() {

	DBG();

	m_start_time = 0;
	m_stop_time = 0;
	m_timediff = 0.0;
}

bool  LineFile::start_timer() {

	DBG();

	reset_timer();
	m_start_time = clock();

	return true;
}

bool  LineFile::stop_timer() {

	DBG();

	m_stop_time = clock();
	m_timediff =(double)(m_stop_time-m_start_time)/ CLOCKS_PER_SEC;

	return true;
}

/*
 * Ermittelt ob r rechts oder links der Strecke liegt. Liegt er links,
 * ist das Dreieck gegen den Uhrzeigersinn (anticlockwise-> kleiner 0) aufgespannt.
 * Liegt es rechts (-> >0) wird es im Urzeigersinn aufgespannt, liegt der Punkt
 * auf der Strecke ( -> 0 ) ergibt sich ein Schnittpunkt.
 */
int LineFile::clockwise(Point &a_p, Point &a_q, Point &a_r){

	//x und y koordinaten der Punkte
	double p1,p2,q1,q2,r1,r2, result;

	p1=a_p.get_x();
	p2=a_p.get_y();
	q1=a_q.get_x();
	q2=a_q.get_y();
	r1=a_r.get_x();
	r2=a_r.get_y();


	result=(p2*r1)-(q2*r1)+(q1*r2)-(p1*r2)-(p2*q1)+(p1*q2);

	return result;
}

void LineFile::print_delta_time() {

	DBG();

	cout << "DeltaT: " << m_timediff << " seconds" << endl;

}

void LineFile::print_calculated_result() {

	DBG();

	cout << "-------------------------------------" << endl;
	cout << "File name: " << m_file_name << endl;
	cout << "Valid lines: " << m_valid_lines_nr << endl;
	cout << "Invalid lines: " << m_invalid_lines_nr << endl;
	cout << "Total lines: " << Line::get_lines_nr() << endl;
	cout << "Intersected lines: " << m_intersected_lines_nr << endl;
	print_delta_time();
	cout << "-------------------------------------" << endl;

}
