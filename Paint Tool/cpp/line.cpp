#include "..\h\line.h"

CLine::CLine() {}

CLine::CLine(int _iStyle, int _iWidth, COLORREF _newColor) {

	m_iPenWidth = _iWidth;
	m_iPenStyle = _iStyle;
	m_iPenColor = _newColor;

}

// virtual
CLine::~CLine() {}

// virtual
void CLine::Draw(HDC _hdc) {

	HPEN pen = CreatePen(m_iPenStyle, m_iPenWidth, m_iPenColor);
	SelectObject(_hdc, pen);
	MoveToEx(_hdc, m_iStartX, m_iStartY, NULL);
	LineTo(_hdc, m_iEndX, m_iEndY);
	DeleteObject(pen);

}