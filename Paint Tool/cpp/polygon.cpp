#include "..\h\polygon.h"

CPolygon::CPolygon() {
	


}

CPolygon::CPolygon(COLORREF _newColor) {
	
	m_Color = _newColor;

	m_pPointList = new POINT[1];
	m_pPointList[0] = { 0, 0 }; // Initialize at 0

	if (!m_pPointList)
		return;

}

// virtual
CPolygon::~CPolygon() {
	
	delete[] m_pPointList;
	m_pPointList = 0;

}

// virtual
void CPolygon::Draw(HDC _hdc) {
	
	SelectObject(_hdc, CreateSolidBrush(m_Color));
	Polygon(_hdc, m_pPointList, m_nPoints);

}
	
void CPolygon::SetColor(COLORREF _newColor) {
	
	m_Color = _newColor;

}

void CPolygon::AddPoint(POINT p) {
	
	m_pPointList[m_nPoints] = p;
	m_nPoints++;

}