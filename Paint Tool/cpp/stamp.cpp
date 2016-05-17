#include "..\h\stamp.h"

CStamp::CStamp() :
	m_hBitMap(0)
{}

CStamp::CStamp(HINSTANCE _hInstance, wchar_t* _filename) {

	//m_
	m_hBitMap = (HBITMAP)LoadImage(_hInstance, _filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//m_hBitMap = LoadBitmap(_hInstance, _filename);
	//GetObject(m_hBitMap, sizeof(BITMAP), &m_bitmapStructure);

}

CStamp::CStamp(HINSTANCE _hInstance, wchar_t* _filename, int _x, int _y) {

	//m_
	m_hBitMap = (HBITMAP)LoadImage(_hInstance, _filename, IMAGE_BITMAP, _x, _y, LR_LOADFROMFILE);
	//m_hBitMap = LoadBitmap(_hInstance, L"C:\\Users\\Juan\\Desktop\\star.bmp");
	m_iStartX = _x;
	m_iStartY = _y;
	//GetObject(m_hBitMap, sizeof(BITMAP), &m_bitmapStructure);
	
}

// virtual
CStamp::~CStamp() {

	DeleteObject(m_hBitMap);

}
	
// virtual
void CStamp::Draw(HDC _hdc) {

	HDC hWindowDC = CreateCompatibleDC(_hdc);

	HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(hWindowDC, m_hBitMap));

	//GetObject(m_hBitMap, sizeof(m_bitmapStructure), &m_bitmapStructure);

	BitBlt(_hdc, m_iStartX, m_iStartY, m_bitmapStructure.bmWidth, m_bitmapStructure.bmHeight, hWindowDC, 0, 0, SRCCOPY);
	
	SelectObject(hWindowDC, hOldBitmap);
	DeleteDC(hWindowDC);
	
}