/*
 * SvgFile.h
 *
 *  Created on: 25.05.2013
 *      Author: Roland
 */

#ifndef SVGFILE_H_
#define SVGFILE_H_

#include "Point.h"

#include <map>
#include <string>
#include <vector>


class SvgFile {

	private:

		std::map<std::string, std::vector<Point> > m_states;
		std::map<std::string, Point> m_cities;
		std::map<std::string, double> m_areas;

		std::string m_file_name;
		unsigned int m_states_nr;
		unsigned int m_cities_nr;

		bool svg2Point(const char *a_state, const std::string &a_line, Point *a_offset);
		bool cstring2Double(const char *a_line, double *a_value);
		std::string::size_type splitString(const std::string &a_str, const char *delimiter, std::string::size_type *a_start);


	public:

		SvgFile();
		SvgFile(const char *a_file);
		virtual ~SvgFile();

		bool read_file(const char *a_file);
		bool start_calculation_area(const char *a_state = NULL);
		void print_cities();
		void print_state(const char *a_state = NULL);


};

#endif /* SVGFILE_H_ */
