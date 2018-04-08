#pragma once

namespace SOC_Utill { 

	const float DEF_W_VIEW = 400.f;
	const float DEF_H_VIEW = 600.f;
	const float DEF_W_RESOLUTION = 2000.f;
	const float DEF_H_RESOLUTION = 3000.f;
	const float DIST_PERSEC = 100.f;
	const int LISTSIZE_SIGN = (int)DEF_W_RESOLUTION / (int)DIST_PERSEC;

	class ProGraph2D
	{
	public:
		typedef std::vector<float> HEIGHT_VECTOR;
	public:
		ProGraph2D(float wResolution = DEF_W_RESOLUTION, float hResolution = DEF_H_RESOLUTION);
		virtual ~ProGraph2D();

		HEIGHT_VECTOR& Get() { return m_pointList; }
//		operator POINT2D_VECTOR& () { return m_pointList; }

		float Insert(float height);
		void Clear();
		void Draw(float thickness = 1.f, float growness_w = 1.f, float growness_h = 1.f, int type = GL_LINE_STRIP);
		void Update(int sign = LISTSIZE_SIGN);
		void Scale(float w, float h) 
		{
			m_wScale = w / m_wResolution;
			m_hScale = h / m_hResolution;
		}

	private:
		float m_wResolution;
		float m_hResolution;
		float m_wScale;
		float m_hScale;
		int m_size;
		HEIGHT_VECTOR m_pointList;
	};
}