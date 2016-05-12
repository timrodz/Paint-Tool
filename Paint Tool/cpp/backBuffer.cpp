#include "..\h\backBuffer.h"

CBackBuffer::CBackBuffer() :
	m_hwnd(0),
	m_BFDC(0),
	m_hBFBitmap(0),
	m_hOldBitmap(0),
	m_iWidth(0),
	m_iHeight(0)
{}

CBackBuffer::~CBackBuffer() {

	SelectObject(m_BFDC, m_hOldBitmap);
	DeleteObject(m_hBFBitmap);
	DeleteObject(m_BFDC);

}

bool CBackBuffer::Initialise(HWND _hwnd, int _iWidth, int _iHeight) {

	m_hwnd = _hwnd;
	m_iWidth = _iWidth;
	m_iHeight = _iHeight;

	HDC hWindowDC = GetDC(m_hwnd);

	m_BFDC = CreateCompatibleDC(hWindowDC);

	m_hBFBitmap = CreateCompatibleBitmap(hWindowDC, m_iWidth, m_iHeight);

	ReleaseDC(m_hwnd, hWindowDC);

	m_hOldBitmap = static_cast<HBITMAP>(SelectObject(m_BFDC, m_hBFBitmap));

	HBRUSH brush_white = static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH));
	HBRUSH brush_old = static_cast<HBRUSH>(SelectObject(m_BFDC, brush_white));

	Rectangle(m_BFDC, 0, 0, m_iWidth, m_iHeight);

	SelectObject(m_BFDC, brush_old);

	return true;

}

HDC CBackBuffer::GetBFDC() const {

	return m_BFDC;

}

int CBackBuffer::GetHeight() const {

	return m_iHeight;

}

int CBackBuffer::GetWidth() const {

	return m_iWidth;

}

void CBackBuffer::Clear() {

	HBRUSH brush_old = static_cast<HBRUSH>(SelectObject(GetBFDC(), GetStockObject(LTGRAY_BRUSH)));

	Rectangle(GetBFDC(), 0, 0, m_iWidth, m_iHeight);

	SelectObject(GetBFDC(), brush_old);

}

void CBackBuffer::Present() {

	HDC hWindowDC = GetDC(m_hwnd);

	BitBlt(hWindowDC, 0, 0, m_iWidth, m_iHeight, m_BFDC, 0, 0, SRCCOPY);

	ReleaseDC(m_hwnd, hWindowDC);

}