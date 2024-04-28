// MyDib.cpp: implementation of the CDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dib.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*
 * Dib�ļ�ͷ��־���ַ���"BM"��
 */
#define DIB_MARKER   ((WORD) ('M' << 8) | 'B')
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDib::CDib()
{
	InitMembers();
}

CDib::~CDib()
{
	ClearMemory();
}

/*************************************************************************
 * �������ƣ�DrawDib()
 * ����˵��:
 *   HDC hDC������豸DC
 *   LPRECT lpDCRect�����ƾ�������
 *   HGLOBAL hDIB ��DIB����ľ��
 *   LPRECT lpDIBRect ��DIB���������
 *   CPalette* pPal����ɫ���ָ��
 * ��������:BOOL
 * ��������:�ú�����Ҫ��������DIB����   
 ************************************************************************/

BOOL CDib::DrawDib(HDC     hDC,
					LPRECT  lpDCRect,
					HGLOBAL    hDIB,
					LPRECT  lpDIBRect,
					CPalette* pPal)
{
	BOOL     bSuccess=FALSE;      // �ػ��ɹ���־
	HPALETTE hOldPal=NULL;        // ��ǰ�ĵ�ɫ��	
	if (hDIB == NULL)// �ж��Ƿ�����Ч��DIB����
	{		
		return FALSE;// ���ǣ��򷵻�
	}	
	lpdib  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);// ����DIB	
	lpDIBBits = GetBits(lpdib);// �ҵ�DIBͼ��������ʼλ��	
	if (pPal != NULL)// ��ȡDIB��ɫ�壬��ѡȡ���豸��������
	{
		HPALETTE hPal = (HPALETTE) pPal->m_hObject;	
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	}	
	::SetStretchBltMode(hDC, COLORONCOLOR);// ������ʾģʽ		
	bSuccess = ::StretchDIBits(hDC,								// �豸�������
							   lpDCRect->left,                 //Ŀ��X����
							   lpDCRect->top,                  // Ŀ��Y����
							   GetRectWidth(lpDCRect),            // Ŀ�����
							   GetRectHeight(lpDCRect),           // Ŀ��߶�
							   lpDIBRect->left,                // ԴX����
							   lpDIBRect->top,                 // ԴY����
							   GetRectWidth(lpDIBRect),           // Դ����
							   GetRectHeight(lpDIBRect),          // Դ�߶�
							   lpDIBBits,                      // ָ��dib���ص�ָ��
							   (LPBITMAPINFO)lpdib,         // ָ��λͼ��Ϣ�ṹ��ָ��
							   DIB_RGB_COLORS,                 // ʹ�õ���ɫ��Ŀ
							   SRCCOPY);                       // ��դ��������	
	::GlobalUnlock(hDIB);	// �������
	if (hOldPal != NULL)// �ָ�ϵͳ��ɫ��
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}	
	return bSuccess;
}

/*************************************************************************
 * �������ƣ�ConstructPalette(HGLOBAL hDIB, CPalette* pPal)
 * ��������:
 *   HGLOBAL hDIB��DIB����ľ��
 *   CPalette* pPal����ɫ���ָ��
 * ��������:BOOL
 * ����˵��:�ú�������DIB����һ���߼���ɫ��  
 ************************************************************************/

BOOL CDib::ConstructPalette(HGLOBAL hDIB, CPalette* pPal)
{	
	
	HANDLE hLogPal;// �߼���ɫ��ľ��	
	int iLoop;// ѭ������	
	BOOL bSuccess = FALSE;// �������	
	if (hDIB == NULL)//�ж��Ƿ�����Ч��DIB����
	{		
		return FALSE;// ����FALSE
	}	
	lpdib = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);// ����DIB
	lpbminfo= (LPBITMAPINFO)lpdib;			
	long wNumColors =GetColorNum(lpdib);// ��ȡDIB����ɫ���е���ɫ��Ŀ	
	if (wNumColors != 0)
	{		
		hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE)// ����Ϊ�߼���ɫ���ڴ�
									+ sizeof(PALETTEENTRY)
									* wNumColors);				
		if (hLogPal == 0)// ���ʧ�����˳�
		{			
			::GlobalUnlock((HGLOBAL) hDIB);// �������			
			return FALSE;
		}		
		LPLOGPALETTE lpPal = (LPLOGPALETTE) ::GlobalLock((HGLOBAL) hLogPal);		
		
		lpPal->palVersion = PalVersion;// ���õ�ɫ��汾��				
		lpPal->palNumEntries = (WORD)wNumColors;// ������ɫ��Ŀ		
		for (iLoop=0; iLoop<(int)wNumColors;iLoop++)// ��ȡ��ɫ��
		{			
			lpPal->palPalEntry[iLoop].peRed =lpbminfo->bmiColors[iLoop].rgbRed;// ��ȡ��ԭɫ����						
			lpPal->palPalEntry[iLoop].peGreen =lpbminfo->bmiColors[iLoop].rgbGreen;							
			lpPal->palPalEntry[iLoop].peBlue =lpbminfo->bmiColors[iLoop].rgbBlue;								
			lpPal->palPalEntry[iLoop].peFlags =0;// ����λ			
		}		
		bSuccess=pPal->CreatePalette(lpPal);// �����߼���ɫ�崴����ɫ�壬������ָ��			
		::GlobalUnlock((HGLOBAL) hLogPal);// �������				
		::GlobalFree((HGLOBAL) hLogPal);// �ͷ��߼���ɫ��
	}		
	::GlobalUnlock((HGLOBAL) hDIB);// �������		
	return bSuccess;// ���ؽ��
}

/*************************************************************************
 * �������ƣ�GetBits(LPSTR lpdib)
 * ��������:
 *   LPSTR lpdib��ָ��DIB�����ָ��
 * ��������:LPSTR 
 * ��������:����DIB���ص���ʼλ�ã�������ָ������ָ��
 ************************************************************************/

LPSTR CDib::GetBits(LPSTR lpdib)
{
	return (lpdib + ((LPBITMAPINFOHEADER)lpdib)->biSize+GetPalSize(lpdib));
//	return (lpdib + *(LPDWORD)lpdib+GetPalSize(lpdib));
}

/*************************************************************************
 * �������ƣ�GetWidth(LPSTR lpdib)
 * ��������:
 *   LPSTR lpdib��ָ��DIB�����ָ��
 * ��������:DWORD
 * ��������:�ú�������DIB��ͼ��Ŀ���
 ************************************************************************/

DWORD CDib::GetWidth(LPSTR lpdib)
{
	return ((LPBITMAPINFOHEADER)lpdib)->biWidth;//����DIB����
}


/*************************************************************************
 * �������ƣ�GetHeight(LPSTR lpdib)
 * ��������:
 *   LPSTR lpdib ��ָ��DIB�����ָ��
 * ��������:DWORD
 * ��������:�ú�������DIB��ͼ��ĸ߶�
 ************************************************************************/


DWORD CDib::GetHeight(LPSTR lpdib)
{
	return ((LPBITMAPINFOHEADER)lpdib)->biHeight;//����DIB�߶�

}
/*************************************************************************
 * �������ƣ�GetPalSize(LPSTR lpdib)
 * ��������:
 *   LPSTR lpdib��ָ��DIB�����ָ��
 * ��������:WORD
 * ��������:�ú�������DIB�е�ɫ��Ĵ�С
 ************************************************************************/

WORD CDib::GetPalSize(LPSTR lpdib)
{	
	return (WORD)(GetColorNum(lpdib) * sizeof(RGBQUAD));// ����DIB�е�ɫ��Ĵ�С
}

/*************************************************************************
 * �������ƣ�GetColorNum(LPSTR lpdib)
 * ��������:
 *   LPSTR lpdib��ָ��DIB�����ָ��
 * ��������:WORD
 * ��������:�ú�������DIB�е�ɫ�����ɫ������
 ************************************************************************/
WORD CDib::GetColorNum(LPSTR lpdib)
{				
		long dwClrUsed = ((LPBITMAPINFOHEADER)lpdib)->biClrUsed;	// ��ȡdwClrUsedֵ	
		if (dwClrUsed != 0)
		{			
			return (WORD)dwClrUsed;// ���dwClrUsed��Ϊ0��ֱ�ӷ��ظ�ֵ
		}		
		WORD wBitCount = ((LPBITMAPINFOHEADER)lpdib)->biBitCount;// ��ȡbiBitCountֵ		
	switch (wBitCount)// �������ص�λ��������ɫ��Ŀ
	{
		case 1:
			return 2;

		case 4:
			return 16;

		case 8:
			return 256;

		default:
			return 0;
	}
}

/*************************************************************************
 * �������ƣ�GetBitCount(LPSTR lpdib)
 * ��������:
 *   LPSTR lpdib��ָ��DIB�����ָ��
 * ��������:WORD 
 * ��������:�ú�������DIBBitCount
 ************************************************************************/
WORD CDib::GetBitCount(LPSTR lpdib)
{
	return ((LPBITMAPINFOHEADER)lpdib)->biBitCount;// ����λ��
}

/*************************************************************************
 * �������ƣ�CopyObject (HGLOBAL hGlob)
 * ��������:
 *   HGLOBAL hGlob ��Ҫ���Ƶ��ڴ�����
 * ��������:HGLOBAL
 * ��������:�ú�������ָ�����ڴ�����
 ************************************************************************/

HGLOBAL CDib::CopyObject (HGLOBAL hGlob)
{
	if (hGlob== NULL)
		return NULL;	
	DWORD dwLen = ::GlobalSize((HGLOBAL)hGlob);// ��ȡָ���ڴ������С		
	HGLOBAL hTemp = ::GlobalAlloc(GHND, dwLen);// �������ڴ�ռ�		
	if (hTemp!= NULL)// �жϷ����Ƿ�ɹ�
	{	
		void* lpTemp = ::GlobalLock((HGLOBAL)hTemp);// ����
		void* lp     = ::GlobalLock((HGLOBAL) hGlob);				
		memcpy(lpTemp, lp, dwLen);// ����		
		::GlobalUnlock(hTemp);// �������
		::GlobalUnlock(hGlob);
	}
	return hTemp;
}

/*************************************************************************
 * �������ƣ�SaveFile(HGLOBAL hDib, CFile& file)
 * ��������:
 *   HGLOBAL hDib��Ҫ�����DIB
 *   CFile& file�������ļ�CFile
 * ��������:BOOL
 * ��������:��ָ����DIB���󱣴浽ָ����CFile��
 *************************************************************************/

BOOL CDib::SaveFile(HGLOBAL hDib, CFile& file)
{
	if (hDib == NULL)
	{		
		return FALSE;// ���DIBΪ�գ�����FALSE
	}	
	lpbmihrd = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) hDib);// ��ȡBITMAPINFO�ṹ��������	
	if (lpbmihrd == NULL)
	{		
		return FALSE;// Ϊ�գ�����FALSE
	}		
	bmfHeader.bfType = DIB_MARKER;	// ����ļ�ͷ	
	dwDIBSize = *(LPDWORD)lpbmihrd + GetPalSize((LPSTR)lpbmihrd);// �ļ�ͷ��С����ɫ����С			
	DWORD dwBmBitsSize;// ���صĴ�С		
	dwBmBitsSize =GetReqByteWidth((lpbmihrd->biWidth)*((DWORD)lpbmihrd->biBitCount)) * lpbmihrd->biHeight;// ��СΪWidth * Height		
	dwDIBSize += dwBmBitsSize;// �����DIBÿ���ֽ���Ϊ4�ı���ʱ�Ĵ�С
	lpbmihrd->biSizeImage = dwBmBitsSize;// ����biSizeImage	
	bmfHeader.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);// �ļ���С		
	bmfHeader.bfReserved1 = 0;// ����������
	bmfHeader.bfReserved2 = 0;
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpbmihrd->biSize// ����ƫ����bfOffBits
											  + GetPalSize((LPSTR)lpbmihrd);
	TRY
	{		
		file.Write(&bmfHeader, sizeof(BITMAPFILEHEADER));// д�ļ�ͷ				
		file.Write(lpbmihrd, dwDIBSize);// дDIBͷ������
	}
	CATCH (CFileException, e)
	{		
		::GlobalUnlock((HGLOBAL) hDib);// �������				
		THROW_LAST();// �׳��쳣
	}
	END_CATCH		
	::GlobalUnlock((HGLOBAL) hDib);// �������	
	return TRUE;// ����TRUE
}


/*************************************************************************
 * �������ƣ�LoadFile(CFile& file)
 * ��������:
 *   CFile& file��Ҫ��ȡ���ļ��ļ�CFile
 * ��������:HGLOBAL
 * ��������:��ָ�����ļ��е�DIB�������ָ�����ڴ�������
 *************************************************************************/

HGLOBAL CDib::LoadFile(CFile& file)
{

	DWORD dwFileSize;
	
	dwFileSize= file.GetLength();// ��ȡ�ļ���С	
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))// ��ȡDIB�ļ�ͷ
	{		
		return NULL;// ��С��һ�£�����NULL��
	}
	if (bmfHeader.bfType != DIB_MARKER)// �ж��Ƿ���DIB����
	{		
		return NULL;// ��������򷵻�NULL��
	}	
	m_hDib= (HGLOBAL) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwFileSize-sizeof(BITMAPFILEHEADER));// ����DIB�ڴ�
	if (m_hDib== 0)
	{		
		return NULL;// ����ʧ�ܣ�����NULL
	}
	/////////////////////////////////////////////////////////////////////////
	//��CDib��ĳ�Ա������ֵ	
	lpdib = (LPSTR) ::GlobalLock((HGLOBAL) m_hDib);// ����
	lpbminfo=(BITMAPINFO*)lpdib;
	lpbmihrd=(BITMAPINFOHEADER*)lpdib;
	lpRgbQuag=(RGBQUAD*)(lpdib+lpbmihrd->biSize);
	int m_numberOfColors =GetColorNum((LPSTR)lpbmihrd);
	if (lpbmihrd->biClrUsed == 0)
       lpbmihrd->biClrUsed =m_numberOfColors;
	DWORD colorTableSize = m_numberOfColors *sizeof(RGBQUAD);
	lpDIBBits=lpdib+lpbmihrd->biSize+colorTableSize;
	/////////////////////////////////////////////////////////////////////////
	
	if (file.Read(lpdib, dwFileSize - sizeof(BITMAPFILEHEADER)) !=// ������
		dwFileSize - sizeof(BITMAPFILEHEADER) )//��С��һ��
	{					
		::GlobalUnlock((HGLOBAL) m_hDib);	// �������			
		::GlobalFree((HGLOBAL) m_hDib);	// �ͷ��ڴ�
		return NULL;
	}		
	::GlobalUnlock((HGLOBAL) m_hDib);// �������		
	return m_hDib;// ����DIB���
}

/*************************************************************************
 * �������ƣ�GetReqByteWidth(int bits)
 * ��������:
 *   int bits��λ��
 * ��������:int
 * ��������:��ȡ��Ҫ�����ֽ�����ӦΪ4�ı���
 *************************************************************************/
int  CDib::GetReqByteWidth(int bits)
{
	int getBytes=(bits + 31) / 32 * 4;
	return  getBytes;
}

/*************************************************************************
 * �������ƣ�GetRectWidth(LPCRECT lpRect)
 * ��������:
 *   LPCRECT lpRect��ָ����������ָ��
 * ��������:long
 * ��������:��ȡ��������Ŀ���
 *************************************************************************/

long  CDib::GetRectWidth(LPCRECT lpRect)
{
	long nWidth=lpRect->right - lpRect->left;
	return nWidth;
}
/*************************************************************************
 * �������ƣ�GetRectHeight(LPCRECT lpRect)
 * ��������:
 *   LPCRECT lpRect��ָ����������ָ��
 * ��������:long
 * ��������:��ȡ��������ĸ߶�
 *************************************************************************/
long  CDib::GetRectHeight(LPCRECT lpRect)
{
	long nHeight=lpRect->bottom - lpRect->top;
	return nHeight;
}

/*************************************************************************
 * �������ƣ�InitMembers() 
 * ��������: void
 * ��������:��ʼ����ĳ�Ա����
 *************************************************************************/
void CDib::InitMembers()
{
	m_hDib=NULL;
	lpbmihrd=NULL;	// ָ��BITMAPINFO�ṹ��ָ��
	lpdib=NULL;
	lpDIBBits=NULL;
	dwDIBSize=0;
	lpRgbQuag=NULL;
}

/*************************************************************************
 * �������ƣ�ClearMemory() 
 * ��������: void
 * ��������:��λ��ĳ�Ա����
 *************************************************************************/
void CDib::ClearMemory()
{
	if(m_hDib!=NULL)
		::GlobalFree(m_hDib);
	lpbmihrd=NULL;	// ָ��BITMAPINFO�ṹ��ָ��
	lpdib=NULL;
	dwDIBSize=0;
	lpRgbQuag=NULL;	
	dwDIBSize=0;
}