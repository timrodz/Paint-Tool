#include "..\h\canvas.h"
#include "..\h\shape.h"
#include "..\h\backBuffer.h"

CCanvas::CCanvas() {

	m_pBackBuffer = new CBackBuffer();
	m_shapes.clear();

}

CCanvas::~CCanvas() {

	delete tempShape;
	tempShape = 0;

	delete m_pBackBuffer;
	m_pBackBuffer = 0;

}

CBackBuffer* CCanvas::GetBackBuffer() {

	return m_pBackBuffer;

}

bool CCanvas::Initialize(HWND _hwnd, int _iWidth, int _iHeight) {

	//if (m_pBackBuffer == nullptr)
	//m_pBackBuffer->Initialize(_hwnd, _iWidth, _iHeight);
	//m_shapes.clear();

	while (m_shapes.size() != 0) {
		UndoShape();
	}

	return true;

}

bool CCanvas::Draw(HDC _hdc) {

	//m_pBackBuffer->Clear();

	static std::vector<IShape*>::const_iterator it;

	for (it = m_shapes.begin(); it != m_shapes.end(); it++) {
		(*it)->Draw(_hdc);
	}

	//m_pBackBuffer->Present();

	return true;

}

void CCanvas::Save(HWND _hwnd) {



}

void CCanvas::AddShape(IShape* _s) {

	m_shapes.push_back(_s);

}

void CCanvas::UndoShape() {
	
	if (m_shapes.size() > 0) {

		tempShape = m_shapes.back();
		m_shapes.pop_back();

	}

}

void CCanvas::RedoShape() {

	if (tempShape != nullptr) {
		m_shapes.push_back(tempShape);
	}

}

void CCanvas::AddStamp(CStamp*) {



}