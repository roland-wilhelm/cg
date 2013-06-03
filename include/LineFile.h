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
		unsigned long long m_intersected_lines_nr;
		unsigned long long m_compared_lines_nr;
		unsigned long long m_valid_lines_nr;
		unsigned int m_invalid_lines_nr;
		std::string m_file_name;
		double m_timeDiff;
		double m_timeMax;
		double m_timeMin;
		unsigned int m_iterations;
		clock_t m_start_time;
		clock_t m_stop_time;
		bool m_firstIteration;

		void reset_timer();
		bool start_timer();
		bool stop_timer();

		/* Do not allow copies */
		LineFile(const LineFile &);
		LineFile& operator =(const LineFile &);

	public:

		LineFile();
		LineFile(const char *a_file);
		~LineFile();

		bool read_file(const char *a_file);
		bool start_calculating_intersected_lines(unsigned int a_iterations = 1);
		void print_delta_time();
		void print_calculated_result();

};

#endif /* LINEFILE_H_ */
