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


typedef struct {

	Point min;
	Point max;

}BoundingBox;


class SvgFile {

	private:

		std::map<std::string, std::vector<std::vector<Point> > > m_states;
		std::map<std::string, Point> m_cities;
		std::map<std::string, double> m_areas;
		std::map<std::string, BoundingBox> m_states_box;

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
		bool start_calculation_area();
		bool point_in_bounding_box(const Point &a_city, const std::string &a_state);
		bool point_in_polygon(const Point &a_point, const std::string &a_state);

		void print_cities();
		void print_areas(std::map<std::string, double> &state_areas_exact);
		void print_state_points(const char *a_state = NULL);
		void print_state_bounding_box(const char *a_state = NULL);

};

#endif /* SVGFILE_H_ */
