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

	if (m_iBrushStyle == SOLID) {
		SelectObject(_hdc, CreateSolidBrush(m_iFillColor));
	}
	else {
		SelectObject(_hdc, CreateHatchBrush(m_iHatchStyle, m_iFillColor));
	}

	SelectObject(_hdc, CreatePen(m_iPenStyle, 1, m_iPenColor));

	Rectangle(_hdc, m_iStartX, m_iStartY, m_iEndX, m_iEndY);

	
	
}

void CRectangle::SetBrushStyle(EBRUSHSTYLE _brushStyle) {

	m_iBrushStyle = _brushStyle;
	
}

void CRectangle::SetFillColor(COLORREF _newColor) {

	m_Color = _newColor;
	
}

void CRectangle::SetPenStyle (int _iPenStyle) {

	m_iPenStyle = _iPenStyle;
	
}

void CRectangle::SetPenColor(COLORREF _newColor) {

	m_iPenColor = _newColor;
	
}

void CRectangle::SetHatchStyle(int _iHatchStyle) {

	m_iHatchStyle = _iHatchStyle;
	
}