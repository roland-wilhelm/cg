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


	cout << "------------------------------------------" << endl;
	cout << "------- Cities and its States ------------" << endl;
	for(map<string, Point>::iterator iterCity = m_cities.begin();
			iterCity != m_cities.end();
			iterCity++) {

		cout << "City: --> " << iterCity->first
			 << " X: " << iterCity->second.get_x()
			 << " Y: " << iterCity->second.get_y()
			 << endl;

		string str("State not found");
		map<string, vector<vector<Point> > >::iterator iterStates = m_states.begin();
		for(; iterStates != m_states.end(); iterStates++) {

			cout << "Trying State: --> " <<  iterStates->first << endl;
			bool result = point_in_polygon(iterCity->second, iterStates->first);
			if(result == true) {

				str = iterStates->first;
				cout << "State: --> " << str << endl;
			}

		}

		cout << endl;
		cout << "------------------------------------------" << endl;

	}

	cout << "------------------------------------------" << endl;
}

void SvgFile::print_state_points(const char *a_state) {

	map<string, vector<vector<Point> > >::iterator iterStates;


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
		for(vector<vector<Point> >::iterator iterOuter = iterStates->second.begin();
				iterOuter != iterStates->second.end();
				iterOuter++) {

			for(vector<Point>::iterator iterInner = iterOuter->begin();
					iterInner != iterOuter->end();
					iterInner++) {

				cout << "X: " << iterInner->get_x()
				<< ", Y: " << iterInner->get_y() << endl;
			}

		}

		if(a_state)
			break;

	}


}

void SvgFile::print_state_bounding_box(const char *a_state) {

	map<string, BoundingBox>::iterator iterStates;


	if(a_state == NULL) {

		// Print all available
		iterStates = m_states_box.begin();

	}
	else {

		iterStates = m_states_box.find(a_state);

	}

	cout << "------------------------------------------" << endl;
	cout << "------ Bounding boxes of the states: -----" << endl;
	if(iterStates == m_states_box.end()) {

		cout << "State: " << a_state << " not found." << endl;
		return;
	}


	for( ; iterStates != m_states_box.end();
			iterStates++) {

		cout << "State: " << setw(30) << iterStates->first
				<< "  Min: " << iterStates->second.min
				<< "  Max: " << iterStates->second.max
				<< "  Max Area: "
				<< setw(10) << ( (iterStates->second.max.get_x() - iterStates->second.min.get_x()) * (iterStates->second.max.get_y() - iterStates->second.min.get_y()) )
				<< endl;


		if(a_state)
			break;

	}

	cout << "------------------------------------------" << endl;

}

void SvgFile::print_areas(map<string, double> &state_areas_exact) {

	cout << "------------------------------------------" << endl;
	cout << "---- States and its calculated Areas -----" << endl;
	map<string, double>::iterator iter = m_areas.begin();
	if(iter == m_areas.end()) {

		cerr << "No areas found." << endl;
		return;
	}

	for(; iter != m_areas.end(); iter++) {

		ostringstream os;
		map<string, double>::iterator iterExact = state_areas_exact.find(iter->first);
		if(iterExact == state_areas_exact.end()) {

			cerr << "No compared value found." << endl;
			os << "Exact value for " << iter->first << " not found";

		}
		else {

			if(iterExact->second == 0)
				os << "Divison by zero";

			os << setw(10) << iterExact->second << "\tDifference(%): " << setw(10) << ((iter->second/iterExact->second - 1)*100);
		}

		cout << "State: " << setw(25) << iter->first << ": " << setw(10) << iter->second
			 << "\texact: " << os.str()
			<< endl;
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

		// determine bounding box

		// searching for bounding box
		map<string, BoundingBox>::iterator iterStates =  m_states_box.find(a_state);

		if(iterStates == m_states_box.end()) {

			// bounding box for state "a_state" not fund, create new with values
			BoundingBox box;
			box.max = result;
			box.min = result;
			m_states_box[string(a_state)] = box;

		}
		else {

			// bounding box already exists for state "a_state"

			if(result.get_x() < m_states_box[string(a_state)].min.get_x() ) {

				m_states_box[string(a_state)].min.set_x(result.get_x());
			}

			if(result.get_x() > m_states_box[string(a_state)].max.get_x() ) {

				m_states_box[string(a_state)].max.set_x(result.get_x());
			}

			if(result.get_y() < m_states_box[string(a_state)].min.get_y() ) {

				m_states_box[string(a_state)].min.set_y(result.get_y());
			}

			if(result.get_y() > m_states_box[string(a_state)].max.get_y() ) {

				m_states_box[string(a_state)].max.set_y(result.get_y());
			}

		}

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
		vector<Point> state;
		const char *id = c->Attribute("id");
		const char *d = c->Attribute("d");

		// create string
		string str(d);
		// no string available, continue
		if(!str.length())
			continue;


		string::size_type start = 0;
		for(;;) {

			string::size_type diff = splitString(str, "MLlHz", &start);
			if(diff > 0) {

				// Token available
				string sub = str.substr(start, diff);
				if(sub == string("z")) {

					m_states[string(id)].push_back(state);
					state.clear();
				}

				bool result = svg2Point(id, sub, &offset, state);
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

bool SvgFile::start_calculation_area() {

	map<string, vector<vector<Point> > >::iterator iterStates = m_states.begin();
	if(iterStates == m_states.end()) {

		cerr << "No state found." << endl;
		return false;
	}

	cout << "------------------------------------------" << endl;
	cout << "------- Start calculating areas: ---------" << endl;
	for( ; iterStates != m_states.end();
			iterStates++) {

		cout << "Start calculating area: " << iterStates->first << endl;

		double total = 0.0;

		for(vector<vector<Point> >::iterator iterOuter = iterStates->second.begin();
				iterOuter != iterStates->second.end();
				iterOuter++) {

			/*
			 * handle last point of the state, because
			 * last point is the start. IMPORTANT: iterOuter->end() -1
			 */
			double area = 0.0;
			for(vector<Point>::iterator iterInner = iterOuter->begin();
					iterInner != iterOuter->end() -1 ;
					iterInner++) {


				double x1 = iterInner->get_x();
				double y1 = iterInner->get_y();

				double x2 = (iterInner+1)->get_x();
				double y2 = (iterInner+1)->get_y();

				area += (( y1 + y2 ) * ( x1 - x2 ));
			}

			total += fabs(area);

		}

		total /= 2;
		total = fabs(total);

		m_areas[iterStates->first] = total;

	}

	cout << "------------------------------------------" << endl;

	/*
	 * Is state within another state???
	 */
	cout << "--- Handle state within another state: ---" << endl;

	map<string, double>::iterator iterAreas = m_areas.begin();
	if(iterAreas == m_areas.end()) {

		cerr << "No areas found." << endl;
		return false;
	}

	// Iterate through all areas
	for(; iterAreas != m_areas.end(); iterAreas++) {

		// Find point to this area
		map<string, vector<vector<Point> > >::iterator thisState = m_states.find(iterAreas->first);

		cout << "------------------------------------------" << endl;
		cout << "Handle state: " << iterAreas->first << endl;

		iterStates = m_states.begin();
		if(iterStates == m_states.end()) {

			cerr << "No States found: " << endl;
			return false;
		}

		// Compare area with all other states
		for(; iterStates != m_states.end();
				iterStates++) {

			// skip same areas
			if(iterAreas->first == iterStates->first)
				continue;

			cout << "Trying State: " << iterStates->first << endl;
			bool result = false;

			vector<Point>::iterator iterVec = thisState->second.begin()->begin();
			for(; iterVec != thisState->second.begin()->end(); iterVec++) {

				result = point_in_polygon(*(iterVec), iterStates->first);
				if(result == false)
					break;

			}

			if(result == true) {

				map<string, double>::iterator thisAreas = m_areas.find(iterStates->first);
				cout << "State: " << iterAreas->first << " is within " << iterStates->first << endl;
				m_areas[iterStates->first] = (thisAreas->second - iterAreas->second);
			}

		}

		cout << "End handle state: " << iterAreas->first << endl;
		cout << "------------------------------------------" << endl;

	}


	return false;

}

bool SvgFile::point_in_bounding_box(const Point &a_city, const string &a_state) {

	bool within_box = false;

	// find state to check whether city is in state
	map<string, BoundingBox>::iterator iter = m_states_box.find(a_state);

	if(iter == m_states_box.end()) {

		// No state found
		cerr << "No state found." << endl;
		return false;
	}

	// state found --> checking
	if( (Line::ccw(iter->second.min, Point(iter->second.min.get_x(), iter->second.max.get_y()), a_city) ) == 1) {

		// Point "a_city" is on the left side of the line --> outside bounding box

	}
	else {

		if( (Line::ccw(Point(iter->second.min.get_x(), iter->second.max.get_y()), iter->second.max, a_city) ) == 1) {

			// Point "a_city" is on the left side of the line --> outside bounding box

		}
		else {

			if( (Line::ccw(iter->second.max, Point(iter->second.max.get_x(), iter->second.min.get_y()), a_city) ) == 1) {

				// Point "a_city" is on the left side of the line --> outside bounding box

			}
			else {

				if( (Line::ccw(Point(iter->second.max.get_x(), iter->second.min.get_y()), iter->second.min, a_city) ) == 1) {

					// Point "a_city" is on the left side of the line --> outside bounding box

				}
				else {

					// Point within bounding point
					within_box = true;
				}
			}
		}
	}


	return within_box;
}

bool SvgFile::point_in_polygon(const Point &a_city, const string &a_state) {


//	if(point_in_bounding_box(a_city, a_state) == false) {
//
//		cout << "Outside of bounding box: " << a_state << endl;
//		return false;
//	}

	// city is within the bounding box of the state

	// Getting the bounding box points
	map<string, BoundingBox>::iterator iter = m_states_box.find(a_state);

	if(iter == m_states_box.end()) {

		// No state found
		cerr << "No state found." << endl;
		return false;
	}


	map<string, vector<vector<Point> > >::iterator iterStates = m_states.find(a_state);
	if(iterStates == m_states.end()) {

		cerr << "State: " << a_state << " not found." << endl;
		return false;
	}

	// First point is out of the state, second the point to be found
	Line line(Point(iter->second.min.get_x() - 1, iter->second.min.get_y()), a_city);
	unsigned int intersections_nr = 0;
	cout << setw(20) << "Compared line (p | q): " << setw(45) << line << endl;
	for(vector<vector<Point> >::iterator iterOuter = iterStates->second.begin();
			iterOuter != iterStates->second.end();
			iterOuter++) {

		/*
		 * handle last point of the state, because
		 * last point is the start. IMPORTANT: iterOuter->end() -1
		 */
		vector<Point>::iterator iterInner = iterOuter->begin();

		/*
		 * Wenn eine Linie sich genau am Schnittpunkt zweier Segmente schneidet,
		 * werden zwei intersections erkannt, ist soweit ja richtig,
		 * aber es darf nur als eine intrsection behandelt werden , da sonst
		 * der Punkt außerhalb und nicht innerhalb liegt.
		 */
		while(Line::ccw(line.get_start_point(), line.get_end_point(), *iterInner) == 0)
			iterInner++;

		for(; iterInner != iterOuter->end() - 1;
				iterInner++) {


			Line line2(iterInner->get_x(), iterInner->get_y(),
						(iterInner+1)->get_x(), (iterInner+1)->get_y());

			if(line.is_intersection(line2) == true) {

				cout << setw(20) << "Intersected line of: " << setw(27) << a_state << setw(20) << line2 << endl;
				intersections_nr++;

			}

		}

	}


	if(intersections_nr % 2 == 1) {

		return true;
	}

	return false;
}
