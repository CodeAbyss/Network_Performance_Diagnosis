// Netprobe1Dlg.cpp : implementation file
//
////////////////////////////////////////////////////

#include "stdafx.h"
#include "Netprobe1.h"
#include "Netprobe1Dlg.h"
#include "host.h"
#include "ES_Network.h"
#include "ES_UDP.h"
#include "ES_TCP.h"
#include "ES_TCPClient.h"
#include "ES_TCPServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool file=false;
bool recvfile=false;
LPCTSTR recvfname = "";
bool check = false;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetprobe1Dlg dialog

CNetprobe1Dlg::CNetprobe1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNetprobe1Dlg::IDD, pParent),UDM_WINSOCK_SELECT(WM_USER+999)
{
	//{{AFX_DATA_INIT(CNetprobe1Dlg)
	m_Size = 1024;
	m_Refresh = 100;
	m_PeerPort = 17733;
	m_Peer = _T("localhost");
	m_PacketNo = 0;
	m_LocalPort = 17733;
	m_Local = _T("localhost");
	m_Rate = 0.0;
	m_BufferNo = 5;
	m_filepath = _T("");
	m_recvfpath = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
}

void CNetprobe1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetprobe1Dlg)
	DDX_Control(pDX, IDC_File, m_FileTextC);
	DDX_Control(pDX, IDC_CHECK1, m_FileC);
	DDX_Control(pDX, IDC_RecvFileS, m_RecvFileS);
	DDX_Control(pDX, IDC_SendFileS, m_SendFileS);
	DDX_Control(pDX, IDC_RecvFile, m_RecvFileC);
	DDX_Control(pDX, IDC_SendFile, m_SendFileC);
	DDX_Control(pDX, IDC_filepath2, m_recvfpathC);
	DDX_Control(pDX, IDC_BufferNo, m_BufferNoC);
	DDX_Control(pDX, IDC_Sync2, m_RecvBlock);
	DDX_Control(pDX, IDC_Sync, m_SendBlock);
	DDX_Control(pDX, IDC_Peer, m_PeerC);
	DDX_Control(pDX, IDC_Size, m_SizeC);
	DDX_Control(pDX, IDC_Refresh, m_RefreshC);
	DDX_Control(pDX, IDC_Peerport, m_PeerPortC);
	DDX_Control(pDX, IDC_PacketNo, m_PacketNoC);
	DDX_Control(pDX, IDC_Localport, m_LocalPortC);
	DDX_Control(pDX, IDC_InputRate, m_RateC);
	DDX_Control(pDX, IDC_Local, m_LocalC);
	DDX_Control(pDX, IDCANCEL, m_ButtonQuit);
	DDX_Control(pDX, IDC_HostInfo, m_ButtonHostInfo);
	DDX_Control(pDX, IDC_Blocking, m_IOMode);
	DDX_Control(pDX, IDC_Async, m_Async);
	DDX_Control(pDX, IDC_UDP, m_UDP);
	DDX_Control(pDX, IDC_Loss, m_Loss);
	DDX_Control(pDX, IDC_PacketLost, m_PacketLoss);
	DDX_Control(pDX, IDC_PacketTansferred, m_PacketTransferred);
	DDX_Control(pDX, IDC_Receive, m_ButtonRecv);
	DDX_Control(pDX, IDC_Send, m_ButtonSend);
	DDX_Control(pDX, IDC_TCP, m_Protocol);
	DDX_Control(pDX, IDC_TimeElapsed, m_TimeElapsed);
	DDX_Control(pDX, IDC_DataRate, m_DataRate);
	DDX_Text(pDX, IDC_Size, m_Size);
	DDX_Text(pDX, IDC_Refresh, m_Refresh);
	DDX_Text(pDX, IDC_Peerport, m_PeerPort);
	DDX_Text(pDX, IDC_Peer, m_Peer);
	DDX_Text(pDX, IDC_PacketNo, m_PacketNo);
	DDX_Text(pDX, IDC_Localport, m_LocalPort);
	DDX_Text(pDX, IDC_Local, m_Local);
	DDX_Text(pDX, IDC_InputRate, m_Rate);	
	DDX_Control(pDX, IDC_Alert, m_Alert);
	DDX_Text(pDX, IDC_BufferNo, m_BufferNo);
	DDX_Text(pDX, IDC_filepath, m_filepath);
	DDX_Text(pDX, IDC_filepath2, m_recvfpath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNetprobe1Dlg, CDialog)
	//{{AFX_MSG_MAP(CNetprobe1Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_HostInfo, OnHostInfo)
	ON_BN_CLICKED(IDC_Send, OnSend)
	ON_BN_CLICKED(IDC_Receive, OnReceive)
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_SendFile, OnSendFile)
	ON_BN_CLICKED(IDC_RecvFile, OnRecvFile)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_DOUBLECLICKED(IDC_CHECK1, OnDoubleclickedCheck1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetprobe1Dlg message handlers

BOOL CNetprobe1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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

	OnCheck1();
	m_Protocol.SetCheck(1);
	m_SendBlock.SetCheck(1);
	m_RecvBlock.SetCheck(1);
	m_Local = "localhost";
	m_LocalPort = 8000;
	m_Peer = "localhost";
	m_PeerPort = 8000;
	m_Rate = 10240;
	m_Refresh = 10;
	m_Size = 1024;
	UpdateData(false);

	QuitFlag = TRUE;

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNetprobe1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNetprobe1Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNetprobe1Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


////////////////////////////////
//	Send Thread Procedure
///////////////////////////////
DWORD WINAPI CNetprobe1Dlg::ThreadSendProc(LPVOID IpInstance)
{	
	CNetprobe1Dlg *pclass = (CNetprobe1Dlg *) IpInstance;

	pclass->RunSendMode(pclass);
	pclass->m_ButtonSend.SetWindowText("Send");
		
	CloseHandle(pclass->hThread1);
	return 0;
}

int CNetprobe1Dlg::StartDelayThread()
{
	DWORD ThreadID;
	hThread4 = CreateThread(0, 
						   0, 
						   CNetprobe1Dlg::ThreadDelayProc,
						   (LPVOID)this, 
						   0, 
						   &ThreadID);

	if (hThread4 == NULL)
			MessageBox("Error starting new thread!", "Netprobe Error", MB_OK);
	return 0;
}

DWORD WINAPI CNetprobe1Dlg::ThreadDelayProc(LPVOID IpInstance)
{
	CNetprobe1Dlg *pclass = (CNetprobe1Dlg *) IpInstance;

	pclass->OKrecv = false;

	Sleep(1000);

//	pclass->OKrecv = true;

	pclass->OnClose();
	AfxMessageBox("Close Connection by Sender");

	DWORD dwExitCode=0;

	TerminateThread(pclass->hThread4,dwExitCode);
	CloseHandle(pclass->hThread4);

	return 0;
}

/////////////////////////////////////////////////////////////////////////
//	Send Thread: for sending packet and updating the sender statistic	 
/////////////////////////////////////////////////////////////////////////
int CNetprobe1Dlg::StartThreadedSend()
{
	DWORD ThreadID;
	hThread1 = CreateThread(0, 
						   0, 
						   CNetprobe1Dlg::ThreadSendProc,
						   (LPVOID)this, 
						   0, 
						   &ThreadID);

	if (hThread1 == NULL)
			MessageBox("Error starting new thread!", "Netprobe Error", MB_OK);
	return 0;
}

////////////////////////////////////////////////////////
//		STOP Send Thread
////////////////////////////////////////////////////////
int CNetprobe1Dlg::StopThreadedSend()
{
	DWORD dwExitCode=0;

	TerminateThread(hThread1,dwExitCode);
	CloseHandle(hThread1);

//	if (file)
//	{	
//		m_filepath="";
//		UpdateData(false);
//	}
	
	return 0;
}

///////////////////////////////////////////////////////////////
//	When the user click the "send" button, run this function
///////////////////////////////////////////////////////////////
void CNetprobe1Dlg::OnSend() 
{
	char temp[100];
	m_ButtonSend.GetWindowText(temp, 100);
	 
	if (strcmp(temp, "Send") == 0)
	{			
		if (!file)
		{
			char temp[20];
			SetDlgItemText(IDC_filepath,"");
			sprintf(temp,"0 Bytes");
			m_SendFileS.SetWindowText(temp);
		}
		
		if (UpdateData(true) && InitVariable(0))
		{	
			if ((lPktSize < 17)&&(file))
			{
				char temp[10];
				sprintf(temp, "%d", lPktSize);
				MessageBox("Please input the Packet Size \r\nmore than 16 for file sending/receiving.", "Netprobe Error", MB_ICONEXCLAMATION);
				return;
			}


			if (m_IOMode.GetCheck())	// M-thread				
			{	
				SimpleBlockingIO = false;
				m_ButtonSend.SetWindowText("Stop Send");	
				m_ButtonRecv.EnableWindow(false);
				m_ButtonHostInfo.EnableWindow(false);
				m_ButtonQuit.EnableWindow(false);
				m_SendBlock.EnableWindow(false);
				m_RecvBlock.EnableWindow(false);
				m_Alert.EnableWindow(false);
				Hold();
			
				m_DataRate.SetWindowText("0 Bps");
				m_Loss.SetWindowText("0 %");
				m_PacketTransferred.SetWindowText("0");		
				m_PacketLoss.SetWindowText("0");	
				m_TimeElapsed.SetWindowText("0 s");
				m_RecvFileS.SetWindowText("0 Bytes");

				if (!file)
				{
					m_filepath == "";
					UpdateData(false);
				}

				StartThreadedSend();			
			}
			else	// Blocking
			{			
				m_ButtonSend.SetWindowText("Stop Send");	
				m_ButtonRecv.EnableWindow(false);
				m_ButtonHostInfo.EnableWindow(false);
				m_ButtonQuit.EnableWindow(false);
				m_SendBlock.EnableWindow(false);
				m_RecvBlock.EnableWindow(false);
				m_Alert.EnableWindow(false);
				Hold();
			
				m_DataRate.SetWindowText("0 Bps");
				m_Loss.SetWindowText("0 %");
				m_PacketTransferred.SetWindowText("0");		
				m_PacketLoss.SetWindowText("0");	
				m_TimeElapsed.SetWindowText("0 s");
				m_RecvFileS.SetWindowText("0 Bytes");
			
				SimpleBlockingIO = true;

				if (!file)
				{
					m_filepath == "";
					UpdateData(false);
				}

				RunSendMode(this);
			}
		
		}
	}
	else
	{	
	//	if (!SimpleBlockingIO)
	//		StopThreadedSend();
		
		lNumToSend = 0;
		bSendForever = false;
		send_complete = true;
	
		if (file)
		{
			f->Close();
			delete f;
///			m_filepath = "";
//			UpdateData(false);
			file = false;
			m_SendFileS.SetWindowText("0 Bytes");
//			m_RecvFileS.SetWindowText("0 Bytes");
		}

		m_ButtonSend.SetWindowText("Send");
		m_ButtonRecv.EnableWindow(true);	
		m_ButtonHostInfo.EnableWindow(true);
		m_ButtonQuit.EnableWindow(true);
		m_SendBlock.EnableWindow(true);
		m_RecvBlock.EnableWindow(true);
		m_Alert.EnableWindow(true);				
		NotHold();
	}
}


/////////////////////////////////////////////////////////////////
//	Receiver Stat Thread: for updating receiver statistic
///////////////////////////////////////////////////////////////
int CNetprobe1Dlg::RecvStatThread()
{
	DWORD ThreadID;
	hThread2 = CreateThread(0, 
					 	    0, 
						    CNetprobe1Dlg::RecvStatProc,
						    (LPVOID)this, 
						    0, 
						    &ThreadID);

	if (hThread2 == NULL)
			MessageBox("Error starting new thread!", "Netprobe Error", MB_OK);
	return 0;
}

////////////////////////////////////////////////////////
//		STOP Receiver Stat Thread
////////////////////////////////////////////////////////
int CNetprobe1Dlg::StopThreadRecvStat()
{
	DWORD dwExitCode=0;

	TerminateThread(hThread2,dwExitCode);
	CloseHandle(hThread2);

	m_ButtonSend.EnableWindow(true);
	m_ButtonHostInfo.EnableWindow(true);
	m_ButtonQuit.EnableWindow(true);

	return 0;
}

////////////////////////////////////////////////////////
//		Receiver Stat Thread Procedure
////////////////////////////////////////////////////////
DWORD WINAPI CNetprobe1Dlg::RecvStatProc(LPVOID IpInstance)
{
	CNetprobe1Dlg *recvstat = (CNetprobe1Dlg *) IpInstance;
	DWORD dwExitCode=0;

	if (recvstat->aStart)
	{
		char outrate[20];
		double lossrate = 0;
		char temp[20];				

		while (recvstat->OKrecv)
		{

			recvstat->ConvertRate(outrate, (double)(recvstat->apacketrecv)*recvstat->lPktSize*1000/recvstat->RecvTimer.Elapsed());
			
			if (recvstat->atotalpacket != 0)
				lossrate = (double) (100 - (100*recvstat->apacketrecv)/(recvstat->atotalpacket+1));
			if (lossrate <0)
				lossrate =0;

			sprintf(temp, "%d", recvstat->atotalpacket+1);
			recvstat->m_PacketTransferred.SetWindowText(temp);
			recvstat->m_DataRate.SetWindowText(outrate);

			sprintf(temp, "%.1f s", recvstat->RecvTimer.Elapsed()/1000);
			recvstat->m_TimeElapsed.SetWindowText(temp);

			sprintf(temp, "%.0f %%", lossrate);
			recvstat->m_Loss.SetWindowText(temp);

			unsigned long Loss = recvstat->atotalpacket+1-recvstat->apacketrecv;
			
			sprintf(temp, "%d", Loss);				
			recvstat->m_PacketLoss.SetWindowText(temp);

			if (recvfile)
			{
				double size;		
				
				size = (recvstat->recvfilesize-(recvstat->atotalpacket+1)*8)/1048576;
				if (size > 1)
					sprintf(temp, "%.2f MBytes", size);	// mega Bytes
				else
				{
					size = (recvstat->recvfilesize-(recvstat->atotalpacket+1)*8)/1024;
					if (size > 1)
						sprintf(temp, "%.2f kBytes", size);	// kilo Bytes
					else
						sprintf(temp, "%.2f Bytes", (recvstat->recvfilesize-(recvstat->atotalpacket+1)*8));	// Bytes
				}
			
				recvstat->m_RecvFileS.SetWindowText(temp);
			}

			recvstat->DisplayTimer.Start();
			Sleep(recvstat->iRefreshInterval);
		}
	
	}
	else
	{
		char outrate[20];
		char recvtemp[20];
		unsigned int packetno=0, losspacket=0;
		double lossrate;

		while (recvstat->OKrecv)
		{	
			packetno = ((strcmp(recvstat->iProtocol, "UDP") == 0) ? recvstat->pUDP->GetTotalPacket() : recvstat->pTCPServer->GetTotalPacket());
			lossrate = ((strcmp(recvstat->iProtocol, "UDP") == 0) ? recvstat->pUDP->GetLossRate() : recvstat->pTCPServer->GetLossRate());
			losspacket = ((strcmp(recvstat->iProtocol, "UDP") == 0) ? recvstat->pUDP->GetLossPacket() : recvstat->pTCPServer->GetLossPacket());
			
			if (losspacket <0)
			{
				losspacket =0;
				lossrate =0;
			}

			recvstat->ConvertRate(outrate, (double)(packetno-losspacket)*recvstat->lPktSize*1000/recvstat->RecvTimer.Elapsed());
					
			sprintf(recvtemp, "%d", packetno);
			recvstat->m_PacketTransferred.SetWindowText(recvtemp);		
			recvstat->m_DataRate.SetWindowText(outrate);

			sprintf(recvtemp, "%.1f s", recvstat->RecvTimer.Elapsed()/1000.0);
			recvstat->m_TimeElapsed.SetWindowText(recvtemp);
			
			sprintf(recvtemp, "%.0f %%", lossrate);
			recvstat->m_Loss.SetWindowText(recvtemp);
			
			sprintf(recvtemp, "%d", losspacket);
			recvstat->m_PacketLoss.SetWindowText(recvtemp);

			if (recvfile)
			{
				double size;		
				
				if (strcmp(recvstat->iProtocol, "UDP") == 0)
					size = (recvstat->recvfilesize-(packetno)*8)/1048576;
				else
					size = (recvstat->recvfilesize-(packetno+1)*8)/1048576;

				if (size > 1)
					sprintf(recvtemp, "%.2f MBytes", size);	// mega Bytes
				else
				{
					if (strcmp(recvstat->iProtocol, "UDP") == 0)
						size = (recvstat->recvfilesize-(packetno)*8)/1024;
					else
						size = (recvstat->recvfilesize-(packetno+1)*8)/1024;

					if (size > 1)
						sprintf(recvtemp, "%.2f kBytes", size);	// kilo Bytes
					else
					{
						if (strcmp(recvstat->iProtocol, "UDP") == 0)
							sprintf(recvtemp, "%.2f Bytes", (recvstat->recvfilesize-(packetno)*8));	// Bytes
						else
							sprintf(recvtemp, "%.2f Bytes", (recvstat->recvfilesize-(packetno+1)*8));	// Bytes
					}
				}
			
				recvstat->m_RecvFileS.SetWindowText(recvtemp);
			}
				
			recvstat->DisplayTimer.Start();
			Sleep(recvstat->iRefreshInterval);
		}
	}

	TerminateThread(recvstat->hThread2,dwExitCode);
	CloseHandle(recvstat->hThread2);

	return 0;
}

////////////////////////////////////////////////////////
//		Send packet and Update stat in Send Thread
////////////////////////////////////////////////////////
int CNetprobe1Dlg::RunSendMode(CNetprobe1Dlg* pclass)
{	
	double iSendDelay = 0;
	unsigned int nBytesRead=0; 

	if (pclass->lRate > 0)
		iSendDelay = pclass->lPktSize*1000/pclass->lRate; // iSendDelay is in milliseconds.

	ES_UDP *pUDP = 0;
	ES_TCPClient *pTCP = 0;
	
	if (strcmp(pclass->iProtocol, "UDP") == 0)
	{
		pUDP = new ES_UDP();

		if (!pUDP->Startup(0, pclass->usPort, pclass->hostname, recvfile, recvfname))
		{
			m_ButtonRecv.EnableWindow(true);
			m_ButtonHostInfo.EnableWindow(true);
			m_ButtonQuit.EnableWindow(true);
			NotHold();
		
			return -1;
		}
	}
	else
	{
		pTCP = new ES_TCPClient;	

		if (pTCP->Connect(pclass->usPort, pclass->hostname) < 0)
		{			
			m_ButtonRecv.EnableWindow(true);
			m_ButtonHostInfo.EnableWindow(true);
			m_ButtonQuit.EnableWindow(true);
			m_ButtonSend.SetWindowText("Send");
			NotHold();

			return -1;
		}
	} 
	m_ButtonRecv.EnableWindow(false);
	m_ButtonHostInfo.EnableWindow(false);
	m_ButtonQuit.EnableWindow(false);
	Hold();

	send_complete = false;

	ES_FlashTimer SendTimer, DisplayTimer;

	unsigned long ulSeqNum = 0;
	char sendtemp[20];
	char outrate[20];

	if (file)
	{
		CString pFileName = m_filepath;
		f = new CFile((LPCTSTR)pFileName, CFile::modeRead);
	}

	while ((!sendbreak)&&((pclass->lNumToSend > ulSeqNum) || (pclass->bSendForever)))
	{
		char *pData = new char[pclass->lPktSize];
		memset(pData, 0, pclass->lPktSize); // Clear data to all zero.

		*((unsigned long *) pData) = ulSeqNum; // Fill in the seq no.
		char* p = pData;
		p += sizeof(unsigned long);
		*((int *) p) = pclass->lPktSize; 

		if (file)
		{
			nBytesRead = f->Read( p+sizeof(int), lPktSize-sizeof(unsigned long)-sizeof(int));
		
			if (nBytesRead<=0)
			{
				m_ButtonHostInfo.EnableWindow(true);
				m_ButtonRecv.EnableWindow(true);
				m_ButtonQuit.EnableWindow(true);
				m_ButtonSend.EnableWindow(true);
				NotHold();
				f->Close();
			
				break;
			}
		}

		if (strcmp(pclass->iProtocol, "UDP") == 0)
		{
			if (file)
			{
				if (pUDP->Send(pData, nBytesRead+sizeof(unsigned long)+sizeof(int)) < 0)
				{
					MessageBox("Send Mode: ES_TCP::Send() failed.", "Netprobe Error", MB_ICONEXCLAMATION);					
					m_ButtonSend.SetWindowText("Send");
					m_ButtonRecv.EnableWindow(true);
					m_ButtonHostInfo.EnableWindow(true);
					m_ButtonQuit.EnableWindow(true);
					NotHold();
					f->Close();

					return -1;
				}
			}
			else if (pUDP->Send(pData, pclass->lPktSize) < 0)
			{
				MessageBox("Send Mode: ES_UDP::Send() failed.", "Netprobe Error", MB_ICONEXCLAMATION);				
				m_ButtonSend.SetWindowText("Send");
				m_ButtonRecv.EnableWindow(true);
				m_ButtonHostInfo.EnableWindow(true);
				m_ButtonQuit.EnableWindow(true);

				NotHold();

				return -1;
			}
		}
		else
		{ // TCP
			if (file)
			{
				if (pTCP->Send(pData, nBytesRead+sizeof(unsigned long)+sizeof(int)) < 0)
				{
					MessageBox("Send Mode: ES_TCP::Send() failed.", "Netprobe Error", MB_ICONEXCLAMATION);					
					m_ButtonRecv.EnableWindow(true);
					m_ButtonHostInfo.EnableWindow(true);
					m_ButtonQuit.EnableWindow(true);
					m_ButtonSend.SetWindowText("Send");
					NotHold();

					f->Close();

					return -1;
				}
			}
			else if (pTCP->Send(pData, pclass->lPktSize) < 0)
			{
				MessageBox("Send Mode: ES_TCP::Send() failed.", "Netprobe Error", MB_ICONEXCLAMATION);					
				m_ButtonRecv.EnableWindow(true);
				m_ButtonHostInfo.EnableWindow(true);
				m_ButtonQuit.EnableWindow(true);
				m_ButtonSend.SetWindowText("Send");
				NotHold();

				return -1;
			}
		}
		
		++ulSeqNum;

		if (DisplayTimer.Elapsed() >= pclass->iRefreshInterval)
		{		
			if ((ulSeqNum)*pclass->lPktSize*1000/SendTimer.Elapsed()<0)
				ConvertRate(outrate,0);
			else
				ConvertRate(outrate, (double)(ulSeqNum)*pclass->lPktSize*1000/SendTimer.Elapsed());

			sprintf(sendtemp, "%d", ulSeqNum);
			pclass->m_PacketTransferred.SetWindowText(sendtemp);			
			pclass->m_DataRate.SetWindowText(outrate);

			sprintf(sendtemp, "%.1f s", SendTimer.Elapsed()/1000.0);
			pclass->m_TimeElapsed.SetWindowText(sendtemp);
			
			DisplayTimer.Start();
		}

		// if send time is longer than refresh interval, we need to refresh
		while ((!sendbreak) && (iSendDelay*ulSeqNum > (unsigned long)SendTimer.Elapsed()))
		{
			MSG msg;
			if (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				AfxGetApp()-> PumpMessage( );
			}
			LONG lIdle = 0;
			while (AfxGetApp()-> OnIdle(lIdle++)) 
			{					
				ConvertRate(outrate, (double)(ulSeqNum)*pclass->lPktSize*1000/SendTimer.Elapsed());

				sprintf(sendtemp, "%d", ulSeqNum);
				pclass->m_PacketTransferred.SetWindowText(sendtemp);			
				pclass->m_DataRate.SetWindowText(outrate);

				sprintf(sendtemp, "%.1f s", SendTimer.Elapsed()/1000.0);
				pclass->m_TimeElapsed.SetWindowText(sendtemp);
				
				DisplayTimer.Start();
				Sleep(pclass->iRefreshInterval);
				
				if (send_complete)
					sendbreak =true;
			};
		}

	//	++ulSeqNum;
		delete [] pData;
	}

	if (strcmp(iProtocol, "TCP") == 0)
	{
		pTCP->Disconnect();
		delete pTCP;
	}
	else // UDP
		delete pUDP;

	m_ButtonHostInfo.EnableWindow(true);
	m_ButtonRecv.EnableWindow(true);
	m_ButtonQuit.EnableWindow(true);
	m_ButtonSend.SetWindowText("Send");
	NotHold();	
	
	file = false;		

	return 0;	
}

////////////////////////////////////////////////////////
//		 
////////////////////////////////////////////////////////
void CNetprobe1Dlg::OnReceive() 
{
	char temp[100];
	m_ButtonRecv.GetWindowText(temp, 100);
	if (strcmp(temp, "Receive") == 0)
	{			
		recvfilesize = 0;

		if (UpdateData(true) && InitVariable(1))
		{	
			if ((recvfile)&&(lPktSize < 17))
			{			
				char temp[10];
				sprintf(temp, "%d", lPktSize);
				MessageBox("Please input the Packet Size \r\nmore than 16 for file sending/receiving.", "Netprobe Error", MB_ICONEXCLAMATION);
				
				return;
			}

			if (m_RecvBlock.GetCheck())		// Blocking
			{
				SimpleBlockingIO = true;	
				
				if (!recvfile)
				{
					m_recvfpath = "";
					UpdateData(false);
				}

				RunRecvMode();
			}
			else if (m_Async.GetCheck())		// Msg-Drvn
			{
				SimpleBlockingIO = false;			
				stoprecv = false;

				if (!recvfile)
				{
					m_recvfpath = "";
					UpdateData(false);
				}

				RunRecvMode();				
			}
			else		// Alert
			{	
				m_ButtonSend.EnableWindow(false);
				m_ButtonHostInfo.EnableWindow(false);
				m_ButtonQuit.EnableWindow(false);
				Hold();

				m_DataRate.SetWindowText("0 Bps");
				m_Loss.SetWindowText("0 %");
				m_PacketTransferred.SetWindowText("0");		
				m_PacketLoss.SetWindowText("0");	
				m_TimeElapsed.SetWindowText("0 s");
				m_SendFileS.SetWindowText("0 Bytes");
				m_RecvFileS.SetWindowText("0 Bytes");

				if (!recvfile)
				{
					m_recvfpath = "";
					UpdateData(false);
				}
		
				if (recvfile)
					f1 = new CFile(m_recvfpath, CFile::modeCreate | CFile::modeWrite);

				StartAlertableRecv();
			}
		}		
	}
	else
	{
		
		m_ButtonRecv.SetWindowText("Receive");		
		OKrecv = false;

		if ((m_RecvBlock.GetCheck())||(m_Async.GetCheck()))
			OnClose();
		else
			StopAlertableRecv();

		m_ButtonSend.EnableWindow(true);
		m_ButtonHostInfo.EnableWindow(true);
		m_ButtonQuit.EnableWindow(true);		
		NotHold();
	}
}

/////////////////////////////////////////////////////////////
//	Recevie procedure
/////////////////////////////////////////////////////////////
int CNetprobe1Dlg::RunRecvMode()
{
	char outrate[20];
	char recvtemp[20];
	int packetno, losspacket;
	double lossrate;

	if (strcmp(iProtocol, "UDP") == 0)
	{
		pUDP = new ES_UDP();
		recvfilesize = 0;
		if (!pUDP->Startup(1, usPort, hostname,recvfile,recvfname))
		{			
			recvfile = false;	
			m_ButtonSend.EnableWindow(true);
			return -1;
		}
	}
	else
	{
		pTCPServer = new ES_TCPServer();
		if (!pTCPServer->Startup(usPort, hostname,recvfile,recvfname))
		{
			recvfile = false;	
			m_ButtonSend.EnableWindow(true);			
			return -1;
		}
	}

	m_ButtonSend.EnableWindow(false);
	m_ButtonHostInfo.EnableWindow(false);
	m_ButtonQuit.EnableWindow(false);
	Hold();

	m_DataRate.SetWindowText("0 Bps");
	m_Loss.SetWindowText("0 %");
	m_PacketTransferred.SetWindowText("0");		
	m_PacketLoss.SetWindowText("0");	
	m_TimeElapsed.SetWindowText("0 s");
	m_SendFileS.SetWindowText("0 Bytes");
	m_RecvFileS.SetWindowText("0 Bytes");
	
//	RecvTimer.Start();
	DisplayTimer.Start();

	bIsRunning = true;	// indicate that the receiving process is running
	
	m_ButtonRecv.SetWindowText("Stop Recv");	

	if (SimpleBlockingIO)	// Blocking
	{
		if (!OnAccept())
		{
			return -1;
		}

		while (bIsRunning && OnRead() > 0)
		{
			packetno = ((strcmp(iProtocol, "UDP") == 0) ? pUDP->GetTotalPacket() : pTCPServer->GetTotalPacket());
			lossrate = ((strcmp(iProtocol, "UDP") == 0) ? pUDP->GetLossRate() : pTCPServer->GetLossRate());
			losspacket = ((strcmp(iProtocol, "UDP") == 0) ? pUDP->GetLossPacket() : pTCPServer->GetLossPacket());
			
			if (losspacket <0)
			{
				losspacket =0;
				lossrate =0;
				recvfilesize = 0;
			}	

			if (packetno <0)
				recvfilesize = 0;

			ConvertRate(outrate, (double)(packetno-losspacket)*lPktSize*1000/RecvTimer.Elapsed());
					
			sprintf(recvtemp, "%d", packetno);
			m_PacketTransferred.SetWindowText(recvtemp);		
			m_DataRate.SetWindowText(outrate);

			sprintf(recvtemp, "%.1f s", RecvTimer.Elapsed()/1000.0);
			m_TimeElapsed.SetWindowText(recvtemp);
			
			sprintf(recvtemp, "%.0f %%", lossrate);
			m_Loss.SetWindowText(recvtemp);
			
			sprintf(recvtemp, "%d", losspacket);
			m_PacketLoss.SetWindowText(recvtemp);

			if (recvfile)
			{
				double size;		
				
				size = (recvfilesize-(packetno+1)*8)/1048576;
			
				if (size > 1)
					sprintf(recvtemp, "%.2f MBytes", size);	// mega Bytes
				else
				{
					size = (recvfilesize-(packetno+1)*8)/1024;
					if (size > 1)
						sprintf(recvtemp, "%.2f kBytes", size);	// kilo Bytes
					else
						sprintf(recvtemp, "%.2f Bytes", (recvfilesize-(packetno+1)*8));	// Bytes
				}
			
				m_RecvFileS.SetWindowText(recvtemp);
			}

			DisplayTimer.Start();
			Sleep(iRefreshInterval);

			MSG msg;
			if (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				AfxGetApp()-> PumpMessage( );
			}
			LONG lIdle = 0;
			while ( AfxGetApp()-> OnIdle( lIdle++ ) ) ;
		}		

		OnClose();
	}	
	else	// Async
	{
		// select the receiver socket handle
		socket = ((strcmp(iProtocol, "UDP") == 0) ? pUDP->GetHandle() : pTCPServer->GetHandle());
		
		// when message comes, dipatch to Window Procedure, identify the message
		int err = WSAAsyncSelect(socket, 
								 m_hWnd, 
								 UDM_WINSOCK_SELECT, 
								 FD_READ | FD_ACCEPT | FD_CLOSE);
		
		if (err != 0)
		{
			recvfile = false;	
			MessageBox("WSAAsyncSelect() failed!", "Netprobe Error", MB_OK);
			OnClose();
		}
		else		
		{
			QuitFlag = false;
			OKrecv = true;	// ready for receiving
		}
	}
	
//	AfxMessageBox(" UDP Finish");

	return 0;
}

// Open the Host Information detail
void CNetprobe1Dlg::OnHostInfo() 
{
	// TODO: Add your control notification handler code here
	//host mydia(IDD_HostInfo);

	Chost mydia(this);
	mydia.DoModal();	
}

void CNetprobe1Dlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}



LRESULT CNetprobe1Dlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	if ((message == UDM_WINSOCK_SELECT) && (bIsRunning) && (!QuitFlag))
	{
		int event = WSAGETSELECTEVENT(lParam);
		int wsaerr = WSAGETSELECTERROR(lParam);
		switch (event) {
			case FD_READ : OnRead(); /* AfxMessageBox(" check point 4"); */ return 0;
			case FD_ACCEPT : OnAccept(); /*AfxMessageBox("Connection Accepted"); */ return 0;
			case FD_CLOSE : //StartDelayThread(); return 0;
				OnClose(); AfxMessageBox("Close Connection by Sender"); return 0;
		}
	}	
	 
	return CDialog::WindowProc(message, wParam, lParam);
}

// convert the data transmission rate
void CNetprobe1Dlg::ConvertRate(char *outrate, double inrate)
{
	double rate = inrate;
 
	rate = inrate/1000000;
	if (rate > 10)
		sprintf(outrate, "%.2f MBps", rate);	// mega Bytes
	else
	{
		rate = inrate/1000;
		if (rate > 10)
			sprintf(outrate, "%.2f kBps", rate);	// kilo Bytes
		else
			sprintf(outrate, "%.2f Bps", inrate);	// Bytes
	}
}

//////////////////////////////////////////////////
//		Variable Initializaton
//////////////////////////////////////////////////
bool CNetprobe1Dlg::InitVariable(int sendrecv)
{
	int choice = sendrecv;

	recvfilesize = 0;
	OKrecv = false;
	send_complete = false;
	sendbreak = false;
	stoprecv = false;
	RTStart = false;	

	iRefreshInterval = m_Refresh;
	if (iRefreshInterval <= 0)
	{
		char temp[10];
		sprintf(temp, "%d", iRefreshInterval);
		MessageBox("Refresh Interval [" + (CString) temp + "] must be positive and non-zero.", "Netprobe Error", MB_ICONEXCLAMATION);
		return false;
	}
	
	if (choice==0) {
	
		if (m_Peer.GetLength() >= ES_Network::MAX_NAME)
		{
			MessageBox("Error hostname too long, terminated.", "Netprobe Error", MB_ICONEXCLAMATION);
			return false;
		}
		else if (m_Peer.IsEmpty())
		{
			MessageBox("Hostname must be specified, terminated.", "Netprobe Error", MB_ICONEXCLAMATION);
			return false;
		}
	
		strcpy(hostname, m_Peer);
		usPort = m_PeerPort;
		}
		
	else
	{
		if (m_Local.GetLength() >= ES_Network::MAX_NAME)
		{
			MessageBox("Error hostname too long, terminated.", "Netprobe Error", MB_ICONEXCLAMATION);
			return false;
		}
		else if (m_Local.IsEmpty())
		{
			MessageBox("Hostname must be specified, terminated.", "Netprobe Error", MB_ICONEXCLAMATION);
			return false;
		}
		strcpy(hostname, m_Local);
		usPort = m_LocalPort;	
	}

	ES_Network network;
	if (network.ValidateHost(hostname) == 0)
	{
		MessageBox("Remote Hostname [" +(CString) hostname +"] is invalid.", "Netprobe Error", MB_ICONEXCLAMATION);
		return false;
	}	

	if (m_Protocol.GetCheck())
		strcpy(iProtocol, "TCP");
	else
		strcpy(iProtocol, "UDP");
	
	lPktSize = m_Size;

	if (lPktSize < 8)
	{
		char temp[10];
		sprintf(temp, "%d", lPktSize);
		MessageBox("Invalid Packet Size Specified [" +(CString) temp + "].\r\nPacket Size should be no less than 8.", "Netprobe Error", MB_ICONEXCLAMATION);
		return false;
	}
	lRate = m_Rate;
	lNumToSend = m_PacketNo;
	bSendForever = false;
	if (lNumToSend == 0)
		bSendForever = true;

	lBufferNum =m_BufferNo;

	return true;
}

//////////////////////////////////////////////////////////////////////////////
//	when FD_READ messages handles by the WindowProc, then call OnRead()
//////////////////////////////////////////////////////////////////////////////
int CNetprobe1Dlg::OnRead()
{
	int err;

	aStart = false;

	if (strcmp(iProtocol, "UDP") == 0)
	{
		err = pUDP->Recv(lPktSize,recvfile);
	
		if (err < 0)
		{
	
			OnClose();

			if (WSAGetLastError() == 10040)
				AfxMessageBox("Receive Mode: Message sent is too long for receive.");
			else if (err == -2)
				MessageBox("Receive Mode: Packet size mismatch, terminated.", "NetProbe Error", MB_ICONEXCLAMATION);
			else if (err != -2) 
				AfxMessageBox("Receive Mode: ES_UDP::Recv() failed.");
						
			NotHold();

			return -err;
		}

		if (pUDP->GetPacketRecv() == 1)
		{			
			// prevent calling 2 RecvStatThread simultaneoudly
			if (!RTStart)
			{	
				RecvTimer.Start();			
				RecvStatThread();
			}
	
			
			RTStart = true;		
			pUDP->setSeqNo();
			pUDP->setPacketNo();
			
		}
	}
	else	// for TCP
	{
		err = pTCPServer->Recv(lPktSize,recvfile);

		if (err <= 0)
		{
	
			OnClose();
			if (err == -2)
				MessageBox("Receive Mode: Packet size mismatch, terminated.", "NetProbe Error", MB_ICONEXCLAMATION);
			
			NotHold();

			return err;
		}

		if (pTCPServer->GetPacketRecv() == 1)
		{
			if (!RTStart)
			{
				RecvTimer.Start();			
				RecvStatThread(); 				
			}

			RTStart = true;
		}		
	}

	if (err >=0)
		recvfilesize += err;

	return 1;
}


bool CNetprobe1Dlg::OnAccept()
{
	if (strcmp(iProtocol, "TCP") == 0)
		return pTCPServer->Accept();
	return true;
}

int CNetprobe1Dlg::OnClose()
{
	if (bIsRunning)
	{
		int er = 0;
	
		do
		{			
			if (strcmp(iProtocol, "UDP") == 0)
				er = pUDP->Recv(lPktSize,recvfile);
			else
				er = pTCPServer->Recv(lPktSize,recvfile);
		} while (er != -1);

		bIsRunning = false;

		if (!SimpleBlockingIO)	// Async
		{
			WSAAsyncSelect(socket, m_hWnd, 0, 0);
			
			StopThreadRecvStat();
	
			OKrecv = false;
			QuitFlag = true;
		}
		else	// blocking or Alertable
			QuitFlag = true;
	
		if (strcmp(iProtocol, "TCP") == 0)
		{
			if (recvfile)
			{
				recvfile = false;
				pTCPServer->CloseFile();
			}

			delete pTCPServer;
		}
		else 
		{
			if (recvfile)
			{
				recvfile = false;
				pUDP->CloseFile();
			}

			delete pUDP;
		}

		m_ButtonRecv.EnableWindow(true);
		m_ButtonSend.EnableWindow(true);
		m_ButtonHostInfo.EnableWindow(true);
		m_ButtonQuit.EnableWindow(true);
		m_ButtonRecv.SetWindowText("Receive");
		m_ButtonSend.SetWindowText("Send");
		NotHold();

		recvfile = false;
	
		return 1;
	}


	return 0;
}

void CNetprobe1Dlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

void CNetprobe1Dlg::Hold()
{
	m_BufferNoC.EnableWindow(false);
	m_LocalC.EnableWindow(false);
	m_LocalPortC.EnableWindow(false);
	m_PeerC.EnableWindow(false);
	m_PeerPortC.EnableWindow(false);
	m_RateC.EnableWindow(false);
	m_SizeC.EnableWindow(false);
	m_RefreshC.EnableWindow(false);
	m_PacketNoC.EnableWindow(false);
	m_Protocol.EnableWindow(false);
	m_IOMode.EnableWindow(false);
	m_Async.EnableWindow(false);
	m_UDP.EnableWindow(false);
	m_SendBlock.EnableWindow(false);
	m_RecvBlock.EnableWindow(false);
	m_Alert.EnableWindow(false);
	m_SendFileC.EnableWindow(false);
	m_RecvFileC.EnableWindow(false);
	m_FileC.EnableWindow(false);
	m_FileTextC.EnableWindow(false);
}

void CNetprobe1Dlg::NotHold()
{
	m_BufferNoC.EnableWindow(true);
	m_LocalC.EnableWindow(true);
	m_LocalPortC.EnableWindow(true);
	m_PeerC.EnableWindow(true);
	m_PeerPortC.EnableWindow(true);
	m_RateC.EnableWindow(true);
	m_SizeC.EnableWindow(true);
	m_RefreshC.EnableWindow(true);
	m_PacketNoC.EnableWindow(true);
	m_Protocol.EnableWindow(true);
	m_IOMode.EnableWindow(true);
	m_Async.EnableWindow(true);
	m_UDP.EnableWindow(true);
	m_SendBlock.EnableWindow(true);
	m_RecvBlock.EnableWindow(true);
	m_Alert.EnableWindow(true);
//	m_SendFileC.EnableWindow(true);
//	m_RecvFileC.EnableWindow(true);
	m_FileC.EnableWindow(true);
	m_FileTextC.EnableWindow(true);
	m_FileC.SetCheck(0);
//	SetDlgItemText(IDC_filepath,"");
	
	check = true;
	file = false;
	recvfile = false;
}

int CNetprobe1Dlg::OnSendFile() 
{
	file = true;

	// TODO: Add your control notification handler code here
	LPCTSTR  lpszFilter ="All Files(*.*)|*.*|C++ Files(*.cpp)|*.cpp|";
	CString str;

	CFileDialog SFile(true, NULL, NULL, NULL, lpszFilter, NULL);
	
	SFile.DoModal();

	m_filepath = SFile.GetPathName();
	UpdateData(false);

	if (m_filepath == "")
	{
		m_PacketNoC.EnableWindow(true);		
		file = false;
	}

	if (file)
	{
		double size;
		char sendtemp[20];

		m_RecvFileC.EnableWindow(false);
		m_Alert.EnableWindow(false);
		m_RecvBlock.EnableWindow(false);
		m_Async.EnableWindow(false);
		m_ButtonRecv.EnableWindow(false);
		m_PacketNoC.EnableWindow(false);
		m_PacketNo = 0;
		m_recvfpath = "";
		UpdateData(false);		
		sprintf(sendtemp, "0 Bytes", 0);	// mega Bytes
		m_RecvFileS.SetWindowText(sendtemp);			

		CString pFileName = m_filepath;
		f = new CFile((LPCTSTR)pFileName, CFile::modeRead);

		size = (double)f->GetLength()/1048576;
		if (size > 1)
			sprintf(sendtemp, "%.2f MBytes", size);	// mega Bytes
		else
		{
			size = (double)f->GetLength()/1024;
			if (size > 1)
				sprintf(sendtemp, "%.2f kBytes", size);	// kilo Bytes
			else
				sprintf(sendtemp, "%.2f Bytes", (double)f->GetLength());	// Bytes
		}
	
		m_SendFileS.SetWindowText(sendtemp);			

		f->Close();
		delete f;
	}
	else
	{
		m_RecvFileC.EnableWindow(true);
		m_Alert.EnableWindow(true);
		m_RecvBlock.EnableWindow(true);
		m_Async.EnableWindow(true);
		m_ButtonRecv.EnableWindow(true);
		m_PacketNoC.EnableWindow(true);	
		m_SendFileS.SetWindowText("0 Bytes");			
	}

	return 0;

}

void CNetprobe1Dlg::OnRecvFile() 
{
	// TODO: Add your control notification handler code here
	
	LPCTSTR  lpszFilter ="All Files(*.*)|*.*|";

	CFileDialog RFile(false, NULL, NULL, NULL, lpszFilter, NULL);
	
	RFile.DoModal();

	m_recvfpath = RFile.GetPathName();
	UpdateData(false);

	recvfile = true;

	if (m_recvfpath == "")
		recvfile = false;
	
	if (recvfile)
	{
		char temp[20];

		m_SendFileC.EnableWindow(false);
		m_SendBlock.EnableWindow(false);
		m_IOMode.EnableWindow(false);
		m_ButtonSend.EnableWindow(false);
		m_RecvFileS.SetWindowText("0 Bytes");
		m_filepath = "";
		UpdateData(false);
		sprintf(temp, "0 Bytes", 0);	// mega Bytes
		m_SendFileS.SetWindowText(temp);			

	}
	else
	{
		m_SendFileC.EnableWindow(true);
		m_SendBlock.EnableWindow(true);
		m_IOMode.EnableWindow(true);
		m_ButtonSend.EnableWindow(true);
		m_recvfpath="";
		UpdateData(false);
		m_RecvFileS.SetWindowText("0 Bytes");			
	}

	recvfname = m_recvfpath; 
	
}

int CNetprobe1Dlg::StartAlertableRecv()
{
	DWORD ThreadID;
	hThread3 = CreateThread(0, 
							0, 
							CNetprobe1Dlg::AlertableRecvProc,
							(LPVOID)this, 
							0, 
							&ThreadID);

	if (hThread3 == NULL)
		MessageBox("Error starting new thread!", "Netprobe Error", MB_OK);
	return 0;
}

int CNetprobe1Dlg::StopAlertableRecv()
{	
	if (bIsRunning)
	{
		aStart = false;
		bIsRunning = false;
		if (closesocket(hSocket) == SOCKET_ERROR)
		{
			AfxMessageBox("Error in closing the socket, terminated.");
			return -1;
		}
		if (strcmp(iProtocol, "TCP") == 0 && closesocket(cSocket) == SOCKET_ERROR)
		{
			AfxMessageBox("Error in closing the socket, terminated.");
			return -1;
		}
		m_ButtonRecv.SetWindowText("Receive");
		CloseHandle(hThread3);

		if (recvfile)
		{
			f1->Close();
			delete f1;
			recvfile = false;
		}

	}

	return 0;
}

DWORD WINAPI CNetprobe1Dlg::AlertableRecvProc(LPVOID IpInstance)
{
	CNetprobe1Dlg *pclass = (CNetprobe1Dlg *) IpInstance;
	pclass->m_ButtonRecv.SetWindowText("Stop Receive");
	pclass->AlertableRecv(pclass);
	CloseHandle(pclass->hThread3);
	return 0;	
}

void CALLBACK CNetprobe1Dlg::SocketIOCompletion(DWORD dwError, 
												DWORD cbTransferred, 
												LPWSAOVERLAPPED lpOverlapped, 
												DWORD dwFlags)
{
	CNetprobe1Dlg *pclass = (CNetprobe1Dlg *)lpOverlapped->hEvent;
	DWORD bytes_received, flags = 0;
	bool toread;
	
	if (!pclass->bIsRunning)
		return;

	if (!pclass->aStart)
	{
		pclass->aStart = true;		
		pclass->OKrecv =true;	
		pclass->RecvTimer.Start();
		pclass->RecvStatThread();		
	}

	if (pclass->aReceived < 8)
		toread = true;
	else
		toread = false;

	if (strcmp(pclass->iProtocol, "TCP") == 0 && cbTransferred == 0)
	{
		pclass->StopAlertableRecv();
		return;
	}

	if (dwError == 0)
	{
		if ((pclass->lPktSize-pclass->aReceived) >= (int) cbTransferred)
		{
			char *ptr;
			memcpy(pclass->pData+pclass->aReceived, pclass->pWsaBuf[pclass->aBufIndex]->buf, cbTransferred);
			pclass->pDatalen += cbTransferred;

			ptr = pclass->pData+pclass->aReceived;

			if (recvfile)
			{
				if (pclass->aReceived <= 8)
					pclass->f1->Write( ptr+sizeof(unsigned long)+sizeof(int)-pclass->aReceived,cbTransferred-sizeof(unsigned long)-sizeof(int)+pclass->aReceived);			
				else
					pclass->f1->Write( pclass->pWsaBuf[pclass->aBufIndex]->buf,cbTransferred);			
			}
		}
		else
		{
			char *datapt = pclass->pWsaBuf[pclass->aBufIndex]->buf;

			if (recvfile)
			{
				if (pclass->aReceived <=8)
					pclass->f1->Write( datapt+sizeof(unsigned long)+sizeof(int)-pclass->aReceived,cbTransferred-sizeof(unsigned long)-sizeof(int)+pclass->aReceived);			
				else
				{
					pclass->f1->Write( datapt,pclass->lPktSize - pclass->aReceived);	

					if ((cbTransferred+pclass->aReceived-pclass->lPktSize) >8)
						pclass->f1->Write( datapt+pclass->lPktSize-pclass->aReceived+sizeof(unsigned long)+sizeof(int), cbTransferred-pclass->lPktSize+pclass->aReceived-sizeof(unsigned long)-sizeof(int));	
				}
			}
			datapt += (pclass->lPktSize - pclass->aReceived);
			memcpy(pclass->pData, datapt, (cbTransferred - pclass->lPktSize + pclass->aReceived));
			pclass->pDatalen = cbTransferred - pclass->lPktSize + pclass->aReceived;
			toread = true;
		}

		if (toread && pclass->pDatalen >= 8)
		{
			char *temp = pclass->pData;
			temp += sizeof(unsigned long);
			
			if (pclass->lPktSize != *((unsigned long *)temp))
			{	
				pclass->StopAlertableRecv();
				AfxMessageBox("Packet size mismatch, terminated.");
				return;
			}

			if (strcmp(pclass->iProtocol, "UDP") == 0)
			{
				if (*((unsigned long *)pclass->pData) < pclass->atotalpacket)
				{
					pclass->RecvTimer.Start();
					pclass->apacketrecv = 0;
				}
			}
			pclass->atotalpacket = *((unsigned long *)pclass->pData);
			pclass->apacketrecv++;
		}

		pclass->aReceived += cbTransferred;
		pclass->aReceived %= pclass->lPktSize;	
		pclass->pDatalen %= pclass->lPktSize;
		pclass->recvfilesize += cbTransferred;
			
	}
	else
	{
		pclass->StopAlertableRecv();
		if (dwError == WSAEMSGSIZE)
			AfxMessageBox("Packet size mismatch, terminated.");
		return;
	}

	memset(pclass->pWsaBuf[pclass->aBufIndex]->buf, 0, pclass->pWsaBuf[pclass->aBufIndex]->len);

	int result = WSARecv(pclass->hSocket, 
					     pclass->pWsaBuf[pclass->aBufIndex], 
						 1, 
						 &bytes_received, 
						 &flags, 
						 lpOverlapped, 
						 SocketIOCompletion);

	if ((result == SOCKET_ERROR) && ((result = WSAGetLastError()) != WSA_IO_PENDING))
	{
			pclass->StopAlertableRecv();
			pclass->NotHold();
			AfxMessageBox("Error in WSARecv, terminated.");
			return;
	}

	if (pclass->aBufIndex < (pclass->lBufferNum-1))
		pclass->aBufIndex++;
	else
		pclass->aBufIndex = 0;

}

int CNetprobe1Dlg::AlertableRecv(CNetprobe1Dlg* pclass)
{
	QuitFlag = false;
	bIsRunning = true;
	aStart = false;
	atotalpacket = 0;
	apacketrecv = 0;
	aBufIndex = 0;
	aReceived = 0;
	pData = new char[lPktSize];
	pDatalen = 0;

	if (strcmp(iProtocol, "TCP") == 0)
		cSocket = WSASocket(PF_INET, 
							SOCK_STREAM, 
							IPPROTO_TCP, 
							(LPWSAPROTOCOL_INFO) 0, 
							(GROUP) 0, 
							WSA_FLAG_OVERLAPPED);
	else
		hSocket = WSASocket(PF_INET, 
							SOCK_DGRAM, 
							IPPROTO_UDP, 
							(LPWSAPROTOCOL_INFO) 0, 
							(GROUP) 0, 
							WSA_FLAG_OVERLAPPED);

	if ((strcmp(iProtocol, "TCP") == 0 && cSocket == INVALID_SOCKET))
	{
		AfxMessageBox("Error in creating socket for TCP,terminated.");
		cSocket = NULL;
		return false;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET; 
	addr.sin_port = htons(usPort);
	addr.sin_addr.s_addr = INADDR_ANY;
	
	int err;
	if (strcmp(iProtocol, "TCP") == 0)
		err = bind(cSocket, (struct sockaddr *) &addr, sizeof(addr));
	else
		err = bind(hSocket, (struct sockaddr *) &addr, sizeof(addr));
	if (err == SOCKET_ERROR)
	{
		AfxMessageBox("Fail in binding, terminated");
		StopAlertableRecv();
		return 0;
	}

	if (bIsRunning && strcmp(iProtocol, "TCP") == 0)
	{
		if (listen(cSocket, 1) == SOCKET_ERROR)
		{
			AfxMessageBox("Error in listen for TCP,terminated.");
			StopAlertableRecv();
			return 0;
		}
	
		struct sockaddr_in newaddr;
		int newaddrlen = sizeof(struct sockaddr_in);

		hSocket = WSASocket(PF_INET, 
							SOCK_STREAM, 
							IPPROTO_TCP, 
							(LPWSAPROTOCOL_INFO) 0, 
							(GROUP) 0, 
							WSA_FLAG_OVERLAPPED);	

		hSocket = accept(cSocket, (struct sockaddr *) &newaddr, &newaddrlen);
		
		if (hSocket == INVALID_SOCKET && bIsRunning)
		{
			AfxMessageBox("Error in accept for TCP,terminated.");
			hSocket = NULL;
			return 0;
		} 
	}

	DWORD bytes_received, flags = 0;
	WSAOVERLAPPED **pOverlap = new WSAOVERLAPPED*[lBufferNum];
	pWsaBuf = new WSABUF*[lBufferNum];
	unsigned int i;

	if (bIsRunning)
	{
		for (i = 0; i < lBufferNum; i++)
		{
			pOverlap[i] = new WSAOVERLAPPED;
			pOverlap[i]->hEvent = this;	

			pWsaBuf[i] = new WSABUF;
			pWsaBuf[i]->buf = new char[lPktSize];
			pWsaBuf[i]->len = lPktSize;

			memset(pWsaBuf[i]->buf, 0, pWsaBuf[i]->len);

			int result = WSARecv(hSocket, 
								 pWsaBuf[i], 
								 1, 
								 &bytes_received, 
								 &flags, 
								 pOverlap[i], 
								 SocketIOCompletion);

			if (bIsRunning && ((result == SOCKET_ERROR) && ((result = WSAGetLastError()) != WSA_IO_PENDING)))
			{
				StopAlertableRecv();
				NotHold();
				if (err == WSAEMSGSIZE)
					AfxMessageBox("Packet size mismatch, terminated.");
				else
					AfxMessageBox("Error in WSARecv, terminated.");
			}
		}
		
		while (bIsRunning)
		{
			if (aStart)
			{
				// test
			}
			
			long tosleep;
			while((tosleep = (long) (iRefreshInterval-DisplayTimer.Elapsed())) > 0)
			{
				if (SleepEx(tosleep, true) == 0)
					break;
			}
			DisplayTimer.Start();
		}

		for (i = 0; i < lBufferNum; i++)
		{
			free(pWsaBuf[i]);
			free(pOverlap[i]);
		}
	}
	 
	free(pWsaBuf);
	free(pOverlap);
	free(pData);

	OKrecv = false;

	m_ButtonSend.EnableWindow(true);
	m_ButtonHostInfo.EnableWindow(true);
	m_ButtonQuit.EnableWindow(true);
		
	NotHold();

	return 0;
}


void CNetprobe1Dlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here

	if (check)
	{
		m_SendFileC.EnableWindow(true);
		m_RecvFileC.EnableWindow(true);
		m_PacketNoC.EnableWindow(true);
		
		m_filepath="";
		m_recvfpath="";
		m_RecvFileS.SetWindowText("0 Bytes");
		m_SendFileS.SetWindowText("0 Bytes");
		UpdateData(false);	
		check = false;
	}
	else
	{
		m_SendFileC.EnableWindow(false);
		m_RecvFileC.EnableWindow(false);
		m_PacketNoC.EnableWindow(true);

		m_filepath="";
		m_recvfpath="";
		m_RecvFileS.SetWindowText("0 Bytes");
		m_SendFileS.SetWindowText("0 Bytes");
		m_ButtonSend.EnableWindow(true);
		m_ButtonRecv.EnableWindow(true);

		m_Alert.EnableWindow(true);
		m_RecvBlock.EnableWindow(true);
		m_Async.EnableWindow(true);

		m_SendBlock.EnableWindow(true);
		m_IOMode.EnableWindow(true);
	
		UpdateData(false);
		check = true;
	}
}

void CNetprobe1Dlg::OnDoubleclickedCheck1() 
{
	// TODO: Add your control notification handler code here

}
