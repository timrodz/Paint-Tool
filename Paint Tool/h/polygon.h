#ifndef __POLYGON_H__
#define __POLYGON_H__

//	Library Includes
#include <windows.h>
#include <windowsx.h>
#include "shape.h"

class CPolygon : public IShape {

public:
	CPolygon();
	CPolygon(COLORREF _newColor);
	CPolygon(EBRUSHSTYLE _iBrushStyle, int _iHatchStyle, COLORREF _FillColor, int _iPenStyle, int _iPenWidth, COLORREF _iPenColor);
	virtual ~CPolygon();

	virtual void Draw(HDC _hdc);
	void CompleteShape(POINT* _points, int _pointAmmount);
	int GetPoints() const;

private:
	bool m_bIsShapeComplete;
	COLORREF m_Color;
	POINT* m_pPointList;
	int m_nPoints;

};

#endif //__POLYGON_H__