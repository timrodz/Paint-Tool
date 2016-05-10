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
	CEllipse(COLORREF _newColor);
	virtual ~CEllipse();

	virtual void Draw(HDC _hdc);
	
	void SetColor(COLORREF _newColor);

private:
	COLORREF m_Color;

};

#endif //__ELLIPSE_H__