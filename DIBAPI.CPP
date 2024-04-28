// ************************************************************************
//  �ļ�����dibapi.cpp
//
//  DIB(Independent Bitmap) API�����⣺
//
//  PaintDIB()          - ����DIB����
//  CreateDIBPalette()  - ����DIB�����ɫ��
//  FindDIBBits()       - ����DIBͼ��������ʼλ��
//  DIBWidth()          - ����DIB����
//  DIBHeight()         - ����DIB�߶�
//  PaletteSize()       - ����DIB��ɫ���С
//  DIBNumColors()      - ����DIB��ɫ����ɫ��Ŀ
//  CopyHandle()        - �����ڴ��
//
//  SaveDIB()           - ��DIB���浽ָ���ļ���
//  ReadDIBFile()       - ��ָ���ļ��ж�ȡDIB����
//
//  DIBToPCX256()		- ��ָ����256ɫDIB���󱣴�Ϊ256ɫPCX�ļ�
//  ReadPCX256()		- ��ȡ256ɫPCX�ļ�
//
// ************************************************************************

#include "stdafx.h"
#include "dibapi.h"
#include <io.h>
#include <errno.h>

#include <math.h>
#include <direct.h>

/*
 * Dib�ļ�ͷ��־���ַ���"BM"��дDIBʱ�õ��ó�����
 */
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

/*************************************************************************
 *
 * �������ƣ�
 *   PaintDIB()
 *
 * ����:
 *   HDC hDC            - ����豸DC
 *   LPRECT lpDCRect    - ���ƾ�������
 *   HDIB hDIB          - ָ��DIB�����ָ��
 *   LPRECT lpDIBRect   - Ҫ�����DIB����
 *   CPalette* pPal     - ָ��DIB�����ɫ���ָ��
 *
 * ����ֵ:
 *   BOOL               - ���Ƴɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�����Ҫ��������DIB�������е�����StretchDIBits()����
 * SetDIBitsToDevice()������DIB����������豸���ɲ���hDCָ
 * �������Ƶľ��������ɲ���lpDCRectָ�������DIB�������ɲ���
 * lpDIBRectָ����
 *
 ************************************************************************/

BOOL WINAPI PaintDIB(HDC     hDC,
					LPRECT  lpDCRect,
					HDIB    hDIB,
					LPRECT  lpDIBRect,
					CPalette* pPal)
{
	LPSTR    lpDIBHdr;            // BITMAPINFOHEADERָ��
	LPSTR    lpDIBBits;           // DIB����ָ��
	BOOL     bSuccess=FALSE;      // �ɹ���־
	HPALETTE hPal=NULL;           // DIB��ɫ��
	HPALETTE hOldPal=NULL;        // ��ǰ�ĵ�ɫ��

	// �ж�DIB�����Ƿ�Ϊ��
	if (hDIB == NULL)
	{
		// ����
		return FALSE;
	}

	// ����DIB
	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIBHdr);

	// ��ȡDIB��ɫ�壬��ѡ����
	if (pPal != NULL)
	{
		hPal = (HPALETTE) pPal->m_hObject;

		// ѡ�е�ɫ��
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	}

	// ������ʾģʽ
	::SetStretchBltMode(hDC, COLORONCOLOR);

	// �ж��ǵ���StretchDIBits()����SetDIBitsToDevice()������DIB����
	if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDIBRect)) &&
	   (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
	{
		// ԭʼ��С���������졣
		bSuccess = ::SetDIBitsToDevice(hDC,                    // hDC
								   lpDCRect->left,             // DestX
								   lpDCRect->top,              // DestY
								   RECTWIDTH(lpDCRect),        // nDestWidth
								   RECTHEIGHT(lpDCRect),       // nDestHeight
								   lpDIBRect->left,            // SrcX
								   (int)DIBHeight(lpDIBHdr) -
									  lpDIBRect->top -
									  RECTHEIGHT(lpDIBRect),   // SrcY
								   0,                          // nStartScan
								   (WORD)DIBHeight(lpDIBHdr),  // nNumScans
								   lpDIBBits,                  // lpBits
								   (LPBITMAPINFO)lpDIBHdr,     // lpBitsInfo
								   DIB_RGB_COLORS);            // wUsage
	}
    else
	{
		// ��ԭʼ��С�����졣
		bSuccess = ::StretchDIBits(hDC,                          // hDC
							   lpDCRect->left,                 // DestX
							   lpDCRect->top,                  // DestY
							   RECTWIDTH(lpDCRect),            // nDestWidth
							   RECTHEIGHT(lpDCRect),           // nDestHeight
							   lpDIBRect->left,                // SrcX
							   lpDIBRect->top,                 // SrcY
							   RECTWIDTH(lpDIBRect),           // wSrcWidth
							   RECTHEIGHT(lpDIBRect),          // wSrcHeight
							   lpDIBBits,                      // lpBits
							   (LPBITMAPINFO)lpDIBHdr,         // lpBitsInfo
							   DIB_RGB_COLORS,                 // wUsage
							   SRCCOPY);                       // dwROP
	}
	
    // �������
	::GlobalUnlock((HGLOBAL) hDIB);
	
	// �ָ���ǰ�ĵ�ɫ��
	if (hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}
	
	// ����
	return bSuccess;
}

/*************************************************************************
 *
 * �������ƣ�
 *   CreateDIBPalette()
 *
 * ����:
 *   HDIB hDIB          - ָ��DIB�����ָ��
 *   CPalette* pPal     - ָ��DIB�����ɫ���ָ��
 *
 * ����ֵ:
 *   BOOL               - �����ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�������DIB����һ���߼���ɫ�壬��DIB�ж�ȡ��ɫ�����浽��ɫ���У�
 * ����ո��߼���ɫ�崴��һ���µĵ�ɫ�壬�����ظõ�ɫ��ľ��������
 * ��������õ���ɫ����ʾDIBͼ��
 *
 ************************************************************************/


BOOL WINAPI CreateDIBPalette(HDIB hDIB, CPalette* pPal)
{
	// ָ���߼���ɫ���ָ��
	LPLOGPALETTE lpPal;
	
	// �߼���ɫ��ľ��
	HANDLE hLogPal;
	
	// ��ɫ��ľ��
	HPALETTE hPal = NULL;
	
	// ѭ������
	int i;
	
	// ��ɫ���е���ɫ��Ŀ
	WORD wNumColors;
	
	// ָ��DIB��ָ��
	LPSTR lpbi;
	
	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFO lpbmi;
	
	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREINFO lpbmc;
	
	// �����Ƿ���Win3.0 DIB�ı��
	BOOL bWinStyleDIB;
	
	// �������
	BOOL bResult = FALSE;
	
	// �ж�DIB�Ƿ�Ϊ��
	if (hDIB == NULL)
	{
		// ����FALSE
		return FALSE;
	}
	
	// ����DIB
	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	
	// ��ȡָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	lpbmi = (LPBITMAPINFO)lpbi;
	
	// ��ȡָ��BITMAPCOREINFO�ṹ��ָ��
	lpbmc = (LPBITMAPCOREINFO)lpbi;
	
	// ��ȡDIB����ɫ���е���ɫ��Ŀ
	wNumColors = ::DIBNumColors(lpbi);
	
	if (wNumColors != 0)
	{
		// ����Ϊ�߼���ɫ���ڴ�
		hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE)
									+ sizeof(PALETTEENTRY)
									* wNumColors);
		
		// ����ڴ治�㣬�˳�
		if (hLogPal == 0)
		{
			// �������
			::GlobalUnlock((HGLOBAL) hDIB);
			
			// ����FALSE
			return FALSE;
		}
		
		lpPal = (LPLOGPALETTE) ::GlobalLock((HGLOBAL) hLogPal);
		
		// ���ð汾��
		lpPal->palVersion = PALVERSION;
		
		// ������ɫ��Ŀ
		lpPal->palNumEntries = (WORD)wNumColors;
		
		// �ж��Ƿ���WIN3.0��DIB
		bWinStyleDIB = IS_WIN30_DIB(lpbi);

		// ��ȡ��ɫ��
		for (i = 0; i < (int)wNumColors; i++)
		{
			if (bWinStyleDIB)
			{
				// ��ȡ��ɫ����
				lpPal->palPalEntry[i].peRed = lpbmi->bmiColors[i].rgbRed;
				
				// ��ȡ��ɫ����
				lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
				
				// ��ȡ��ɫ����
				lpPal->palPalEntry[i].peBlue = lpbmi->bmiColors[i].rgbBlue;
				
				// ����λ
				lpPal->palPalEntry[i].peFlags = 0;
			}
			else
			{
				// ��ȡ��ɫ����
				lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed;
				
				// ��ȡ��ɫ����
				lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen;
				
				// ��ȡ��ɫ����
				lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue;
				
				// ����λ
				lpPal->palPalEntry[i].peFlags = 0;
			}
		}
		
		// �����߼���ɫ�崴����ɫ�壬������ָ��
		bResult = pPal->CreatePalette(lpPal);
		
		// �������
		::GlobalUnlock((HGLOBAL) hLogPal);
		
		// �ͷ��߼���ɫ��
		::GlobalFree((HGLOBAL) hLogPal);
	}
	
	// �������
	::GlobalUnlock((HGLOBAL) hDIB);
	
	// ���ؽ��
	return bResult;
}

/*************************************************************************
 *
 * �������ƣ�
 *   FindDIBBits()
 *
 * ����:
 *   LPSTR lpbi         - ָ��DIB�����ָ��
 *
 * ����ֵ:
 *   LPSTR              - ָ��DIBͼ��������ʼλ��
 *
 * ˵��:
 *   �ú�������DIB��ͼ�����ص���ʼλ�ã�������ָ������ָ�롣
 *
 ************************************************************************/


LPSTR WINAPI FindDIBBits(LPSTR lpbi)
{
	return (lpbi + *(LPDWORD)lpbi + ::PaletteSize(lpbi));
}


/*************************************************************************
 *
 * �������ƣ�
 *   DIBWidth()
 *
 * ����:
 *   LPSTR lpbi         - ָ��DIB�����ָ��
 *
 * ����ֵ:
 *   DWORD              - DIB��ͼ��Ŀ���
 *
 * ˵��:
 *   �ú�������DIB��ͼ��Ŀ��ȡ�����Windows 3.0 DIB������BITMAPINFOHEADER
 * �е�biWidthֵ��������������BITMAPCOREHEADER�е�bcWidthֵ��
 *
 ************************************************************************/


DWORD WINAPI DIBWidth(LPSTR lpDIB)
{
	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFOHEADER lpbmi;
	
	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREHEADER lpbmc;

	// ��ȡָ��
	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	// ����DIB��ͼ��Ŀ���
	if (IS_WIN30_DIB(lpDIB))
	{
		// ����Windows 3.0 DIB������lpbmi->biWidth
		return lpbmi->biWidth;
	}
	else
	{
		// ����������ʽ��DIB������lpbmc->bcWidth
		return (DWORD)lpbmc->bcWidth;
	}
}


/*************************************************************************
 *
 * �������ƣ�
 *   DIBHeight()
 *
 * ����:
 *   LPSTR lpDIB        - ָ��DIB�����ָ��
 *
 * ����ֵ:
 *   DWORD              - DIB��ͼ��ĸ߶�
 *
 * ˵��:
 *   �ú�������DIB��ͼ��ĸ߶ȡ�����Windows 3.0 DIB������BITMAPINFOHEADER
 * �е�biHeightֵ��������������BITMAPCOREHEADER�е�bcHeightֵ��
 *
 ************************************************************************/


DWORD WINAPI DIBHeight(LPSTR lpDIB)
{
	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFOHEADER lpbmi;
	
	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREHEADER lpbmc;

	// ��ȡָ��
	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	// ����DIB��ͼ��Ŀ���
	if (IS_WIN30_DIB(lpDIB))
	{
		// ����Windows 3.0 DIB������lpbmi->biHeight
		return lpbmi->biHeight;
	}
	else
	{
		// ����������ʽ��DIB������lpbmc->bcHeight
		return (DWORD)lpbmc->bcHeight;
	}
}


/*************************************************************************
 *
 * �������ƣ�
 *   PaletteSize()
 *
 * ����:
 *   LPSTR lpbi         - ָ��DIB�����ָ��
 *
 * ����ֵ:
 *   WORD               - DIB�е�ɫ��Ĵ�С
 *
 * ˵��:
 *   �ú�������DIB�е�ɫ��Ĵ�С������Windows 3.0 DIB��������ɫ��Ŀ��
 * RGBQUAD�Ĵ�С����������������ɫ��Ŀ��RGBTRIPLE�Ĵ�С��
 *
 ************************************************************************/


WORD WINAPI PaletteSize(LPSTR lpbi)
{
	// ����DIB�е�ɫ��Ĵ�С
	if (IS_WIN30_DIB (lpbi))
	{
		//������ɫ��Ŀ��RGBQUAD�Ĵ�С
		return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBQUAD));
	}
	else
	{
		//������ɫ��Ŀ��RGBTRIPLE�Ĵ�С
		return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
	}
}


/*************************************************************************
 *
 * �������ƣ�
 *   DIBNumColors()
 *
 * ����:
 *   LPSTR lpbi         - ָ��DIB�����ָ��
 *
 * ����ֵ:
 *   WORD               - ���ص�ɫ������ɫ������
 *
 * ˵��:
 *   �ú�������DIB�е�ɫ�����ɫ�����������ڵ�ɫλͼ������2��
 * ����16ɫλͼ������16������256ɫλͼ������256���������ɫ
 * λͼ��24λ����û�е�ɫ�壬����0��
 *
 ************************************************************************/
WORD WINAPI DIBNumColors(LPSTR lpbi)
{
	WORD wBitCount;

	// ����Windows��DIB, ʵ����ɫ����Ŀ���Ա����ص�λ��Ҫ�١�
	// ��������������򷵻�һ�����Ƶ���ֵ��
	
	// �ж��Ƿ���WIN3.0 DIB
	if (IS_WIN30_DIB(lpbi))
	{
		DWORD dwClrUsed;
		
		// ��ȡdwClrUsedֵ
		dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
		
		if (dwClrUsed != 0)
		{
			// ���dwClrUsed��ʵ���õ�����ɫ������Ϊ0��ֱ�ӷ��ظ�ֵ
			return (WORD)dwClrUsed;
		}
	}

	// ��ȡ���ص�λ��
	if (IS_WIN30_DIB(lpbi))
	{
		// ��ȡbiBitCountֵ
		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	}
	else
	{
		// ��ȡbiBitCountֵ
		wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;
	}
	
	// �������ص�λ��������ɫ��Ŀ
	switch (wBitCount)
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
 *
 * �������ƣ�
 *   DIBBitCount()
 *
 * ����:
 *   LPSTR lpbi         - ָ��DIB�����ָ��
 *
 * ����ֵ:
 *   WORD               - ���ص�ɫ������ɫ������
 *
 * ˵��:
 *   �ú�������DIBBitCount��
 *
 ************************************************************************/
WORD WINAPI DIBBitCount(LPSTR lpbi)
{
	WORD wBitCount;

	// ��ȡ���ص�λ��
	if (IS_WIN30_DIB(lpbi))
	{
		// ��ȡbiBitCountֵ
		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	}
	else
	{
		// ��ȡbiBitCountֵ
		wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;
	}
	
	// ����wBitCount
	return wBitCount;
}

/*************************************************************************
 *
 * �������ƣ�
 *   CopyHandle()
 *
 * ����:
 *   HGLOBAL h          - Ҫ���Ƶ��ڴ�����
 *
 * ����ֵ:
 *   HGLOBAL            - ���ƺ�����ڴ�����
 *
 * ˵��:
 *   �ú�������ָ�����ڴ����򡣷��ظ��ƺ�����ڴ����򣬳���ʱ����0��
 *
 ************************************************************************/

HGLOBAL WINAPI CopyHandle (HGLOBAL h)
{
	if (h == NULL)
		return NULL;

	// ��ȡָ���ڴ������С
	DWORD dwLen = ::GlobalSize((HGLOBAL) h);
	
	// �������ڴ�ռ�
	HGLOBAL hCopy = ::GlobalAlloc(GHND, dwLen);
	
	// �жϷ����Ƿ�ɹ�
	if (hCopy != NULL)
	{
		// ����
		void* lpCopy = ::GlobalLock((HGLOBAL) hCopy);
		void* lp     = ::GlobalLock((HGLOBAL) h);
		
		// ����
		memcpy(lpCopy, lp, dwLen);
		
		// �������
		::GlobalUnlock(hCopy);
		::GlobalUnlock(h);
	}

	return hCopy;
}



/*************************************************************************
 *
 * �������ƣ�
 *   SaveDIB()
 *
 * ����:
 *   HDIB hDib          - Ҫ�����DIB
 *   CFile& file        - �����ļ�CFile
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE����CFileException
 *
 * ˵��:
 *   �ú�����ָ����DIB���󱣴浽ָ����CFile�С���CFile�ɵ��ó���򿪺͹رա�
 *
 *************************************************************************/


BOOL WINAPI SaveDIB(HDIB hDib, CFile& file)
{
	// Bitmap�ļ�ͷ
	BITMAPFILEHEADER bmfHdr;
	
	// ָ��BITMAPINFOHEADER��ָ��
	LPBITMAPINFOHEADER lpBI;
	
	// DIB��С
	DWORD dwDIBSize;

	if (hDib == NULL)
	{
		// ���DIBΪ�գ�����FALSE
		return FALSE;
	}

	// ��ȡBITMAPINFO�ṹ��������
	lpBI = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) hDib);
	
	if (lpBI == NULL)
	{
		// Ϊ�գ�����FALSE
		return FALSE;
	}
	
	// �ж��Ƿ���WIN3.0 DIB
	if (!IS_WIN30_DIB(lpBI))
	{
		// ��֧���������͵�DIB����
		
		// �������
		::GlobalUnlock((HGLOBAL) hDib);
		
		// ����FALSE
		return FALSE;
	}

	// ����ļ�ͷ

	// �ļ�����"BM"
	bmfHdr.bfType = DIB_HEADER_MARKER;

	// ����DIB��Сʱ����򵥵ķ����ǵ���GlobalSize()����������ȫ���ڴ��С��
	// ����DIB�����Ĵ�С�������Ƕ༸���ֽڡ���������Ҫ����һ��DIB����ʵ��С��
	
	// �ļ�ͷ��С����ɫ����С
	// ��BITMAPINFOHEADER��BITMAPCOREHEADER�ṹ�ĵ�һ��DWORD���Ǹýṹ�Ĵ�С��
	dwDIBSize = *(LPDWORD)lpBI + ::PaletteSize((LPSTR)lpBI);
	
	// ����ͼ���С
	if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4))
	{
		// ����RLEλͼ��û�������С��ֻ������biSizeImage�ڵ�ֵ
		dwDIBSize += lpBI->biSizeImage;
	}
	else
	{
		// ���صĴ�С
		DWORD dwBmBitsSize;

		// ��СΪWidth * Height
		dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;
		
		// �����DIB�����Ĵ�С
		dwDIBSize += dwBmBitsSize;

		// ����biSizeImage���ܶ�BMP�ļ�ͷ��biSizeImage��ֵ�Ǵ���ģ�
		lpBI->biSizeImage = dwBmBitsSize;
	}


	// �����ļ���С��DIB��С��BITMAPFILEHEADER�ṹ��С
	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	
	// ����������
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;

	// ����ƫ����bfOffBits�����Ĵ�СΪBitmap�ļ�ͷ��С��DIBͷ��С����ɫ����С
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize
											  + PaletteSize((LPSTR)lpBI);
	// ����д�ļ�
	TRY
	{
		// д�ļ�ͷ
		file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
		
		// дDIBͷ������
		file.WriteHuge(lpBI, dwDIBSize);
	}
	CATCH (CFileException, e)
	{
		// �������
		::GlobalUnlock((HGLOBAL) hDib);
		
		// �׳��쳣
		THROW_LAST();
	}
	END_CATCH
	
	// �������
	::GlobalUnlock((HGLOBAL) hDib);
	
	// ����TRUE
	return TRUE;
}


/*************************************************************************
 *
 * �������ƣ�
 *   ReadDIBFile()
 *
 * ����:
 *   CFile& file        - Ҫ��ȡ���ļ��ļ�CFile
 *
 * ����ֵ:
 *   HDIB               - �ɹ�����DIB�ľ�������򷵻�NULL��
 *
 * ˵��:
 *   �ú�����ָ�����ļ��е�DIB�������ָ�����ڴ������С���BITMAPFILEHEADER
 * ������ݶ����������ڴ档
 *
 *************************************************************************/

HDIB WINAPI ReadDIBFile(CFile& file)
{
	BITMAPFILEHEADER bmfHeader;
	DWORD dwBitsSize;
	HDIB hDIB;
	LPSTR pDIB;

	// ��ȡDIB���ļ������ȣ��ֽڣ�
	dwBitsSize = file.GetLength();

	// ���Զ�ȡDIB�ļ�ͷ
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
	{
		// ��С���ԣ�����NULL��
		return NULL;
	}

	// �ж��Ƿ���DIB���󣬼��ͷ�����ֽ��Ƿ���"BM"
	if (bmfHeader.bfType != DIB_HEADER_MARKER)
	{
		// ��DIB���󣬷���NULL��
		return NULL;
	}

	// ΪDIB�����ڴ�
	hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
	if (hDIB == 0)
	{
		// �ڴ����ʧ�ܣ�����NULL��
		return NULL;
	}
	
	// ����
	pDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	// ������
	if (file.ReadHuge(pDIB, dwBitsSize - sizeof(BITMAPFILEHEADER)) !=
		dwBitsSize - sizeof(BITMAPFILEHEADER) )
	{
		// ��С���ԡ�
		
		// �������
		::GlobalUnlock((HGLOBAL) hDIB);
		
		// �ͷ��ڴ�
		::GlobalFree((HGLOBAL) hDIB);
		
		// ����NULL��
		return NULL;
	}
	
	// �������
	::GlobalUnlock((HGLOBAL) hDIB);
	
	// ����DIB���
	return hDIB;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBToPCX256()
 *
 * ����:
 *   LPSTR lpDIB        - ָ��DIB�����ָ��
 *   CFile& file        - Ҫ������ļ�
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����True�����򷵻�False��
 *
 * ˵��:
 *   �ú�����ָ����256ɫDIB���󱣴�Ϊ256ɫPCX�ļ���
 *
 *************************************************************************/
BOOL WINAPI DIBToPCX256(LPSTR lpDIB, CFile& file)
{
	// ѭ������
	LONG	i;
	LONG	j;
	
	// DIB�߶�
	WORD	wHeight;
	
	// DIB����
	WORD	wWidth;
	
	// �м����
	BYTE	bChar1;
	BYTE	bChar2;
	
	// ָ��Դͼ�����ص�ָ��
	BYTE *	lpSrc;
	
	// ָ������ͼ�����ݵ�ָ��
	BYTE *	lpDst;
	
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	// �ظ����ؼ���
	int		iCount;
	
	// ��������ʹ�õ��ֽ���
	DWORD	dwBuffUsed;
	
	// ָ��DIB����ָ��
	LPSTR   lpDIBBits;
	
	// ��ȡDIB�߶�
	wHeight = (WORD) DIBHeight(lpDIB);
	
	// ��ȡDIB����
	wWidth  = (WORD) DIBWidth(lpDIB);
	
	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = FindDIBBits(lpDIB);
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(wWidth * 8);
	
	
	//*************************************************************************
	// PCX�ļ�ͷ
	PCXHEADER pcxHdr;
	
	// ���ļ�ͷ��ֵ
	
	// PCX��ʶ��
	pcxHdr.bManufacturer = 0x0A;
	
	// PCX�汾��
	pcxHdr.bVersion = 5;
	
	// PCX���뷽ʽ��1��ʾRLE���룩
	pcxHdr.bEncoding = 1;
	
	// ����λ����256ɫΪ8λ��
	pcxHdr.bBpp = 8;
	
	// ͼ���������Ļ�����Ͻ�X���꣨������Ϊ��λ��
	pcxHdr.wLeft = 0;
	
	// ͼ���������Ļ�����Ͻ�Y���꣨������Ϊ��λ��
	pcxHdr.wTop = 0;
	
	// ͼ���������Ļ�����½�X���꣨������Ϊ��λ��
	pcxHdr.wRight = wWidth - 1;
	
	// ͼ���������Ļ�����½�Y���꣨������Ϊ��λ��
	pcxHdr.wBottom = wHeight - 1;
	
	// ͼ���ˮƽ�ֱ���
	pcxHdr.wXResolution = wWidth;
	
	// ͼ��Ĵ�ֱ�ֱ���
	pcxHdr.wYResolution = wHeight;
	
	// ��ɫ�����ݣ�����256ɫPCX�����壬ֱ�Ӹ�ֵΪ0��
	for (i = 0; i < 48; i ++)
	{
		pcxHdr.bPalette[i] = 0;
	}
	
	// �������趨Ϊ0��
	pcxHdr.bReserved = 0;
	
	// ͼ��ɫ��ƽ����Ŀ������256ɫPCX�趨Ϊ1����
	pcxHdr.bPlanes = 1;
	
	// ͼ��Ŀ��ȣ��ֽ�Ϊ��λ��������Ϊż����
//	if ((wWidth & 1) == 0)
//	{
		pcxHdr.wLineBytes = wWidth;
//	}
//	else
//	{
//		pcxHdr.wLineBytes = wWidth + 1;
//	}
	
	// ͼ���ɫ������ͣ�1��ʾ��ɫ���ߵ�ɫͼ��2��ʾͼ���ǻҶ�ͼ��
	pcxHdr.wPaletteType = 1;
	
	// ������ͼ�����Ļ���ȣ�����Ϊ��λ��
	pcxHdr.wSrcWidth = 0;
	
	// ������ͼ�����Ļ�߶ȣ�����Ϊ��λ��
	pcxHdr.wSrcDepth = 0;
	
	// ������ȡֵ�趨Ϊ0��
	for (i = 0; i < 54; i ++)
	{
		pcxHdr.bFiller[i] = 0;
	}
	
	// д���ļ�ͷ
	file.Write((LPSTR)&pcxHdr, sizeof(PCXHEADER));
	
	//*******************************************************************************
	// ��ʼ����
	
	// ����һƬ������(2��ԭʼͼ���С)�Ա��������
	lpDst = new BYTE[wHeight * wWidth * 2];
	
	// ָ����ǰ�Ѿ����˶��ٻ��������ֽ�����
	dwBuffUsed = 0;
	
	// ÿ��
	for (i = 0; i < wHeight; i++)
	{
		// ָ��DIB��i�У���0�����ص�ָ��
		lpSrc = (BYTE *)lpDIBBits + lLineBytes * (wHeight - 1 - i);
		
		// ��bChar1��ֵ
		bChar1 = *lpSrc;
		
		// ����iCountΪ1
		iCount = 1;
		
		// ʣ����
		for (j = 1; j < wWidth; j ++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc++;
			
			// ��ȡ��һ������
			bChar2 = *lpSrc;
			
			// �ж��Ƿ��bChar1��ͬ����iCount < 63
			if ((bChar1 == bChar2) && (iCount < 63))
			{
				// ��ͬ��������1
				iCount ++;
				
				// ��������һ��
			}
			else
			{
				// ��ͬ������iCount = 63
				
				// д�뻺����
				if ((iCount > 1) || (bChar1 >= 0xC0))
				{
					// �����볤��Ϣ
					lpDst[dwBuffUsed] = iCount | 0xC0;
					
					// ����bChar1
					lpDst[dwBuffUsed + 1] = bChar1;
					
					// ����dwBuffUsed
					dwBuffUsed += 2;
				}
				else
				{
					// ֱ�ӱ����ֵ
					lpDst[dwBuffUsed] = bChar1;
					
					// ����dwBuffUsed
					dwBuffUsed ++;
				}
				
				// ���¸�bChar1��ֵ
				bChar1 = bChar2;
				
				// ����iCountΪ1
				iCount = 1;
			}
		}
		
		// ����ÿ�����һ���ֱ���
		if ((iCount > 1) || (bChar1 >= 0xC0))
		{
			// �����볤��Ϣ
			lpDst[dwBuffUsed] = iCount | 0xC0;
			
			// ����bChar1
			lpDst[dwBuffUsed + 1] = bChar1;
			
			// ����dwBuffUsed
			dwBuffUsed += 2;
		}
		else
		{
			// ֱ�ӱ����ֵ
			lpDst[dwBuffUsed] = bChar1;
			
			// ����dwBuffUsed
			dwBuffUsed ++;
		}
	}
	
	// д�������
	file.WriteHuge((LPSTR)lpDst, dwBuffUsed);
	
	// �ͷ��ڴ�
	delete lpDst;
	
	//**************************************************************************
	// д���ɫ����Ϣ
	
	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFO lpbmi;
	
	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREINFO lpbmc;
	
	// �����Ƿ���Win3.0 DIB�ı��
	BOOL bWinStyleDIB;
	
	// ����һƬ�������Ա����ɫ��
	lpDst = new BYTE[769];
	
	// ��ɫ����ʼ�ֽ�
	* lpDst = 0x0C;
	
	// ��ȡָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	lpbmi = (LPBITMAPINFO)lpDIB;
	
	// ��ȡָ��BITMAPCOREINFO�ṹ��ָ��
	lpbmc = (LPBITMAPCOREINFO)lpDIB;
	
	// �ж��Ƿ���WIN3.0��DIB
	bWinStyleDIB = IS_WIN30_DIB(lpDIB);
	
	// ��ȡ��ǰDIB��ɫ��
	for (i = 0; i < 256; i ++)
	{
		if (bWinStyleDIB)
		{
			// ��ȡDIB��ɫ���ɫ����
			lpDst[i * 3 + 1] = lpbmi->bmiColors[i].rgbRed;
			
			// ��ȡDIB��ɫ����ɫ����
			lpDst[i * 3 + 2] = lpbmi->bmiColors[i].rgbGreen;
			
			// ��ȡDIB��ɫ����ɫ����
			lpDst[i * 3 + 3] = lpbmi->bmiColors[i].rgbBlue;
		}
		else
		{
			// ��ȡDIB��ɫ���ɫ����
			lpDst[i * 3 + 1] = lpbmc->bmciColors[i].rgbtRed;
			
			// ��ȡDIB��ɫ����ɫ����
			lpDst[i * 3 + 2] = lpbmc->bmciColors[i].rgbtGreen;
			
			// ��ȡDIB��ɫ����ɫ����
			lpDst[i * 3 + 3] = lpbmc->bmciColors[i].rgbtBlue;
		}
	}
	
	// д���ɫ����Ϣ
	file.Write((LPSTR)lpDst, 769);
	
	// ����
	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   ReadPCX256()
 *
 * ����:
 *   CFile& file        - Ҫ��ȡ���ļ�
 *
 * ����ֵ:
 *   HDIB               - �ɹ�����DIB�ľ�������򷵻�NULL��
 *
 * ˵��:
 *   �ú�������ȡָ����256ɫPCX�ļ�������ȡ�Ľ��������һ��δѹ��
 * �����DIB�����С�
 *
 *************************************************************************/
HDIB WINAPI ReadPCX256(CFile& file)
{
	// PCX�ļ�ͷ
	PCXHEADER pcxHdr;
	
	// DIB��С���ֽ�����
	DWORD	dwDIBSize;
	
	// DIB���
	HDIB	hDIB;
	
	// DIBָ��
	LPSTR	pDIB;
	
	// ѭ������
	LONG	i;
	LONG	j;
	
	// �ظ����ؼ���
	int		iCount;
	
	// DIB�߶�
	WORD	wHeight;
	
	// DIB����
	WORD	wWidth;
	
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	// �м����
	BYTE	bChar;
	
	// ָ��Դͼ�����ص�ָ��
	BYTE *	lpSrc;
	
	// ָ������ͼ�����ݵ�ָ��
	BYTE *	lpDst;
	
	// ��ʱָ��
	BYTE *	lpTemp;
	
	// ���Զ�ȡPCX�ļ�ͷ
	if (file.Read((LPSTR)&pcxHdr, sizeof(PCXHEADER)) != sizeof(PCXHEADER))
	{
		// ��С���ԣ�����NULL��
		return NULL;
	}
	
	// �ж��Ƿ���256ɫPCX�ļ�������һ���ֽ��Ƿ���0x0A��
	if ((pcxHdr.bManufacturer != 0x0A) || (pcxHdr.bBpp != 8) || (pcxHdr.bPlanes != 1))
	{
		// ��256ɫPCX�ļ�������NULL��
		return NULL;
	}
	
	// ��ȡͼ��߶�
	wHeight = pcxHdr.wBottom - pcxHdr.wTop + 1;
	
	// ��ȡͼ�����
	wWidth  = pcxHdr.wRight - pcxHdr.wLeft + 1;
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(wWidth * 8);
	
	// ����DIB���ȣ��ֽڣ�
	dwDIBSize = sizeof(BITMAPINFOHEADER) + 1024 + wHeight * lLineBytes;
	
	// ΪDIB�����ڴ�
	hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwDIBSize);
	if (hDIB == 0)
	{
		// �ڴ����ʧ�ܣ�����NULL��
		return NULL;
	}
	
	// ����
	pDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	
	// ָ��BITMAPINFOHEADER��ָ��
	LPBITMAPINFOHEADER lpBI;
	
	// ��ֵ
	lpBI = (LPBITMAPINFOHEADER) pDIB;
	
	// ��lpBI��Ա��ֵ
	lpBI->biSize = 40;
	lpBI->biWidth = wWidth;
	lpBI->biHeight = wHeight;
	lpBI->biPlanes = 1;
	lpBI->biBitCount = 8;
	lpBI->biCompression = BI_RGB;
	lpBI->biSizeImage = wHeight * lLineBytes;
	lpBI->biXPelsPerMeter = pcxHdr.wXResolution;
	lpBI->biYPelsPerMeter = pcxHdr.wYResolution;
	lpBI->biClrUsed = 0;
	lpBI->biClrImportant = 0;
	
	// �����ڴ��Զ�ȡ����������
	lpSrc = new BYTE[file.GetLength() - sizeof(PCXHEADER) - 769];
	lpTemp = lpSrc;
	
	// ��ȡ����������
	if (file.ReadHuge(lpSrc, file.GetLength() - sizeof(PCXHEADER) - 769) !=
		file.GetLength() - sizeof(PCXHEADER) - 769 )
	{
		// ��С���ԡ�
		
		// �������
		::GlobalUnlock((HGLOBAL) hDIB);
		
		// �ͷ��ڴ�
		::GlobalFree((HGLOBAL) hDIB);
		
		// ����NULL��
		return NULL;
	}
	
	// ����DIB������λ��
	lpDst = (BYTE *) FindDIBBits(pDIB);
	
	// һ��һ�н���
	for (j = 0; j <wHeight; j++)
	{
		i = 0;
		while (i < wWidth)
		{
			// ��ȡһ���ֽ�
			bChar = *lpTemp;
			lpTemp++;
			
			if ((bChar & 0xC0) == 0xC0)
			{
				// �г�
				iCount = bChar & 0x3F;
				
				// ��ȡ��һ���ֽ�
				bChar = *lpTemp;
				lpTemp++;
				
				// bChar�ظ�iCount�α���
				memset(&lpDst[(wHeight - j - 1) * lLineBytes + i], bChar, iCount);

				// �Ѿ���ȡ���صĸ�����iCount
				i += iCount;
			}
			else
			{
				// ���浱ǰ�ֽ�
				lpDst[(wHeight - j - 1) * lLineBytes + i] = bChar;
				
				// �Ѿ���ȡ���صĸ�����1
				i += 1;
			}
		}
	}
	
	// �ͷ��ڴ�
	delete lpSrc;
	
	//*************************************************************
	// ��ɫ��
	
	// ����ɫ���־λ
	file.Read(&bChar, 1);
	if (bChar != 0x0C)
	{
		// ����
		
		// �������
		::GlobalUnlock((HGLOBAL) hDIB);
		
		// �ͷ��ڴ�
		::GlobalFree((HGLOBAL) hDIB);
		
		// ����NULL��
		return NULL;
	}
	
	// �����ڴ��Զ�ȡ����������
	lpSrc = new BYTE[768];
	
	// ����DIB�е�ɫ���λ��
	lpDst = (BYTE *) pDIB + sizeof(BITMAPINFOHEADER);
	
	// ��ȡ��ɫ��
	if (file.Read(lpSrc, 768) != 768)
	{
		// ��С���ԡ�
		
		// �������
		::GlobalUnlock((HGLOBAL) hDIB);
		
		// �ͷ��ڴ�
		::GlobalFree((HGLOBAL) hDIB);
		
		// ����NULL��
		return NULL;
	}
	
	// ����ɫ�帳ֵ
	for (i = 0; i < 256; i++)
	{
		lpDst[i * 4] = lpSrc[i * 3 + 2];
		lpDst[i * 4 + 1] = lpSrc[i * 3 + 1];
		lpDst[i * 4 + 2] = lpSrc[i * 3];
		lpDst[i * 4 + 3] = 0;
	}
	
	// �ͷ��ڴ�
	delete lpSrc;
	
	// �������
	::GlobalUnlock((HGLOBAL) hDIB);
	
	// ����DIB���
	return hDIB;
}