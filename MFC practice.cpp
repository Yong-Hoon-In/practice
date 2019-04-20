
// ChildView.h: CChildView 클래스의 인터페이스
//


#pragma once


// CChildView 창

class CChildView : public CWnd
{
// 생성입니다.
public:
	CChildView();

// 특성입니다.
public:

// 작업입니다.
public:
	HENHMETAFILE m_hmf;
	
// 재정의입니다.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildView();

	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication7.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	CMetaFileDC dc;
	dc.CreateEnhanced(NULL, NULL, NULL, NULL);
	dc.Rectangle(0, 0, 10, 10);
	dc.Ellipse(1, 1, 9, 9);
	m_hmf = dc.CloseEnhanced();
}

CChildView::~CChildView()
{
	::DeleteEnhMetaFile(m_hmf);
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(3), nullptr);//초기값:reinterpret_cast<HBRUSH>(COLOR_WINDOW+1)



	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.


//	dc.Rectangle(0, 0, 200, 100);
//	dc.Ellipse(200, 100, 500, 200);
//	for (int i = 0; i < 255; i++) {
//		for (int j = 0; j < 255; j++) {
//			dc.SetPixel(i, j, RGB(i, j, 0));
//		}
//	}
	CPoint points[11];

	// 72' 씩 회전된 꼭지점으로 이루어진 점 생성
	// 여기서 알아서 모양을 만드세요.
	double Pi = 3.14159265359;

	for (int i = 0; i < 10; i += 2)
	{
		// 별의 바깥쪽 꼭지점 (큰 반지름으로 만듬)
		points[i].x = (long)(25 * cos((double)(i * 72 * Pi) / 360.0));
		points[i].y = (long)(25 * sin((double)(i * 72 * Pi) / 360.0));

		// 별의 안쪽 꼭지점 (작은 반지름으로 만듬)
		points[i + 1].x = (long)(10 * cos((double)((i + 1) * 72 * Pi) / 360.0));
		points[i + 1].y = (long)(10 * sin((double)((i + 1) * 72 * Pi) / 360.0));
	}

	// 끝점은 첫점과 같게 맞춤니다.
	points[10] = points[0];

	CRgn rgnStar;
	rgnStar.CreatePolygonRgn(points, 11, WINDING);

	// 클라이언트 영역을 가져옵니다.
	CRect r;
	GetClientRect(&r);

	// 그림 그릴 브러시를 초기화합니다.
	CBrush brush;
	brush.CreateSolidBrush(RGB(0, 0, 0));

	// 영역을 초기화 합니다.
	CRgn rgn;
	rgn.CreateRectRgn(0, 0, 0, 0);
	rgn.CopyRgn(&rgnStar);
	rgn.OffsetRgn(500,500);

	// 별을 그립니다.
	dc.FillRgn(&rgn, &brush);

	CPen pen;
	pen.CreatePen(PS_DASH,1, RGB(255, 30, 30));
	dc.SelectObject(pen);

	CRect rect1;
	GetClientRect(&rect1);
	dc.MoveTo(0, rect1.bottom / 2);
	dc.LineTo(rect1.right, rect1.bottom / 2);
	dc.MoveTo(rect1.right / 2, 0);
	dc.LineTo(rect1.right / 2, rect1.bottom);
	POINT points1[] = { {rect1.right / 2,0},{rect1.right,rect1.bottom / 2 },{rect1.right / 2,rect1.bottom},{0,rect1.bottom / 2},{rect1.right / 2,0} };
	dc.Polyline(points1, 5);

	CRect rec;
	GetClientRect(&rec);
	dc.SetTextColor(RGB(255, 2, 2));
	dc.SetBkColor(RGB(255, 255, 0));
	dc.DrawText(CString("Draw Text"), &rec, 0);
	dc.DrawText(CString("Draw Text"), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	dc.SetTextAlign(TA_CENTER);
	dc.SetTextColor(RGB(0, 0, 255));
	dc.SetBkColor(RGB(0, 255, 0));
	dc.TextOutW(rec.right / 2, 3 * rec.bottom / 4, CString("Textoutw"));

	
	dc.SetMapMode(MM_LOMETRIC);
	dc.Rectangle(0, 0, 1000, -500);
	dc.SetMapMode(MM_HIMETRIC);
	dc.Ellipse(0, 0, 10000, -5000);
	dc.SetMapMode(MM_TEXT);
	dc.SetBkMode(OPAQUE);
	
	dc.TextOutW(500, 50, CString("opaque"));
	dc.SetBkMode(TRANSPARENT);
	dc.TextOutW(500, 100, CString("transparant"));
	dc.SetBkMode(OPAQUE);
	dc.SetBkColor(RGB(100, 100, 255));
	dc.TextOutW(500, 150, CString("opaque"));
	CBrush(HS_FDIAGONAL,RGB(100,100,100));

	CBitmap bit;
	bit.LoadBitmapW(IDB_BITMAP1);
	CBrush brush3(&bit);
	dc.SelectObject(&brush3);
	dc.RoundRect(450, 50, 600, 200, 50, 50);

	CDC mem;
	mem.CreateCompatibleDC(&dc);
	mem.SelectObject(&bit);
	BITMAP bitinfo;
	bit.GetBitmap(&bitinfo);
	dc.BitBlt(10, 10, bitinfo.bmWidth, bitinfo.bmHeight, &mem, 0, 0, SRCCOPY);
	dc.StretchBlt(10, 100, bitinfo.bmWidth * 4, bitinfo.bmHeight * 2, &mem, 0, 0, bitinfo.bmWidth, bitinfo.bmHeight, SRCCOPY); 
	mem.Rectangle(5, 5, 15, 15);
	dc.BitBlt(350, 10, bitinfo.bmWidth, bitinfo.bmHeight, &mem, 0, 0, SRCCOPY);
	dc.StretchBlt(350, 100, bitinfo.bmWidth * 4, bitinfo.bmHeight * 2, &mem, 0, 0, bitinfo.bmWidth, bitinfo.bmHeight, SRCCOPY);


	CFont font;
	font.CreatePointFont(400, _T("Arial"));
	dc.SelectObject(&font);
	dc.TextOutW(100, 100, CString("hell"));
	
	dc.SelectStockObject(WHITE_PEN);
	dc.MoveTo(300, 400);
	dc.LineTo(400, 500);


}



void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//	CClientDC dc(this);
//	CRect rect(point.x - 20, point.y - 20, point.x + 20, point.y + 20);
//	dc.PlayMetaFile(m_hmf, &rect);
	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//	CClientDC dc(this);
//	CRect rect(point.x - 50, point.y - 50, point.x + 50, point.y + 50);
//	dc.PlayMetaFile(m_hmf, &rect);
	CWnd::OnRButtonDown(nFlags, point);
}



