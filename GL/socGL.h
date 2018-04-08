#ifndef _SOCGL_H_
#define _SOCGL_H_

// ��� �Լ� ������ ����Ʈ�� �����.
// �׷��� �װŷ� �������� ��!

namespace SOC_GL {

	class GLApp : public Singleton<GLApp>
	{
	public:
		GLApp();
		~GLApp();

	public:
		void Init();
		bool Render_Init();
		void Render(double fElapsedTime = 0.0);
		void SetProjection(bool bPerspective) { m_bPerspective = bPerspective; }

		void ReSizeScene(int width = 1024, int height = 768);
		void ReSizeScene(bool bPerspective, int width = 1024, int height = 768);
		void UpdateProjection(bool bPerspective, float aspect, int width = 1024, int height = 768, double n = 1, double f = -1);
		void UpdateProjection(int width = 1024, int height = 768, double n = 1, double f = -1);

	public:
		float m_aspect;
		bool m_bPerspective;
		PIXELFORMATDESCRIPTOR m_pfd;
	};
}

#endif