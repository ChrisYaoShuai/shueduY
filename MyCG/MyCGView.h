
// MyCGView.h: CMyCGView 类的接口
//

#pragma once
#include "stack_node.h"
#include<stack>
#include "math.h"
#include "CP2.h"
using namespace std;

class CMyCGView : public CView
{
protected: // 仅从序列化创建
	CMyCGView() noexcept;
	DECLARE_DYNCREATE(CMyCGView)

// 特性
public:
	CMyCGDoc* GetDocument() const;
	virtual BOOL PreTranslateMessage(MSG* pMsg);    //建立键盘映射

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DoubleBuffer(CDC* pDC);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMyCGView();
	int flag;     //标志位  
	//345为直线   678多边形创建   9为种子填充   
	//10创建裁剪直线 11创建裁剪矩形 12创建裁剪多边形   14为创建变换多边形
	//15为平移  16为旋转  17为缩放
	//18为二阶贝塞尔曲线创建   19为三阶   20为四阶
	int start;    //创建起始位
	CPoint p0;    //直线起点
	CPoint p1;    //直线终点
	void DrawLineByBh(CDC *pDC, int x0, int y0, int x1, int y1, COLORREF color);
	void DrawLineByMidpoint(CDC *pDC, int x0, int y0, int x1, int y1, COLORREF color);
	void DrawLineByDda(CDC *pDC, int x0, int y0, int x1, int y1, COLORREF color);
	void DrawObject(CDC* pDC);    //创建多边形函数
	class  MyPolygon
	{
	public:
		int m_VerticeNumber;
		CPoint m_Vertex[100];
		COLORREF m_LineColor;
	};
	void Setpoint();
	int getMiny(MyPolygon ThePolygon);
	int getMaxy(MyPolygon ThePolygon);
	void Swap(int* a, int i, int j);
	int qsort(int* a, int begin, int end);
	void randqsort(int* a, int begin, int n);
	void ScanFill(CDC* pDC,  CPoint startPoint, COLORREF fillCol);

	//数据结构 分类表和活性表
	//扫描线算法所用函数

	 int nCount;       //多边形顶点计数器
	 BOOL bLdown,is_close;      //起点判别式，封闭判别式
	 CPoint* P;       //多边形点组
	 MyPolygon ThePolygon;


	 //边缘填充算法
	 int MaxX,MaxY;
	 void GetMaxX();
	 void GetMaxY();   //获得屏幕函数
	 void bianyuanDraw();

	 //种子填充算法
	 void BoundaryFill4(CPoint Seed);
	 CPoint seed;
	 CPoint  PointLeft, PointTop, PointRight, PointBottom;

	 //CS裁剪
	 CPoint CSpoint1, CSpoint2;    //存储CS直线的点
	 CPoint CSre1, CSre2;          //存储矩形框的左上，右下顶点
	
	 int encode(int x,int y);
	 void CS_LineClip();

	 //SH裁剪
	 void SHClip(int mylen, POINT in[], int& outlen, POINT out[], POINT p0, POINT p1);     //裁剪                 //SH裁剪
	 bool Inside(POINT test, POINT p0, POINT p1);                   //判断关系
	 void Cross(POINT s, POINT p, POINT p0, POINT p1, POINT& i);     //求交
	 void Insert(POINT newpoint, int& mylen, POINT p[]);     //插入
	 void clipSH();     //整合

	 //变换函数
	 void  TransMove( int dx, int dy, CDC* pDC);
	 void  TransScalc( double sx, double sy, CDC* pDC);
	 void  TransRot(double n, CDC* pDC);
	 void  Rotmsg();
	 void  Movemsg();
	 void  Scalmsg();

	 //贝塞尔曲线
	 int n;//曲线的次数
	 CP2*  H;//曲线的控制点数组

	 void DrawBezier(CDC* pDC);//绘制Bezier曲线
	 int Cni(const int& n, const int& i);//计算C_ni
	 int Factorial(int n);//计算阶乘
	 void DrawControlPolygon(CDC* pDC);//绘制控制多边形
	 BOOL comp;    //标志曲线线段是否画完
	 BOOL down;    //用于判断鼠标是否按下，用于贝塞尔选点拖动选点
	 int p;        //用于改变选点的计数器

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:


// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDrawLineByDDA();
	afx_msg void OnDrawLineByMidpoint();
	afx_msg void OnDrawLineByBresenham();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void Onsaomiaocreate();
	afx_msg void Onbianyuancreate();
	afx_msg void Onliantongcreate();
	afx_msg void OnClear();
	afx_msg void OnFillsaomiao();
	afx_msg void Onfillbianyuan();
	afx_msg void OnseedFill();
	afx_msg void Oncaijianline();
	afx_msg void OncreateCSre();
	afx_msg void OnCS_Clip();
	afx_msg void OnSHcreateLine();
	afx_msg void OnSHcreateRE();
	afx_msg void OnMoveCreate();
	afx_msg void OnPingYi();
	afx_msg void OnMoveRote();
	afx_msg void OnMoveScl();
	afx_msg void OnTwoCreatePolygon();
	afx_msg void OnThreeCreatePolygon();
	afx_msg void OnFourCreatePolygon();
	afx_msg void OnTwoBeizer();
	afx_msg void OnCreateBeizer();
};

#ifndef _DEBUG  // MyCGView.cpp 中的调试版本
inline CMyCGDoc* CMyCGView::GetDocument() const
   { return reinterpret_cast<CMyCGDoc*>(m_pDocument); }
#endif

