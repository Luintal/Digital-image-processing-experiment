
// MFCTest1View.cpp: CMFCTest1View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCTest1.h"
#endif

#include "MFCTest1Doc.h"
#include "MFCTest1View.h"
#include "PointTrans.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CDlgImgInfo.h"
#include "CDIgZengLiang.h"
#include "CDlgHistogram.h"
#include "DIBAPI.H"
#include "CDlgPointInvert.h"
#include "CDlgInverse_V.h"
#include "DIg_Tran_V.h"
#include "DIg_Tran_Reverse_L.h"
#include "DIg_Tran_L.h"
#include "CDlgLinerPara.h"
#include "DlgPointStre.h"
#include "DlgBitPlaneSlicing.h"
#include "function.h"
#include "DlgGammaTran.h"
#include "CDlgSmooth.h"
#include "DlgMidFilter.h"
#include "Dlg_EnhaSharp.h"
#include <vector>
#include <algorithm> 
#include "DlgMorphMenu.h"

// CMFCTest1View

IMPLEMENT_DYNCREATE(CMFCTest1View, CView)

BEGIN_MESSAGE_MAP(CMFCTest1View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_DispImgInfo, &CMFCTest1View::OnDispimginfo)
	ON_COMMAND(ID_ChangeImgLight, &CMFCTest1View::OnChangeimglight)
	ON_COMMAND(ID_View_Histogram, &CMFCTest1View::OnViewHistyogram)
	ON_COMMAND(ID_24ColTo8Col, &CMFCTest1View::On24colto8col)
	ON_COMMAND(ID_24ColTo8Gray, &CMFCTest1View::On24colto8gray)
	ON_COMMAND(ID_CvtGray, &CMFCTest1View::OnCvtgray)
	ON_COMMAND(ID_CvtHalftone, &CMFCTest1View::OnCvthalftone)
	ON_COMMAND(ID_CvtBinary, &CMFCTest1View::OnCvtbinary)
	ON_COMMAND(ID_PointInvert, &CMFCTest1View::OnPointinvert)
	ON_COMMAND(ID_Tran_Reverse_V, &CMFCTest1View::OnTranReverseV)
	ON_COMMAND(ID_Tran_V, &CMFCTest1View::OnTranV)
	ON_COMMAND(ID_Tran_Reverse_L, &CMFCTest1View::OnTranReverseL)
	ON_COMMAND(ID_Tran_L, &CMFCTest1View::OnTranL)
	ON_COMMAND(ID_LinerPara, &CMFCTest1View::OnLinerpara)
	ON_COMMAND(ID_POINT_STRE, &CMFCTest1View::OnPointStre)
	ON_COMMAND(ID_BitPlaneSlicing, &CMFCTest1View::OnBitplaneslicing)
	ON_COMMAND(ID_LOGTran, &CMFCTest1View::OnLogtran)
	ON_COMMAND(ID_GammaTran, &CMFCTest1View::OnGammatran)
	ON_COMMAND(ID_EnhaSmooth, &CMFCTest1View::OnEnhasmooth)
	ON_COMMAND(ID_ENHA_MidianF, &CMFCTest1View::OnEnhaMidianf)
	ON_COMMAND(ID_Noise, &CMFCTest1View::OnNoise)
	ON_COMMAND(ID_ENHA_SHARP, &CMFCTest1View::OnEnhaSharp)
	ON_COMMAND(ID_MaxFilter, &CMFCTest1View::OnMaxfilter)
	ON_COMMAND(ID_MinFilter, &CMFCTest1View::OnMinfilter)
	ON_COMMAND(ID_MidPointFilter, &CMFCTest1View::OnMidpointfilter)
	ON_COMMAND(ID_AdapMedFilter, &CMFCTest1View::OnAdapmedfilter)
	ON_COMMAND(ID_EDGE_ROBERT, &CMFCTest1View::OnEdgeRobert)
	ON_COMMAND(ID_EDGE_SOBEL, &CMFCTest1View::OnEdgeSobel)
	ON_COMMAND(ID_RESTORE_SALTNOISE, &CMFCTest1View::OnRestoreSaltnoise)
	ON_COMMAND(ID_Morphology_MainMenu, &CMFCTest1View::OnMorphologyMainmenu)
END_MESSAGE_MAP()

// CMFCTest1View 构造/析构

CMFCTest1View::CMFCTest1View() noexcept
{
	// TODO: 在此处添加构造代码

}

CMFCTest1View::~CMFCTest1View()
{
}

BOOL CMFCTest1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCTest1View 绘图

void CMFCTest1View::OnDraw(CDC* pDC)
{
	CMFCTest1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	HGLOBAL hDIB = pDoc->GetHObject();

	// 判断DIB是否为空
	if (hDIB != NULL)
	{
		LPSTR lpDibSection = (LPSTR) ::GlobalLock((HGLOBAL)hDIB);

		// 获取DIB宽度
		int cxDIB = (int)pDoc->m_dib.GetWidth(lpDibSection);

		// 获取DIB高度
		int cyDIB = (int)pDoc->m_dib.GetHeight(lpDibSection);
		::GlobalUnlock((HGLOBAL)hDIB);
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		CRect rcDest = rcDIB;
		// 输出DIB
		pDoc->m_dib.DrawDib(pDC->m_hDC, &rcDest, pDoc->GetHObject(),
			&rcDIB, pDoc->GetDocPal());
	}
}


// CMFCTest1View 打印

BOOL CMFCTest1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCTest1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCTest1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CMFCTest1View 诊断

#ifdef _DEBUG
void CMFCTest1View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCTest1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCTest1Doc* CMFCTest1View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCTest1Doc)));
	return (CMFCTest1Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCTest1View 消息处理程序


void CMFCTest1View::OnDispimginfo()
{
	// TODO: 在此添加命令处理程序代码
	// 显示图像基本信息
	CMFCTest1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	/*
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		   AfxMessageBox(L"对不起，不是256色位图！");// 警告
		   ::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// 解除锁定
		   return;									//返回
	 }										//判断是否是8-bpp位图,不是则返回
	 */
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////	
	CDlgImgInfo dlgPara;// 创建对话框		
	dlgPara.m_Width = lSrcWidth;
	dlgPara.m_Height = lSrcHeight;
	dlgPara.m_bitCount = lpSrcBitCount;

	if (dlgPara.DoModal() != IDOK)// 显示对话框， 
	{
		return;
	}
}


void CMFCTest1View::OnChangeimglight()
{
	// TODO: 在此添加命令处理程序代码
	// 增亮/减淡图像
	CMFCTest1Doc* pDoc = GetDocument();// 获取文档    
	//////////////////////////////////////////////////////////////////////////////////////////////////  
	long lSrcLineBytes;        //图象每行的字节数
	long    lSrcWidth;      //图象的宽度和高度
	long    lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR    lpSrcDib;        //指向源图象的指针    
	LPSTR    lpSrcStartBits;    //指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);            // 找到DIB图象像素起始位置    
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);                    // 获取图象的宽度        
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);                    // 获取图象的高度    
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);        // 计算图象每行的字节数

	// 指向源图像的指针
	unsigned char* lpSrc;

	int offset;
	CDIgZengLiang dlgPara;// 创建对话框            
	if (dlgPara.DoModal() != IDOK)// 显示对话框，设定平移量
	{
		return;
	}
	offset = dlgPara.m_offset;
	int temp,k;
	if (lpSrcBitCount == 8) {
		// 修改颜色表
		RGBQUAD* pColorTable = (RGBQUAD*)(lpSrcDib + sizeof(BITMAPINFOHEADER)); // 获取调色板指针
		for (int i = 0; i < 256; i++) {
			int red = pColorTable[i].rgbRed + offset;
			int green = pColorTable[i].rgbGreen + offset;
			int blue = pColorTable[i].rgbBlue + offset;

			pColorTable[i].rgbRed = red > 255 ? 255 : (red < 0 ? 0 : red);
			pColorTable[i].rgbGreen = green > 255 ? 255 : (green < 0 ? 0 : green);
			pColorTable[i].rgbBlue = blue > 255 ? 255 : (blue < 0 ? 0 : blue);
		}
	}
	else if (lpSrcBitCount == 24) {
		// 修改像素数据
			//逐行扫描
		for (int i = 0; i < lSrcHeight; i++)
		{
			//逐列扫描
			for (int j = 0; j < lSrcWidth; j++)
			{
				// 指向DIB第i行，第j个象素的指针
				for (k = 0; k < 3; k++)
				{
					lpSrc = (unsigned char*)lpSrcStartBits + lSrcLineBytes * (lSrcHeight - 1 - i) + j * 3 + k;
					// 变换
					temp = *lpSrc + offset;
					if (temp > 255)
					{
						// 直接赋值为255
						*lpSrc = 255;
					}
					else if (temp < 0)
					{
						// 直接赋值为0
						*lpSrc = 0;
					}
					else
					{
						// 四舍五入
						*lpSrc = (unsigned char)(temp + 0.5);
					}

				}
			}
		}
	}

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定
}

void CMFCTest1View::OnViewHistyogram()
{
	// 显示图像直方图
	CMFCTest1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	unsigned char* lpSrc = nullptr;
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetBitCount(lpSrcDib) != 8)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是8位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数

	CDlgHistogram dlgPara;
	//显示直方图
	// 初始化变量值
	dlgPara.m_lpDIBBits = lpSrcStartBits;
	dlgPara.m_lWidth = lSrcWidth;
	dlgPara.m_lHeight = lSrcHeight;
	dlgPara.m_iLowGray = 0;
	dlgPara.m_iHighGray = 255;
	dlgPara.m_lLineBytes = lSrcLineBytes;

	if (dlgPara.DoModal() != IDOK) // 显示对话框 
	{
		return;
	}

	//////////////////////////
	delete dlgPara;
	EndWaitCursor();
	//设置文档修改标志
	//  pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCTest1View::On24colto8col()
{
	// TODO: 在此添加命令处理程序代码
	// 24位彩色转8位彩色
	CMFCTest1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetBitCount(lpSrcDib) != 24)// 判断是否是24-bpp位图
	{
		AfxMessageBox(L"对不起，不是24位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////

	Tran24ColTo8Col(lpSrcDib, lpSrcStartBits, lSrcWidth, lSrcHeight);// 24位彩色转8位彩色	 


	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCTest1View::On24colto8gray()
{
	// TODO: 在此添加命令处理程序代码
	// 24位彩色转8位灰度
	CMFCTest1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetBitCount(lpSrcDib) != 24)// 判断是否是24-bpp位图
	{
		AfxMessageBox(L"对不起，不是24位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////

	Tran24ColTo8Gray(lpSrcDib, lpSrcStartBits, lSrcWidth, lSrcHeight);// 24位彩色转8位灰度	 


	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}

// 8/24位彩色转8/24位灰度
void CMFCTest1View::OnCvtgray()
{
	CMFCTest1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	if (pDoc->m_dib.GetBitCount(lpSrcDib) == 24)
	{
		// 指向源图像的指针
		unsigned char* lpSrc;
		// 循环变量
		LONG	i;
		LONG	j;
		// 图像每行的字节数
		LONG	lLineBytes;

		// 计算图像每行的字节数
		lLineBytes = WIDTHBYTES(lSrcWidth * 24);

		int R, G, B, Gray;
		for (i = 0; i < lSrcHeight; i++)
		{
			for (j = 0; j < lSrcWidth; j++)
			{
				lpSrc = (unsigned char*)lpSrcStartBits + i * lLineBytes + j * 3;
				B = *lpSrc;
				lpSrc = (unsigned char*)lpSrcStartBits + i * lLineBytes + j * 3 + 1;
				G = *lpSrc;
				lpSrc = (unsigned char*)lpSrcStartBits + i * lLineBytes + j * 3 + 2;
				R = *lpSrc;
				Gray = ((R + G + B) / 3.0 + 0.5);

				lpSrc = (unsigned char*)lpSrcStartBits + i * lLineBytes + j * 3;
				*lpSrc = Gray;
				lpSrc = (unsigned char*)lpSrcStartBits + i * lLineBytes + j * 3 + 1;
				*lpSrc = Gray;
				lpSrc = (unsigned char*)lpSrcStartBits + i * lLineBytes + j * 3 + 2;
				*lpSrc = Gray;

			}
		}
	}

	if (pDoc->m_dib.GetBitCount(lpSrcDib) == 8)
	{
		// 指向源图像的指针
		unsigned char* lpSrc;
		// 循环变量
		LONG	i;
		LONG	j;
		// 图像每行的字节数
		LONG	lLineBytes;

		// 计算图像每行的字节数
		lLineBytes = WIDTHBYTES(lSrcWidth * 8);

		// 灰度映射表
		BYTE bMap[256];
		RGBQUAD* pTable, * pTable1;//颜色表指针

		pTable = (RGBQUAD*)(lpSrcDib + sizeof(BITMAPINFOHEADER));
		for (i = 0; i < 256; i++)//生成新的调色板,并转换灰度
		{
			// 计算该颜色对应的灰度值g=0.299*r+0.587*g+0.114*b	
			bMap[i] = (BYTE)(0.299 * pTable->rgbRed + 0.587 * pTable->rgbGreen + 0.114 * pTable->rgbBlue + 0.5);
			pTable->rgbRed = (BYTE)i;
			pTable->rgbGreen = (BYTE)i;
			pTable->rgbBlue = (BYTE)i;
			pTable->rgbReserved = (BYTE)0;
			pTable++;
		}
		// 更换每个象素的颜色索引（即按照灰度映射表换成灰度值）
		//逐行扫描
		for (i = 0; i < lSrcHeight; i++)
		{
			//逐列扫描
			for (j = 0; j < lSrcWidth; j++)
			{
				// 指向DIB第i行，第j个象素的指针
				lpSrc = (unsigned char*)lpSrcStartBits + lLineBytes * (lSrcHeight - 1 - i) + j;
				// 变换
				*lpSrc = bMap[*lpSrc];
			}
		}
	}

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

}

// 灰度图像的半影调化（图案法）
void CMFCTest1View::OnCvthalftone()
{
	CMFCTest1Doc* pDoc = GetDocument();	// 获取文档
	long lSrcLineBytes;		// 图像每行的字节数
	long lSrcWidth;			// 图像的宽度和高度
	long lSrcHeight;
	int lpSrcBitCount;		// 图像的位深
	LPSTR lpSrcDib;			// 指向源图像的指针
	LPSTR lpSrcStartBits;	// 指向源像素的指针
	lpSrcDib = (LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHObject());	// 锁定DIB
	if (!lpSrcDib) return;
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);						// 找到DIB图像像素起始位置
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);							// 获取图像的宽度
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);						// 获取图像的高度
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);						// 获取图像的位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);	// 计算图像每行的字节数
	// 定义8x8阈值矩阵
	int thresholdMatrix[8][8] = { 0,32,8,40,2,34,10,42,
					 48,16,56,24,50,18,58,26,
					 12,44,4,36,14,46,6,38,
					 60,28,52,20,62,30,54,22,
					 3,35,11,43,1,33,9,41,
					 51,19,59,27,49,17,57,25,
					 15,47,7,39,13,45,5,37,
					 63,31,55,23,61,29,53,21 };

	// 每行
	for (LONG i = 0; i < lSrcHeight; i++)
	{
		// 每列
		for (LONG j = 0; j < lSrcWidth; j++)
		{
			// 获取像素指针
			unsigned char* lpPixel = (unsigned char*)lpSrcStartBits + lSrcLineBytes * (lSrcHeight - 1 - i) + j;
			// 计算在阈值矩阵中的位置
			int x = j % 8;
			int y = i % 8;
			int threshold = thresholdMatrix[y][x];
			// 根据阈值进行二值化
			if (*lpPixel / 255.0 > threshold / 64.0)
			{
				*lpPixel = 255;
			}
			else
			{
				*lpPixel = 0;
			}
		}
	}
	// 设置文档修改标志
	pDoc->SetModifiedFlag(true);
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


// 灰度图转二值图
void CMFCTest1View::OnCvtbinary()
{
	CMFCTest1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////

		// 指向源图像的指针
	unsigned char* lpSrc;

	// 循环变量
	LONG	i;
	LONG	j;

	// 图像每行的字节数
	LONG	lLineBytes;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lSrcWidth * 8);

	// 每行
	for (i = 0; i < lSrcHeight; i++)
	{
		// 每列
		for (j = 0; j < lSrcWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpSrcStartBits + lLineBytes * (lSrcHeight - 1 - i) + j;

			// 判断是否小于阈值
			if ((*lpSrc) < 123)
			{
				// 直接赋值为0
				*lpSrc = 0;
			}
			else
			{
				// 直接赋值为255
				*lpSrc = 255;
			}
		}
	}


	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

}

// 灰度图反色
void CMFCTest1View::OnPointinvert()
{
	CMFCTest1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)	//判断是否是8位位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());  // 解除锁定
		return;
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	CDlgPointInvert dlgPara;// 创建对话框	
	if (dlgPara.DoModal() != IDOK)// 显示对话框 
	{
		return;
	}

	// 调用函数
	FLOAT fA = -1.0;
	FLOAT fB = 255.0;
	LinerTrans(lpSrcStartBits, lSrcWidth, lSrcHeight, fA, fB);

	// 设置文档修改标志
	pDoc->SetModifiedFlag(true);
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}

//倒V型变换
void CMFCTest1View::OnTranReverseV()
{
	// TODO: 在此添加命令处理程序代码
	

	CMFCTest1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bip位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	CDlgInverse_V dlgPara;// 创建对话框	
	dlgPara.m_threshold = 127;
	if (dlgPara.DoModal() != IDOK)// 显示对话框，设定平移量
	{
		return;
	}

	int type, threshold;
	threshold = dlgPara.m_threshold;
	type = 1;
	Segmental2_Linear_Tran(lpSrcStartBits, lSrcWidth, lSrcHeight, type, threshold);

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCTest1View::OnTranV()
{
	// TODO: 在此添加命令处理程序代码
	// V型变换	
	CMFCTest1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bip位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	DIg_Tran_V dlgPara;// 创建对话框	
	dlgPara.m_thresholdV = 127;
	if (dlgPara.DoModal() != IDOK)// 显示对话框，设定平移量
	{
		return;
	}

	int type, threshold;
	threshold = dlgPara.m_thresholdV;
	type = 2;
	Segmental2_Linear_Tran(lpSrcStartBits, lSrcWidth, lSrcHeight, type, threshold);

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

}


void CMFCTest1View::OnTranReverseL()
{
	// TODO: 在此添加命令处理程序代码
	// 倒L型变换	
	CMFCTest1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	DIg_Tran_Reverse_L dlgPara;// 创建对话框	
	dlgPara.m_thresholdRL = 127;
	if (dlgPara.DoModal() != IDOK)// 显示对话框，设定平移量
	{
		return;
	}
	
	int type;
	type = 3;
	Segmental2_Linear_Tran(lpSrcStartBits, lSrcWidth, lSrcHeight, type);

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}

// L型变换
void CMFCTest1View::OnTranL()
{
	// TODO: 在此添加命令处理程序代码
	// L型变换
	CMFCTest1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bip位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	DIg_Tran_L dlgPara;// 创建对话框	
	dlgPara.m_thresholdL = 127;
	if (dlgPara.DoModal() != IDOK)// 显示对话框，设定平移量
	{
		return;
	}
	
	int type;
	type = 4;
	Segmental2_Linear_Tran(lpSrcStartBits, lSrcWidth, lSrcHeight, type);

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}

//线性变换
void CMFCTest1View::OnLinerpara()
{
	CMFCTest1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	CDlgLinerPara dlgPara;// 创建对话框			
	if (dlgPara.DoModal() != IDOK)// 显示对话框，设定平移量
	{
		return;
	}
	FLOAT fA = dlgPara.m_fA;
	FLOAT fB = dlgPara.m_fB;  //(2f)
	LinerTrans(lpSrcStartBits, lSrcWidth, lSrcHeight, fA, fB);

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

}

void CMFCTest1View::OnPointStre()
{
	// 获取文档
	CMFCTest1Doc* pDoc = GetDocument();

	// 锁定DIB
	LPSTR lpDIB = (LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHObject());
	if (!lpDIB) return;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的灰度拉伸，其它的可以类推）
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)
	{
		// 提示用户
		AfxMessageBox(L"只支持8位灰度图像处理！");

		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}

	// 找到DIB图像像素起始位置
	
	LPSTR lpDIBBits = pDoc->m_dib.GetBits(lpDIB);			// 找到DIB图象像素起始位置
	long lSrcWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度		
	long lSrcHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度
	// 创建对话框
	DlgPointStre dlgPara;

	// 初始化变量值
	dlgPara.m_bX1 = 50;
	dlgPara.m_bY1 = 30;
	dlgPara.m_bX2 = 200;
	dlgPara.m_bY2 = 220;

	// 显示对话框，提示用户设定拉伸位置
	if (dlgPara.DoModal() != IDOK)
	{
		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}

	// 获取用户的设定
	BYTE bX1 = dlgPara.m_bX1;
	BYTE bY1 = dlgPara.m_bY1;
	BYTE bX2 = dlgPara.m_bX2;
	BYTE bY2 = dlgPara.m_bY2;

	// 更改光标形状
	BeginWaitCursor();

	// 调用GrayStretch()函数进行灰度拉伸
	GrayStretch(lpDIBBits, lSrcWidth, lSrcHeight, bX1, bY1, bX2, bY2);

	// 设置文档修改标志
	pDoc->SetModifiedFlag(TRUE);

	// 更新视图
	pDoc->UpdateAllViews(NULL);

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}


//位平面分解
void CMFCTest1View::OnBitplaneslicing()
{
	// TODO: 在此添加命令处理程序代码
	// 位平面分解

	CMFCTest1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	unsigned char* lpSrc;
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetBitCount(lpSrcDib) != 8)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是8位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数

	DlgBitPlaneSlicing dlgPara;

	dlgPara.lSrcLineBytes = lSrcLineBytes;		//图象每行的字节数
	dlgPara.lSrcWidth = lSrcWidth;      //图象的宽度和高度
	dlgPara.lSrcHeight = lSrcHeight;
	dlgPara.lpSrcBitCount = lpSrcBitCount;       //图像的位深
	dlgPara.lpSrcDib = lpSrcDib;		//指向源图象的指针	
	dlgPara.lpSrcStartBits = lpSrcStartBits;	//指向源像素的指针


	//位平面分解
	if (dlgPara.DoModal() != IDOK) // 显示对话框 
	{
		return;
	}

	//////////////////////////
	delete dlgPara;
	EndWaitCursor();
	//设置文档修改标志
	//  pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}

// 对数变换
void CMFCTest1View::OnLogtran()
{
	// TODO: 在此添加命令处理程序代码
	CMFCTest1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	unsigned char* lpSrc;
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetBitCount(lpSrcDib) != 8)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是8位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数

	BitPlaneSlicing(lpSrcStartBits, lSrcWidth, lSrcLineBytes, lSrcHeight, 6);
	LogTranslation(lpSrcStartBits, lSrcWidth, lSrcHeight, lSrcLineBytes);


	EndWaitCursor();
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCTest1View::OnGammatran()
{
	// TODO: 在此添加命令处理程序代码
	// gamma 变换
	CMFCTest1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	unsigned char* lpSrc;
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetBitCount(lpSrcDib) != 8)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是8位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数

	DlgGammaTran dlgPara;
	//位平面分解
	if (dlgPara.DoModal() != IDOK) // 显示对话框 
	{
		return;
	}
	float gamma = dlgPara.m_fValGamma;

	long i;                 //行循环变量
	long j;                 //列循环变量

	float val;
	for (i = 0; i < lSrcHeight; i++)// 行
	{
		for (j = 0; j < lSrcWidth; j++)// 列
		{
			lpSrc = (unsigned char*)lpSrcStartBits + lSrcLineBytes * (lSrcHeight - 1 - i) + j;// 指向DIB第i行，第j个像素的指针
			val = (*lpSrc);
			val = pow(val, gamma);
			val = 255 * val / pow(255, gamma);

			*lpSrc = (int)val;
		}
	}



	//////////////////////////
	delete dlgPara;
	EndWaitCursor();
	//设置文档修改标志
	//  pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
	
}

//平滑处理
void CMFCTest1View::OnEnhasmooth()
{
	// TODO: 在此添加命令处理程序代码

	// 获取文档
	CMFCTest1Doc* pDoc = GetDocument();

	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;

	// 模板高度
	int		iTempH;

	// 模板宽度
	int		iTempW;

	// 模板系数
	FLOAT	fTempC;

	// 模板中心元素X坐标
	int		iTempMX;

	// 模板中心元素Y坐标
	int		iTempMY;

	// 模板元素数组
	double	aValue[25];

	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());

	// 找到DIB图像象素起始位置
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	long lpDIBWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度		
	long lpDIBHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度

	// 判断是否是8-bip位图（这里为了方便，只处理8-bip位图的平滑，其它的可以类推）
	if (pDoc->m_dib.GetBitCount(lpDIB) != 8)
	{
		// 提示用户
		MessageBox(L"目前只支持256色位图的平滑！", L"系统提示", MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}

	// 创建对话框
	CDlgSmooth dlgPara;

	// 给模板数组赋初值（为平均模板）
	aValue[0] = 1.0;
	aValue[1] = 1.0;
	aValue[2] = 1.0;
	aValue[3] = 0.0;
	aValue[4] = 0.0;
	aValue[5] = 1.0;
	aValue[6] = 1.0;
	aValue[7] = 1.0;
	aValue[8] = 0.0;
	aValue[9] = 0.0;
	aValue[10] = 1.0;
	aValue[11] = 1.0;
	aValue[12] = 1.0;
	aValue[13] = 0.0;
	aValue[14] = 0.0;
	aValue[15] = 0.0;
	aValue[16] = 0.0;
	aValue[17] = 0.0;
	aValue[18] = 0.0;
	aValue[19] = 0.0;
	aValue[20] = 0.0;
	aValue[21] = 0.0;
	aValue[22] = 0.0;
	aValue[23] = 0.0;
	aValue[24] = 0.0;

	// 初始化对话框变量值
	dlgPara.m_intType = 0;
	dlgPara.m_iTempH = 3;
	dlgPara.m_iTempW = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_fTempC = (FLOAT)(1.0 / 9.0);
    dlgPara.m_fpArray = aValue;

	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	// 获取用户设定的平移量
	iTempH = dlgPara.m_iTempH;
	iTempW = dlgPara.m_iTempW;
	iTempMX = dlgPara.m_iTempMX;
	iTempMY = dlgPara.m_iTempMY;
	fTempC = dlgPara.m_fTempC;

	// 删除对话框
	delete dlgPara;

	// 更改光标形状
	BeginWaitCursor();

// 调用Template()函数平滑DIB
if (::Template(lpDIBBits, lpDIBWidth, lpDIBHeight,
	iTempH, iTempW, iTempMX, iTempMY, aValue, fTempC))
{

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);

	// 更新视图
	pDoc->UpdateAllViews(NULL);
}
//else
//{
//	// 提示用户
//	MessageBox( L"分配内存失败！", L"系统提示", MB_ICONINFORMATION | MB_OK);
//}

// 解除锁定
::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

// 恢复光标
EndWaitCursor();

}

//中值滤波
void CMFCTest1View::OnEnhaMidianf()
{
	// TODO: 在此添加命令处理程序代码
	// 获取文档	
	CMFCTest1Doc* pDoc = GetDocument();
	// 指向DIB的指针
	LPSTR	lpDIB;
	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	// 滤波器的高度
	int iFilterH;

	// 滤波器的宽度
	int iFilterW;

	// 中心元素的X坐标
	int iFilterMX;

	// 中心元素的Y坐标
	int iFilterMY;

	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());

	// 找到DIB图像象素起始位置
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	long lpDIBWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度
	long lpDIBHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度

	// 判断是否是8-bip位图（这里为了方便，只处理8-bip位图的中值滤波，其它的可以类推）
	if (pDoc->m_dib.GetBitCount(lpDIB) != 8)
	{
		// 提示用户
		MessageBox(L"目前只支持256色位图的中值滤波！", L"系统提示", MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}

	// 创建对话框
	CDlgMidFilter dlgPara;
    
	// 初始化对话框变量值
	dlgPara.m_iFilterType = 0;
	dlgPara.m_iFilterH = 3;
	dlgPara.m_iFilterW = 1;
	dlgPara.m_iFilterMX = 0;
	dlgPara.m_iFilterMY = 1;

	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	// 获取用户的设定
	iFilterH = dlgPara.m_iFilterH;
	iFilterW = dlgPara.m_iFilterW;
	iFilterMX = dlgPara.m_iFilterMX;
	iFilterMY = dlgPara.m_iFilterMY;

	// 删除对话框
	delete dlgPara;

	// 更改光标形状
	BeginWaitCursor();

	// 调用MedianFilter()函数中值滤波
	if(::MedianFilter(lpDIBBits, lpDIBWidth, lpDIBHeight, 
		iFilterH, iFilterW, iFilterMX, iFilterMY))
	{
		// 设置文档修改标志
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox(L"分配内存失败！", L"系统提示", MB_ICONINFORMATION | MB_OK);
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();

}

// 添加随机噪声
BOOL WINAPI AddNoise(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	// 指向源图像的指针
	LPSTR	lpSrc;

	//循环变量
	long i;
	long j;

	// 图像每行的字节数
	LONG lLineBytes;

	//像素值
	unsigned char pixel;

	//噪声
	BYTE NoisePoint;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	//生成伪随机种子
	srand((unsigned)time(NULL));

	//在图像中加噪
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lLineBytes; i++)
		{
			NoisePoint = rand() / 1024;

			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char*)lpDIBBits + lLineBytes * j + i;

			//取得像素值
			pixel = (unsigned char)*lpSrc;

			*lpSrc = (unsigned char)(pixel * 224 / 256 + NoisePoint);
		}
	}
	// 返回
	return true;
}

//添加噪声
void CMFCTest1View::OnNoise()
{
	// TODO: 在此添加命令处理程序代码
	// 获取文档
	CMFCTest1Doc* pDoc = GetDocument();

	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;

	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的模糊操作，其它的可以类推）
	if (pDoc->m_dib.GetBitCount(lpDIB) != 8)
	{
		// 提示用户
		MessageBox(L"目前只支持256色位图的运算！", L"系统提示", MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}

	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	long lpDIBWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度
	long lpDIBHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度

	// 调用NoiseBlurDIB()函数对DIB进行模糊加噪处理
	if (AddNoise(lpDIBBits, lpDIBWidth, lpDIBHeight))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox(L"分配内存失败或图像尺寸不符合要求！", L"系统提示", MB_ICONINFORMATION | MB_OK);
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();

}

//拉普拉斯锐化
void CMFCTest1View::OnEnhaSharp()
{
	// TODO: 在此添加命令处理程序代码
	// 获取文档
	CMFCTest1Doc* pDoc = GetDocument();
	int neighborhoodType; // 4邻域
	// 指向DIB的指针
	LPSTR lpDIB;

	// 指向DIB象素指针
	LPSTR    lpDIBBits;

	// 模板高度
	int		iTempH;

	// 模板宽度
	int		iTempW;

	// 模板系数
	FLOAT	fTempC;

	// 模板中心元素X坐标
	int		iTempMX;

	// 模板中心元素Y坐标
	int		iTempMY;

	// 模板元素数组
	double	aValue[9];

	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());

	// 找到DIB图像象素起始位置
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	long lpDIBWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度
	long lpDIBHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的锐化，其它的可以类推）
	if (pDoc->m_dib.GetBitCount(lpDIB) != 8)
	{
		// 提示用户
		MessageBox(L"目前只支持256色位图的锐化！", L"系统提示",
			MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}

	// 创建对话框
	Dlg_EnhaSharp dlgPara;

	// 初始化变量值
	dlgPara.m_neighborhoodType = 0;

	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	// 获取用户设定的平移量
	neighborhoodType = dlgPara.m_neighborhoodType;


	// 更改光标形状
	BeginWaitCursor();

	// 设置拉普拉斯模板参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	// 根据邻域系统设置拉普拉斯模板参数
	if (neighborhoodType == 0) // 4邻域
	{
		aValue[0] = 0.0; aValue[1] = -1.0; aValue[2] = 0.0;
		aValue[3] = -1.0; aValue[4] = 5.0; aValue[5] = -1.0;
		aValue[6] = 0.0; aValue[7] = -1.0; aValue[8] = 0.0;
	}
	else if (neighborhoodType == 1) // 8邻域
	{
		aValue[0] = -1.0; aValue[1] = -1.0; aValue[2] = -1.0;
		aValue[3] = -1.0; aValue[4] = 9.0; aValue[5] = -1.0;
		aValue[6] = -1.0; aValue[7] = -1.0; aValue[8] = -1.0;
	}

	// 调用Template()函数用拉普拉斯模板锐化DIB
	if (::Template(lpDIBBits, lpDIBWidth, lpDIBHeight,
		iTempH, iTempW, iTempMX, iTempMY, aValue, fTempC))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox(L"分配内存失败！",L"系统提示", MB_ICONINFORMATION | MB_OK);
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}

// 最大值滤波器函数
BOOL MaxFilter(LPSTR lpDIBBits, LONG lmageWidth, LONG lmageHeight, int mFilterH, int mFilterW, int mFilterMX, int mFilterMY)
{
	LPBYTE lpImage = (LPBYTE) new char[lmageWidth * lmageHeight];
	if (lpImage == NULL)
	{
		return FALSE;
	}
	memcpy(lpImage, lpDIBBits, lmageWidth * lmageHeight);

	for (int i = mFilterMY; i < lmageHeight - mFilterH + mFilterMY + 1; i++)
	{
		for (int j = mFilterMX; j < lmageWidth - mFilterW + mFilterMX + 1; j++)
		{
			unsigned char maxVal = 0; // 初始化最大值为最低可能值
			for (int k = 0; k < mFilterH; k++)
			{
				for (int l = 0; l < mFilterW; l++)
				{
					unsigned char* lpSrc = (unsigned char*)lpDIBBits + lmageWidth * (lmageHeight - 1 - i + mFilterMY - k) + j - mFilterMX + l;
					if (*lpSrc > maxVal) maxVal = *lpSrc; // 更新最大值
				}
			}
			unsigned char* lpDst = (unsigned char*)lpImage + lmageWidth * (lmageHeight - 1 - i) + j;
			*lpDst = maxVal; // 将最大值写入目标图像
		}
	}

	memcpy(lpDIBBits, lpImage, lmageWidth * lmageHeight);
	delete[] lpImage;
	return TRUE;
}


//最大值滤波
void CMFCTest1View::OnMaxfilter()
{
	// TODO: 在此添加命令处理程序代码
	// 获取文档
	CMFCTest1Doc* pDoc = GetDocument();
	// 锁定DIB
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());
	// 找到DIB图像象素起始位置
	LPSTR lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	long lpDIBWidth = pDoc->m_dib.GetWidth(lpDIB);
	long lpDIBHeight = pDoc->m_dib.GetHeight(lpDIB);

	// 判断是否是8-bip位图
	if (pDoc->m_dib.GetBitCount(lpDIB) != 8)
	{
		MessageBox(L"目前只支持256色位图的最大值滤波！", L"系统提示", MB_ICONINFORMATION | MB_OK);
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
		return;
	}

	BeginWaitCursor();

	// 调用MaxFilter()函数进行最大值滤波，固定参数
	if (::MaxFilter(lpDIBBits, lpDIBWidth, lpDIBHeight, 3, 3, 1, 1))
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		MessageBox(L"分配内存失败！", L"系统提示", MB_ICONINFORMATION | MB_OK);
	}

	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
	EndWaitCursor();



}

// 最小值滤波器函数
BOOL MinFilter(LPSTR lpDIBBits, LONG lmageWidth, LONG lmageHeight, int mFilterH, int mFilterW, int mFilterMX, int mFilterMY)
{
	LPBYTE lpImage = (LPBYTE)new char[lmageWidth * lmageHeight];
	if (lpImage == NULL)
	{
		return FALSE;
	}
	memcpy(lpImage, lpDIBBits, lmageWidth * lmageHeight);

	for (int i = mFilterMY; i < lmageHeight - mFilterH + mFilterMY + 1; i++)
	{
		for (int j = mFilterMX; j < lmageWidth - mFilterW + mFilterMX + 1; j++)
		{
			unsigned char minVal = 255; // 初始化最小值为最高可能值
			for (int k = 0; k < mFilterH; k++)
			{
				for (int l = 0; l < mFilterW; l++)
				{
					unsigned char* lpSrc = (unsigned char*)lpDIBBits + lmageWidth * (lmageHeight - 1 - i + mFilterMY - k) + j - mFilterMX + l;
					if (*lpSrc < minVal) minVal = *lpSrc; // 更新最小值
				}
			}
			unsigned char* lpDst = (unsigned char*)lpImage + lmageWidth * (lmageHeight - 1 - i) + j;
			*lpDst = minVal; // 将最小值写入目标图像
		}
	}

	memcpy(lpDIBBits, lpImage, lmageWidth * lmageHeight);
	delete[] lpImage;
	return TRUE;
}

void CMFCTest1View::OnMinfilter()
{
	// TODO: 在此添加命令处理程序代码
	 // 获取文档
	CMFCTest1Doc* pDoc = GetDocument();
	// 锁定DIB
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());
	// 找到DIB图像象素起始位置
	LPSTR lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	long lpDIBWidth = pDoc->m_dib.GetWidth(lpDIB);
	long lpDIBHeight = pDoc->m_dib.GetHeight(lpDIB);

	// 判断是否是8-bip位图
	if (pDoc->m_dib.GetBitCount(lpDIB) != 8)
	{
		MessageBox(L"目前只支持256色位图的最小值滤波！", L"系统提示", MB_ICONINFORMATION | MB_OK);
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
		return;
	}

	BeginWaitCursor();

	// 调用MinFilter()函数进行最小值滤波，固定参数
	if (::MinFilter(lpDIBBits, lpDIBWidth, lpDIBHeight, 3, 3, 1, 1))
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		MessageBox(L"分配内存失败！", L"系统提示", MB_ICONINFORMATION | MB_OK);
	}

	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
	EndWaitCursor();

}

// 中点滤波器函数
BOOL MidpointFilter(LPSTR lpDIBBits, LONG lmageWidth, LONG lmageHeight, int mFilterH, int mFilterW, int mFilterMX, int mFilterMY)
{
	LPBYTE lpImage = (LPBYTE) new char[lmageWidth * lmageHeight];
	if (lpImage == NULL)
	{
		return FALSE;
	}
	memcpy(lpImage, lpDIBBits, lmageWidth * lmageHeight);

	for (int i = mFilterMY; i < lmageHeight - mFilterH + mFilterMY + 1; i++)
	{
		for (int j = mFilterMX; j < lmageWidth - mFilterW + mFilterMX + 1; j++)
		{
			unsigned char maxVal = 0; // 初始化最大值为最低可能值
			unsigned char minVal = 255; // 初始化最小值为最高可能值
			for (int k = 0; k < mFilterH; k++)
			{
				for (int l = 0; l < mFilterW; l++)
				{
					unsigned char* lpSrc = (unsigned char*)lpDIBBits + lmageWidth * (lmageHeight - 1 - i + mFilterMY - k) + j - mFilterMX + l;
					if (*lpSrc > maxVal) maxVal = *lpSrc; // 更新最大值
					if (*lpSrc < minVal) minVal = *lpSrc; // 更新最小值
				}
			}
			unsigned char* lpDst = (unsigned char*)lpImage + lmageWidth * (lmageHeight - 1 - i) + j;
			*lpDst = (unsigned char)((maxVal + minVal) / 2); // 将最大值和最小值的平均值写入目标图像
		}
	}

	memcpy(lpDIBBits, lpImage, lmageWidth * lmageHeight);
	delete[] lpImage;
	return TRUE;
}

// 中点滤波
void CMFCTest1View::OnMidpointfilter()
{
	// TODO: 在此添加命令处理程序代码
	// 获取文档
	CMFCTest1Doc* pDoc = GetDocument();
	// 锁定DIB
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());
	// 找到DIB图像象素起始位置
	LPSTR lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	long lpDIBWidth = pDoc->m_dib.GetWidth(lpDIB);
	long lpDIBHeight = pDoc->m_dib.GetHeight(lpDIB);

	// 判断是否是8-bip位图
	if (pDoc->m_dib.GetBitCount(lpDIB) != 8)
	{
		MessageBox(L"目前只支持256色位图的中点滤波！", L"系统提示", MB_ICONINFORMATION | MB_OK);
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
		return;
	}

	BeginWaitCursor();

	// 调用MidpointFilter()函数进行中点滤波，固定参数
	if (::MidpointFilter(lpDIBBits, lpDIBWidth, lpDIBHeight, 3, 3, 1, 1))
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		MessageBox(L"分配内存失败！", L"系统提示", MB_ICONINFORMATION | MB_OK);
	}

	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
	EndWaitCursor();
}

// 自适应中值滤波器函数
BOOL AdaptiveMedianFilter(LPSTR lpDIBBits, LONG lmageWidth, LONG lmageHeight, int minSize, int maxSize)
{
	LPBYTE lpImage = (LPBYTE) new char[lmageWidth * lmageHeight];
	if (lpImage == NULL)
	{
		return FALSE;
	}
	memcpy(lpImage, lpDIBBits, lmageWidth * lmageHeight);

	std::vector<unsigned char> window;

	for (int y = 0; y < lmageHeight; y++)
	{
		for (int x = 0; x < lmageWidth; x++)
		{
			int filterSize = minSize;
			bool filterDone = false;
			unsigned char pixel = 0;

			while (!filterDone && filterSize <= maxSize)
			{
				window.clear();
				int halfSize = filterSize / 2;

				for (int i = -halfSize; i <= halfSize; i++)
				{
					for (int j = -halfSize; j <= halfSize; j++)
					{
						int nx = x + j;
						int ny = y + i;

						if (nx >= 0 && nx < lmageWidth && ny >= 0 && ny < lmageHeight)
						{
							window.push_back(lpDIBBits[lmageWidth * ny + nx]);
						}
					}
				}

				std::sort(window.begin(), window.end());
				unsigned char med = window[window.size() / 2];
				unsigned char medMin = window.front();
				unsigned char medMax = window.back();

				if (medMin < med && med < medMax)
				{
					pixel = med;
					filterDone = true;
				}
				else
				{
					filterSize += 2; // 增加窗口尺寸
				}
			}

			if (!filterDone)
			{
				pixel = lpDIBBits[lmageWidth * y + x]; // 使用原像素值
			}

			lpImage[lmageWidth * y + x] = pixel;
		}
	}

	memcpy(lpDIBBits, lpImage, lmageWidth * lmageHeight);
	delete[] lpImage;
	return TRUE;
}

void CMFCTest1View::OnAdapmedfilter()
{
	// TODO: 在此添加命令处理程序代码
	// 获取文档
	CMFCTest1Doc* pDoc = GetDocument();
	// 锁定DIB
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());
	// 找到DIB图像象素起始位置
	LPSTR lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	long lpDIBWidth = pDoc->m_dib.GetWidth(lpDIB);
	long lpDIBHeight = pDoc->m_dib.GetHeight(lpDIB);

	// 判断是否是8-bip位图
	if (pDoc->m_dib.GetBitCount(lpDIB) != 8)
	{
		MessageBox(L"目前只支持256色位图的自适应中值滤波！", L"系统提示", MB_ICONINFORMATION | MB_OK);
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
		return;
	}

	BeginWaitCursor();

	// 调用AdaptiveMedianFilter()函数进行自适应中值滤波，固定参数
	if (::AdaptiveMedianFilter(lpDIBBits, lpDIBWidth, lpDIBHeight, 3, 7))
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		MessageBox(L"分配内存失败！", L"系统提示", MB_ICONINFORMATION | MB_OK);
	}

	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
	EndWaitCursor();
}

/*************************************************************************
 *
 * 函数名称：
 *   RobertDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 * 返回值:
 *   BOOL               - 边缘检测成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用Robert边缘检测算子对图像进行边缘检测运算。
 *
 * 要求目标图像为灰度图像。
 ************************************************************************/

BOOL WINAPI RobertDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{

	// 指向源图像的指针
	LPSTR	lpSrc;

	// 指向缓存图像的指针
	LPSTR	lpDst;

	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//循环变量
	long i;
	long j;

	//像素值
	double result;
	unsigned char pixel[4];

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为255
	lpDst = (char*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, static_cast<size_t>(lWidth) * lHeight);


	//使用水平方向的结构元素进行腐蚀
	for (j = lHeight - 1; j > 0; j--)
	{
		for (i = 0; i < lWidth - 1; i++)
		{
			//由于使用2×2的模板，为防止越界，所以不处理最下边和最右边的两列像素

			// 指向源图像第j行，第i个象素的指针			
			lpSrc = (char*)lpDIBBits + lWidth * j + i;

			// 指向目标图像第j行，第i个象素的指针			
			lpDst = (char*)lpNewDIBBits + lWidth * j + i;

			//取得当前指针处2*2区域的像素值，注意要转换为unsigned char型
			pixel[0] = (unsigned char)*lpSrc;
			pixel[1] = (unsigned char)*(lpSrc + 1);
			pixel[2] = (unsigned char)*(lpSrc - lWidth);
			pixel[3] = (unsigned char)*(lpSrc - lWidth + 1);

			//计算目标图像中的当前点
			result = sqrt((pixel[0] - pixel[3]) * (pixel[0] - pixel[3]) + \
				(pixel[1] - pixel[2]) * (pixel[1] - pixel[2]));
			*lpDst = (unsigned char)result;

		}
	}

	// 复制腐蚀后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;
}


void CMFCTest1View::OnEdgeRobert()
{
	// TODO: 在此添加命令处理程序代码

	//Robert边缘检测运算

	// 获取文档
	CMFCTest1Doc* pDoc = GetDocument();

	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;

	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的边缘检测，其它的可以类推）
	if (pDoc->m_dib.GetBitCount(lpDIB) != 8)
	{
		// 提示用户
		MessageBox(L"目前只支持256色位图的运算！", L"系统提示", MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}

	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	long lpDIBWidth = pDoc->m_dib.GetWidth(lpDIB);
	long lpDIBHeight = pDoc->m_dib.GetHeight(lpDIB);

	// 调用RobertDIB()函数对DIB进行边缘检测
	if (RobertDIB(lpDIBBits, WIDTHBYTES(lpDIBWidth * 8), lpDIBHeight))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox(L"分配内存失败！", L"系统提示", MB_ICONINFORMATION | MB_OK);
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();

}



/*************************************************************************
 *
 * 函数名称：
 *   SobelDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 * 返回值:
 *   BOOL               - 边缘检测成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用Sobel边缘检测算子对图像进行边缘检测运算。
 *
 * 要求目标图像为灰度图像。
 ************************************************************************/

BOOL WINAPI SobelDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{

	// 指向缓存图像的指针
	LPSTR	lpDst1;
	LPSTR	lpDst2;

	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits1;
	HLOCAL	hNewDIBBits1;
	LPSTR	lpNewDIBBits2;
	HLOCAL	hNewDIBBits2;

	//循环变量
	long i;
	long j;

	// 模板高度
	int		iTempH;

	// 模板宽度
	int		iTempW;

	// 模板系数
	FLOAT	fTempC;

	// 模板中心元素X坐标
	int		iTempMX;

	// 模板中心元素Y坐标
	int		iTempMY;

	//模板数组
	double aTemplate[9];

	// 暂时分配内存，以保存新图像
	hNewDIBBits1 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits1 = (char*)LocalLock(hNewDIBBits1);

	// 暂时分配内存，以保存新图像
	hNewDIBBits2 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits2 = (char*)LocalLock(hNewDIBBits2);

	// 拷贝源图像到缓存图像中
	lpDst1 = (char*)lpNewDIBBits1;
	memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
	lpDst2 = (char*)lpNewDIBBits2;
	memcpy(lpNewDIBBits2, lpDIBBits, static_cast<size_t>(lWidth) * lHeight);

	// 设置Sobel模板参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 2;
	iTempMY = 2;
	aTemplate[0] = 1.0;
	aTemplate[1] = 2.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = -2.0;
	aTemplate[8] = -1.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits1, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// 设置Sobel模板参数
	aTemplate[0] = -1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = -2.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 2.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = 1.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth - 1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char*)lpNewDIBBits1 + lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char*)lpNewDIBBits2 + lWidth * j + i;

			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;

		}
	}

	// 复制经过模板运算后的图像到源图像
	memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits1);
	LocalFree(hNewDIBBits1);

	LocalUnlock(hNewDIBBits2);
	LocalFree(hNewDIBBits2);
	// 返回
	return TRUE;
}


//Sobel算子
void CMFCTest1View::OnEdgeSobel()
{
	// TODO: 在此添加命令处理程序代码

	//Sobel边缘检测运算

	// 获取文档
	CMFCTest1Doc* pDoc = GetDocument();

	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;

	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的边缘检测，其它的可以类推）
	if (pDoc->m_dib.GetBitCount(lpDIB) != 8)
	{
		// 提示用户
		MessageBox(L"目前只支持256色位图的运算！", L"系统提示", MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}

	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	long lpDIBWidth = pDoc->m_dib.GetWidth(lpDIB);
	long lpDIBHeight = pDoc->m_dib.GetHeight(lpDIB);

	// 调用SobelDIB()函数对DIB进行边缘检测
	if (SobelDIB(lpDIBBits, WIDTHBYTES(lpDIBWidth * 8), lpDIBHeight))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox(L"分配内存失败！", L"系统提示", MB_ICONINFORMATION | MB_OK);
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();

}

//添加椒盐噪声
BOOL WINAPI SaltNoiseDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	// 指向源图像的指针
	LPSTR lpSrc;

	// 循环变量
	long i;
	long j;

	// 图像每行的字节数
	LONG lLineBytes;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// 生成伪随机种子
	srand((unsigned)time(NULL));

	// 在图像中加噪
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lLineBytes; i++)
		{
			// 产生随机数决定是否加噪以及加哪种噪声
			int randomValue = rand();
			if (randomValue > 31500)
			{
				// 指向源图像倒数第j行，第i个像素的指针
				lpSrc = (char*)lpDIBBits + lLineBytes * j + i;

				if (randomValue % 2 == 0) {
					// 图像中当前点置为黑色
					*lpSrc = 0;
				}
				else {
					// 图像中当前点置为白色
					*lpSrc = 255;
				}
			}
		}
	}
	// 返回
	return true;
}


//椒盐噪声
void CMFCTest1View::OnRestoreSaltnoise()
{
	// TODO: 在此添加命令处理程序代码
	// 获取文档
	CMFCTest1Doc* pDoc = GetDocument();

	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;

	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的模糊操作，其它的可以类推）
	if (pDoc->m_dib.GetBitCount(lpDIB) != 8)
	{
		// 提示用户
		MessageBox(L"目前只支持256色位图的运算！", L"系统提示", MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}

	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	long lpDIBWidth = pDoc->m_dib.GetWidth(lpDIB);
	long lpDIBHeight = pDoc->m_dib.GetHeight(lpDIB);

	// 调用SaltNoiseDIB()函数对DIB进行加噪处理
	if (SaltNoiseDIB(lpDIBBits, lpDIBWidth, lpDIBHeight))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox(L"分配内存失败！",L"系统提示", MB_ICONINFORMATION | MB_OK);
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}

BOOL WINAPI ErosionDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int ForeMode, int structure[3][3])
{
	// 根据 ForeMode 设置前景和背景色
	BYTE foreground = (ForeMode == 0) ? 255 : 0;  // 白色或黑色前景
	BYTE background = (ForeMode == 0) ? 0 : 255;  // 反色

	// 创建缓存图像
	std::vector<BYTE> buffer(lWidth * lHeight, background);
	LPSTR lpNewDIBBits = reinterpret_cast<LPSTR>(buffer.data());

	// 根据不同的 nMode 设置结构元素和进行腐蚀处理
	switch (nMode)
	{
	case 0:  // 水平方向
		for (LONG j = 0; j < lHeight; j++) {
			for (LONG i = 1; i < lWidth - 1; i++) {
				BYTE left = *(lpDIBBits + lWidth * j + i - 1);
				BYTE middle = *(lpDIBBits + lWidth * j + i);
				BYTE right = *(lpDIBBits + lWidth * j + i + 1);

				if (left == foreground && middle == foreground && right == foreground)
					*(lpNewDIBBits + lWidth * j + i) = foreground;
			}
		}
		break;

	case 1:  // 垂直方向
		for (LONG i = 0; i < lWidth; i++) {
			for (LONG j = 1; j < lHeight - 1; j++) {
				BYTE top = *(lpDIBBits + lWidth * (j - 1) + i);
				BYTE middle = *(lpDIBBits + lWidth * j + i);
				BYTE bottom = *(lpDIBBits + lWidth * (j + 1) + i);

				if (top == foreground && middle == foreground && bottom == foreground)
					*(lpNewDIBBits + lWidth * j + i) = foreground;
			}
		}
		break;

	case 2:  // 自定义结构元素
		for (LONG j = 1; j < lHeight - 1; j++) {
			for (LONG i = 1; i < lWidth - 1; i++) {
				BOOL allForeground = TRUE;
				for (int m = -1; m <= 1 && allForeground; m++) {
					for (int n = -1; n <= 1 && allForeground; n++) {
						if (structure[m + 1][n + 1] == 0) {
							BYTE pixel = *(lpDIBBits + lWidth * (j + m) + (i + n));
							if (pixel != foreground) {
								allForeground = FALSE;
							}
						}
					}
				}
				*(lpNewDIBBits + lWidth * j + i) = allForeground ? foreground : background;
			}
		}
		break;
	}

	// 复制腐蚀后的图像回原始图像指针
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	return TRUE;
}

//膨胀
BOOL WINAPI DilationDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int ForeMode, int structure[3][3])
{
	// 根据 ForeMode 设置前景和背景色
	BYTE foreground = (ForeMode == 0) ? 255 : 0;  // 白色或黑色前景
	BYTE background = (ForeMode == 0) ? 0 : 255;  // 反色

	// 创建缓存图像
	std::vector<BYTE> buffer(lWidth * lHeight, background);
	LPSTR lpNewDIBBits = reinterpret_cast<LPSTR>(buffer.data());

	// 根据不同的 nMode 设置结构元素和进行膨胀处理
	switch (nMode)
	{
	case 0:  // 水平方向
		for (LONG j = 0; j < lHeight; j++) {
			for (LONG i = 1; i < lWidth - 1; i++) {
				BYTE left = *(lpDIBBits + lWidth * j + i - 1);
				BYTE middle = *(lpDIBBits + lWidth * j + i);
				BYTE right = *(lpDIBBits + lWidth * j + i + 1);

				if (left == foreground || middle == foreground || right == foreground)
					*(lpNewDIBBits + lWidth * j + i) = foreground;
			}
		}
		break;

	case 1:  // 垂直方向
		for (LONG i = 0; i < lWidth; i++) {
			for (LONG j = 1; j < lHeight - 1; j++) {
				BYTE top = *(lpDIBBits + lWidth * (j - 1) + i);
				BYTE middle = *(lpDIBBits + lWidth * j + i);
				BYTE bottom = *(lpDIBBits + lWidth * (j + 1) + i);

				if (top == foreground || middle == foreground || bottom == foreground)
					*(lpNewDIBBits + lWidth * j + i) = foreground;
			}
		}
		break;

	case 2:  // 自定义结构元素
		for (LONG j = 1; j < lHeight - 1; j++) {
			for (LONG i = 1; i < lWidth - 1; i++) {
				BOOL anyForeground = FALSE;
				for (int m = -1; m <= 1 && !anyForeground; m++) {
					for (int n = -1; n <= 1 && !anyForeground; n++) {
						if (structure[m + 1][n + 1] == 0) {
							BYTE pixel = *(lpDIBBits + lWidth * (j + m) + (i + n));
							if (pixel == foreground) {
								anyForeground = TRUE;
							}
						}
					}
				}
				*(lpNewDIBBits + lWidth * j + i) = anyForeground ? foreground : background;
			}
		}
		break;
	}

	// 复制膨胀后的图像回原始图像指针
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	return TRUE;
}

//开启
BOOL WINAPI OpeningDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int ForeMode, int structure[3][3])
{
	// 首先进行腐蚀处理
	if (!ErosionDIB(lpDIBBits, lWidth, lHeight, nMode, ForeMode, structure)) {
		return FALSE;  // 如果腐蚀失败，则直接返回失败
	}

	// 接着进行膨胀处理
	if (!DilationDIB(lpDIBBits, lWidth, lHeight, nMode, ForeMode, structure)) {
		return FALSE;  // 如果膨胀失败，也返回失败
	}

	return TRUE;  // 两个操作都成功执行，则返回成功
}

//闭合
BOOL WINAPI ClosingDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int ForeMode, int structure[3][3])
{
	// 首先进行膨胀处理
	if (!DilationDIB(lpDIBBits, lWidth, lHeight, nMode, ForeMode, structure)) {
		return FALSE;  // 如果膨胀失败，则直接返回失败
	}

	// 接着进行腐蚀处理
	if (!ErosionDIB(lpDIBBits, lWidth, lHeight, nMode, ForeMode, structure)) {
		return FALSE;  // 如果腐蚀失败，也返回失败
	}

	return TRUE;  // 两个操作都成功执行，则返回成功
}

//内边界
BOOL WINAPI InnerBoundaryDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int ForeMode, int structure[3][3])
{
	// 创建一个临时缓存以保存腐蚀后的图像
	std::vector<BYTE> buffer(lWidth * lHeight);
	memcpy(buffer.data(), lpDIBBits, lWidth * lHeight);  // 复制原图像到缓存

	// 对临时图像执行腐蚀操作
	if (!ErosionDIB(reinterpret_cast<LPSTR>(buffer.data()), lWidth, lHeight, nMode, ForeMode, structure)) {
		return FALSE;  // 如果腐蚀失败，则直接返回失败
	}

	// 原图像与腐蚀后的图像相减，提取内边界
	for (LONG i = 0; i < lHeight * lWidth; i++) {
		BYTE originalPixel = static_cast<BYTE>(lpDIBBits[i]);
		BYTE erodedPixel = static_cast<BYTE>(buffer[i]);
		lpDIBBits[i] = (originalPixel != erodedPixel) ? 255 : 0;  // 假设边界为白色，非边界为黑色
	}

	return TRUE;
}


//外边界
BOOL WINAPI OuterBoundaryDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int ForeMode, int structure[3][3])
{
	// 创建一个临时缓存以保存膨胀后的图像
	std::vector<BYTE> buffer(lWidth * lHeight);
	memcpy(buffer.data(), lpDIBBits, lWidth * lHeight);  // 复制原图像到缓存

	// 对临时图像执行膨胀操作
	if (!DilationDIB(reinterpret_cast<LPSTR>(buffer.data()), lWidth, lHeight, nMode, ForeMode, structure)) {
		return FALSE;  // 如果膨胀失败，则直接返回失败
	}

	// 原图像与膨胀后的图像相减，提取外边界
	for (LONG i = 0; i < lHeight * lWidth; i++) {
		BYTE originalPixel = static_cast<BYTE>(lpDIBBits[i]);
		BYTE dilatedPixel = static_cast<BYTE>(buffer[i]);
		lpDIBBits[i] = (originalPixel != dilatedPixel) ? 255 : 0;  // 假设边界为白色，非边界为黑色
	}

	return TRUE;
}


//形态学处理总菜单
void CMFCTest1View::OnMorphologyMainmenu()
{
	// TODO: 在此添加命令处理程序代码
	//获取文档
	CMFCTest1Doc* pDoc = GetDocument();

	//获取DIB指针
	LPSTR lpDIB = (LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHObject());

	//指向DIB图像象素的指针
	LPSTR lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	long lpDIBWidth = pDoc->m_dib.GetWidth(lpDIB);
	long lpDIBHeight = pDoc->m_dib.GetHeight(lpDIB);

	//判断是否是8-bip位图
	if (pDoc->m_dib.GetBitCount(lpDIB) != 8)
	{
		MessageBox(L"目前只支持256色位图的形态学处理！", L"系统提示", MB_ICONINFORMATION | MB_OK);
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
		return;
	}

	int nMode=-1;
	int morphType=-1;
	int ForeMode= -1;
	int structure[3][3]{ };
	//显示形态学处理对话框
	DlgMorphMenu dlgPara;

	if (dlgPara.DoModal() != IDOK)
	{
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
		return;
	}

	//获取对话框参数
	nMode = dlgPara.m_nMode;
	morphType = dlgPara.m_morphmenu;
	ForeMode = dlgPara.m_ForeMode;
	//debug显示nMode
	//CString str1;
	//str1.Format(L"%d", nMode);
	//MessageBox(str1);
	if (nMode == 2)
	{
		structure[0][0] = dlgPara.m_nStructure1;
		structure[0][1] = dlgPara.m_nStructure2;
		structure[0][2] = dlgPara.m_nStructure3;
		structure[1][0] = dlgPara.m_nStructure4;
		structure[1][1] = dlgPara.m_nStructure5;
		structure[1][2] = dlgPara.m_nStructure6;
		structure[2][0] = dlgPara.m_nStructure7;
		structure[2][1] = dlgPara.m_nStructure8;
		structure[2][2] = dlgPara.m_nStructure9;
		////debug提示框显示struct
		//CString str;
		//str.Format(L"%d %d %d\n%d %d %d\n%d %d %d", structure[0][0], structure[0][1], structure[0][2], structure[1][0], structure[1][1], structure[1][2], structure[2][0], structure[2][1], structure[2][2]);
		//MessageBox(str);
	}
	
	//根据用户选择的操作类型进行相应的操作
	// // 删除对话框
	delete dlgPara;
	// 更改光标形状
	BeginWaitCursor();
	//腐蚀
	if (morphType == 0) {
		
		//调用腐蚀函数
		if (ErosionDIB(lpDIBBits, lpDIBWidth, lpDIBHeight, nMode,ForeMode, structure))
		{
			pDoc->SetModifiedFlag(TRUE);
			pDoc->UpdateAllViews(NULL);
		}
		else
		{
			MessageBox(L"分配内存失败！", L"系统提示", MB_ICONINFORMATION | MB_OK);
		}

		
	}
	//膨胀
	else if (morphType == 1) {
		//调用膨胀函数
		if (DilationDIB(lpDIBBits, lpDIBWidth, lpDIBHeight, nMode, ForeMode, structure))
		{
			pDoc->SetModifiedFlag(TRUE);
			pDoc->UpdateAllViews(NULL);
		}
		else
		{
			MessageBox(L"分配内存失败！", L"系统提示", MB_ICONINFORMATION | MB_OK);
		}
	}
	//开启
	else if (morphType == 2) {
		//调用开启函数
		if (OpeningDIB(lpDIBBits, lpDIBWidth, lpDIBHeight, nMode, ForeMode, structure))
		{
			pDoc->SetModifiedFlag(TRUE);
			pDoc->UpdateAllViews(NULL);
		}
		else
		{
			MessageBox(L"分配内存失败！", L"系统提示", MB_ICONINFORMATION | MB_OK);
		}
	}
	//闭合
	else if (morphType == 3) {
		//调用闭合函数
		if (ClosingDIB(lpDIBBits, lpDIBWidth, lpDIBHeight, nMode, ForeMode, structure))
		{
			pDoc->SetModifiedFlag(TRUE);
			pDoc->UpdateAllViews(NULL);
		}
		else
		{
			MessageBox(L"分配内存失败！", L"系统提示", MB_ICONINFORMATION | MB_OK);
		}
	}
	//内边界
	else if (morphType == 4) {
		//调用内边界函数
		if (InnerBoundaryDIB(lpDIBBits, lpDIBWidth, lpDIBHeight, nMode, ForeMode, structure))
		{
			pDoc->SetModifiedFlag(TRUE);
			pDoc->UpdateAllViews(NULL);
		}
		else
		{
			MessageBox(L"分配内存失败！", L"系统提示", MB_ICONINFORMATION | MB_OK);
		}
	}
	//外边界
	else if (morphType == 5) {
		//调用外边界函数
		if (OuterBoundaryDIB(lpDIBBits, lpDIBWidth, lpDIBHeight, nMode, ForeMode, structure))
		{
			pDoc->SetModifiedFlag(TRUE);
			pDoc->UpdateAllViews(NULL);
		}
		else
		{
			MessageBox(L"分配内存失败！", L"系统提示", MB_ICONINFORMATION | MB_OK);
		}
	}


	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	//恢复光标
	EndWaitCursor();
}

