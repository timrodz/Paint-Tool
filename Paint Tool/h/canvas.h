#ifndef __CANVAS_H__
#define __CANVAS_H__

//	Library Includes
#include <windows.h>
#include <windowsx.h>
#include <vector>

enum EShape {
	LINE,
	RECTANGLE,
	ELLIPSE,
	POLYGON,
	STAMP,
	NONE
};

class IShape; //Forward Declaration
class CLine;
class CStamp;
class CBackBuffer;

class CCanvas {

public:
	CCanvas();
	~CCanvas();

	CBackBuffer* GetBackBuffer();

	bool Initialize(HWND _hwnd, int _iWidth, int _iHeight);
	bool Draw(HDC _hdc);
	void Save(HWND _hwnd);
	void AddShape(IShape* _s);
	void Undo();
	void Redo();
	int GetEncoderCLSID(const WCHAR* format, CLSID* pClsid);

private:
	CBackBuffer* m_pBackBuffer; // A canvas has a backbuffer. Stops flickering
	std::vector<CStamp*> m_vecStamps;
	std::vector<IShape*> m_shapes; // container to store shapes
	std::vector<IShape*> m_tempShapes;

};

#endif //__CANVAS_H__