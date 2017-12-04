// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__9690BC7D_3001_4B1E_B512_564C6B13857E__INCLUDED_)
#define AFX_STDAFX_H__9690BC7D_3001_4B1E_B512_564C6B13857E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT



/*
     SIZE 为棋盘的长和宽
	 OFFSETX,OFFSETY 分别为棋盘边界距离差
	 WIDTH 为棋格宽度

	 WHITE_CHESS 表示 白棋
	 BLACK_CHESS 表示 黑棋
	 SPACE 表示空格，同时返回值也可表示未分出胜负
	 PEACE_CHESS 表示和棋

 */

#define SIZE 15
#define OFFSETX 7 
#define OFFSETY 5
#define WIDTH 35 


#define WHITE_CHESS 'W'
#define BLACK_CHESS 'B'
#define SPACE ' '
#define PEACE_CHESS '='


#define CLR_BLACK RGB(0,0,0)
#define CLR_WHITE RGB(255,255,255)
#define CLR_RED   RGB(255,0,0)

// 字体的背景色
#define CLR_BK_WHITE RGB (226,226,226)
#define CLR_BK_BLACK RGB (53,53,53)

//棋子的背景色
#define CLR_MAGENTA RGB(255,0,255)



#include <Winsock2.h>   //因为调用了 winsock 2.0 版本的函数



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__9690BC7D_3001_4B1E_B512_564C6B13857E__INCLUDED_)
