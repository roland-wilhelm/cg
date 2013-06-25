/*
 * SvgFile.cpp
 *
 *  Created on: 25.05.2013
 *      Author: Roland
 */

#include "SvgFile.h"
#include "log.h"
#include "Point.h"
#include "Line.h"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <tinyxml2.h>
#include <iomanip>


using namespace std;
using namespace tinyxml2;

SvgFile::SvgFile() :
		m_file_name(string()),
		m_states_nr(0),
		m_cities_nr(0) {

}

SvgFile::SvgFile(const char *a_file) :
		m_file_name(string(a_file)),
		m_states_nr(0),
		m_cities_nr(0) {


	read_file(a_file);
}

SvgFile::~SvgFile() {

	m_cities.clear();
	m_states.clear();

}

bool SvgFile::start_calculation() {

	map<string, State>::iterator iterStates = m_states.begin();
	if(iterStates == m_states.end()) {

		cerr << "No State found." << endl;
		return false;
	}

	cout << "------------------------------------------" << endl;
	cout << "------- Start Calculating Areas: ---------" << endl;
	for( ; iterStates != m_states.end();
			iterStates++) {

		cout << "Area: " << iterStates->first << endl;
		iterStates->second.start_calculating();
		cout << "------------------------------------------" << endl;
	}

	cout << "------------------------------------------" << endl;



	cout << "------------------------------------------" << endl;
	cout << "------- Start Locating Cities ------------" << endl;
	vector<City>::iterator iterCity = m_cities.begin();
	if(iterCity == m_cities.end()) {

		cerr << "No City found." << endl;
		return false;
	}
	for(; iterCity != m_cities.end(); iterCity++) {

		cout << "City: " << iterCity->get_name() << endl;
		iterCity->locate_and_push_city_to_state(m_states);
		cout << "------------------------------------------" << endl;

	}

	cout << "------------------------------------------" << endl;

	return true;
}


void SvgFile::print_result(map<string, double> &state_areas_exact) {

	cout << "------------------------------------------" << endl;
	cout << "---- States and its calculated Areas -----" << endl;
	map<string, State>::iterator iterStates = m_states.begin();
	if(iterStates == m_states.end()) {

		cerr << "No areas found." << endl;
		return;
	}

	for(; iterStates != m_states.end(); iterStates++) {

		ostringstream os;
		map<string, double>::iterator iterExact = state_areas_exact.find(iterStates->first);
		if(iterExact == state_areas_exact.end()) {

			cerr << "No compared value found." << endl;
			os << "Exact value for " << iterStates->first << " not found";

		}
		else {

			if(iterExact->second == 0)
				os << "Divison by zero";

			os << setw(22) << iterExact->second << endl
			   << "  Difference(%): " << setw(13) << ((iterStates->second.get_area()/iterExact->second - 1)*100);
		}

		cout << iterStates->second
			 << "  Exact: " << os.str()
			 << endl;
			 iterStates->second.print_cities();
	    cout << endl;
	    cout << "------------------------------------------" << endl;
	}

	cout << "------------------------------------------" << endl;
		cout << "------------ All Cities --------------" << endl;
		vector<City>::iterator iterCity = m_cities.begin();
		if(iterCity == m_cities.end()) {

			cerr << "No Cities found." << endl;
			return;
		}

		for(; iterCity != m_cities.end(); iterCity++) {

			cout << *iterCity << endl;
		    cout << "------------------------------------------" << endl;
		}
	cout << "------------------------------------------" << endl;
}

bool SvgFile::svg2Point(const char *a_state, const string &a_str, Point *a_offset, vector<Point> &a_state_points) {

	double x = 0.0, y = 0.0;
	bool conversion = false, available = false;
	// start position after prefix M, L, l, z or H
	string::size_type start = 1;
	Point result;


	if(!a_str.length())
		return false;

	switch(a_str[0]) {

		case 'M':
		case 'L':
		case 'l': {

			string::size_type end = a_str.find_first_of(",", start);
			if(end > start) {

				// Token available
				string sub = a_str.substr(start, (end - start));
				conversion = cstring2Double(sub.c_str(), &x);
				if(conversion == false) {

					//assert(NULL);
					return false;
				}
				start = end;

			}

			start = a_str.find_first_not_of(",", start);
			if(start < a_str.npos) {

				// Token available
				string sub = a_str.substr(start, (a_str.npos - start));
				conversion = cstring2Double(sub.c_str(), &y);
				if(conversion == false) {

					//assert(NULL);
					return false;
				}

			}

			break;
		}

		case 'H': {

			if(start < a_str.npos) {

				// Token available
				string sub = a_str.substr(start, (a_str.npos - start));
				conversion = cstring2Double(sub.c_str(), &x);
				if(conversion == false) {

					cerr << "Failure conversion: " << sub << endl;
					assert(NULL);
					return false;
				}

			}

			break;
		}

		default:

			break;


	}

	if(a_str[0] == 'M') {

		a_offset->set_point(x, y);
		result = *a_offset;
		available = true;
	}
	else if(a_str[0] == 'L') {

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

		// ignore here
	}
	else if(a_str[0] == 'H') {

		result.set_point(x, a_offset->get_y());
		a_offset->set_point(result);
		available = true;
	}
	else {

		cerr << "Invalid character " << a_str[0] << endl;
	}

	if(available == true) {

		a_state_points.push_back(result);

	}

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
		cerr << "Conversion failure: " << a_line << endl;
		//assert(NULL);
	}
	else {

		conversion = true;
		*a_value = value;
	}

	return conversion;
}

string::size_type SvgFile::splitString(const string &a_str, const char *delimiter, string::size_type *a_start) {

	string::size_type count = 0;
	static string invalid_char = " \n\t";

	// skip leading " ", \n and \t characters (invalid characters)
	// find first appearance of delimiter
	string::size_type start = a_str.find_first_of(delimiter, *a_start);
	if(start == string::npos)
		return 0;	// nothing to do

	// Store new start position
	*a_start = start;

	// find next appearance of delimiter or invalid characters starting at "start + 1"
	string::size_type stop = a_str.find_first_of(invalid_char.append(delimiter), start + 1);

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
		vector<Point> area;
		const char *id = c->Attribute("id");
		const char *d = c->Attribute("d");

		// create data string
		string str(d);
		// no string available, continue
		if(!str.length())
			continue;

		// Create new State
		cout << "Parsing new State " << id << endl;
		m_states[string(id)] = State(id);
		string::size_type start = 0;
		for(;;) {

			string::size_type diff = splitString(str, "MLlHz", &start);
			if(diff > 0) {

				// Token available
				string sub = str.substr(start, diff);
				if( (sub == string("z")) && (area.empty() == false) ) {

					// Push filled vector of points to the state and clear it for the next area
					m_states[string(id)].push_area_to_state(area);
					area.clear();

				}

				bool result = svg2Point(id, sub, &offset, area);
				if(result == false) {

					return false;
				}
				start += diff;

			}
			else {

				break;
			}

		}

		m_states_nr++;

	}


	// Parse each city location
	XMLElement *childCities = doc.FirstChildElement("svg")->FirstChildElement("path")->ToElement();
	for( XMLElement *c = childCities; c != NULL; c = c->NextSiblingElement() ) {


		double x = 0.0, y = 0.0;
		bool conversion = false;

		const char *id = c->Attribute("id");
		cout << "Parsing new City " << id << endl;

		const char *string_x = c->Attribute("sodipodi:cx");
		conversion = cstring2Double(string_x, &x);

		const char *string_y = c->Attribute("sodipodi:cy");
		conversion = cstring2Double(string_y, &y);

		if((conversion == true) && id) {

			City city(id, Point(x,y));
			m_cities.push_back(city);


		}
		else {

			cerr << "Failure conversion." << endl;
			assert(NULL);
		}

	}


	return true;
}

void SvgFile::locate_states() {

	map<string, State>::iterator iterState = m_states.begin();
	if(iterState == m_states.end()) {

		cerr << "No States available." << endl;
		return;
	}

	cout << "------------------------------------------" << endl;
	cout << "-------- Start Locating States: ----------" << endl;
	map<string, State>::iterator iterStateOther = m_states.begin();
	// z.b. State Parallel
	for( ; iterState != m_states.end();
			iterState++) {

		cout << "State " << iterState->first << endl;
		iterStateOther = m_states.begin();
		for(; iterStateOther != m_states.end(); iterStateOther++) {

			cout << "  Check State " << iterStateOther->first << endl;
			if(iterState == iterStateOther) {

				cout << "  Skip State " << iterStateOther->first << endl;
				continue;
			}

			std::list<Area>::const_iterator iterArea = iterState->second.get_areas().begin();
			for(; iterArea != iterState->second.get_areas().end(); iterArea++) {

				bool result = false;
				std::vector<Point>::const_iterator iterPoint = iterArea->get_points().begin();
				for(; iterPoint != (iterArea->get_points().end() - 1); iterPoint++) {

					result = iterStateOther->second.point_in_state(*iterPoint);
					if(result == false) {

						//cout << "Points not within " << iterStateOther->first << endl;
						break;
					}

				}

				if(result == false) {

					cout << "  Points not within " << iterStateOther->first << endl;

				}

				if(result == true) {

					cout << "--> Points within " << iterStateOther->first << endl;

				}

			}

		}

	}



	cout << "------------------------------------------" << endl;


}
