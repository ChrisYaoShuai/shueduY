
// MyCGView.cpp: CMyCGView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MyCG.h"
#endif

#include "MyCGDoc.h"
#include "MyCGView.h"
#include "math.h"
#include "CP2.h"
#define Round(f) int(floor(f+0.5))
#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyCGView

IMPLEMENT_DYNCREATE(CMyCGView, CView)

BEGIN_MESSAGE_MAP(CMyCGView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_DDA32772, &CMyCGView::OnDrawLineByDDA)
	ON_COMMAND(ID_32774, &CMyCGView::OnDrawLineByMidpoint)
	ON_COMMAND(ID_BRESENHAM32776, &CMyCGView::OnDrawLineByBresenham)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_32783, &CMyCGView::Onsaomiaocreate)
	ON_COMMAND(ID_32786, &CMyCGView::Onbianyuancreate)
	ON_COMMAND(ID_32789, &CMyCGView::Onliantongcreate)
//	ON_WM_ERASEBKGND()
//ON_WM_ERASEBKGND()
//ON_WM_ERASEBKGND()
ON_COMMAND(ID_32792, &CMyCGView::OnClear)
ON_COMMAND(ID_32793, &CMyCGView::OnFillsaomiao)
ON_COMMAND(ID_32794, &CMyCGView::Onfillbianyuan)
ON_COMMAND(ID_32791, &CMyCGView::OnseedFill)
ON_COMMAND(ID_32797, &CMyCGView::Oncaijianline)
ON_COMMAND(ID_32798, &CMyCGView::OncreateCSre)

ON_COMMAND(ID_32800, &CMyCGView::OnSHcreateLine)
ON_COMMAND(ID_32801, &CMyCGView::OnSHcreateRE)
ON_COMMAND(ID_32810, &CMyCGView::OnMoveCreate)
ON_COMMAND(ID_32807, &CMyCGView::OnPingYi)
//ON_WM_KEYDOWN()
ON_COMMAND(ID_32809, &CMyCGView::OnMoveRote)
ON_COMMAND(ID_32808, &CMyCGView::OnMoveScl)
ON_COMMAND(ID_32814, &CMyCGView::OnTwoCreatePolygon)
ON_COMMAND(ID_32816, &CMyCGView::OnThreeCreatePolygon)
ON_COMMAND(ID_32818, &CMyCGView::OnFourCreatePolygon)
END_MESSAGE_MAP()

// CMyCGView 构造/析构

CMyCGView::CMyCGView() noexcept
{
	// TODO: 在此处添加构造代码
	flag = -1;
	start = -1;
	bLdown = false;
	is_close = false;
	nCount = 0;
	P = new CPoint[100];
	int MaxX = 0;
	int MaxY = 0;
	CPoint seed;
	CPoint CSpoint1, CSpoint2;    //存储CS直线的点
	CPoint CSre1, CSre2;          //存储矩形框的左上，右下顶点
	CPoint pt[100], pts[100], ptse[100], ph[100];
	comp = false;
	n = 0;
	H = new CP2[100];
	down = false;
}

CMyCGView::~CMyCGView()
{
}

BOOL CMyCGView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMyCGView 绘图

void CMyCGView::OnDraw(CDC* /*pDC*/)
{
	CMyCGDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 在此处为本机数据添加绘制代码
}


// CMyCGView 打印

BOOL CMyCGView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMyCGView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMyCGView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CMyCGView 诊断

#ifdef _DEBUG
void CMyCGView::AssertValid() const
{
	CView::AssertValid();
}

void CMyCGView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyCGDoc* CMyCGView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyCGDoc)));
	return (CMyCGDoc*)m_pDocument;
}
#endif //_DEBUG


// CMyCGView 消息处理程序



void CMyCGView::DrawLineByDda(CDC* pDC, int x0, int y0, int x1, int y1, COLORREF color)
{
	int i;
	float length, x, y, dx, dy;
	length = abs(x1 - x0);
	if (abs(y1 - y0) > length)
	{
		length = abs(y1 - y0);
	}
	dx = (float)((x1 - x0) / length);
	dy = (float)((y1 - y0) / length);
	x = x0;
	y = y0;
	for (i = 1; i <= length; i++)
	{
		pDC->SetPixel((int)x, (int)y, color);
		x = x + dx;
		y = y + dy;
	}
}

void CMyCGView::DrawLineByBh(CDC* pDC, int x0, int y0, int x1, int y1, COLORREF color)
{
	int temp;
	if (x1 < x0)
	{
		temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 = temp;
	}
	int x, y, dx, dy, e, i;
	x = x0;
	y = y0;
	dx = x1 - x0;
	dy = y1 - y0;
	if (abs(dx) >= abs(dy))
	{
		e = -dx;
		for (i = 0; i <= dx; i++)
		{
			if (y0 > y1)
			{
				dx = x1 - x0;
				dy = y0 - y1;
				pDC->SetPixel(x, y, color);
				x++;
				e = e + 2 * dy;
				if (e >= 0)
				{
					y--;
					e = e - 2 * dx;
				}
			}
			else
			{
				pDC->SetPixel(x, y, color);
				x++;
				e = e + 2 * dy;
				if (e >= 0)
				{
					y++;
					e = e - 2 * dx;
				}
			}
		}
	}
	else
	{
		if (y0 > y1)
		{
			dx = x1 - x0;
			dy = y0 - y1;
			pDC->SetPixel(x, y, color);
			e = -dy;
			for (i = 0; i <= dy; i++)
			{
				pDC->SetPixel(x, y, color);
				y--;
				e = e + 2 * dx;
				if (e >= 0)
				{
					x++;
					e = e - 2 * dy;
				}
			}
		}
		else
		{
			e = -dy;
			for (i = 0; i <= dy; i++)
			{
				pDC->SetPixel(x, y, color);
				y++;
				e = e + 2 * dx;
				if (e >= 0)
				{
					x++;
					e = e - 2 * dy;
				}
			}
		}
	}
}

void CMyCGView::DrawLineByMidpoint(CDC* pDC, int x0, int y0, int x1, int y1, COLORREF color)
{
	int a, b, d, d1, d2, x, y;

	x = x0;
	y = y0;
	a = y0 - y1;
	b = x1 - x0;

	int cx = (b >= 0 ? 1 : (b = -b, -1));
	int cy = (a <= 0 ? 1 : (a = -a, -1));

	if (-a <= b)
	{
		d = 2 * a + b;
		d1 = 2 * a;
		d2 = 2 * (a + b);
		while (x != x1)
		{
			if (d < 0)
			{
				y = y + cy;
				d = d + d2;
			}
			else
			{
				d = d + d1;
			}
			x = x + cx;
			pDC->SetPixel(x, y, color);
		}
	}
	else
	{
		d = a + 2 * b;
		d1 = 2 * b;
		d2 = 2 * (a + b);
		while (y != y1)
		{
			if (d < 0)
			{
				d = d + d1;
			}
			else
			{
				x = x + cx;
				d = d + d2;
			}
			y = y + cy;
			pDC->SetPixel(x, y, color);
		}
	}
}

void CMyCGView::OnDrawLineByDDA()
{
	// TODO: 在此添加命令处理程序代码
	flag = 3;
	start = 0;
}

void CMyCGView::OnDrawLineByMidpoint()
{
	// TODO: 在此添加命令处理程序代码
	flag = 4;
	start = 0;
}

void CMyCGView::OnDrawLineByBresenham()
{
	// TODO: 在此添加命令处理程序代码
	flag = 5;
	start = 0;
}
//鼠标移动
void CMyCGView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC* pDC = GetDC();
	
	if (3 <= flag && flag <= 5 )
	{
		int draw_mode = pDC->SetROP2(R2_XORPEN);
		pDC->SelectStockObject(NULL_BRUSH);
		if (start == 1)
		{
			CPoint pre_point, cur_point;
			cur_point = point;

			if (flag == 3)
			{
				DrawLineByDda(pDC, p0.x, p0.y, cur_point.x, cur_point.y, RGB(255, 255, 255));
				RedrawWindow();
				DrawLineByDda(pDC, p0.x, p0.y, cur_point.x, cur_point.y, RGB(255, 255, 255));
				
				
			}
			else if (flag == 4)
			{
				DrawLineByMidpoint(pDC, p0.x, p0.y, cur_point.x, cur_point.y, RGB(255, 255, 255));
				RedrawWindow();
				DrawLineByMidpoint(pDC, p0.x, p0.y, cur_point.x, cur_point.y, RGB(255, 255, 255));
				
			}
			else if (flag == 5)
			{
				DrawLineByBh(pDC, p0.x, p0.y, cur_point.x, cur_point.y, RGB(255, 255, 255));
				RedrawWindow();
				DrawLineByMidpoint(pDC, p0.x, p0.y, cur_point.x, cur_point.y, RGB(255, 255, 255));
				
			}
		}
	}
	else if (6 <= flag && flag <= 8  || flag==12 ||flag==14)
	{
		if (!is_close)
		RedrawWindow();
		if (bLdown) {
			if (abs(point.x - P[0].x) <= 5 && abs(point.y - P[0].y) <= 5) {
				SetCursor(LoadCursor(NULL, IDC_HAND));   //进入引力域  变为手指
			}
		}
		if (!is_close)
		{
			P[nCount] = point;
		}//没闭合就画线
		DrawObject(pDC);
		Invalidate(FALSE);
	}
	else if (18 <= flag && flag <= 20)
	{
		if (nCount > n )
		{
			DrawBezier(pDC);
			for (int i = 0;i < nCount;i++)
				if (abs(point.x - H[i].x) <= 8 && abs(point.y - H[i].y) <= 8)
				{
					SetCursor(LoadCursor(NULL, IDC_HAND));   //进入引力域  变为手指
					p = i;
					if (down)
					{
						H[p].x = point.x;
						H[p].y = point.y;
						RedrawWindow();
						DrawBezier(pDC);
					}
				}
		}                              //改变选点
		if (!comp)
		{
			RedrawWindow();
		}
		if (!comp)
		{
			H[nCount].x = point.x;
			H[nCount].y = point.y;
		}
		DrawControlPolygon(pDC);
		Invalidate(FALSE);
		ReleaseDC(pDC);
	}
	CView::OnMouseMove(nFlags, point);
}
//鼠标按下
void CMyCGView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	down = TRUE;
	if (3 <= flag && flag <= 5  || flag==10 || flag==11|| flag==13)   //直线创建   当flag=11和13时 创建裁剪矩形框    
	{
		CDC* pDC = GetDC();
		pDC->SelectStockObject(NULL_BRUSH);
		if (start == 0)  //是起点
		{
			p0 = p1 = point; //记录第一次单击鼠标位置，定起点
			start++;
			if (flag == 11||flag==13)
			{
				CSre1 = point;
			}
			else if (flag == 10)
			{
				CSpoint1 = point;
			}
		}
		ReleaseDC(pDC);
	}
	if (6 <= flag && flag <= 8 ||flag==12  ||flag ==14)    
	{
		CDC* pDC = GetDC();
		if (bLdown && abs(point.x - P[0].x) <= 5 && abs(point.y - P[0].y) <= 5) 
		{
			P[nCount] = P[0];     // 存所有点的，ncount是当前点的下标
			is_close = true;      // 封闭了,值改为true
			RedrawWindow();
		}
		else if(!is_close) P[nCount++] = point;//如果没在那个范围就正常存点
		bLdown = TRUE;          //，用来判断是否有起点了
		ReleaseDC(pDC);
		
	}
	if (flag == 9)
	{
		seed = point;
		BoundaryFill4(seed);
		flag = -1;
		
	}
	if (18 <= flag && flag <= 20)
	{
		CDC* pDC = GetDC();
		if (nCount>n)
		{
			comp = true;
			RedrawWindow();
		}
		else if (!comp)
		{
			H[nCount].x = point.x;
			H[nCount].y = point.y;
			nCount++;
		}
		ReleaseDC(pDC);
	}
	CView::OnLButtonDown(nFlags, point);
}
//鼠标弹起
void CMyCGView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	down = false;
	CDC* pDC = GetDC();
	pDC->SelectStockObject(NULL_BRUSH);
	if (start == 1)
	{
		p1 = point;  //记录第二次单击鼠标的位置，定终点
		start--;   // 为新绘图作准备
		if (flag == 3)
		{
			DrawLineByDda(pDC, p0.x, p0.y, p1.x, p1.y, RGB(255, 0, 0));
		}
		else if (flag == 4)
		{
			DrawLineByMidpoint(pDC, p0.x, p0.y, p1.x, p1.y, RGB(137, 100, 0));
		}
		else if (flag == 5)
		{
			DrawLineByBh(pDC, p0.x, p0.y, p1.x, p1.y, RGB(29, 0, 255));
		}
		else if (flag == 10)
		{
			
			pDC->MoveTo(p0.x, p0.y);
			pDC->LineTo(p1.x, p1.y);

			CSpoint2 = point;
		}
		else if (flag == 11)
		{
			
			RedrawWindow();
			pDC->Rectangle(p0.x,p0.y,p1.x,p1.y);
			CSre2 = point;
			CS_LineClip();
			
		}
		else if (flag == 13)
		{
			RedrawWindow();
			pDC->Rectangle(p0.x, p0.y, p1.x, p1.y);
			CSre2 = point;
			clipSH();
			
		}
	}
	ReleaseDC(pDC);
	CView::OnLButtonUp(nFlags, point);
}

void CMyCGView::Onsaomiaocreate()
{
	// TODO: 在此添加命令处理程序代码
	flag = 6;
	start = 0;
	

}

void CMyCGView::Onbianyuancreate()
{
	// TODO: 在此添加命令处理程序代码
	flag = 7;
	start = 0;
	
	
}

void CMyCGView::Onliantongcreate()
{
	// TODO: 在此添加命令处理程序代码
	flag = 8;
	start = 0;
	

}

void CMyCGView::DoubleBuffer(CDC* pDC)
{
	CRect rect;//定义客户区
	GetClientRect(&rect);//获得客户区的大小
	CDC MemDC;//内存设备上下文
	CBitmap NewBitmap, * pOldBitmap;//内存中承载图像的临时位图
	MemDC.CreateCompatibleDC(pDC);//建立与屏幕pDC兼容的MemDC 
	NewBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());//创建兼容位图 
	pOldBitmap = MemDC.SelectObject(&NewBitmap); //将兼容位图选入MemDC 
	MemDC.FillSolidRect(rect, pDC->GetBkColor());//按原来背景填充客户区，否则是黑色 
	DrawControlPolygon(pDC);//绘图函数
	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);//将内存位图拷贝到屏幕
	MemDC.SelectObject(pOldBitmap);//恢复位图
	NewBitmap.DeleteObject();//删除位图
	MemDC.DeleteDC();//删除MemD
}

void CMyCGView::DrawObject(CDC* pDC)
{
     pDC->SelectStockObject(GRAY_BRUSH);
	//P[]是一个CPoint数组
	for (int i = 0;i <= nCount;i++) {
		if (!i)
		{    //P[0]是起点所以是MoveTo先移动到第一个点
			pDC->MoveTo(P[i]);
		}
		else pDC->LineTo(P[i]);     // 其余点画
	}

	if (bLdown) {//如果bLBDown为真就说明第一个点确定了
		pDC->Rectangle(P[0].x - 5, P[0].y - 5, P[0].x + 5, P[0].y + 5);
		//在第一个点绘制一个灰色的矩形，这个矩形就是引力域
	}
	
	ReleaseDC(pDC);
}
//创建多边形

void CMyCGView::OnClear()
{
	// TODO: 在此添加命令处理程序代码
	RedrawWindow();
	flag = -1;
	start = -1;
	bLdown = false;
	is_close = false;
	nCount = 0;
	delete[] P;
	P = new CPoint[100];
	n = 0;
	comp = false;
	delete[] H;
	H = new CP2[100];
	down = false;
}
//清除

void CMyCGView::OnFillsaomiao()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	Setpoint();
	ScanFill(pDC,P[0],ThePolygon.m_LineColor);
	ReleaseDC(pDC);

}
//扫描线填充


//扫描线算法函数
void CMyCGView::Setpoint()
{
	ThePolygon.m_VerticeNumber=nCount;
	for (int i = 0;i < nCount;i++)
	{
		ThePolygon.m_Vertex[i] = P[i];
	}
	ThePolygon.m_LineColor = RGB(255, 0, 0);
}
int CMyCGView::getMiny(MyPolygon ThePolygon)
{
	int min = ThePolygon.m_Vertex[0].y;
	for (int i = 1;i < ThePolygon.m_VerticeNumber;i++)
		if (min > ThePolygon.m_Vertex[i].y)
			min = ThePolygon.m_Vertex[i].y;
	return min;
}
int CMyCGView::getMaxy(MyPolygon ThePolygon)
{
	int max = ThePolygon.m_Vertex[0].y;
	for (int i = 1;i < ThePolygon.m_VerticeNumber;i++)
		if (max < ThePolygon.m_Vertex[i].y)
			max = ThePolygon.m_Vertex[i].y;
	return max;
}
void CMyCGView::Swap(int* a, int i, int j) {
	int temp = a[i];
	a[i] = a[j];
	a[j] = temp;
}
int CMyCGView::qsort(int* a, int begin, int end) {
	int i, j, temp;
	i = begin - 1;j = begin;
	for (;j < end;j++)
	{
		if (a[j] <= a[end - 1])
			Swap(a, ++i, j);
	}
	return i;
}
void CMyCGView::randqsort(int* a, int begin, int n) {
	while (begin >= n)
		return;
	srand((unsigned)time(NULL));
	int key = (begin + rand() % (n - begin));
	Swap(a, key, n - 1);
	int m = qsort(a, begin, n);
	randqsort(a, begin, m);
	randqsort(a, m + 1, n);
}
void CMyCGView::ScanFill(CDC* pDC, CPoint startPoint, COLORREF fillCol)
{
	int miny, maxy;
	int sx, sy, tx, ty;
	miny = getMiny(ThePolygon);
	maxy = getMaxy(ThePolygon);
	int* index = new int[100];
	int* judge = new int[ThePolygon.m_VerticeNumber];
	memset(index, -1, sizeof(int) * 100);
	memset(judge, 0, sizeof(int) * ThePolygon.m_VerticeNumber);
	int x;
	CPoint Point;
	for (int i = miny;i <= maxy;i++)
	{
		//记录扫描线与边线的交点
		int temp = 0;
		for (int i1 = 0, l = ThePolygon.m_VerticeNumber, j1 = l - 1; i1 < l; j1 = i1, i1++) {

			sx = ThePolygon.m_Vertex[i1].x;
			sy = ThePolygon.m_Vertex[i1].y;
			tx = ThePolygon.m_Vertex[j1].x;
			ty = ThePolygon.m_Vertex[j1].y;
			int lowy, heighty;
			lowy = (sy < ty) ? sy : ty;
			heighty = (sy > ty) ? sy : ty;
			//水平线
			if (ty == sy)
			{
				if (i == ty)
				{
					int xmax, xmin;
					xmax = (sx > tx) ? sx : tx;
					xmin = (sx < tx) ? sx : tx;
					for (int xx = xmin;xx <= xmax;xx++)
					{
						Point.x = xx;Point.y = i;
						pDC->SetPixelV(Point, fillCol);
					}
				}
				continue;
			}
			//没有交点
			if (i<lowy || i>heighty)
				continue;
			x = sx + (i - sy) * (tx - sx) / (ty - sy);
			//判断交点(x,i)是不是顶点
			if ((x == ThePolygon.m_Vertex[i1].x && i == ThePolygon.m_Vertex[i1].y))
			{
				//判断顶点是不是极值点  
				//即判断与交点相关联的两条线的另外两个顶点是不是在交点的同一侧
				if (i1 != l - 1 && judge[i1] == 0) {
					if ((i - ThePolygon.m_Vertex[i1 + 1].y) * (i - ThePolygon.m_Vertex[j1].y) < 0)//异号
						index[temp++] = x;
					else//同号  极值点 记录两次
					{
						index[temp++] = x;
						index[temp++] = x;
					}
				}
				else if (i1 == l - 1 && judge[i1] == 0)
				{
					if ((i - ThePolygon.m_Vertex[0].y) * (i - ThePolygon.m_Vertex[j1].y) < 0) {//异号
						index[temp++] = x;

					}
					else//同号  极值点  记录两次
					{
						index[temp++] = x;
						index[temp++] = x;
					}
				}
				judge[i1] = 1;
				continue;
			}
			else if ((x == ThePolygon.m_Vertex[j1].x && i == ThePolygon.m_Vertex[j1].y))
			{
				if (j1 != 0 && judge[j1] == 0) {
					if ((i - ThePolygon.m_Vertex[i1].y) * (i - ThePolygon.m_Vertex[j1 - 1].y) < 0) {//异号
						index[temp++] = x;

					}
					else//同号  极值点
					{
						index[temp++] = x;
						index[temp++] = x;
					}

				}
				else if (j1 == 0 && judge[j1] == 0)
				{
					if ((i - ThePolygon.m_Vertex[i1].y) * (i - ThePolygon.m_Vertex[l - 1].y) < 0)//异号
						index[temp++] = x;
					else//同号  极值点
					{
						index[temp++] = x;
						index[temp++] = x;
					}
				}
				judge[j1] = 1;
				continue;
			}
			//交点不是顶点
			index[temp++] = x;
		}

		//将index排序
		randqsort(index, 0, temp);//随机快速

		//填充多边形
		for (int n = 0, m = n + 1;m < temp && index[n] != -1;n += 2, m = n + 1)
		{
			for (int xx = index[n];xx <= index[m];xx++)
			{
				Point.x = xx;Point.y = i;
				pDC->SetPixelV(Point, fillCol);
			}
		}
		//清零
		for (int k = 0;k < 100;k++)
			index[k] = -1;
	}
	delete[] index;
	delete[] judge;
}
//

//边缘填充算法
void CMyCGView::Onfillbianyuan()     //边缘填充
{
	// TODO: 在此添加命令处理程序代码
	GetMaxX();
	GetMaxY();
	bianyuanDraw();
}
void CMyCGView::GetMaxX()
{
	CRect rect;
	GetClientRect(rect);
	MaxX = rect.right;
}

void CMyCGView::GetMaxY()
{
	CRect rect;
	GetClientRect(rect);
	MaxY = rect.bottom;
}

void CMyCGView::bianyuanDraw()
{
	COLORREF CBackColor = RGB(255, 255, 255);//白色
	Setpoint();
	ThePolygon.m_LineColor = RGB(0, 255, 255);
	CClientDC dc(this);
	int m, n, ymin, ymax;
	double x, y, k;
	for (int i = 0;i < nCount;i++)//对于多边形所有边循环
	{
		m = i, n = i + 1;//对点的循环
		if (nCount == n) n = 0;
		k = (double(ThePolygon.m_Vertex[m].x - ThePolygon.m_Vertex[n].x)) / (ThePolygon.m_Vertex[m].y - ThePolygon.m_Vertex[n].y);//计算1/k	
		if (ThePolygon.m_Vertex[m].y < ThePolygon.m_Vertex[n].y)//得到每条边的y最大和y最小值
		{
			ymin = ThePolygon.m_Vertex[m].y;
			ymax = ThePolygon.m_Vertex[n].y;
			x = ThePolygon.m_Vertex[m].x;
		}
		else
		{
			ymin = ThePolygon.m_Vertex[n].y;
			ymax = ThePolygon.m_Vertex[m].y;
			x = ThePolygon.m_Vertex[n].x;
		}
		for (y = ymin;y < ymax;y++)//对每一条边循环
		{
			for (int j = Round(x);j < MaxX;j++)//对每一条扫描线与边的交点的右侧像素循环
			{
				if (dc.GetPixel(j, Round(y)) == ThePolygon.m_LineColor)//如果像素的颜色是填充色
				{
					dc.SetPixel(j, Round(y), CBackColor);//改为背景色
				}
				else
				{
					dc.SetPixel(j, Round(y), ThePolygon.m_LineColor);//使用填充色填充
				}
			}
			x += k;//计算下一个x起点值
		}
	}
}
//
void CMyCGView::OnseedFill()
{
	flag = 9;
	// TODO: 在此添加命令处理程序代码
}

//种子填充算法
void CMyCGView::BoundaryFill4(CPoint seed)
{      //多边形顶点存在stack H中
	CClientDC* pdc;
	pdc = new CClientDC(this);
	CPoint head;
	head = seed;
	stack<CPoint> s;
	s.push(head);
	int boundaryColor = RGB(0, 0, 0);
	int fillColor = RGB(0, 0, 255);
	while (!s.empty())
	{
		CPoint base = s.top();//访问栈顶
		s.pop();//栈顶元素出栈
	
		pdc->SetPixel(base, fillColor);

		PointTop.x = base.x;	//(x, y+1)
		PointTop.y = base.y - 1;	//(x, y-1)

		if (pdc->GetPixel(PointTop) != boundaryColor && pdc->GetPixel(PointTop) != fillColor)
		{
			
			s.push(PointTop);
		}
		PointLeft.x = base.x - 1;//(x-1, y)
		PointLeft.y = base.y;//(x-1, y)
		if (pdc->GetPixel(PointLeft) != boundaryColor && pdc->GetPixel(PointLeft) != fillColor)
		{
			s.push(PointLeft);
		}
		PointBottom.x = base.x;//(x, y-1)
		PointBottom.y = base.y + 1;//(x, y+1)
		if (pdc->GetPixel(PointBottom) != boundaryColor && pdc->GetPixel(PointBottom) != fillColor)
		{
			s.push(PointBottom);
		}
		PointRight.x = base.x + 1;//(x+1, y)
		PointRight.y = base.y;//(x+1, y)
		if (pdc->GetPixel(PointRight) != boundaryColor && pdc->GetPixel(PointRight) != fillColor)
		{
			s.push(PointRight);
		}
	}
	
	

}


//Cohen-Sutherland裁剪

void CMyCGView::Oncaijianline()
{
	// TODO: 在此添加命令处理程序代码
	start = 0;
	flag = 10;
	CDC* pDC = GetDC();
	CString str;
	str = "鼠标拖动画线";
	pDC->TextOutW(10,50,str);
}
void CMyCGView::OncreateCSre()
{
	// TODO: 在此添加命令处理程序代码
	flag = 11;
	start = 0;
	CDC* pDC = GetDC();
	CString str,str1;
	str = "鼠标拖动画裁剪框";
	str1 = "由于doublebuffer函数不起作用，没法做到实时显示QAQ呜呜";
	pDC->TextOutW(10, 70, str);
	pDC->TextOutW(10, 90, str1);
}
int CMyCGView::encode(int x, int y)
{
	int c = 0;
	if (x < CSre1.x) c |= LEFT;         // 置位CL
	if (x > CSre2.x) c |= RIGHT;      // 置位CR
	if (y < CSre1.y) c |= BOTTOM;   // 置位CB
	if (y > CSre2.y ) c |= TOP;          // 置位CT
	return c;

}      //编码
void CMyCGView::CS_LineClip()
{
	int code1, code2, code;
	int x, y;
	code1 = encode(CSpoint1.x, CSpoint1.y);
	code2 = encode(CSpoint2.x, CSpoint2.y);             // 端点坐标编码
	while (code1 != 0 || code2 != 0)    //完全可见  则退出循环   进入循环则代表至少一个点在外面
	{
		if ((code1 & code2) != 0)
			return;       //完全不可见   退出循环
		code = code1;
		if (code1 == 0)
		{
			code = code2;
		}          //循环到窗外的点
		if ((LEFT & code) != 0)                 // 线段与窗口左边延长线相交
		{
			x = CSre1.x;
			y = CSpoint1.y + (CSpoint2.y - CSpoint1.y) * (CSre1.x - CSpoint1.x) / (CSpoint2.x - CSpoint1.x);
		}
		else if ((RIGHT & code) != 0)        // 线段与窗口右边延长线相交
		{
			x = CSre2.x;
			y = CSpoint1.y + (CSpoint2.y - CSpoint1.y) * (CSre2.x - CSpoint1.x) / (CSpoint2.x - CSpoint1.x);
		}
		else if ((BOTTOM & code) != 0)     // 线段与窗口上边延长线相交
		{
			y = CSre1.y;
			x = CSpoint1.x + (CSpoint2.x - CSpoint1.x) * (CSre1.y - CSpoint1.y) / (CSpoint2.y - CSpoint1.y);
		}
		else if ((TOP & code) != 0)         // 线段与窗口下边延长线相交
		{
			y = CSre2.y;
			x = CSpoint1.x + (CSpoint2.x - CSpoint1.x) * (CSre2.y - CSpoint1.y) / (CSpoint2.y - CSpoint1.y);
		}
		if (code == code1)
		{ CSpoint1.x = x;    CSpoint1.y = y;    code1 = encode(x, y); } //裁去P1到交点
		else { CSpoint2.x = x;     CSpoint2.y = y;  code2 = encode(x, y); }                     //裁去P2到交点
	}
	CDC* pDC = GetDC();
	pDC->MoveTo(CSpoint1.x, CSpoint1.y);
	pDC->LineTo(CSpoint2.x, CSpoint2.y);

}


//SH裁剪

void CMyCGView::OnSHcreateLine()
{
	// TODO: 在此添加命令处理程序代码
	flag = 12;
	start = 0;
}

void CMyCGView::OnSHcreateRE()
{
	// TODO: 在此添加命令处理程序代码
	flag = 13;
	start = 0;
}


bool CMyCGView::Inside(POINT test, POINT p0, POINT p1) {

	if (p1.x > p0.x) {

		//裁剪边是窗口的下边

		if (test.y >= p0.y) {

			return 1;

		}

	}

	else if (p1.x < p0.x) {

		//裁剪边是窗口的上边

		if (test.y <= p0.y) {

			return 1;

		}

	}

	else if (p1.y > p0.y) {

		//裁剪边是窗口的右边

		if (test.x <= p0.x) {

			return 1;

		}

	}

	else if (p1.y < p0.y) {

		//裁剪边是窗口的左边

		if (test.x >= p0.x) {

			return 1;

		}

	}

	return 0;

}

void CMyCGView::Cross(POINT s, POINT p, POINT p0, POINT p1, POINT& i)
{
	if (p0.y == p1.y) {

		//水平裁剪边

		i.y = p0.y;

		i.x = s.x + (p0.y - s.y) * (p.x - s.x) / (p.y - s.y);

	}

	else {

		//竖直裁剪边

		i.x = p0.x;

		i.y = s.y + (p0.x - s.x) * (p.y - s.y) / (p.x - s.x);

	}
}

void CMyCGView::Insert(POINT newpoint, int& mylen, POINT p[])
{
	p[mylen].x = newpoint.x;

	p[mylen].y = newpoint.y;

	mylen++;

	//顶点数+=1
}

void CMyCGView::SHClip(int mylen, POINT in[], int& outlen, POINT out[], POINT p0, POINT p1)
{
	POINT s, p, i;

	outlen = 0;

	s = in[mylen - 1];

	for (int j = 0; j < mylen; j++) {

		p = in[j];

		if (Inside(p, p0, p1)) {

			if (Inside(s, p0, p1)) {

				Insert(p, outlen, out);

			}

			else {

				Cross(s, p, p0, p1, i);

				Insert(i, outlen, out);

				Insert(p, outlen, out);

			}

		}

		else if (Inside(s, p0, p1)) {

			Cross(s, p, p0, p1, i);

			Insert(i, outlen, out);

		}

		s = p;

	}
}

void CMyCGView::clipSH()
{ 
	float XR = CSre2.x;
	float XL = CSre1.x;
	float YB = CSre1.y;
	float YT = CSre2.y;
	POINT Edge[] = { {XR,YB},{XR,YT},{XL,YT},{XL,YB} };
	POINT outp1[100], outp2[100], outp3[100], outp4[100];
	int len1, len2, len3, len4;
	int inlen = nCount;
	//右边窗口裁剪边

	SHClip(inlen, P, len1, outp1, Edge[0], Edge[1]);

	//上边窗口裁剪边

	SHClip(len1, outp1, len2, outp2, Edge[1], Edge[2]);

	//左边窗口裁剪边

	SHClip(len2, outp2, len3, outp3, Edge[2], Edge[3]);

	//下边窗口裁剪边

	SHClip(len3, outp3, len4, outp4, Edge[3], Edge[0]);
	CDC* pDC = GetDC();
	pDC->Polygon(outp4, len4);
}

//第四次作业

void CMyCGView::OnMoveCreate()
{
	// TODO: 在此添加命令处理程序代码
	flag = 14;
	is_close = false;
	bLdown = false;
}
void  CMyCGView::TransMove( int dx, int dy, CDC *pDC)
{
	RedrawWindow();
	for (int i = 0; i <= nCount; i++)
	{
		P[i].x += dx;
		P[i].y += dy;
	}
	pDC->Polygon(P, nCount);
}
void  CMyCGView::TransScalc( double sx, double sy, CDC* pDC)
{
	RedrawWindow();
	for (int i = 1; i < nCount; i++)
	{
		P[i].x = P[i].x *sx + P[0].x*(1-sx);
		P[i].y = P[i].y * sy + P[0].y * (1 - sy);
	}
	pDC->Polygon(P, nCount);
}
void  CMyCGView::TransRot(double n, CDC* pDC)
{
	RedrawWindow();
	int x, y;
	for (int i = 1;i < nCount; i++)
	{
		x = P[i].x;
		y = P[i].y;
		P[i].x= x * cos(n) - y * sin(n) + P[0].x * (1 - cos(n)) + P[0].y * sin(n);
		P[i].y = x * sin(n) + y * cos(n) + P[0].y * (1 - cos(n)) - P[0].x * sin(n);
	}
	pDC->Polygon(P, nCount);
}
//   三个Move函数
void CMyCGView::OnPingYi()
{
	// TODO: 在此添加命令处理程序代码
	flag = 15;
	Movemsg();
	
}
BOOL CMyCGView::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		CPen PenBlack(PS_SOLID, 1, RGB(0, 0, 0));
		CPen PenWhite(PS_SOLID, 1, RGB(255, 255, 255));
		CDC* pDC = GetDC();
		if (15 <= flag && flag <= 17)
		{
			if (flag == 15)
			{
				
				switch (pMsg->wParam)
				{
				case 'W':
					TransMove(0, -10,pDC);
					Movemsg();
					break;
				case 'S':
					TransMove( 0, 10,pDC);
					Movemsg();
					break;
				case 'D':
					TransMove(10, 0,pDC);
					Movemsg();
					break;
				case 'A':
					TransMove( -10, 0,pDC);
					Movemsg();
					break;
				}
				
			}
			else if (flag == 16)
			{
				double n = 0.17453292519943295;
				switch (pMsg->wParam)
				{
				case 'W':
					TransRot( n * 2,pDC);
					Rotmsg();
					break;
				case 'S':
					TransRot( -n * 2,pDC);
					Rotmsg();
					break;
				case 'D':
					TransRot( n,pDC);
					Rotmsg();
					break;
				case 'A':
					TransRot( -n,pDC);
					Rotmsg();
					break;
				}
			}
			else if (flag == 17)
			{
				switch (pMsg->wParam)
				{
				case 'W':
					TransScalc(1, 1.1,pDC);
					Scalmsg();
					break;
				case 'S':
					TransScalc(1, 0.9,pDC);
					Scalmsg();
					break;
				case 'D':
					TransScalc(1.1, 1,pDC);
					Scalmsg();
					break;
				case 'A':
					TransScalc( 0.9, 1,pDC);
					Scalmsg();
					break;
				case 'E':
					TransScalc(1.1, 1.1,pDC);
					Scalmsg();
					break;
				case 'Q':
					TransScalc(0.9, 0.9,pDC);
					Scalmsg();
					break;
				}
			}
		}
	}
	return CView::PreTranslateMessage(pMsg);
}      //键盘映射

void CMyCGView::OnMoveRote()
{
	// TODO: 在此添加命令处理程序代码
	flag = 16;
	Rotmsg();
}

void CMyCGView::OnMoveScl()
{
	// TODO: 在此添加命令处理程序代码
	flag = 17;
	Scalmsg();
	
}
//消息通知函数
void  CMyCGView::Rotmsg()
{
	CDC* pDC = GetDC();
	CString str1, str2;
	str1 = "[W]: 顺时针旋转20度   [S]: 逆时针旋转20度                 ";
	str2 = "[D]: 顺时针旋转10度   [A]: 逆时针旋转10度                 ";
	pDC->TextOut(50, 5, str1);
	pDC->TextOut(50, 30,str2);
	ReleaseDC(pDC);
}

void  CMyCGView::Movemsg()
{
	CDC* pDC = GetDC();
	CString str1, str2;
	str1 = "[W]: 上移10像素   [S]: 下移10像素";
	str2 = "[D]: 右移5像素   [A]: 左移5像素";
	pDC->TextOut(50, 5, str1);
	pDC->TextOut(50, 30, str2);
	ReleaseDC(pDC);
}

void  CMyCGView::Scalmsg()
{
	CDC* pDC = GetDC();
	CString str1, str2,str3;
	str1 = "[W]: y方向扩大 [S]: y方向缩小                        ";
	str2 = "[D]: X方向扩大 [A]: X方向缩小                        ";
	str3 = "[E]:   整体扩大 [Q]:   整体缩小                        ";
	pDC->TextOut(50, 5, str1);
	pDC->TextOut(50, 30, str2);
	pDC->TextOut(50, 55, str3);
	ReleaseDC(pDC);
}


//贝塞尔曲线
void CMyCGView::DrawBezier(CDC* pDC)
{
	CPen PenRed(PS_SOLID, 1, RGB(255, 0, 0));
	pDC->SelectObject(PenRed);
	pDC->MoveTo(Round(H[0].x), Round(H[0].y));
	double tStep = 0.01;//参数步长
	for (double t = 0.0;t <= 1.0;t = t + tStep)
	{
		double x = 0.0;double y = 0.0;
		for (int i = 0;i <= n;i++)
		{
			x += H[i].x * Cni(n, i) * pow(t, i) * pow(1 - t, n - i);
			y += H[i].y * Cni(n, i) * pow(t, i) * pow(1 - t, n - i);
		}
		pDC->LineTo(Round(x), Round(y));
	}
}

int CMyCGView::Cni(const int& n, const int& i)
{
	return (Factorial(n) / (Factorial(i) * Factorial(n - i)));
}

int CMyCGView::Factorial(int n)
{
	int factorial;
	if (n == 0 || n == 1)
		factorial = 1;
	else
		factorial = n * Factorial(n - 1);
	return factorial;
}

void CMyCGView::DrawControlPolygon(CDC* pDC)
{
	CPen PenBlack(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(PenBlack);
	pDC->MoveTo(Round(H[0].x), Round(H[0].y));
	for (int i = 0;i <= n;i++)
	{
		pDC->LineTo(Round(H[i].x), Round(H[i].y));
		pDC->Ellipse(Round(H[i].x) - 8, Round(H[i].y) - 8, Round(H[i].x) + 8, Round(H[i].y) + 8);
	}
}

void CMyCGView::OnTwoCreatePolygon()
{
	// TODO: 在此添加命令处理程序代码
	n = 2;
	flag = 18;
	comp = false;
	nCount = 0;
}


void CMyCGView::OnThreeCreatePolygon()
{
	// TODO: 在此添加命令处理程序代码
	n = 3;
	flag = 19;
	comp = false;
	nCount = 0;
}


void CMyCGView::OnFourCreatePolygon()
{
	// TODO: 在此添加命令处理程序代码
	n = 4;
	flag = 20;
	comp = false;
	nCount = 0;
}


