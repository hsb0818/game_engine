#pragma once

namespace SOC_GL {

	class SOCFont : public BaseMP
	{
	public:
		SOCFont() {}
		~SOCFont() { KillFont(); }

		GLvoid Init(HDC hdc, int fontSize = -24)
		{
			HFONT font;
			m_base = glGenLists(96);
			font = CreateFont(fontSize,
				0,
				0,
				0,
				FW_BOLD, 
				FALSE, 
				FALSE, 
				FALSE, 
				ANSI_CHARSET, 
				OUT_TT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				ANTIALIASED_QUALITY,
				FF_DONTCARE|DEFAULT_PITCH,
				L"Courier New");

			SelectObject(hdc, font);
			wglUseFontBitmaps(hdc, 32, 96, m_base);
		}

		GLvoid KillFont(GLvoid) { glDeleteLists(m_base, 96); }

		GLvoid Render(const char *text, float x, float y, SOC_Color3f color)
		{
			glColor4f(color.r, color.g, color.b, color.a);
			glRasterPos2f(x, y);

			glPushAttrib(GL_LIST_BIT);
			glListBase(m_base - 32);
			glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
			glPopAttrib();
		}

	private:
		GLuint m_base;
	};
}