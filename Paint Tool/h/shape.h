#ifndef __SHAPE_H__
#define __SHAPE_H__

//	Library Includes
#include <windows.h>
#include <windowsx.h>

class IShape {

public:
	IShape();
	virtual ~IShape();

	virtual void Draw(HDC _hdc) = 0;  // pure virtual

	void SetStartX(int _iStartX);
	void SetEndX(int _iEndX);
	void SetStartY(int _iStartY);
	void SetEndY(int _iEndY);
	//void SetColor(COLORREF _newColor);

protected:
	int m_iStartX; //top left
	int m_iEndX;   // bottom left, changes with mouse
	int m_iStartY; // top left
	int m_iEndY;  // bottom left, changes with mouse
	//COLORREF m_ColorRef;

};

#endif //__SHAPE_H__