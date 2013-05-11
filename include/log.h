/*
 * log.h
 *
 *  Created on: 11.04.2013
 *      Author: Roland
 */

#ifndef LOG_H_
#define LOG_H_

#include <cstdio>

/*
 * Enable extended prints for tests purposes;
 */
#define DEBUG_TEST

/*
 * Enable Debug Mode
 */
//#define DEBUG
#ifdef DEBUG
#define DBG(fmt, args...) printf("\n%s: %s:%d: " fmt, __FILE__, __FUNCTION__, __LINE__, ## args);;
#else
#define DBG(fmt, args...)
#endif



#endif /* LOG_H_ */
