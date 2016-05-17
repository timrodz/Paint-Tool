#include "..\h\polygon.h"

CPolygon::CPolygon() {
	


}

CPolygon::CPolygon(COLORREF _newColor) {
	
	m_Color = _newColor;

	m_pPointList = new POINT[3];

}

// virtual
CPolygon::~CPolygon() {
	
	delete[] m_pPointList;
	m_pPointList = 0;

}

// virtual
void CPolygon::Draw(HDC _hdc) {
	
	HPEN pen = CreatePen(PS_SOLID, 1, m_Color);
	SelectObject(_hdc, pen);
	MoveToEx(_hdc, m_iStartX, m_iStartY, NULL);
	LineTo(_hdc, m_iEndX, m_iEndY);
	DeleteObject(pen);

	//HBRUSH brush = CreateSolidBrush(m_Color);
	//SelectObject(_hdc, brush);
	/*POINT Pt[7];
	Pt[0].x = 20;
	Pt[0].y = 50;
	Pt[1].x = 180;
	Pt[1].y = 50;
	Pt[2].x = 180;
	Pt[2].y = 20;
	Pt[3].x = 230;
	Pt[3].y = 70;
	Pt[4].x = 180;
	Pt[4].y = 120;
	Pt[5].x = 180;
	Pt[5].y = 90;
	Pt[6].x = 20;
	Pt[6].y = 90;
	Polygon(_hdc, m_pPointList, sizeof(m_pPointList) / (sizeof(m_pPointList[0])));*/
	//DeleteObject(brush);


}
	
void CPolygon::SetColor(COLORREF _newColor) {
	
	m_Color = _newColor;

}

void CPolygon::AddPoint(POINT p) {
	
	m_pPointList[m_nPoints] = p;
	m_nPoints++;

}