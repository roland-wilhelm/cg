/*
 * State.cpp
 *
 *  Created on: 07.06.2013
 *      Author: Roland
 */

#include "State.h"
#include <string>
#include "Point.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <map>
#include "City.h"
#include "Area.h"
#include <sstream>
#include "Line.h"


using namespace std;


State::State() : m_name(std::string()),
				 m_area(0.0) {


}

State::State(const char *a_name) : m_name(std::string(a_name)),
							       m_area(0.0) {



}

State::State(const std::string &a_name) : m_name(a_name),
								          m_area(0.0) {


}

State::~State() {

	m_areas.clear();
	m_cities.clear();
}

bool State::start_calculating() {

	calculate_bounding_box();
	locate_areas();
	calculate_area();


	return true;
}

bool State::calculate_area() {

	cout << "Calculating area: " << m_name << " --> ";
	list<Area>::iterator iterAreas = m_areas.begin();
	if(iterAreas == m_areas.end()) {

		cerr << "No areas available." << endl;
		return false;
	}

	ostringstream os;
	for(; iterAreas != m_areas.end();
			iterAreas++) {

		if(iterAreas->is_within_state() == true) {

			m_area -= iterAreas->get_area();
			os << " - " << iterAreas->get_area();
		}
		else {

			m_area += iterAreas->get_area();
			os << " + " << iterAreas->get_area();
		}

	}

	os << " = " << m_area;
	cout << os.str() << endl;

	return true;

}

bool State::calculate_bounding_box() {

	cout << "Calculating bounding box: " << m_name << endl;

	list<Area>::iterator iterAreas = m_areas.begin();
	if(iterAreas == m_areas.end()) {

		cerr << "No areas available." << std::endl;
		return false;
	}

	m_bounding_box_min = iterAreas->get_box_min();
	m_bounding_box_max = iterAreas->get_box_max();
	iterAreas++;
	for(; iterAreas != m_areas.end();
			iterAreas++) {

		if(iterAreas->get_box_min().get_x() < m_bounding_box_min.get_x() ) {

			m_bounding_box_min.set_x(iterAreas->get_box_min().get_x());
		}

		if(iterAreas->get_box_max().get_x() > m_bounding_box_max.get_x() ) {

			m_bounding_box_max.set_x(iterAreas->get_box_max().get_x());
		}

		if(iterAreas->get_box_min().get_y() < m_bounding_box_min.get_y() ) {

			m_bounding_box_min.set_y(iterAreas->get_box_min().get_y());
		}

		if(iterAreas->get_box_max().get_y() > m_bounding_box_max.get_y() ) {

			m_bounding_box_max.set_y(iterAreas->get_box_max().get_y());
		}

	}

	return true;

}

bool State::locate_areas() {

	cout << "Handle State: " << m_name << endl;


	std::list<Area>::iterator iterAreas = m_areas.begin();
	if(iterAreas == m_areas.end()) {

		cerr << "No areas available." << endl;
		return false;
	}

	// Sortieren der Flächen, hier geringste Fläche zuerst
	m_areas.sort();
	// Mit der größten Fläche beginnen
	m_areas.reverse();


	// Die größte Fläche muss nicht überprüft werden, ob sie in kleineren liegt.
	list<Area>::iterator iterAreaHugh = iterAreas;
	iterAreas++;
	if(iterAreas == m_areas.end()) {

		cout << "One huge area:" << endl;
		iterAreas->set_within_state(false);
		return true;
	}


	// Iteriere über alle Flächen des Bundeslandes, ausser der Größten.
	for(; iterAreas != m_areas.end();
			iterAreas++) {

		vector<Point>::const_iterator iterPoints = iterAreas->get_points().begin();
		if(iterPoints == iterAreas->get_points().end()) {

			std::cerr << "No points available." << endl;
			continue;
		}

		bool result = false;
		int intersections = 0;
		for(; iterPoints != (iterAreas->get_points().end() - 1); iterPoints++) {

			result = iterAreaHugh->point_in_area(*iterPoints, &intersections);
			if(result == false) {

				// Entweder alle Punkt liegen in der Fläche oder keine.
				break;
			}

		}
		if(result == true) {

			iterAreas->set_within_state(true);
			cout << "Area within " << m_name << endl;

		}
		else {

			iterAreas->set_within_state(false);
		}

	}

	return true;

}

void  State::push_city_to_state(const char *a_city_name, City &a_city) {

	m_cities[string(a_city_name)] = a_city;
}

void  State::push_city_to_state(const std::string &a_city_name, City &a_city) {

	m_cities[a_city_name] = a_city;
}

void  State::push_area_to_state(Area &a_area) {

	m_areas.push_back(a_area);

}

void  State::push_area_to_state(std::vector<Point> &a_area) {

	m_areas.push_back(Area(a_area));

}

bool State::point_in_state(const Point &a_point) {


	bool result = point_in_bounding_box(a_point);
	if(result == false) {

		cout << "    Point out of bounding box of State: " << m_name << endl;
		return false;
	}

	// Go ahead
	list<Area>::iterator iterAreas = m_areas.begin();
	if(iterAreas == m_areas.end()) {

		cerr << "No areas available." << endl;
		return false;
	}

	// Sortieren der Flächen, hier geringste zuerst
	m_areas.sort();
	// Mit der größten Fläche beginnen
	m_areas.reverse();


	int total = 0;
	for(; iterAreas != m_areas.end();
			iterAreas++) {

		int intersections = 0;
		iterAreas->point_in_area(a_point, &intersections);
		total += intersections;

	}

	if(total % 2 == 1)
		return true;


	return false;

}

bool State::point_in_bounding_box(const Point &a_to_be_located) {

	bool within_box = false;


	if( (Line::ccw(m_bounding_box_min, Point(m_bounding_box_min.get_x(), m_bounding_box_max.get_y()), a_to_be_located) ) == 1) {

		// Point "a_city" is on the left side of the line --> outside bounding box

	}
	else {

		if( (Line::ccw(Point(m_bounding_box_min.get_x(), m_bounding_box_max.get_y()), m_bounding_box_max, a_to_be_located) ) == 1) {

			// Point "a_city" is on the left side of the line --> outside bounding box

		}
		else {

			if( (Line::ccw(m_bounding_box_max, Point(m_bounding_box_max.get_x(), m_bounding_box_min.get_y()), a_to_be_located) ) == 1) {

				// Point "a_city" is on the left side of the line --> outside bounding box

			}
			else {

				if( (Line::ccw(Point(m_bounding_box_max.get_x(), m_bounding_box_min.get_y()), m_bounding_box_min, a_to_be_located) ) == 1) {

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

void State::print_cities() {

	map<string, City>::iterator iterCity = m_cities.begin();
	if(iterCity == m_cities.end()) {

		cerr << "No Cities available." << endl;
		return;
	}

	for(; iterCity != m_cities.end(); iterCity++) {

		cout << iterCity->second;
	}

}

ostream& operator <<(ostream &os, State &a_state) {

	os 		<< "State: " << "\t" << a_state.m_name << endl
			<< "  Bounding Box: " << setw(17)
			<< "  Min: " << a_state.m_bounding_box_min
			<< "  Max: " << a_state.m_bounding_box_max << endl
			<< "  Area: "
			<< setw(23)
			<< a_state.m_area
			<< endl;

		return os;
	}
