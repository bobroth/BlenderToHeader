
// BlenderToHeaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BlenderToHeader.h"
#include "BlenderToHeaderDlg.h"
#include "afxdialogex.h"

#include <iostream>
#include <fstream>
using namespace std;

#define	CR						_T("\x0d")
#define	CRLF					_T("\x0d\x0a")
#define	CRLFOLD					"\x0d\x0a"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef struct
{
	int nVerticies;
	int nPositions;
	int nTextures;
	int nNormals;
	int nFaces;
}MODEL, *LPMODEL;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBlenderToHeaderDlg dialog



CBlenderToHeaderDlg::CBlenderToHeaderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBlenderToHeaderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBlenderToHeaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBlenderToHeaderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CBlenderToHeaderDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CBlenderToHeaderDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &CBlenderToHeaderDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBlenderToHeaderDlg message handlers

BOOL CBlenderToHeaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// ************ REMOVE ****************
	SetDlgItemText(IDC_EDIT1, _T("D:\\Blender\\Projects\\Pink Link\\PinkLink.obj"));
	SetDlgItemText(IDC_EDIT2, _T("C:\\Users\\Robert\\workspace\\CubeG1\\jni\\Blend.h"));
	// ************************************
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBlenderToHeaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBlenderToHeaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBlenderToHeaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBlenderToHeaderDlg::OnBnClickedButton1() // Source File
{
		CFileDialog cFD(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Blender Output(*.obj)|*.obj|"), NULL, 0, TRUE);
		cFD.m_ofn.lpstrTitle = _T("Blender Object File (.obj)");
		int i = cFD.DoModal();
		if (i != IDCANCEL)
		{
			cSource = cFD.GetPathName();
			SetDlgItemText(IDC_EDIT1, cSource);
		}
}


void CBlenderToHeaderDlg::OnBnClickedButton2()
{
	CFolderPickerDialog cFP;
	cFP.m_ofn.lpstrTitle = _T("Destination");
	cFP.m_ofn.lpstrInitialDir = _T("C:\\");
	if (cFP.DoModal() == IDOK) {
		cDestination = cFP.GetPathName(); 
		cDestination += _T("\\Blend.h");
		SetDlgItemText(IDC_EDIT2, cDestination);
	}
}


void CBlenderToHeaderDlg::OnBnClickedOk()
{
	CStdioFile cSourceFile;
	CStdioFile cDestinationFile;
	int nVerticies = 0;
	int nNormals = 0;
	int nFaces = 0;
	int nTextures = 0;
	bool bFirstPass = true;
	int nPosition = 0;
	double d0, d1, d2;
	int n0, n1, n2;
	wchar_t wszOutput[128];

	bool bTest;
	CString cString;
	CString cS;
	CString &cLocalSource = cS;
	GetDlgItemText(IDC_EDIT1, cLocalSource);
	cSource = cLocalSource;
	CString &cLocalDestination = cS;
	GetDlgItemText(IDC_EDIT2, cLocalDestination);
	cDestination = cLocalDestination;

	bool bHappyEnding = false;
	if (!cSourceFile.Open(cSource, CFile::modeRead | CFile::shareDenyNone))
	{
		MessageBox(_T("Cannot Open Source File"), _T("Conversion Failed"), MB_OK);
		return;
	}
	// Get Counts
	MODEL stModel = { 0 };
	cSourceFile.Seek(0L, CFile::begin);
	while (cSourceFile.ReadString(cString))
	{
		if (cString.Left(2) == _T("v "))
			++stModel.nPositions;
		if (cString.Left(3) == _T("vn "))
			++stModel.nNormals;
		if (cString.Left(2) == _T("f "))
			++stModel.nFaces;
		if (cString.Left(3) == _T("vt "))
			++stModel.nTextures;
	}
	stModel.nVerticies = stModel.nFaces * 3;

	// Dynamically Allocate adPositions[][3];
	double **adPositions = (double**)malloc(stModel.nPositions * sizeof(double *) + (stModel.nPositions * (3 * sizeof(double))));
	size_t i;
	double *data = (double*)(adPositions + stModel.nPositions);
	for (i = 0; i < stModel.nPositions; i++)
		adPositions[i] = data + i * 3;
	

	if (cDestinationFile.Open(cDestination, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone))
	{
		cDestinationFile.WriteString(_T("// ********** BlenderToHeader **********\n"));
		cDestinationFile.WriteString(_T("//           copyright 2016 by\n"));
		cDestinationFile.WriteString(_T("//      Pink Leopard Software, Inc.\n"));
		cDestinationFile.WriteString(_T("//          all rights reserved\n\n"));


		cDestinationFile.WriteString(_T("// Globals\n"));
		cString.Format(_T("int nVerticies = %d;\n"), stModel.nVerticies);
		cDestinationFile.WriteString(cString);
		cString.Format(_T("int nNormals = %d;\n"), stModel.nNormals);
		cDestinationFile.WriteString(cString);
		cString.Format(_T("int nFaces = %d;\n"), stModel.nFaces);
		cDestinationFile.WriteString(cString);
		cString.Format(_T("int nTextures = %d;\n"), stModel.nTextures);
		cDestinationFile.WriteString(cString);
		cString.Format(_T("int nPositions = %d;\n\n"), stModel.nPositions);
		cDestinationFile.WriteString(cString);

		// In Verticies
		cDestinationFile.WriteString(_T("// Verticies\n"));
		cDestinationFile.WriteString(_T("GLfloat adVerticies[] = { "));

		cSourceFile.Seek(0L, CFile::begin);
		cSourceFile.ReadString(cString);

		while (cString.Left(2) != _T("v "))
		{
			bTest = cSourceFile.ReadString(cString);
			if (!bTest)
				goto SkipToNormals;
		}

		bFirstPass = true;
		do {
			if (cString.Left(2) != _T("v "))
			{
				break;
			}
			bFirstPass = false;
			cString = cString.Mid(2);

			adPositions[nPosition][0] = _wtof(cString);
			cString = cString.Mid(cString.Find(_T(" ")) + 1);
			adPositions[nPosition][1] = _wtof(cString);
			cString = cString.Mid(cString.Find(_T(" ")) + 1);
			adPositions[nPosition][2] = _wtof(cString);

			d0 = adPositions[nPosition][0];
			d1 = adPositions[nPosition][1];
			d2 = adPositions[nPosition][2];
			++nPosition;
		} while (cSourceFile.ReadString(cString));


		// Out Verticies
		bHappyEnding = false;
		cSourceFile.Seek(0L, CFile::begin);
		cSourceFile.ReadString(cString);
		while (cString.Left(2) != _T("f "))
		{
			bTest = cSourceFile.ReadString(cString);
			if (!bTest)
				goto SkipToNormals;
		}

		bFirstPass = true;
		do {
			if (cString.Left(2) != _T("f "))
			{
				cDestinationFile.WriteString(_T(" };\n\n"));
				bHappyEnding = true;
				break;
			}
			if (!bFirstPass)
				cDestinationFile.WriteString(_T(","));
			bFirstPass = false;
			cDestinationFile.WriteString(_T("\n"));
			cString = cString.Mid(2);
			cString.Replace(_T("//"), _T("/1/"));

			n0 = _wtoi(cString) - 1;
//			cString = cString.Mid(cString.Find(_T("/")) + 1);
//			n1 = _wtoi(cString) - 1;
//			cString = cString.Mid(cString.Find(_T("/")) + 1);
//			n2 = _wtoi(cString) - 1;
			cString = cString.Mid(cString.Find(_T(" ")) + 1);
//			swprintf_s(wszOutput, 128, _T("%0.6f, %0.6f, %0.6f,\n"), adPositions[n0][0], adPositions[n1][1], adPositions[n2][2]);
//			cDestinationFile.WriteString(wszOutput);

			n1 = _wtoi(cString) - 1;
//			cString = cString.Mid(cString.Find(_T("/")) + 1);
//			n1 = _wtoi(cString) - 1;
//			cString = cString.Mid(cString.Find(_T("/")) + 1);
//			n2 = _wtoi(cString) - 1;
			cString = cString.Mid(cString.Find(_T(" ")) + 1);
//			swprintf_s(wszOutput, 128, _T("%0.6f, %0.6f, %0.6f,\n"), adPositions[n0][0], adPositions[n1][1], adPositions[n2][2]);
//			cDestinationFile.WriteString(wszOutput);

			n2 = _wtoi(cString) - 1;
//			cString = cString.Mid(cString.Find(_T("/")) + 1);
//			n1 = _wtoi(cString) - 1;
//			cString = cString.Mid(cString.Find(_T("/")) + 1);
//			n2 = _wtoi(cString) - 1;
			swprintf_s(wszOutput, 128, _T("%0.6f, %0.6f, %0.6f,\n"), adPositions[n0][0], adPositions[n0][1], adPositions[n0][2]);
			cDestinationFile.WriteString(wszOutput);
			swprintf_s(wszOutput, 128, _T("%0.6f, %0.6f, %0.6f,\n"), adPositions[n1][0], adPositions[n1][1], adPositions[n1][2]);
			cDestinationFile.WriteString(wszOutput);
			swprintf_s(wszOutput, 128, _T("%0.6f, %0.6f, %0.6f"), adPositions[n2][0], adPositions[n2][1], adPositions[n2][2]);
			cDestinationFile.WriteString(wszOutput);

		} while (cSourceFile.ReadString(cString));
		if (!bHappyEnding)
		{
			cDestinationFile.WriteString(_T(" };\n\n"));
		}

		// Normals
SkipToNormals:
		bHappyEnding = false;
		cSourceFile.Seek(0L, CFile::begin);
		cSourceFile.ReadString(cString);
		while (cString.Left(3) != _T("vn "))
		{
			bTest = cSourceFile.ReadString(cString);
			if (!bTest)
				goto SkipToTextures;
		}

		cDestinationFile.WriteString(_T("// Normals\n"));
		cDestinationFile.WriteString(_T("GLfloat adNormals[] = { "));
		bFirstPass = true;
		do {
			if (cString.Left(3) != _T("vn "))
			{
				cDestinationFile.WriteString(_T(" };\n\n"));
				bHappyEnding = true;
				break;
			}
			if (!bFirstPass)
				cDestinationFile.WriteString(_T(","));
			bFirstPass = false;
			cDestinationFile.WriteString(_T("\n"));
			cString = cString.Mid(3);
			cString.Replace(_T(" "), _T(","));
			cDestinationFile.WriteString(cString);
			++nNormals;
		} while (cSourceFile.ReadString(cString));
		if (!bHappyEnding)
		{
			cDestinationFile.WriteString(_T(" };\n\n"));
		}

	
	SkipToTextures:
		// Textures
		bHappyEnding = false;
		cSourceFile.Seek(0L, CFile::begin);
		cSourceFile.ReadString(cString);
		while (cString.Left(3) != _T("vt "))
		{
			bTest = cSourceFile.ReadString(cString);
			if (!bTest)
				goto SkipToWrapUp;
		}

		cDestinationFile.WriteString(_T("// Textures\n"));
		cDestinationFile.WriteString(_T("GLfloat adTextures[][2] = { "));
		bFirstPass = true;
		do {
			if (cString.Left(3) != _T("vt "))
			{
				cDestinationFile.WriteString(_T(" };\n\n"));
				bHappyEnding = true;
				break;
			}
			if (!bFirstPass)
				cDestinationFile.WriteString(_T(","));
			bFirstPass = false;
			cDestinationFile.WriteString(_T("\n"));
			cString = cString.Mid(3);
			cString.Replace(_T(" "), _T(","));
			cDestinationFile.WriteString(cString);
			++nTextures;
		} while (cSourceFile.ReadString(cString));
		if (!bHappyEnding)
		{
			cDestinationFile.WriteString(_T(" };\n\n"));
		}
	}
	else {
		MessageBox(_T("Cannot Open Destination File"), _T("Conversion Failed"), MB_OK);
		return;
	}

SkipToWrapUp:
	cDestinationFile.Close();
	cSourceFile.Close();
	CDialogEx::OnOK();
}
