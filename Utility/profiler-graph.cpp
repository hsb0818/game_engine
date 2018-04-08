#include "../pch/pch.h"
#include "../GInclude.h"

namespace SOC_Utill {

	ProGraph2D::ProGraph2D(float wResolution, float hResolution)
		: m_size(0)
		, m_wResolution(wResolution)
		, m_hResolution(hResolution)
	{
		m_pointList.reserve(32);
		Scale(DEF_W_VIEW, DEF_H_VIEW);
	}

	ProGraph2D::~ProGraph2D()
	{
	}

	float ProGraph2D::Insert(float height)
	{
		// y frame transformation.
		m_pointList.push_back(R_SOC_APP().GetScreenHeight() - height);
		++m_size;

		return (*m_pointList._Mylast);
	}

	void ProGraph2D::Clear()
	{
		HEIGHT_VECTOR(m_pointList).swap(m_pointList);
	}

	void ProGraph2D::Update(int sign)
	{
		if (m_size > sign)
		{
			m_pointList.erase(m_pointList.begin(), m_pointList.begin() + sign);
			m_size = m_pointList.size();
		}
	}

	void ProGraph2D::Draw(float thickness, float growness_w, float growness_h, int type)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(0.f, 50.f, 0.f);
		glScalef(m_wScale, m_hScale, 1.f);
//		glScalef(m_wScale, 1.f, 1.f);
		glTranslatef(0.0f, 0.f, 0.0f);

		glLineWidth(thickness);

		glBegin(type);
		{
			for (int i=0; i<m_size; ++i)
			{
				glVertex2f(i * DIST_PERSEC * growness_w, m_pointList[i] * growness_h);
			}
		}
		glEnd();
	}
}