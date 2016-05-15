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

	//HPEN pen = CreatePen(m_iStyle, m_iWidth, m_Color);

	//HPEN old_pen = static_cast<HPEN>(SelectObject(_hdc, pen));

	SelectObject(_hdc, CreatePen(m_iStyle, m_iWidth, m_Color));

	MoveToEx(_hdc, m_iStartX, m_iStartY, NULL);
	LineTo(_hdc, m_iEndX, m_iEndY);
	
	//SelectObject(_hdc, old_pen);
	//DeleteObject(pen);

}

void CLine::SetWidth(int _iNewWidth) {

	m_iWidth = _iNewWidth;

}