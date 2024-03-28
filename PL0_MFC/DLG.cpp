// DLG.cpp: 实现文件
//

#include "pch.h"
#include "PL0_MFC.h"
#include "afxdialogex.h"
#include "DLG.h"


// DLG 对话框

IMPLEMENT_DYNAMIC(DLG, CDialogEx)

DLG::DLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PL0_MFC_DIALOG, pParent)
{

}

DLG::~DLG()
{
}

void DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DLG, CDialogEx)
END_MESSAGE_MAP()


// DLG 消息处理程序
