#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

//	Library Includes
#include <windows.h>
#include <windowsx.h>
#include "shape.h"


class CRectangle : public IShape {

public:
	CRectangle();
	CRectangle(EBRUSHSTYLE _iBrushStyle, int _iHatchStyle, COLORREF _FillColor, int _iPenWidth, int _iPenStyle, COLORREF _PenColor);
	virtual ~CRectangle();

	virtual void Draw(HDC _hdc);

	void SetBrushStyle(EBRUSHSTYLE _brushStyle);
	void SetFillColor(COLORREF _newColor);
	void SetPenStyle(int _iPenStyle);
	void SetPenColor(COLORREF _newColor);
	void SetHatchStyle(int _iHatchStyle);

};

#endif //__RECTANGLE_H__