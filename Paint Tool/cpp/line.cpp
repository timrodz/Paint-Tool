#include "..\h\line.h"

CLine::CLine() {}

CLine::CLine(int _iStyle, int _iWidth, COLORREF _newColor) {

	m_iStyle = _iStyle;
	m_iWidth = _iWidth;
	m_Color = _newColor;

} 

// virtual
CLine::~CLine() {}

// virtual
void CLine::Draw(HDC _hdc) {

	HPEN green_pen = CreatePen(m_iStyle, m_iWidth, m_Color);

	// Select the pen into the context:
	HPEN old_pen = static_cast<HPEN>(SelectObject(_hdc, green_pen));
	MoveToEx(_hdc, m_iStartX, m_iStartY, NULL); // Draw the line...
	LineTo(_hdc, m_iEndX, m_iEndY);
	SelectObject(_hdc, old_pen); // Restore old pen.
	DeleteObject(green_pen); // Delete the green pen.

}

void CLine::SetWidth(int _iNewWidth) {

	m_iWidth = _iNewWidth;

}