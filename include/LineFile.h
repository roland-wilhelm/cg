/*
 * LineFile.h
 *
 *  Created on: 09.04.2013
 *      Author: Roland
 */

#ifndef LINEFILE_H_
#define LINEFILE_H_

#include "Line.h"

#include <vector>
#include <string>
#include <time.h>

class LineFile {

	private:

		std::vector<Line *> m_lines;
		unsigned int m_intersected_lines_nr;
		unsigned int m_valid_lines_nr;
		unsigned int m_invalid_lines_nr;
		std::string m_file_name;
		double m_timediff;
		clock_t m_start_time;
		clock_t m_stop_time;

		void reset_timer();
		bool start_timer();
		bool stop_timer();
		int clockwise(Point &a_p, Point &a_q, Point &a_r);

		/* Do not allow copies */
		LineFile(const LineFile &);
		LineFile& operator =(const LineFile &);

	public:

		LineFile();
		LineFile(const char *a_file);
		virtual ~LineFile();

		bool read_file(const char *a_file);
		bool start_calculating_intersected_lines();
		void print_delta_time();
		void print_calculated_result();

};

#endif /* LINEFILE_H_ */
