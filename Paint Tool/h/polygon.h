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
	virtual ~CPolygon();

	virtual void Draw(HDC _hdc);
	
	void SetColor(COLORREF _newColor);
	void AddPoint(POINT p);

private:
	COLORREF m_Color;
	POINT* m_pPointList;
	int m_nPoints;

};

#endif //__POLYGON_H__