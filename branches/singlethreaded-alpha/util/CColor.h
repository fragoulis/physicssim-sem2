#ifndef __TLIB_UTIL_CCOLOR_H__
#define __TLIB_UTIL_CCOLOR_H__
#pragma once

namespace tlib
{
namespace util
{

/** 
 * Color taken directly from the tutorials
 */
class CColor {
private:
	float m_rgba[4];

public:
	void Assign(float r, float g, float b, float a);
    void Assign(float rgba[]);
    void SetAlpha( float a ) { m_rgba[3] = a; }

	CColor();
	CColor(float r, float g, float b, float a);
	CColor(float rgba[]);

	const float* rgba() const;

	static CColor black();
	static CColor white();
	static CColor red();
	static CColor green();
	static CColor blue();
	static CColor yellow();
	static CColor null();
};

} // end namespace util
} // end namespace tlib

#endif // __TLIB_UTIL_CCOLOR_H__