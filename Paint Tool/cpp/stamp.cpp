#include "..\h\stamp.h"

CStamp::CStamp() :
	m_hBitMap(0)
{}

CStamp::CStamp(HBITMAP _bitmap, int _x, int _y) {

	m_hBitMap = _bitmap;
	m_iStartX = _x;
	m_iStartY = _y;
	GetObject(m_hBitMap, sizeof(BITMAP), &m_bitmapStructure);

}

// virtual
CStamp::~CStamp() {

	DeleteObject(m_hBitMap);

}

// virtual
void CStamp::Draw(HDC _hdc) {

	HDC hWindowDC = CreateCompatibleDC(_hdc);

	HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(hWindowDC, m_hBitMap));

	BitBlt(_hdc, m_iStartX, m_iStartY, m_bitmapStructure.bmWidth, m_bitmapStructure.bmHeight, hWindowDC, 0, 0, SRCCOPY);

	SelectObject(hWindowDC, hOldBitmap);
	DeleteDC(hWindowDC);

}