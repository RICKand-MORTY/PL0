#pragma once
#include "afxdialogex.h"


// DLG_INPUT 对话框

class DLG_INPUT : public CDialogEx
{
	DECLARE_DYNAMIC(DLG_INPUT)

public:
	CString m_EditContent; // 用于存储编辑控件内容的变量
	DLG_INPUT(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DLG_INPUT();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INPUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedInputOk();
};
