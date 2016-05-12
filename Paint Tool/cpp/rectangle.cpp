#include "..\h\rectangle.h"

CRectangle::CRectangle() {


	
}

CRectangle::CRectangle(EBRUSHSTYLE _iBrushStyle, int _iHatchStyle,  COLORREF _FillColor, int _iPenStyle, COLORREF _iPenColor) {

	m_iBrushStyle = _iBrushStyle;
	m_iHatchStyle = _iHatchStyle;
	m_iFillColor = _FillColor;
	m_iPenStyle = _iPenStyle;
	m_iPenColor = _iPenColor;
	
}
	
// virtual
CRectangle::~CRectangle() {


	
}

// virtual
void CRectangle::Draw(HDC _hdc) {

	SelectObject(_hdc, CreateSolidBrush(m_iFillColor));

	Rectangle(_hdc, m_iStartX, m_iStartY, m_iEndX, m_iEndY);
	
}

void CRectangle::SetBrushStyle(EBRUSHSTYLE _brushStyle) {


	
}

void CRectangle::SetFillColor(COLORREF _newColor) {


	
}

void CRectangle::SetPenStyle (int _iPenStyle) {


	
}

void CRectangle::SetPenColor(COLORREF _newColor) {


	
}

void CRectangle::SetHatchStyle(int _iHatchStyle) {


	
}