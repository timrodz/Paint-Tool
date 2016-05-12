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

	HBRUSH brush = CreateSolidBrush(m_Color);

	SelectObject(_hdc, brush); // Restore old pen.
	Ellipse(_hdc, m_iStartX, m_iStartY, m_iEndX, m_iEndY);
	DeleteObject(brush); // Delete the green pen.
	
}
	
void CEllipse::SetColor(COLORREF _newColor) {


	
}