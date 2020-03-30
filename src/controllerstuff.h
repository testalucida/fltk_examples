/*
 * controllerstuff.h
 *
 *  Created on: 27.03.2020
 *      Author: martin
 */

#ifndef CONTROLLERSTUFF_H_
#define CONTROLLERSTUFF_H_

#include "symbolstuff.h"

class Canvas;

class CanvasController {
public:
	CanvasController(Canvas* p);
	static void canvasEventCallback_static(int evt, void* pThis);
	void onCanvasEvent(int evt);
	static void symbolSelectedCallback_static(SymbolId id, void* pThis) {
			((CanvasController*)pThis)->onSymbolSelected(id);
	}
	void onSymbolSelected(SymbolId id);

private:
	Canvas* _pCanvas;
	SymbolId _symbolId = SymbolId::NONE;
	bool _isSymbolDialogOpen = false;
};

#endif /* CONTROLLERSTUFF_H_ */
