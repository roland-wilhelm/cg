/*
 * SvgFile.h
 *
 *  Created on: 25.05.2013
 *      Author: Roland
 */

#ifndef SVGFILE_H_
#define SVGFILE_H_

#include "Point.h"
#include "State.h"
#include <map>
#include <string>
#include <vector>


class SvgFile {

	private:

		std::map<std::string, State> m_states;
		std::vector<City> m_cities;

		std::string m_file_name;
		unsigned int m_states_nr;
		unsigned int m_cities_nr;

		bool svg2Point(const char *a_state, const std::string &a_line, Point *a_offset, std::vector<Point> &a_state_points);
		bool cstring2Double(const char *a_line, double *a_value);
		std::string::size_type splitString(const std::string &a_str, const char *delimiter, std::string::size_type *a_start);


	public:

		SvgFile();
		SvgFile(const char *a_file);

		~SvgFile();

		bool read_file(const char *a_file);
		void locate_states();
		bool start_calculation();
		//bool point_in_bounding_box(const Point &a_to_be_located, const BoundingBox &a_box);

		void print_result(std::map<std::string, double> &state_areas_exact);


};

#endif /* SVGFILE_H_ */
