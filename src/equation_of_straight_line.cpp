/*
 * equation_of_straight_line.cpp
 *
 *  Created on: 07.04.2020
 *      Author: martin
 */

#include <iostream>
#include <fltk_ext/Line.h>

using namespace std;



int equation_of_lines() {
	//given: 2 points (-5,7) and (7,-8)
	//create equation of line in the form f(x) = m*x + b
	Line l( {0, 0}, {4, 4} );
	Point p = {16, 16};
	fprintf( stderr, "p is on line: %s\n", l.isPointInSegment( p ) ? "true" : "false" );
	return 1;
}

