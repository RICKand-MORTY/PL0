// DLG_INPUT.cpp: 实现文件
//

#include "pch.h"
#include "PL0_MFC.h"
#include "afxdialogex.h"
#include "DLG_INPUT.h"


// DLG_INPUT 对话框

IMPLEMENT_DYNAMIC(DLG_INPUT, CDialogEx)

DLG_INPUT::DLG_INPUT(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INPUT, pParent)
{

}

DLG_INPUT::~DLG_INPUT()
{
}

void DLG_INPUT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DLG_INPUT, CDialogEx)
	ON_BN_CLICKED(IDC_INPUT_OK, &DLG_INPUT::OnBnClickedInputOk)
END_MESSAGE_MAP()


// DLG_INPUT 消息处理程序


void DLG_INPUT::OnBnClickedInputOk()
{
	GetDlgItemText(IDC_INPUT_ED, m_EditContent); // 获取编辑控件的内容
	EndDialog(IDC_INPUT_OK); // 关闭对话框
}
