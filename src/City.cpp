/*
 * City.cpp
 *
 *  Created on: 07.06.2013
 *      Author: Roland
 */


#include "City.h"
#include "Point.h"
#include "State.h"
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

City::City()
		: m_name_city(string()),
		  m_point(),
		  m_name_state(string()) {

}

City::City(const char *a_city, const Point &a_point)
					: m_name_city(string(a_city)),
					  m_point(a_point),
					  m_name_state(string()) {

}

City::City(const string &a_city, const Point &a_point)
				: m_name_city(a_city),
				  m_point(a_point),
				  m_name_state(string()) {

}

City::~City() {

}

bool City::locate_and_push_city_to_state(map<string, State> &a_states) {

	cout << "Locate City: " << m_name_city << "\t" << m_point << endl;

	map<string, State>::iterator iterStates = a_states.begin();
	if(iterStates == a_states.end()) {

		cerr << "No States available." << endl;
		return false;
	}

	for(; iterStates != a_states.end(); iterStates++) {

		cout << "  Trying State: " << iterStates->second.get_name() << endl;
		bool result = iterStates->second.point_in_state(m_point);
		if(result == true) {

			cout << "State --> " << iterStates->first << endl;
			m_name_state = iterStates->first;
			iterStates->second.push_city_to_state(m_name_city, *this);
		}

	}

	return true;
}

void City::set_name(const char *a_city) {

	m_name_city = string(a_city);
}

void City::set_name(const std::string &a_city) {

	m_name_city = a_city;

}

const std::string& City::get_name() const {

	return m_name_city;

}

const Point& City::get_point() const {

	return m_point;

}

ostream& operator <<(ostream &os, City &a_city) {

	os 	 << "City:  --> " << "\t" << a_city.m_name_city
		 << setw(10)
		 << a_city.m_point
		 << endl
		 << "State: --> " << "\t"
		 << ((a_city.m_name_state.empty() == false) ? a_city.m_name_state : "Unknown");


	return os;
}
