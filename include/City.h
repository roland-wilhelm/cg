/*
 * City.h
 *
 *  Created on: 06.06.2013
 *      Author: Roland
 */

#ifndef CITY_H_
#define CITY_H_

#include <string>
#include "Point.h"
#include "State.h"
#include <iostream>
#include <map>

class Point;
class State;

class City {


private:

	std::string m_name_city;
	Point m_point;
	std::string m_name_state;

public:

	City();
	City(const char *a_city, const Point &a_point);
	City(const std::string &a_city, const Point &a_point);
	~City();

	bool locate_and_push_city_to_state(std::map<std::string, State> &a_states);

	void set_name(const char *a_city);
	void set_name(const std::string &a_city);

	const std::string& get_name() const;
	const Point& get_point() const;

	friend std::ostream& operator <<(std::ostream &os, City &a_city);


};


#endif /* CITY_H_ */
