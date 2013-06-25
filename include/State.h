/*
 * State.h
 *
 *  Created on: 07.06.2013
 *      Author: Roland
 */

#ifndef STATE_H_
#define STATE_H_

#include <string>
#include "Point.h"
#include <ostream>
#include <vector>
#include <list>
#include <map>
#include "City.h"
#include "Area.h"

class City;

class State {


private:

	std::string m_name;
	double m_area;
	Point m_bounding_box_min;
	Point m_bounding_box_max;
	std::list<Area> m_areas;
	std::map<std::string, City> m_cities;


	bool calculate_area();
	bool calculate_bounding_box();
	bool locate_areas();
	bool point_in_bounding_box(const Point &a_to_be_located);

public:

	State();
	State(const char *a_name);
	State(const std::string &a_name);
	~State();

	bool start_calculating();

	void set_name(const char *a_city) { m_name = std::string(a_city); }
	void set_name(const std::string &a_city) { m_name = a_city; }

	void push_city_to_state(const char *a_city_name, City &a_city);
	void push_city_to_state(const std::string &a_city_name, City &a_city);

	void push_area_to_state(Area &a_area);
	void push_area_to_state(std::vector<Point> &a_area);

	const std::string& get_name() const { return m_name; }
	double get_area() const { return m_area; }
	const Point& get_box_min() const { return m_bounding_box_min; }
	const Point& get_box_max() const { return m_bounding_box_max; }

	const std::list<Area>& get_areas() const { return m_areas; }

	bool point_in_state(const Point &a_point);
	void print_cities();


	friend std::ostream& operator <<(std::ostream &os, State &a_state);

};

#endif /* STATE_H_ */
