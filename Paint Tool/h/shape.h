#ifndef __SHAPE_H__
#define __SHAPE_H__

//	Library Includes
#include <windows.h>
#include <windowsx.h>

enum EBRUSHSTYLE {
	SOLID,
	HATCH
};

class IShape {

public:
	IShape();
	virtual ~IShape();

	virtual void Draw(HDC _hdc) = 0;  // pure virtual

	void SetStartX(int _iStartX);
	void SetEndX(int _iEndX);
	void SetStartY(int _iStartY);
	void SetEndY(int _iEndY);

	const int GetStartX() const;
	const int GetEndX() const;
	const int GetStartY() const;
	const int GetEndY() const;

protected:
	int m_iStartX; //top left
	int m_iEndX;   // bottom left, changes with mouse
	int m_iStartY; // top left
	int m_iEndY;  // bottom left, changes with mouse

	EBRUSHSTYLE m_iBrushStyle;
	COLORREF m_iFillColor;
	COLORREF m_Color;
	int m_iHatchStyle;
	int m_iPenStyle;
	int m_iPenWidth;
	int m_iPenColor;

};

#endif //__SHAPE_H__