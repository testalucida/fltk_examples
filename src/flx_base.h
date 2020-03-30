/*
 * flx_base.h
 *
 *  Created on: 26.03.2020
 *      Author: martin
 */

#ifndef FLX_BASE_H_
#define FLX_BASE_H_

#include <FL/Enumerations.H>

struct Size {
	int w = 0;
	int h = 0;
};

class TextMeasure {
public:
	~TextMeasure() {}
	static TextMeasure& inst();
	Size get_size(char const *pTxt, Fl_Font, Fl_Fontsize) const;
	Size get_size(int chars, Fl_Font, Fl_Fontsize) const;
private:
	TextMeasure();
	const char* _pTeststring = "wegfeiaulm";
	int _len_teststring = 0;
	static TextMeasure* _pThis;
};


#endif /* FLX_BASE_H_ */
