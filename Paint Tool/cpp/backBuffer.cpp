#include "..\h\backBuffer.h"

//CBackBuffer::CBackBuffer() :
//	m_hWnd(0),
//	m_hDC(0),
//	m_hSurface(0),
//	m_hOldBitmap(0),
//	m_iWidth(0),
//	m_iHeight(0)
//{}

CBackBuffer::CBackBuffer() : 
	m_hWnd(0), 
	m_hDC(0), 
	m_hSurface(0), 
	m_hOldObject(0), 
	m_iWidth(0), 
	m_iHeight(0)
{

}

CBackBuffer::~CBackBuffer() {

	/*SelectObject(m_HDC, m_hOldBitmap);
	DeleteObject(m_hBFBitmap);
	DeleteObject(m_BFDC);*/

	SelectObject(m_hDC, m_hOldObject);

	DeleteObject(m_hSurface);
	DeleteObject(m_hDC);

}

bool CBackBuffer::Initialize(HWND _hWnd, int _iWidth, int _iHeight) {

	/*m_hwnd = _hwnd;
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

	return true;*/

	m_hWnd = _hWnd;

	m_iWidth = _iWidth;
	m_iHeight = _iHeight;

	HDC hWindowDC = ::GetDC(m_hWnd);

	m_hDC = CreateCompatibleDC(hWindowDC);

	m_hSurface = CreateCompatibleBitmap(hWindowDC, m_iWidth, m_iHeight);

	ReleaseDC(m_hWnd, hWindowDC);

	m_hOldObject = static_cast<HBITMAP>(SelectObject(m_hDC, m_hSurface));

	HBRUSH brushWhite = static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH));
	HBRUSH oldBrush = static_cast<HBRUSH>(SelectObject(m_hDC, brushWhite));

	Rectangle(m_hDC, 0, 0, m_iWidth, m_iHeight);

	SelectObject(m_hDC, oldBrush);

	return (true);

}

HDC CBackBuffer::GetBFDC() const {

	return m_hDC;

}

int CBackBuffer::GetHeight() const {

	return m_iHeight;

}

int CBackBuffer::GetWidth() const {

	return m_iWidth;

}

void CBackBuffer::Clear() {

	/*HBRUSH brush_old = static_cast<HBRUSH>(SelectObject(GetBFDC(), GetStockObject(LTGRAY_BRUSH)));

	Rectangle(GetBFDC(), 0, 0, m_iWidth, m_iHeight);

	SelectObject(GetBFDC(), brush_old);*/

	HBRUSH hOldBrush = static_cast<HBRUSH>(SelectObject(GetBFDC(), GetStockObject(LTGRAY_BRUSH)));

	Rectangle(GetBFDC(), 0, 0, GetWidth(), GetHeight());

	SelectObject(GetBFDC(), hOldBrush);

}

void CBackBuffer::Present() {

	/*HDC hWindowDC = GetDC(m_hWnd);

	BitBlt(hWindowDC, 0, 0, m_iWidth, m_iHeight, m_HDC, 0, 0, SRCCOPY);

	ReleaseDC(m_hWnd, hWindowDC);*/

	HDC hWndDC = ::GetDC(m_hWnd);

	BitBlt(hWndDC, 0, 0, m_iWidth, m_iHeight, m_hDC, 0, 0, SRCCOPY);

	ReleaseDC(m_hWnd, hWndDC);

}