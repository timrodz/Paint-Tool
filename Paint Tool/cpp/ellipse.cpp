#include "..\h\ellipse.h"

CEllipse::CEllipse() {


	
}

CEllipse::CEllipse(COLORREF _newColor) {

	m_Color = _newColor;
	
}

// virtual
CEllipse::~CEllipse() {


	
}

// virtual
void CEllipse::Draw(HDC _hdc) {

	SelectObject(_hdc, CreateSolidBrush(m_Color));

	Ellipse(_hdc, m_iStartX, m_iStartY, m_iEndX, m_iEndY);
	
}
	
void CEllipse::SetColor(COLORREF _newColor) {

	m_Color = _newColor;
	
}