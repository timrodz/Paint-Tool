#include "..\h\polygon.h"

CPolygon::CPolygon() {}

CPolygon::CPolygon(EBRUSHSTYLE _iBrushStyle, int _iHatchStyle, COLORREF _FillColor, int _iPenStyle, int _iPenWidth, COLORREF _iPenColor) {

	m_pPointList = new POINT[100];
	m_iBrushStyle = _iBrushStyle;
	m_iHatchStyle = _iHatchStyle;
	m_iFillColor = _FillColor;
	m_iPenStyle = _iPenStyle;
	m_iPenWidth = _iPenWidth;
	m_iPenColor = _iPenColor;
	m_nPoints = 0;

}

// virtual
CPolygon::~CPolygon() {

	delete[] m_pPointList;
	m_pPointList = 0;

}

// virtual
void CPolygon::Draw(HDC _hdc) {

	// Create Objects
	HBRUSH brush = nullptr;

	if (m_iBrushStyle == SOLID) {
		brush = CreateSolidBrush(m_iFillColor);
	}
	else {
		brush = CreateHatchBrush(m_iHatchStyle, m_iFillColor);

	}

	HPEN pen = CreatePen(m_iPenStyle, m_iPenWidth, m_iPenColor);

	SelectObject(_hdc, brush);
	SelectObject(_hdc, pen);

	Polygon(_hdc, m_pPointList, m_nPoints);

	DeleteObject(brush);
	DeleteObject(pen);

}

void CPolygon::AddPoint(POINT _p) {

	m_pPointList[m_nPoints] = _p;
	m_nPoints++;

}

int CPolygon::GetPoints() const {

	return m_nPoints;

}