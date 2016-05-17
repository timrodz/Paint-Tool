#include "..\h\shape.h"

// virtual
IShape::IShape() {}

IShape::~IShape() {}

void IShape::SetStartX(int _iStartX) {

	m_iStartX = _iStartX;

}

void IShape::SetEndX(int _iEndX) {

	m_iEndX = _iEndX;

}

void IShape::SetStartY(int _iStartY) {

	m_iStartY = _iStartY;

}

void IShape::SetEndY(int _iEndY) {

	m_iEndY = _iEndY;

}

const int IShape::GetStartX() const {

	return m_iStartX;

}

const int IShape::GetEndX() const {

	return m_iEndX;

}

const int IShape::GetStartY() const {

	return m_iStartY;

}

const int IShape::GetEndY() const {

	return m_iEndY;

}