#ifndef __ELLIPSE_H__
#define __ELLIPSE_H__

//	Library Includes
#include <windows.h>
#include <windowsx.h>
#include "shape.h"

class CEllipse : public IShape
{
public:
	CEllipse();
	CEllipse(EBRUSHSTYLE _iBrushStyle, int _iHatchStyle, COLORREF _FillColor, int _iPenStyle, int _iPenWidth, COLORREF _PenColor);
	virtual ~CEllipse();

	virtual void Draw(HDC _hdc);

};

#endif //__ELLIPSE_H__