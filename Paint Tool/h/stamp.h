#ifndef __STAMP_H__
#define __STAMP_H__

#include <Windows.h>
#include "shape.h"

class CStamp : public IShape {

public:
	CStamp();
	CStamp(HINSTANCE _hInstance, wchar_t* _filename);
	CStamp(HINSTANCE _hInstance, wchar_t* _filename, int _x, int _);
	virtual ~CStamp();
	
	virtual void Draw(HDC _hdc);
	
	void SetStartX(int _iX);
	void SetStartY(int _iY);

private:
	HBITMAP m_hBitMap;
	BITMAP m_bitmapStructure;

};

#endif //__STAMP_H__