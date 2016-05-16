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

	SelectObject(_hdc, CreatePen(m_iStyle, m_iWidth, m_Color));
	MoveToEx(_hdc, m_iStartX, m_iStartY, NULL);
	LineTo(_hdc, m_iEndX, m_iEndY);

}

void CLine::SetWidth(int _iNewWidth) {

	m_iWidth = _iNewWidth;

}