//////////////////////////////////////////////////////////////////////////
//
// �������������
//
// ���ߣ�΢���ƽ��(BOGY)
//
// �汾��For HGE v1.7
//
//////////////////////////////////////////////////////////////////////////
#ifndef GDIFONT_H
#define GDIFONT_H

#include "hge/hgesprite.h"
#include <string>
using namespace std;

class GfxFont
{
public:
	GfxFont(const char* lpsFontName, int nFaceSize, BOOL bBold = FALSE, BOOL bItalic = FALSE, BOOL bAntialias = TRUE);
	~GfxFont(void);

public:
	// ��Ⱦ�ı�
	virtual void	Print( float x, float y, const char *format, ... );
	virtual void	Render(float x, float y, const wchar_t* text );

	// �������ȡ��ɫ
	virtual void	SetColor( DWORD dwColor, int i = -1 );
	virtual DWORD	GetColor( int i = 0 );

	// ��ȡ�ı����
	virtual SIZE	GetTextSize( const wchar_t* text );

	// ���������ȡ�ַ�
	virtual wchar_t	GetCharacterFromPos( const wchar_t* text, float pixel_x, float pixel_y );

	// �����ּ��
	virtual void	SetKerningWidth( float kerning );
	virtual void	SetKerningHeight( float kerning );

	// ��ȡ�ּ��
	virtual float	GetKerningWidth();
	virtual float	GetKerningHeight();

	virtual float	GetStringWidth( const char* text );
	virtual float	GetHeight();

	// �����С
	virtual float	GetFontSize();
	
	// ��ȡ�ַ����
	inline float	GetWidthFromCharacter( wchar_t c, bool original = false );

	static wstring	StringAsWideStr( const string & value );
	static string	WideStrAsString( const wstring & str );
private:
	static const wchar_t * ToWideStr(const char * info, 
		wchar_t * buf, size_t size);
	static const char * ToString(const wchar_t * info, char * buf, size_t size);

	// �����ַ���ȡ����
	unsigned int	GetGlyphByCharacter( wchar_t c );
	inline void		CacheCharacter(unsigned int idx, wchar_t c);

	typedef struct tagEngineFontGlyph
	{
		HTEXTURE	t;
		float		w;
		float		h;
		float		x;
		float		y;
		float		c;
	}TENGINEFONTGLYPH;

	static const unsigned int font_count = 0xFFFF;// = sizeof(wchar_t);
	TENGINEFONTGLYPH	m_Glyphs[font_count];
	UINT				m_nAntialias;//�����
	LONG				m_nAscent;//����
	DWORD				m_dwFontColor;
	float				m_nFontSize;
	float				m_nKerningWidth;
	float				m_nKerningHeight;

	class HGE*			m_pHGE;
	class hgeSprite*	m_pSprite;

	// GDI�豸
	HDC					m_hMemDC;
	HFONT				m_hFont;
};
#endif//GDIFONT_H
