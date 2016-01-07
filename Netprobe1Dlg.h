// Netprobe1Dlg.h : header file
//

#if !defined(AFX_NETPROBE1DLG_H__48A53967_0409_4E30_B004_90DF1FADFA27__INCLUDED_)
#define AFX_NETPROBE1DLG_H__48A53967_0409_4E30_B004_90DF1FADFA27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ES_UDP.h"
#include "ES_TCPServer.h"
#include "ES_TIMER.h"

/////////////////////////////////////////////////////////////////////////////
// CNetprobe1Dlg dialog

class CNetprobe1Dlg : public CDialog
{
private:
	double recvfilesize;
	CFile *f;
	CFile *f1;
	int pDatalen;
	SOCKET cSocket;
	char * pData;
	bool aStart;		// alertable starts
	int aReceived;
	SOCKET hSocket;
	WSABUF **pWsaBuf;
	unsigned int aBufIndex;
	unsigned long atotalpacket;
	int apacketrecv;
	unsigned int lBufferNum;

	bool OKrecv;
	bool send_complete;
	bool sendbreak;
	bool stoprecv;
	bool RTStart;
	SOCKET socket;
	bool QuitFlag;	
	bool bIsRunning;
	const unsigned int UDM_WINSOCK_SELECT;
	ES_FlashTimer DisplayTimer;
	ES_FlashTimer RecvTimer;
	ES_UDP* pUDP;
	ES_TCPServer* pTCPServer;
	bool SimpleBlockingIO;
	unsigned int lNumToSend;
	bool bSendForever;
	double lRate;
	unsigned long lPktSize;
	char iProtocol[20];
	int usPort;
	char hostname[1024];
	int iRefreshInterval;
//	unsigned long ulSeqNum;

// Construction
public:
	void NotHold();
	void Hold();
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CNetprobe1Dlg(CWnd* pParent = NULL);	// standard constructor	
	int RunSendMode(CNetprobe1Dlg* pclass);
	
	int AlertableRecv(CNetprobe1Dlg* pclass);
	static void CALLBACK SocketIOCompletion(IN DWORD dwError, IN DWORD cbTransferred, IN LPWSAOVERLAPPED lpOverlapped, IN DWORD dwFlags);
	static DWORD WINAPI AlertableRecvProc(LPVOID IpInstance);
	int StopAlertableRecv();
	int StartAlertableRecv();

	int RecvStatThread();
	static DWORD WINAPI RecvStatProc(LPVOID );
	int StopThreadRecvStat();

	int RunRecvMode();
	static DWORD WINAPI ThreadSendProc(LPVOID );

	int StartDelayThread();
	static DWORD WINAPI ThreadDelayProc(LPVOID );

	int OnClose();
	bool OnAccept();
	int OnRead();
	int StopThreadedSend();
	int StartThreadedSend();
		
	HANDLE hThread1;	
	HANDLE hThread2;
	HANDLE hThread3;
	HANDLE hThread4;

	void ConvertRate(char* outrate, double inrate);
	bool InitVariable(int sendrecv);

// Dialog Data
	//{{AFX_DATA(CNetprobe1Dlg)
	enum { IDD = IDD_NETPROBE1_DIALOG };
	CStatic	m_SoFSC;
	CEdit	m_filepathC;
	CButton	m_TranFileStaticC;
	CStatic	m_RFSC;
	CButton	m_RecvFileStaticC;
	CStatic	m_FileTextC;
	CButton	m_FileC;
	CStatic	m_RecvFileS;
	CStatic	m_SendFileS;
	CButton	m_RecvFileC;
	CButton	m_SendFileC;
	CEdit	m_recvfpathC;
	CButton	m_FileSend;
	CEdit	m_BufferNoC;
	CButton	m_RecvBlock;
	CButton	m_SendBlock;
	CEdit	m_PeerC;
	CEdit	m_SizeC;
	CEdit	m_RefreshC;
	CEdit	m_PeerPortC;
	CEdit	m_PacketNoC;
	CEdit	m_LocalPortC;
	CEdit	m_RateC;
	CEdit	m_LocalC;
	CButton	m_ButtonQuit;
	CButton	m_ButtonHostInfo;
	CButton	m_IOMode;
	CButton m_Async;
	CButton m_UDP;
	CStatic	m_Loss;
	CStatic	m_PacketLoss;
	CStatic	m_PacketTransferred;
	CButton	m_ButtonRecv;
	CButton	m_ButtonSend;
	CButton	m_Protocol;
	CStatic	m_TimeElapsed;
	CStatic	m_DataRate;
	UINT	m_Size;
	UINT	m_Refresh;
	UINT	m_PeerPort;
	CString	m_Peer;
	UINT	m_PacketNo;
	UINT	m_LocalPort;
	CString	m_Local;
	double	m_Rate;
	CButton	m_Alert;
	UINT	m_BufferNo;
	CString	m_filepath;
	CString	m_recvfpath;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetprobe1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	

	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNetprobe1Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnHostInfo();
	afx_msg void OnSend();
	afx_msg void OnReceive();
	virtual void OnCancel();
	afx_msg void OnCancelMode();
	afx_msg int OnSendFile();
	afx_msg void OnRecvFile();
	afx_msg void OnCheck1();
	afx_msg void OnDoubleclickedCheck1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETPROBE1DLG_H__48A53967_0409_4E30_B004_90DF1FADFA27__INCLUDED_)
