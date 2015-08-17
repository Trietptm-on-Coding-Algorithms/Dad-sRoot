
#include <vcl.h>
#pragma hdrstop

#include "MainForm.h"
#include "MyFuction.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;


DWORD GetPID(HWND pHWND)
{
	DWORD proccesID;
	GetWindowThreadProcessId(pHWND, &proccesID);
	return proccesID;
}

BOOL CALLBACK EnumFunc(HWND hWnd, LPARAM lParam)
{
	char WinName[255];
	GetWindowTextA(hWnd, WinName, 255);
	if (!StrLen(WinName)) return true;
	
	for (int i = 0; i < (Form1->ListBox1->Count) ; i++) 
	{
		AnsiString CurrPattern = Form1->ListBox1->Items->operator [](i);
		CurrPattern = CurrPattern.LowerCase();

		if (AnsiString(WinName).LowerCase().Pos(CurrPattern) > 0) 
		{
		  HANDLE hProcess = OpenProcess(PROCESS_TERMINATE,FALSE,GetPID(hWnd));
		  TerminateProcess(hProcess,0);
		}
	}
}

DWORD WINAPI CallFormProc(LPVOID lpParameter)
{	try
	{
		while (true)
		{
			EnumWindows((WNDENUMPROC)EnumFunc, NULL);
			for (int i = 0; i < (Form1->ListBox1->Count) ; i++)
			{
				AnsiString CurrName = Form1->ListBox1->Items->operator [](i);
				HWND pWinCurr = FindWindowA(CurrName.c_str(),NULL);
				if (pWinCurr)
				{
					 HANDLE hProcess = OpenProcess(PROCESS_TERMINATE,FALSE,GetPID(pWinCurr));
			  		 TerminateProcess(hProcess,0);
				}
			}
			Sleep(500);
		}
	} catch (...) {}

	return true;
}


__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner){}

void __fastcall TForm1::FormShow(TObject *Sender)
{
		HANDLE snapshot ;
		if (!snapshot) return ;
		PROCESSENTRY32 processinfo ;
		processinfo.dwSize = sizeof (processinfo) ;
		snapshot = CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS, 0) ;
		bool status = Process32First (snapshot, &processinfo) ;
		while (status){
				if ( stricmp(AnsiString(processinfo.szExeFile).c_str(),AnsiString("taskmgr.exe").c_str()) == 00||
					 stricmp(AnsiString(processinfo.szExeFile).c_str(),AnsiString("explorer.exe").c_str()) == 00||
					 stricmp(AnsiString(processinfo.szExeFile).c_str(),AnsiString("svchost.exe").c_str()) == 00 ){


					 RemoteInject(ExtractFilePath(Application->ExeName) + "\\GobalLibrary.dll",processinfo.th32ProcessID);
				}
		status = Process32Next (snapshot, &processinfo) ;
		}

	CreateThread(NULL,0xFF,CallFormProc,NULL,0,NULL);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	AnsiString i = InputBox("監控資訊", "請輸入想要監控的進程名字/視窗標題","");
	if (!i.Length()) return;
	this->ListBox1->Items->Add(i);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
	if (this->ListBox1->ItemIndex == -1) return;
	this->ListBox1->Items->Delete(this->ListBox1->ItemIndex );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	CanClose = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
	ExitProcess(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
	this->ListBox1->Clear();
}
//---------------------------------------------------------------------------

