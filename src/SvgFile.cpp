/*
 * SvgFile.cpp
 *
 *  Created on: 25.05.2013
 *      Author: Roland
 */

#include "SvgFile.h"
#include "log.h"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <cmath>

#include <tinyxml2.h>


using namespace std;
using namespace tinyxml2;

SvgFile::SvgFile() :
		m_file_name(string()),
		m_states_nr(0),
		m_cities_nr(0) {

	DBG();
}

SvgFile::SvgFile(const char *a_file) :
		m_file_name(string(a_file)),
		m_states_nr(0),
		m_cities_nr(0) {

	DBG();

	read_file(a_file);
}

SvgFile::~SvgFile() {

	m_states.clear();
	m_cities.clear();
}

void SvgFile::print_cities() {


	for(map<string, Point>::iterator iter = m_cities.begin();
			iter != m_cities.end();
			iter++) {

		cout << "City: " << iter->first
			 << "  X: " << iter->second.get_x()
			 << ", Y: " << iter->second.get_y() << endl;
	}
}

void SvgFile::print_state(const char *a_state) {

	map<string, vector<Point> >::iterator iterStates;


	if(a_state == NULL) {

		// Print all available
		iterStates = m_states.begin();

	}
	else {

		iterStates = m_states.find(a_state);

	}

	if(iterStates == m_states.end()) {

		cout << "State: " << a_state << " not found." << endl;
		return;
	}


	for( ; iterStates != m_states.end();
			iterStates++) {

		cout << "State points: " << iterStates->first << endl;
		for(vector<Point>::iterator iterPoints = iterStates->second.begin();
				iterPoints != iterStates->second.end();
				iterPoints++) {

			cout << "X: " << iterPoints->get_x()
			 << ", Y: " << iterPoints->get_y() << endl;
		}

		if(a_state)
			break;

	}


}

bool SvgFile::svg2Point(const char *a_state, const string &a_str, Point *a_offset) {

	double x = 0.0, y = 0.0;
	bool conversion = false, available = false;
	// start position after prefix M, L or l
	string::size_type start = 1;
	Point result;


	if(!a_str.length())
		return false;

	string::size_type diff = splitString(a_str, ",", &start);
	if(diff > 0) {

		// Token available
		string sub = a_str.substr(start, diff);
		conversion = cstring2Double(sub.c_str(), &x);
		if(conversion == false) {

			cerr << "Failure conversion: " << sub << endl;
			assert(NULL);
			return false;
		}
		start += diff;

	}
//	else {
//
//		cerr << "Failure split string: " << a_str << endl;
//		return false;
//	}

	diff = splitString(a_str, ",", &start);
	if(diff > 0) {

		// Token available
		string sub = a_str.substr(start, diff);
		conversion = cstring2Double(sub.c_str(), &y);
		if(conversion == false) {

			cerr << "Failure conversion: " << sub << endl;
			assert(NULL);
			return false;
		}

	}
//	else {
//
//		cerr << "Failure split string: " << a_str << endl;
//		return false;
//	}

	if(a_str[0] == 'M') {

		a_offset->set_point(x, y);
		result = *a_offset;
		available = true;
	}
	else if(a_str[0] == 'L') {

		/*
		 * FIXME: do not ignor 'L'  L ist anscheinend == M --> für die Flächenberechnung erforderlich
		 */
		result.set_point(x, y);
		a_offset->set_point(result);
		available = true;
	}
	else if(a_str[0] == 'l') {

		result = *a_offset + Point(x, y);
		a_offset->set_point(result);
		available = true;
	}
	else if(a_str[0] == 'z') {

		// ignore
	}
	else if(a_str[0] == 'H') {

		// ignore
	}

	if(available == true)
		m_states[string(a_state)].push_back(result);

	return true;
}

bool SvgFile::cstring2Double(const char *a_line, double *a_value) {

	bool conversion = false;
	double value = 0.0;
	istringstream istream;

	istream.str(string(a_line));
	istream >> value;
	if(!istream.eof() || istream.fail()) {

		conversion = false;
		assert(NULL);
	}
	else {

		conversion = true;
		*a_value = value;
	}

	return conversion;
}

string::size_type SvgFile::splitString(const string &a_str, const char *delimiter, string::size_type *a_start) {

	string::size_type count = 0;

	// First delete "delimiter"
	string::size_type start = a_str.find_first_not_of(delimiter, *a_start);
	if(start == string::npos)
		return 0;	// nothing to do

	// Store new start position
	*a_start = start;

	// find first appearance of delimiter starting at "start"
	string::size_type stop = a_str.find_first_of(delimiter, start);

	if(stop != string::npos) {

		if(stop > start) {

			// Token available
			count = stop - start;

		}
	}
	else if(start < a_str.size()) {

		count = a_str.size() - start;

	}
	else {

		count = 0;
	}

	return count;
}

bool SvgFile::read_file(const char *a_file) {

	DBG("File %s", a_file);

	assert(a_file);

	if(!a_file) {

		cerr << "File " <<  a_file << " not valid." << endl;
		return false;
	}

	if(m_states.empty() == false) {

		DBG("States already stored. Remove them and reading once again.");
		m_states.clear();
		m_cities.clear();
	}

	m_file_name = string(a_file);

	XMLDocument doc;
	cout << "Start loading file " << m_file_name << endl;
	XMLError loadResult = doc.LoadFile(m_file_name.c_str());
	if(loadResult) {

		cerr << "File " <<  m_file_name << " not opened. Error" << loadResult << endl;
		return false;
	}

	// Parse each state and its appropriate lines
	XMLElement *childStates = doc.FirstChildElement("svg")->FirstChildElement("g")->FirstChildElement("path")->ToElement();
	for( XMLElement *c = childStates; c != NULL; c = c->NextSiblingElement() ) {

		Point offset;
		const char *id = c->Attribute("id");
		const char *d = c->Attribute("d");

		// create string
		string str(d);
		// no string available, continue
		if(!str.length())
			continue;


		string::size_type start = 0;
		for(;;) {

			string::size_type diff = splitString(str, " \n\t", &start);
			if(diff > 0) {

				// Token available
				string sub = str.substr(start, diff);
				bool result = svg2Point(id, sub, &offset);
				if(result == false) {

					return false;
				}
				start += diff;

			}
			else {

				break;
			}

		}

		//cout << id << ": " << d << endl;
		m_states_nr++;

	}


	// Parse each city location
	XMLElement *childCities = doc.FirstChildElement("svg")->FirstChildElement("path")->ToElement();
	for( XMLElement *c = childCities; c != NULL; c = c->NextSiblingElement() ) {


		double x = 0.0, y = 0.0;
		bool conversion = false;

		const char *id = c->Attribute("id");
		const char *string_x = c->Attribute("sodipodi:cx");
		conversion = cstring2Double(string_x, &x);

		const char *string_y = c->Attribute("sodipodi:cy");
		conversion = cstring2Double(string_y, &y);

		if((conversion == true) && id) {

			string city(id);
			map<string, Point>::iterator iter = m_cities.find(city);
			if(iter != m_cities.end()) {

				cout << "City " << city << "already stored --> skipped." << endl;
				continue;

			}

			m_cities[city] = Point(x, y);
			DBG("Saved City %s: %f, %f", id, x, y);
			m_cities_nr++;

		}
		else {

			cerr << "Failure conversion." << endl;
			assert(NULL);
		}

	}



	return true;
}

bool SvgFile::start_calculation_area(const char *a_state) {

	map<string, vector<Point> >::iterator iterStates;


	cout << "Start calculating areas: " << endl;

	if(a_state == NULL) {

		// Print all available
		iterStates = m_states.begin();

	}
	else {

		iterStates = m_states.find(a_state);

	}

	if(iterStates == m_states.end()) {

		cout << "State: " << a_state << " not found." << endl;
		return false;
	}


	for( ; iterStates != m_states.end();
			iterStates++) {

		cout << "Start calculating area: " << iterStates->first << endl;
		double area = 0.0;
		for(vector<Point>::iterator iterPoints = iterStates->second.begin();
				iterPoints != iterStates->second.end();
				iterPoints++) {

			/*
			 * handle last point of the state
			 */
			if( (iterPoints+1) == iterStates->second.end()) {

				break;
			}

//			double x1 = iterPoints->get_x();
//			double y1 = iterPoints->get_y();
//
//			double x2 = (iterPoints+1)->get_x();
//			double y2 = (iterPoints+1)->get_y();
//			cout << (y1+y2)*(x1-x2) << endl;

			area += (( iterPoints->get_y() + (iterPoints+1)->get_y() )
					* ( iterPoints->get_x() - (iterPoints+1)->get_x() ));

		}

		area /= 2;
		area = fabs(area);
		cout << "Calculated area for " << iterStates->first << " is " << area << endl;
		m_areas[iterStates->first] = area;

		if(a_state)
			break;

	}

	return false;

}
