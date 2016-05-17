#ifndef __CANVAS_H__
#define __CANVAS_H__

//	Library Includes
#include <windows.h>
#include <windowsx.h>
#include <vector>

typedef enum {
	LINE = 0,
	BOX,
	ELLIPSE,
	MAX_SHAPE
} EShape;

class IShape; //Forward Declaration
class CLine;
class CStamp;
class CBackBuffer;

class CCanvas { // container of all shapes 

public:
	CCanvas();
	~CCanvas();

	CBackBuffer* GetBackBuffer();

	bool Initialize(HWND _hwnd, int _iWidth, int _iHeight);
	bool Draw(HDC _hdc);
	void Save(HWND _hwnd);
	void AddShape(IShape* _s);
	void AddStamp(CStamp* _s);

private:
	CBackBuffer* m_pBackBuffer; // A canvas has a backbuffer. Stops flickering
	std::vector<CStamp*> m_vecStamps;
	std::vector<IShape*> m_shapes; // container to store shapes
};

#endif //__CANVAS_H__