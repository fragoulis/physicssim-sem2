#include "CColor.h"
using namespace tlib::util;

void CColor::Assign( float rgba[] ) {
	m_rgba[0] = rgba[0];
	m_rgba[1] = rgba[1];
	m_rgba[2] = rgba[2];
	m_rgba[3] = rgba[3];
}

void CColor::Assign(float r, float g, float b, float a) {
	m_rgba[0] = r;
	m_rgba[1] = g;
	m_rgba[2] = b;
	m_rgba[3] = a;
}

CColor::CColor() { 
	Assign(0.0, 0.0, 0.0, 0.0);
}

CColor::CColor(float r, float g, float b, float a) {
	Assign(r, g, b, a);
}

CColor::CColor(float rgba[]) {
	Assign(rgba[0], rgba[1], rgba[2], rgba[3]);
}

const float* CColor::rgba() const {
	return m_rgba;
}

CColor CColor::black() {
	return CColor(0.0, 0.0, 0.0, 1.0);
}

CColor CColor::white() {
	return CColor(1.0, 1.0, 1.0, 1.0);
}

CColor CColor::red() {
	return CColor(1.0, 0.0, 0.0, 1.0);
}

CColor CColor::green() {
	return CColor(0.0, 1.0, 0.0, 1.0);
}

CColor CColor::blue() {
	return CColor(0.0, 0.0, 1.0, 1.0);
}

CColor CColor::yellow() {
	return CColor(1.0, 1.0, 0.0, 1.0);
}

CColor CColor::null() {
	return CColor(0.0, 0.0, 0.0, 0.0);
}
