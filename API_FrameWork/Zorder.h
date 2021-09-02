#pragma once
#include "singleton.h"
//Z레이어설정
#define ZFLOORMAP	0
#define ZCOLMAP		1
#define ZEFFECT1	2
#define ZCOL1		3
#define ZCOL2		4
#define ZSLOWFADE	5
#define ZUNIT		6
#define ZCOL3		7
#define ZCOL4		8
#define ZEFFECT2	9
#define ZABOVEMAP	10

#define ZUIFIRST	1
#define ZUISECOND	2
#define ZUITHIRD	3
#define ZUIFIRTH	4
#define ZUIFADE		5
#define ZUIMOUSE	6
#define ZUITOP		7
#define ZUITOP2		8

//최대레이어
#define ZMAXLAYER	10

//색상정의
#define MINT		RGB(88, 245, 206)

enum ZORDERKIND
{
	IMG_NOMAL, IMG_NOMAL_SOUR, IMG_FRAME, IMG_ALPHA, IMG_ALPHA_SOUR, IMG_ALPHA_FRAME,
	IMG_ROTATE, IMG_ROTATE_FRAME, IMG_ROTATE_ALPHA, IMG_ROTATE_ALPHAFRAME,
	IMG_STRETCH, IMG_STRETCH_FRAME, IMG_ROTATESTRETCH, IMG_ROTATESTRETCHFRAME,
	IMG_ANI, IMG_ANIALPHA, OBJ_RECT, OBJ_COLORRECT, OBJ_RECTROTATE,
	TXT_TEXTOUT, TXT_DRAWTEXT
};

enum STRETCHRENDERKIND
{
	STRETCH_WHOLE_RATIO, STRETCH_EACH_SCALE
};

struct tagZorder
{
	ZORDERKIND kind;	
	STRETCHRENDERKIND stretchKind;
	image* img;
	float destX, destY;
	float z;
	float bottom;
	int frameX, frameY;
	int sourX, sourY;
	int sourWidth, sourHeight;
	float angle;
	float ratio;
	float newWidth;
	float newHeight;
	BYTE alpha;
	animation* ani;
	string txt;				// 텍스트
	HFONT font;				//글자폰트
	RECT txtRC;				//글자를 출력할 위치
	UINT format;			//drawText 포맷
	COLORREF txtColor;		//글자색
	COLORREF rectColor;		//글자색
	RECT rc;

	tagZorder(ZORDERKIND _kind, image* _img, int _z, int _x, int _y)
	{
		kind = _kind;
		img = _img;
		z = _z;
		destX = _x;
		destY = _y;
		frameX = frameY = sourX = sourY = sourWidth = sourHeight = alpha = angle = ratio = alpha = newWidth = newHeight = bottom = format = NULL;
		ani = nullptr;
		stretchKind;
	}
};

class Zorder : public Singleton<Zorder>
{
public:
	//메멘토패턴
	class Memento
	{
	private:
		friend class Zorder;
		friend class Caretaker;

		vector<tagZorder> _sVector;
		vector<tagZorder> _sBwVector;

		Memento(vector<tagZorder> moment, vector<tagZorder> bwmoment) 
			: _sVector(moment), _sBwVector(bwmoment)
		{}

		vector<tagZorder> getVnormal() { return _sVector; }
		vector<tagZorder> getVbw() { return _sBwVector; }
	};
private :
	vector<tagZorder> _vZorder;
	vector<tagZorder> _vUiZorder;
	vector<tagZorder> _vMoment;
	vector<tagZorder> _vBWMoment;
public:
	Zorder() {}
	~Zorder() {}

	void release();

	//랜더종류
	void ZorderRender(image* img, float z, float bottom, int destX, int destY);
	void ZorderRender(image* img, float z, float bottom, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);
	void ZorderFrameRender(image* img, float z, float bottom, int destX, int destY, int frameX = 0, int frameY = 0);
	void ZorderAlphaRender(image* img, float z, float bottom, int destX, int destY, BYTE alpha);
	void ZorderAlphaRender(image* img, float z, float bottom, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha);
	void ZorderAlphaFrameRender(image* img, float z, float bottom, int destX, int destY, int frameX, int frameY, BYTE alpha);
	void ZorderRotateRender(image* img, float z, float bottom, int centerX, int centerY, float angle);
	void ZorderRotateFrameRender(image* img, float z, float bottom, int centerX, int centerY, float angle, int frameX = 0, int frameY = 0);
	void ZorderRotateAlphaRender(image* img, float z, float bottom, int centerX, int centerY, float angle, BYTE alpha);
	void ZorderRotateAlphaFrameRender(image* img, float z, float bottom, int centerX, int centerY, float angle, int frameX, int frameY, BYTE alpha);
	void ZorderStretchRender(image* img, float z, float bottom, int centerX, int centerY, float ratio);
	void ZorderStretchRender(image* img, float z, float bottom, int centerX, int centerY, float newWidth, float newHeight);
	void ZorderStretchFrameRender(image* img, float z, float bottom, int centerX, int centerY, int frameX, int frameY, float ratio);
	void ZorderStretchFrameRender(image* img, float z, float bottom, int centerX, int centerY, int frameX, int frameY, float newWidth, float newHeight);
	void ZorderRotateStretchRender(image* img, float z, float bottom, int centerX, int centerY, float angle, float ratio);
	void ZorderRotateStretchFrameRender(image* img, float z, float bottom, int centerX, int centerY, int frameX, int frameY, float angle, float ratio);
	void ZorderAniRender(image* img, float z, float bottom, int destX, int destY, animation* ani);
	void ZorderAniAlphaRender(image* img, float z, float bottom, int destX, int destY, animation* ani, BYTE alpha);

	//도형,글씨
	void ZorderRectangle(RECT rc, float z);
	void ZorderRectangleColor(RECT rc, float z, COLORREF color);
	void ZorderRectangleRotate(RECT rc, float z, float angle, COLORREF color);
	void ZorderTextOut(string txt, float z, int destX, int destY, COLORREF color);
	void ZorderDrawText(string txt, float z, RECT txtRC, HFONT font, COLORREF color, UINT format);


	//Ui랜더종류
	void UIRender(image* img, float z, float bottom, int destX, int destY);
	void UIRender(image* img, float z, float bottom, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);
	void UIFrameRender(image* img, float z, float bottom, int destX, int destY, int frameX = 0, int frameY = 0);
	void UIAlphaRender(image* img, float z, float bottom, int destX, int destY, BYTE alpha);
	void UIAlphaRender(image* img, float z, float bottom, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha);
	void UIAlphaFrameRender(image* img, float z, float bottom, int destX, int destY, int frameX, int frameY, BYTE alpha);
	void UIRotateRender(image* img, float z, float bottom, int centerX, int centerY, float angle);
	void UIRotateFrameRender(image* img, float z, float bottom, int centerX, int centerY, float angle, int frameX = 0, int frameY = 0);
	void UIRotateAlphaRender(image* img, float z, float bottom, int centerX, int centerY, float angle, BYTE alpha);
	void UIRotateAlphaFrameRender(image* img, float z, float bottom, int centerX, int centerY, float angle, int frameX, int frameY, BYTE alpha);
	void UIStretchRender(image* img, float z, float bottom, int centerX, int centerY, float ratio);
	void UIStretchRender(image* img, float z, float bottom, int centerX, int centerY, float newWidth, float newHeight);
	void UIStretchFrameRender(image* img, float z, float bottom, int centerX, int centerY, int frameX, int frameY, float ratio);
	void UIStretchFrameRender(image* img, float z, float bottom, int centerX, int centerY, int frameX, int frameY, float newWidth, float newHeight);
	void UIRotateStretchRender(image* img, float z, float bottom, int centerX, int centerY, float angle, float ratio);
	void UIRotateStretchFrameRender(image* img, float z, float bottom, int centerX, int centerY, int frameX, int frameY, float angle, float ratio);
	void UIAniRender(image* img, float z, float bottom, int destX, int destY, animation* ani);
	void UIAniAlphaRender(image* img, float z, float bottom, int destX, int destY, animation* ani, BYTE alpha);
	void UIRectangle(RECT rc, float z);
	void UIRectangleColor(RECT rc, float z, COLORREF color);
	void UIRectangleRotate(RECT rc, float z, float angle, COLORREF color);
	void UITextOut(string txt, float z, int destX, int destY, COLORREF color);
	void UIDrawText(string txt, float z, RECT txtRC, HFONT font, COLORREF color, UINT format);

	//세이브종류
	void SaveRender(image* img, image* bw, float z, float bottom, int destX, int destY);
	void SaveRender(image* img, image* bw, float z, float bottom, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);
	void SaveFrameRender(image* img, image* bw, float z, float bottom, int destX, int destY, int frameX = 0, int frameY = 0);
	void SaveAlphaRender(image* img, image* bw, float z, float bottom, int destX, int destY, BYTE alpha);
	void SaveAlphaRender(image* img, image* bw, float z, float bottom, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha);
	void SaveAlphaFrameRender(image* img, image* bw, float z, float bottom, int destX, int destY, int frameX, int frameY, BYTE alpha);
	void SaveRotateRender(image* img, image* bw, float z, float bottom, int centerX, int centerY, float angle);
	void SaveRotateFrameRender(image* img, image* bw, float z, float bottom, int centerX, int centerY, float angle, int frameX = 0, int frameY = 0);
	void SaveRotateAlphaRender(image* img, image* bw, float z, float bottom, int centerX, int centerY, float angle, BYTE alpha);
	void SaveRotateAlphaFrameRender(image* img, image* bw, float z, float bottom, int centerX, int centerY, float angle, int frameX, int frameY, BYTE alpha);
	void SaveStretchRender(image* img, image* bw, float z, float bottom, int centerX, int centerY, float ratio);
	void SaveStretchRender(image* img, image* bw, float z, float bottom, int centerX, int centerY, float newWidth, float newHeight);
	void SaveStretchFrameRender(image* img, image* bw, float z, float bottom, int centerX, int centerY, int frameX, int frameY, float ratio);
	void SaveStretchFrameRender(image* img, image* bw, float z, float bottom, int centerX, int centerY, int frameX, int frameY, float newWidth, float newHeight);
	void SaveRotateStretchRender(image* img, image* bw, float z, float bottom, int centerX, int centerY, float angle, float ratio);
	void SaveRotateStretchFrameRender(image* img, image* bw, float z, float bottom, int centerX, int centerY, int frameX, int frameY, float angle, float ratio);
	void SaveAniRender(image* img, image* bw, float z, float bottom, int destX, int destY, animation* ani);
	void SaveAniAlphaRender(image* img, image* bw, float z, float bottom, int destX, int destY, animation* ani, BYTE alpha);

	//세이브종류
	void SaveRectangle(RECT rc, float z);
	void SaveRectangleColor(RECT rc, float z, COLORREF color);
	void SaveRectangleRotate(RECT rc, float z, float angle, COLORREF color);
	void SaveTextOut(string txt, float z, int destX, int destY, COLORREF color);
	void SaveDrawText(string txt, float z, RECT txtRC, HFONT font, COLORREF color, UINT format);


	//정렬
	void Sort(int i, int j);	//Z만 정렬시키는 연습용.
	inline void ZorderSort();	//Z부터 X, Y 순서로 정렬하는 실전용.
	inline void UiZorderSort();	//Z부터 X, Y 순서로 정렬하는 실전용.
	static bool ZordorCompare(const tagZorder& z1, const tagZorder& z2);	//비교기준.

	//출력
	void ZorderTotalRender(HDC hdc);
	void ZorderUITotalRender(HDC hdc);

public:
	//메멘토패턴 저장,복구
	Memento save()const { return Memento(_vMoment, _vBWMoment); }
	void restore(const Memento& m, bool isBW){ (isBW) ? _vZorder = m._sBwVector : _vZorder = m._sVector; }
	void ZorderSaveClear();
};

