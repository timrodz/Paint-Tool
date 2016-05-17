#include "..\h\canvas.h"
#include "..\h\shape.h"
#include "..\h\stamp.h"
#include "..\h\backBuffer.h"

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

	// Drawing the stamps
	/*static std::vector<CStamp*>::const_iterator stamps;
	for (stamps = m_vecStamps.begin(); stamps != m_vecStamps.end(); stamps++) {
		(*stamps)->Draw(m_pBackBuffer->GetBFDC());
	}*/

	m_pBackBuffer->Present();

	return true;

}

void CCanvas::Save(HWND _hwnd) {

	if (!_hwnd)
		return;



}

void CCanvas::AddShape(IShape* _s) {

	m_shapes.push_back(_s);

}

void CCanvas::AddStamp(CStamp* _s) {

	m_vecStamps.push_back(_s);

}