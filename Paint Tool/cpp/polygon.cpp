#include "..\h\polygon.h"

CPolygon::CPolygon() {



}

CPolygon::CPolygon(COLORREF _newColor) {

	m_bIsShapeComplete = false;
	m_Color = _newColor;
	m_pPointList = new POINT[3];

}

CPolygon::CPolygon(EBRUSHSTYLE _iBrushStyle, int _iHatchStyle, COLORREF _FillColor, int _iPenStyle, int _iPenWidth, COLORREF _iPenColor) {

	m_iBrushStyle = _iBrushStyle;
	m_iHatchStyle = _iHatchStyle;
	m_iFillColor = _FillColor;
	m_iPenStyle = _iPenStyle;
	m_iPenWidth = _iPenWidth;
	m_iPenColor = _iPenColor;

	m_bIsShapeComplete = false;
	m_pPointList = new POINT[1];

}

// virtual
CPolygon::~CPolygon() {

	delete[] m_pPointList;
	m_pPointList = 0;

}

// virtual
void CPolygon::Draw(HDC _hdc) {

	if (!m_bIsShapeComplete) {

		HPEN pen = CreatePen(m_iPenStyle, m_iPenWidth, m_iPenColor);
		SelectObject(_hdc, pen);
		MoveToEx(_hdc, m_iStartX, m_iStartY, NULL);
		LineTo(_hdc, m_iEndX, m_iEndY);
		DeleteObject(pen);

	}
	else {

		HPEN pen = CreatePen(m_iPenStyle, m_iPenWidth, m_iPenColor);
		HBRUSH brush = CreateSolidBrush(m_Color);
		SelectObject(_hdc, brush);
		SelectObject(_hdc, pen);
		Polygon(_hdc, m_pPointList, m_nPoints);
		DeleteObject(brush);
		DeleteObject(pen);

	}

}

void CPolygon::CompleteShape(POINT* _points, int _pointAmmount) {

	m_nPoints = _pointAmmount;

	for (int i = 0; i < m_nPoints; i++) {

		m_pPointList[i] = _points[i];

	}

	m_bIsShapeComplete = true;

}

int CPolygon::GetPoints() const {

	return m_nPoints;

}