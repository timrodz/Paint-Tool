#ifndef __BACKBUFFER_H__
#define __BACKBUFFER_H__

//Library Includes
#include <Windows.h>

class CBackBuffer
{
public:
	CBackBuffer();
	~CBackBuffer();

	bool Initialize(HWND _hwnd, int _iWidth, int _iHeight);

	void Clear();
	void Present();

	HDC GetBFDC() const;
	void SetBFBitmap(HBITMAP _bitmap);
	HBITMAP GetBFBitmap() const;
	HWND GetHWND() const;
	int GetHeight() const;
	int GetWidth() const;

	// Can't be copied
private:
	CBackBuffer(const CBackBuffer&);
	CBackBuffer& operator = (const CBackBuffer&);

private:
	HWND m_hwnd;
	HDC m_BFDC;				//Handle to the in-memory DC for holding the backbuffer 
	HBITMAP m_hBFBitmap;	//handle to the bitmap that represents the backbuffer
	HBITMAP m_hOldBitmap;	//Used in select object to save the old bitmap.
	int m_iWidth;
	int m_iHeight;

};

#endif //__BACKBUFFER_H__