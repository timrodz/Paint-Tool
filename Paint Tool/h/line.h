#ifndef __LINE_H__
#define __LINE_H__

//	Library Includes
#include <windows.h>
#include <windowsx.h>
#include "shape.h"

class CLine : public IShape {

public:
	CLine();
	CLine(int _iPenStyle, int _iPenWidth, COLORREF _newColor); 
	virtual ~CLine();

	virtual void Draw(HDC _hdc);
	
};

#endif //__LINE_H__