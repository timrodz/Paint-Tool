#include "..\h\canvas.h"
#include "..\h\shape.h"
#include "..\h\stamp.h"
#include "..\h\backBuffer.h"
#include "..\util.h"
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

CCanvas::CCanvas() {


	m_pBackBuffer = nullptr;

}

CCanvas::~CCanvas() {

	delete m_pBackBuffer;
	m_pBackBuffer = 0;

}

CBackBuffer* CCanvas::GetBackBuffer() {

	return m_pBackBuffer;

}

bool CCanvas::Initialize(HWND _hwnd, int _iWidth, int _iHeight) {

	m_shapes.clear();
	m_vecStamps.clear();
	m_tempShapes.clear();

	if (m_pBackBuffer == nullptr) {

		m_pBackBuffer = new CBackBuffer();
		m_pBackBuffer->Initialize(_hwnd, _iWidth, _iHeight);

	}

	return true;

}

bool CCanvas::Draw(HDC _hdc) {

	m_pBackBuffer->Clear();

	// Drawing the shapes
	static std::vector<IShape*>::const_iterator shapes;
	for (shapes = m_shapes.begin(); shapes != m_shapes.end(); shapes++) {

		(*shapes)->Draw(m_pBackBuffer->GetBFDC());

	}



	m_pBackBuffer->Present();

	return true;

}

void CCanvas::Save(HWND _hwnd) {

	if (!_hwnd)
		return;

	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = _hwnd;
	ofn.lpstrFilter = L"PNG Files (*.png)\0*.png\0JPEG Files (*.jpeg)\0*.jpeg\0Bitmap Files (*.bmp)\0*.bmp\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"png";

	// Do something usefull with the filename stored in szFileName 
	if (GetSaveFileName(&ofn)) {

		Bitmap* pBitmap = new Bitmap(m_pBackBuffer->GetBFBitmap(), NULL);

		BYTE* pImageData = new BYTE[m_pBackBuffer->GetWidth() * 4 * m_pBackBuffer->GetHeight()];

		Bitmap bmp(m_pBackBuffer->GetWidth(), m_pBackBuffer->GetHeight(), 4 * m_pBackBuffer->GetWidth(), PixelFormat32bppARGB, pImageData);
		Gdiplus::Graphics *g = Gdiplus::Graphics::FromImage(&bmp);
		g->Clear(Color(255, 255, 255, 255));

		CLSID clsid;

		GetEncoderCLSID(L"image/png", &clsid);
		pBitmap->Save(szFileName, &clsid, NULL);

		delete[] pImageData;
		delete g;

	}

}

void CCanvas::AddShape(IShape* _s) {

	m_shapes.push_back(_s);

}

void CCanvas::Undo() {

	if (m_shapes.size() >= 1) {
		m_tempShapes.push_back(m_shapes.back());
		m_shapes.pop_back();
	}

	InvalidateRect(m_pBackBuffer->GetHWND(), NULL, false);

}

void CCanvas::Redo() {

	if (m_tempShapes.size() >= 1) {

		AddShape(m_tempShapes.back());
		m_tempShapes.pop_back();

	}

	InvalidateRect(m_pBackBuffer->GetHWND(), NULL, false);

}

int CCanvas::GetEncoderCLSID(const WCHAR* format, CLSID* pClsid) {

	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));

	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure

}