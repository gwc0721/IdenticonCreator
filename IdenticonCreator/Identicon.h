#ifndef _IDENTICON_H
#define _IDENTICON_H

#include <string>
#include <Windows.h>
#include <d2d1.h>
#include "Canvas.h"

// �ж� value �Ƿ��� vector ������
#ifndef inVector
#define inVector(value, vector) (std::find(vector.begin(), vector.end(), value) != vector.end())
#endif

// ����ͼ�λ�ͼ��������ָ�붨��
// index: ��ͼС�������   rotate: ��ʼ��ת��   color: ������ɫ   cell: С��߳�   canvas: ����ʾ��
#define IDRAWING_FUNC [](int index, int rotate, UINT32 color, float cell, Canvas* canvas)
typedef void(*IDRAWING_FUNCPTR)(int index, int rotate, UINT32 color, float cell, Canvas* canvas);

class Identicon {

public:
	// ���ġ���Ե����ͼ�λ�ͼ���������鼰�䳤��
	static const int CENTER_SHAPES_LENGTH;
	static const int OUTER_SHAPES_LENGTH;
	static const IDRAWING_FUNCPTR CENTER_SHAPES[];
	static const IDRAWING_FUNCPTR OUTER_SHAPES[];

	// Identicon ����ĳ�ʼ��
	static void init(HWND hWnd);

	// �������ݣ����� MD5 ������ identicon ͼ��
	static void create(LPSTR text);
	static void create(LPWSTR text);
	static void create(LPCSTR text);
	static void create(LPCWSTR text);
	static void create(std::string text);
	static void create(std::wstring text);

	// �����ѻ���� md5 �� identicon ͼ��
	static void draw();
	// ����ָ�� md5 ֵ�� identicon ͼ��
	static void draw(std::string md5);

	// �� string ���͵� 16 ������ֵת��Ϊ int
	// str: ��ת���ַ���   len: �ַ�������(���ܳ��� 7����������)
	static int hex2int(const char *str, int len = 1);

	// �� HSL ɫ�����ɫת��Ϊ unsigned int ��ʾ�� RGB ��ɫ (0x00rrggbb)
	static UINT hsl(double hue, double saturation, double lightness);
	static UINT correctedHsl(double h, double s, double l);

protected:
	static int cell; // һС��ı߳�
	static std::string curmd5; // ����� md5
	static Canvas *canvas; // Canvas ʵ��

	// hsl ���ڲ��м䷽��
	static inline double hue2rgb(double p, double q, double h);

};


// ��ʼ�� D2D ·��
#define IDRAWING_PATH_BEGIN \
	ID2D1PathGeometry *PathGeometry = nullptr;\
	ID2D1GeometrySink *pSink = NULL;\
	MainWindow::getD2DFactory()->CreatePathGeometry(&PathGeometry);\
	PathGeometry->Open(&pSink);

// �պ� D2D ·��
#define IDRAWING_PATH_CLOSE \
	pSink->Close();

// ����·������ʾ�ıպ�ͼ��
#define IDRAWING_PATH_DRAW \
	RenderTarget->FillGeometry(PathGeometry, brush);

// �ͷ� D2D ·��
#define IDRAWING_PATH_END \
	SafeRelease(&pSink);\
	SafeRelease(&PathGeometry);\

// ��ʼ��ͼ
#define IDRAWING_DRAW_BEGIN \
	ID2D1SolidColorBrush *brush;\
	ID2D1RenderTarget *RenderTarget = canvas->RenderTarget;\
	RenderTarget->CreateSolidColorBrush(\
		D2D1::ColorF(color),\
		&brush\
	);\
	RenderTarget->BeginDraw();\
	index = index % 4;\
	rotate = rotate % 4;\
	int x = index % 2;\
	int y = index > 1 ? 1 : 0;\
	D2D1_MATRIX_3X2_F translation = D2D1::Matrix3x2F::Translation(x * cell, y * cell);\
	D2D1_MATRIX_3X2_F cellrotate = x && y ? D2D1::Matrix3x2F::Identity() : D2D1::Matrix3x2F::Rotation(rotate * 90.0f, D2D1::Point2F(cell * .5f, cell * .5f));\
	int _totalr = x ^ y ? 8 : (x ? 1 : 4);\
	for(int _rotate = 0; _rotate < _totalr; ++_rotate) {\
		if(_rotate == 4) translation = D2D1::Matrix3x2F::Translation(y * cell, x * cell);\
		if(x && y) _rotate = rotate;\
		D2D1_MATRIX_3X2_F _rotation = D2D1::Matrix3x2F::Rotation(_rotate * 90.0f, D2D1::Point2F(cell * 2.f, cell * 2.f));\
		RenderTarget->SetTransform(cellrotate * translation * _rotation);

// �û��� brush �������С��
#define IDRAWING_DRAW_FILL \
	RenderTarget->FillRectangle(D2D1::RectF(\
		0.f,\
		0.f,\
		cell + 0.f,\
		cell + 0.f\
	), brush);

// ���û��� brush ����ɫΪ��ɫ
#define IDRAWING_DRAW_COLORRESET \
	brush->SetColor(D2D1::ColorF(D2D1::ColorF::White, 1.f));

// ��ͼ����
#define IDRAWING_DRAW_END \
	}\
	SafeRelease(&brush);\
	if (RenderTarget->EndDraw() == D2DERR_RECREATE_TARGET) {\
		SafeRelease(&RenderTarget);\
	}

#endif