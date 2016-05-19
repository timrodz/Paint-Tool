#ifndef __STAMP_H__
#define __STAMP_H__

#include <Windows.h>
#include "shape.h"

class CStamp : public IShape {

public:
	CStamp();
	CStamp(HBITMAP _bitmap, int _x, int _y);
	virtual ~CStamp();

	virtual void Draw(HDC _hdc);

	void SetStartX(int _iX);
	void SetStartY(int _iY);

private:
	HBITMAP m_hBitMap;
	BITMAP m_bitmapStructure;

};

#endif //__STAMP_H__