// stars.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <stdlib.h>
#include "resource.h"
#include <winreg.h>
#include "engine.h"
#include "starobject.h"


ENGINE engine;

#define start_none	0
#define start_set	1
#define start_saver 2
#define but_ok 0
#define but_cancel 1
#define but_apply 2
#define but_help 3
#define but_det1 4
#define but_det2 5
#define but_det3 6
#define but_speed 8
#define but_plan 7
#define res_desktop 9
#define res_640 10
#define res_800 11
#define res_1024 12
#define res_1280 13
#define res_1600 14

HWND hwnd;
HINSTANCE hinstance;
HWND buttons[16];
bool planetsyn;
int detail;	//1 = low,2 = med,3 = high
int speed; // 1 to 100
int resolution;
int scalex,scaley;
LARGE_INTEGER timerfreq;
LARGE_INTEGER starttime;
LARGE_INTEGER currenttime;	
unsigned int numbstars,nummstars,numfstars;
backstar *bstar;
forestar *fstar;
midstar *mstar;
planet *plan;
bool pauseon = false;
int nummoves = 0;	
char regcode[11] = {0,0,0,0,0,0,0,0,0,0,0};
IMAGE_OBJECT *imagereg;
bool regyn;
int width,height,bpp;
	

void setbuttons()
{
	if(planetsyn) SendMessage(buttons[but_plan],BM_SETCHECK,1,0);
	else SendMessage(buttons[but_plan],BM_SETCHECK,0,0);
	
	if(detail == 1) 
	{
		SendMessage(buttons[but_det1],BM_SETCHECK,1,0);
		SendMessage(buttons[but_det2],BM_SETCHECK,0,0);
		SendMessage(buttons[but_det3],BM_SETCHECK,0,0);
	}
	if(detail == 2) 
	{
		SendMessage(buttons[but_det1],BM_SETCHECK,0,0);
		SendMessage(buttons[but_det2],BM_SETCHECK,1,0);
		SendMessage(buttons[but_det3],BM_SETCHECK,0,0);
	}
	if(detail == 3) 
	{
		SendMessage(buttons[but_det1],BM_SETCHECK,0,0);
		SendMessage(buttons[but_det2],BM_SETCHECK,0,0);
		SendMessage(buttons[but_det3],BM_SETCHECK,1,0);
	}

	if(resolution == res_desktop) 
	{
		SendMessage(buttons[res_desktop],BM_SETCHECK,1,0);
		SendMessage(buttons[res_640],BM_SETCHECK,0,0);
		SendMessage(buttons[res_800],BM_SETCHECK,0,0);
		SendMessage(buttons[res_1024],BM_SETCHECK,0,0);
		SendMessage(buttons[res_1280],BM_SETCHECK,0,0);
		SendMessage(buttons[res_1600],BM_SETCHECK,0,0);
	}

	if(resolution == res_640) 
	{
		SendMessage(buttons[res_desktop],BM_SETCHECK,0,0);
		SendMessage(buttons[res_640],BM_SETCHECK,1,0);
		SendMessage(buttons[res_800],BM_SETCHECK,0,0);
		SendMessage(buttons[res_1024],BM_SETCHECK,0,0);
		SendMessage(buttons[res_1280],BM_SETCHECK,0,0);
		SendMessage(buttons[res_1600],BM_SETCHECK,0,0);
	}
	if(resolution == res_800) 
	{
		SendMessage(buttons[res_desktop],BM_SETCHECK,0,0);
		SendMessage(buttons[res_640],BM_SETCHECK,0,0);
		SendMessage(buttons[res_800],BM_SETCHECK,1,0);
		SendMessage(buttons[res_1024],BM_SETCHECK,0,0);
		SendMessage(buttons[res_1280],BM_SETCHECK,0,0);
		SendMessage(buttons[res_1600],BM_SETCHECK,0,0);
	}
	if(resolution == res_1024) 
	{
		SendMessage(buttons[res_desktop],BM_SETCHECK,0,0);
		SendMessage(buttons[res_640],BM_SETCHECK,0,0);
		SendMessage(buttons[res_800],BM_SETCHECK,0,0);
		SendMessage(buttons[res_1024],BM_SETCHECK,1,0);
		SendMessage(buttons[res_1280],BM_SETCHECK,0,0);
		SendMessage(buttons[res_1600],BM_SETCHECK,0,0);
	}
	if(resolution == res_1280) 
	{
		SendMessage(buttons[res_desktop],BM_SETCHECK,0,0);
		SendMessage(buttons[res_640],BM_SETCHECK,0,0);
		SendMessage(buttons[res_800],BM_SETCHECK,0,0);
		SendMessage(buttons[res_1024],BM_SETCHECK,0,0);
		SendMessage(buttons[res_1280],BM_SETCHECK,1,0);
		SendMessage(buttons[res_1600],BM_SETCHECK,0,0);
	}
	if(resolution == res_1600) 
	{
		SendMessage(buttons[res_desktop],BM_SETCHECK,0,0);
		SendMessage(buttons[res_640],BM_SETCHECK,0,0);
		SendMessage(buttons[res_800],BM_SETCHECK,0,0);
		SendMessage(buttons[res_1024],BM_SETCHECK,0,0);
		SendMessage(buttons[res_1280],BM_SETCHECK,0,0);
		SendMessage(buttons[res_1600],BM_SETCHECK,1,0);
	}
	SetScrollPos(buttons[but_speed],SB_CTL,speed,true);
}

void saveregistry()
{
	HKEY skey;
	unsigned long disp;
	RegCreateKeyEx(HKEY_CURRENT_USER,"Software",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&skey,&disp);
	RegCreateKeyEx(skey,"hoopty_interactive",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&skey,&disp);
	RegCreateKeyEx(skey,"stars",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&skey,&disp);
	RegSetValueEx(skey,"planets",0,REG_DWORD,(unsigned char*)(&planetsyn),sizeof(int));
	RegSetValueEx(skey,"detail",0,REG_DWORD,(unsigned char*)(&detail),sizeof(int));
	RegSetValueEx(skey,"resolution",0,REG_DWORD,(unsigned char*)(&resolution),sizeof(int));
	RegSetValueEx(skey,"speed",0,REG_DWORD,(unsigned char*)(&speed),sizeof(int));
	RegSetValueEx(skey,"code",0,REG_SZ,(unsigned char *)regcode,sizeof(char)*11);
	RegCloseKey(skey);
}

void loadregistry()
{
	HKEY skey;
	unsigned long size,type;
	size = sizeof(int);
	if((RegOpenKeyEx(HKEY_CURRENT_USER,"Software",0,KEY_ALL_ACCESS,&skey)) != ERROR_SUCCESS)
		return;
	RegOpenKeyEx(skey,"hoopty_interactive",0,KEY_ALL_ACCESS,&skey);
	if((RegOpenKeyEx(skey,"stars",0,KEY_ALL_ACCESS,&skey)) != ERROR_SUCCESS)
		return;
	RegQueryValueEx(skey,"planets",NULL,NULL,(unsigned char*)(&planetsyn),&size);
	RegQueryValueEx(skey,"detail",NULL,&type,(unsigned char*)(&detail),&size);
	RegQueryValueEx(skey,"resolution",NULL,&type,(unsigned char*)(&resolution),&size);
	RegQueryValueEx(skey,"speed",NULL,&type,(unsigned char*)(&speed),&size);
	type = REG_SZ; size = 11;
	RegQueryValueEx(skey,"code",NULL,&type,(unsigned char*)(regcode),&size);
	RegCloseKey(skey);
}


bool checkcode(char code[11])
{
	int sum,count;
	sum = 0;
	for(count = 0;count < 10;count++)
	{
		sum += code[count];
	}
	if(sum == 526) return true;
	else return false;
}



BOOL CALLBACK aboutboxproc(HWND hdlg,unsigned int message,WPARAM wparam, LPARAM lparam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		return true;
	case WM_COMMAND:
		switch(LOWORD(wparam))
		{
		case IDOK:
			EndDialog(hdlg,0);
			return true;
		case reg_but:
			if(GetWindowText(GetDlgItem(hdlg,IDC_EDIT1),regcode,11) == 10)
				if(checkcode(regcode)) MessageBox(hwnd,"You have registered sucessfuly","Thank You",MB_OK);
				else MessageBox(hwnd,"You code is incorect","Try Again",MB_OK);
			
			else
				MessageBox(hwnd,"You code is incorect","Try Again",MB_OK);
			
		}
	}
	return false;
}


LRESULT CALLBACK WndProcset(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int xwidth,ywidth;
	TEXTMETRIC tm;
	switch(message)
	{
		case WM_CREATE:
			RECT temp;
			GetWindowRect(hwnd,&temp);
			hdc = GetDC(hwnd);
			GetTextMetrics(hdc,&tm);
			xwidth = tm.tmAveCharWidth;
			ywidth = tm.tmHeight + tm.tmExternalLeading;
			ReleaseDC(hwnd,hdc);
			MoveWindow(hwnd,temp.left,temp.top,xwidth*50,ywidth*19,true);
			CreateWindow("STATIC","Starfield Density",WS_CHILD | WS_VISIBLE | SS_LEFT,xwidth*2,ywidth*8,xwidth*17,ywidth,hwnd,NULL,hinstance,NULL);
			CreateWindow("STATIC","SPEED",WS_CHILD | WS_VISIBLE | SS_LEFT,xwidth*6,ywidth*3,xwidth*8,ywidth,hwnd,NULL,hinstance,NULL);
			CreateWindow("STATIC","RESOLUTION",WS_CHILD | WS_VISIBLE | SS_LEFT,xwidth*26,(int)(ywidth*0.5),xwidth*14,ywidth,hwnd,NULL,hinstance,NULL);
			buttons[but_ok] = CreateWindow("BUTTON","OK",WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,xwidth*2,ywidth*15,xwidth*7,(int)(ywidth*1.5),hwnd,(HMENU)but_ok,hinstance,NULL);
			buttons[but_cancel] = CreateWindow("BUTTON","CANCEL",WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,xwidth*11,ywidth*15,xwidth*10,(int)(ywidth*1.5),hwnd,(HMENU)but_cancel,hinstance,NULL);
			buttons[but_apply] = CreateWindow("BUTTON","APPLY",WS_DISABLED | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,xwidth*24,ywidth*15,xwidth*10,(int)(ywidth*1.5),hwnd,(HMENU)but_apply,hinstance,NULL);
			buttons[but_plan] = CreateWindow("BUTTON","Enable Planets",WS_CHILD | WS_VISIBLE | BS_CHECKBOX,xwidth*2,(int)(ywidth*0.5),xwidth*18,ywidth,hwnd,(HMENU)but_plan,hinstance,NULL);
			buttons[but_help] = CreateWindow("BUTTON","ABOUT",WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,xwidth*37,ywidth*15,xwidth*9,(int)(ywidth*1.5),hwnd,(HMENU)but_help,hinstance,NULL);
			buttons[but_det1] = CreateWindow("BUTTON","Low",WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,xwidth*2,ywidth*9,xwidth*10,ywidth,hwnd,(HMENU)but_det1,hinstance,NULL);
			buttons[but_det2] = CreateWindow("BUTTON","Medium",WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,xwidth*2,ywidth*10,xwidth*10,ywidth,hwnd,(HMENU)but_det2,hinstance,NULL);
			buttons[but_det3] = CreateWindow("BUTTON","High",WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,xwidth*2,ywidth*11,xwidth*10,ywidth,hwnd,(HMENU)but_det3,hinstance,NULL);
			buttons[but_speed] = CreateWindow("SCROLLBAR",NULL,WS_CHILD | WS_VISIBLE | SBS_HORZ,xwidth*2,(int)(ywidth*4.5),xwidth*15,ywidth*2,hwnd,(HMENU)but_speed,hinstance,NULL);
			SetScrollRange(buttons[but_speed],SB_CTL,7,25,true);
			buttons[res_desktop] = CreateWindow("BUTTON","Desktop",WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,xwidth*26,(int)(ywidth*1.7),xwidth*14,ywidth,hwnd,(HMENU)res_desktop,hinstance,NULL);
			buttons[res_640] = CreateWindow("BUTTON","640 x 480",WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,xwidth*26,(int)(ywidth*2.8),xwidth*14,ywidth,hwnd,(HMENU)res_640,hinstance,NULL);
			buttons[res_800] = CreateWindow("BUTTON","800 x 600",WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,xwidth*26,(int)(ywidth*3.9),xwidth*14,ywidth,hwnd,(HMENU)res_800,hinstance,NULL);
			buttons[res_1024] = CreateWindow("BUTTON","1024 x 768",WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,xwidth*26,(int)(ywidth*5),xwidth*14,ywidth,hwnd,(HMENU)res_1024,hinstance,NULL);
			buttons[res_1280] = CreateWindow("BUTTON","1280 x 1024",WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,xwidth*26,(int)(ywidth*6.1),xwidth*14,ywidth,hwnd,(HMENU)res_1280,hinstance,NULL);
			buttons[res_1600] = CreateWindow("BUTTON","1600 x 1200",WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,xwidth*26,(int)(ywidth*7.2),xwidth*14,ywidth,hwnd,(HMENU)res_1600,hinstance,NULL);
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_PAINT:
			InvalidateRect(hwnd,NULL,TRUE);
			hdc = BeginPaint(hwnd,&ps);
			EndPaint(hwnd,&ps);
			return 0;
		case WM_HSCROLL:
			switch(LOWORD(wParam))
			{
			case SB_TOP:
				EnableWindow(buttons[but_apply],true);
				speed = 7;
				setbuttons();
				return 0;
			case SB_BOTTOM:
				EnableWindow(buttons[but_apply],true);
				speed = 25;
				setbuttons();
				return 0;
			case SB_LINELEFT:
				EnableWindow(buttons[but_apply],true);
				speed--;
				if(speed < 7) speed = 7;
				setbuttons();
				return 0;
			case SB_LINERIGHT:
				EnableWindow(buttons[but_apply],true);
				speed++;
				if(speed > 25) speed = 25;
				setbuttons();
				return 0;
			case SB_PAGELEFT:
				EnableWindow(buttons[but_apply],true);
				speed -= 5;
				if(speed < 7) speed = 7;
				setbuttons();
				return 0;
			case SB_PAGERIGHT:
				EnableWindow(buttons[but_apply],true);
				speed += 5;
				if(speed > 25) speed = 25;
				setbuttons();
				return 0;
			case SB_THUMBPOSITION:
			case SB_THUMBTRACK:
				EnableWindow(buttons[but_apply],true);
				speed = HIWORD(wParam);
				setbuttons();
				return 0;
			}

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case but_ok:
				saveregistry();
				PostQuitMessage(0);
				return 0;
			case but_cancel:
				PostQuitMessage(0);
				return 0;
			case but_apply:
				EnableWindow(buttons[but_apply],false);
				saveregistry();
				return 0;
			case but_help:
				if(checkcode(regcode)) DialogBox(hinstance,"about_boxreg",hwnd,aboutboxproc);
				else DialogBox(hinstance,"about_box",hwnd,aboutboxproc);
				return 0;
			case but_plan:
				EnableWindow(buttons[but_apply],true);
				planetsyn = !planetsyn;
				setbuttons();
				return 0;
			case but_det1:
				EnableWindow(buttons[but_apply],true);
				detail = 1;
				setbuttons();
				return 0;
			case but_det2:
				EnableWindow(buttons[but_apply],true);
				detail = 2;
				setbuttons();
				return 0;
			case but_det3:
				EnableWindow(buttons[but_apply],true);
				detail = 3;
				setbuttons();
				return 0;
			case res_desktop:
				EnableWindow(buttons[but_apply],true);
				resolution = res_desktop;
				setbuttons();
				return 0;
			case res_640:
				EnableWindow(buttons[but_apply],true);
				resolution = res_640;
				setbuttons();
				return 0;
			case res_800:
				EnableWindow(buttons[but_apply],true);
				resolution = res_800;
				setbuttons();
				return 0;
			case res_1024:
				EnableWindow(buttons[but_apply],true);
				resolution = res_1024;
				setbuttons();
				return 0;
			case res_1280:
				EnableWindow(buttons[but_apply],true);
				resolution = res_1280;
				setbuttons();
				return 0;
			case res_1600:
				EnableWindow(buttons[but_apply],true);
				resolution = res_1600;
				setbuttons();
				return 0;
			
			}

	}

	
	return DefWindowProc(hwnd,message,wParam,lParam);
}

LRESULT CALLBACK WndProcsaver(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	unsigned int count;
	switch(message)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return false;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN: 
		case WM_RBUTTONDOWN: 
		case WM_KEYDOWN:
			PostQuitMessage(0);
			return false;
		case WM_TIMER:
			float timepassed;
			QueryPerformanceCounter(&currenttime);
			timepassed = (currenttime.QuadPart - starttime.QuadPart)/(float)timerfreq.QuadPart;
			starttime.QuadPart = currenttime.QuadPart;
			timepassed *= speed*0.1f;
		/*	for(count = 0;count<numbstars;count++)
				bstar[count].movestar(timepassed);
			for(count = 0;count<nummstars;count++)
				mstar[count].movestar(timepassed);
		*/	for(count = 0;count<numbstars;count+=10)
			{
				bstar[count].movestar(timepassed);
				bstar[count+1].movestar(timepassed);
				bstar[count+2].movestar(timepassed);
				bstar[count+3].movestar(timepassed);
				bstar[count+4].movestar(timepassed);
				bstar[count+5].movestar(timepassed);
				bstar[count+6].movestar(timepassed);
				bstar[count+7].movestar(timepassed);
				bstar[count+8].movestar(timepassed);
				bstar[count+9].movestar(timepassed);
			}
			for(count =0;count<nummstars;count+=10)
			{
				mstar[count].movestar(timepassed);
				mstar[count+1].movestar(timepassed);
				mstar[count+2].movestar(timepassed);
				mstar[count+3].movestar(timepassed);
				mstar[count+4].movestar(timepassed);
				mstar[count+5].movestar(timepassed);
				mstar[count+6].movestar(timepassed);
				mstar[count+7].movestar(timepassed);
				mstar[count+8].movestar(timepassed);
				mstar[count+9].movestar(timepassed);
			}
				for(count = 0;count<numfstars;count+=10)
			{
				fstar[count].movestar(timepassed);
				fstar[count+1].movestar(timepassed);
				fstar[count+2].movestar(timepassed);
				fstar[count+3].movestar(timepassed);
				fstar[count+4].movestar(timepassed);
				fstar[count+5].movestar(timepassed);
				fstar[count+6].movestar(timepassed);
				fstar[count+7].movestar(timepassed);
				fstar[count+8].movestar(timepassed);
				fstar[count+9].movestar(timepassed);
			}
			if(planetsyn) plan->moveplan(timepassed);	
			engine.Begin_Frame();
			engine.Lock_Frame();
			/*for(count = 0;count<numbstars;count++)
				bstar[count].drawstar();
			*/
			for(count = 0;count<numbstars;count+=10)
			{
				bstar[count].drawstar();
				bstar[count+1].drawstar();
				bstar[count+2].drawstar();
				bstar[count+3].drawstar();
				bstar[count+4].drawstar();
				bstar[count+5].drawstar();
				bstar[count+6].drawstar();
				bstar[count+7].drawstar();
				bstar[count+8].drawstar();
				bstar[count+9].drawstar();
			}
				for(count = 0;count<nummstars;count+=10)
			{
				mstar[count].drawstar();
				mstar[count+1].drawstar();
				mstar[count+2].drawstar();
				mstar[count+3].drawstar();
				mstar[count+4].drawstar();
				mstar[count+5].drawstar();
				mstar[count+6].drawstar();
				mstar[count+7].drawstar();
				mstar[count+8].drawstar();
				mstar[count+9].drawstar();
			}
			engine.Unlock_Frame();
			/*for(count = 0;count<nummstars;count++)
				mstar[count].drawbigstar();
			*/
			for(count = 0;count<nummstars;count+=10)
			{
				mstar[count].drawbigstar();
				mstar[count+1].drawbigstar();
				mstar[count+2].drawbigstar();
				mstar[count+3].drawbigstar();
				mstar[count+4].drawbigstar();
				mstar[count+5].drawbigstar();
				mstar[count+6].drawbigstar();
				mstar[count+7].drawbigstar();
				mstar[count+8].drawbigstar();
				mstar[count+9].drawbigstar();
			}
				for(count = 0;count<numfstars;count+=10)
			{
				fstar[count].drawstar();
				fstar[count+1].drawstar();
				fstar[count+2].drawstar();
				fstar[count+3].drawstar();
				fstar[count+4].drawstar();
				fstar[count+5].drawstar();
				fstar[count+6].drawstar();
				fstar[count+7].drawstar();
				fstar[count+8].drawstar();
				fstar[count+9].drawstar();
			}
			if(planetsyn) plan->drawplan();
			if(!regyn) 	imagereg->DrawImageStreched(0,height>>1,(width-640),(height/480)*30);
	
			engine.End_Frame();
			engine.Show_Frame();
			return false;
		case WM_MOUSEMOVE:
			if(nummoves > 10) PostQuitMessage(0);
			else nummoves++;
			return false;
		case WM_SYSCOMMAND:
			if(wParam == SC_SCREENSAVE) 
			{
				return false;
			}
		
			
}
return DefWindowProc(hwnd,message,wParam,lParam);
}


int howstart(char *startparam)
{
	if((*startparam) == NULL) return start_set;
	int temp = start_none;
	while((*startparam) != NULL)
	{
		switch (tolower(*startparam))
		{
		case 's':
			temp = start_saver;
			break;
		case 'c':
			temp = start_set;
			break;
		}
		startparam++;
	}


	return temp;
}



int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	HANDLE mutex;
 char name[6] = "stars";
 name[5] = 0;
 mutex = CreateMutex(NULL,TRUE,name);
 if(GetLastError() == ERROR_ALREADY_EXISTS) return 0;
  
	 planetsyn = false;
	detail = 1;
	speed = 1;
	resolution = res_desktop;
	hinstance = hInstance;
	int saverstart;
	saverstart = howstart(lpCmdLine);
	if(saverstart == start_saver)
	{
		SystemParametersInfo(SPI_SETSCREENSAVERRUNNING,true,NULL,0);
		loadregistry();
		regyn = checkcode(regcode);
		switch(resolution)
		{
		case res_desktop:
			width = GetSystemMetrics(SM_CXSCREEN); 
			height = GetSystemMetrics(SM_CYSCREEN);
			break;
		case res_640:
			width = 640;
			height = 480;
			break;
		case res_800:
			width = 800;
			height = 600;
			break;
		case res_1024:
			width = 1024;
			height = 768;
			break;
		case res_1280:
			width = 1280;
			height = 1024;
			break;
		case res_1600:
			width = 1600;
			height = 1200;
			break;
		}

		HDC hdc;
		hdc = GetDC(hwnd);
		bpp = GetDeviceCaps(hdc,BITSPIXEL);
		ReleaseDC(hwnd,hdc);
		MSG msg;
		char appname[6];
		strcpy(appname,"stars");
		WNDCLASS wc;
		wc.style = CS_OWNDC | CS_NOCLOSE;
		wc.lpfnWndProc = WndProcsaver;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
		wc.hCursor = LoadCursor(NULL,IDC_ARROW);
		wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = appname;
		RegisterClass(&wc);
		hwnd = CreateWindowEx(WS_EX_TOPMOST,appname,NULL,WS_POPUP,0,0,width,height,NULL,NULL,hinstance,NULL);
		ShowWindow(hwnd,SW_SHOW);
		SetFocus(hwnd);
		ShowCursor(false);
	//	if(!engine.Startup(hwnd,0))
	//	MessageBox(hwnd,"Could Not Start Directx. Make Sure Directx 3 is Installed","ERROR",MB_OK);
		if(resolution == res_desktop && bpp == 32)
		{
			if(!engine.Startup(hwnd,1,width,height,NULL))
		MessageBox(hwnd,"Could Not Start Directx. Make Sure Directx 3 is Installed","ERROR",MB_OK);
	//	if(!engine.SetDisplayMode(NULL,NULL,NULL))
	//			MessageBox(hwnd,"Could Not Set Display Mode.","ERROR",MB_OK);
		}
		else 
		{
			if(!engine.Startup(hwnd,0,width,height,32))
		MessageBox(hwnd,"Could Not Start Directx. Make Sure Directx 3 is Installed","ERROR",MB_OK);
	//	if(!engine.SetDisplayMode(width,height,32))
		//		MessageBox(hwnd,"Could Not Set Display Mode.","ERROR",MB_OK);
		}
		imagereg = engine.CreateImageObject();
		imagereg->ALoadImage("unregbmp",640,30);
		if(detail == 1) 
		{
			numbstars = 1000;
			nummstars = 100;
			numfstars = 10;
		}
		if(detail == 2) 
		{
			numbstars = 2000;
			nummstars = 200;
			numfstars = 20;
		}
		if(detail == 3) 
		{
			numbstars = 4000;
			nummstars = 400;
			numfstars = 40;
		}
		if(planetsyn) 
		{
			plan = new planet;
			plan->setres(width,height);
		}
		bstar = new backstar[numbstars];
		mstar = new midstar[nummstars];
		fstar = new forestar[numfstars];
		
		bstar[0].setres(width,height);
		mstar[0].setres(width,height);
		fstar[0].setres(width,height);
		unsigned int count,count2;
		for(count2 = 0;count2 < (numbstars/10);count2++)
		{
			for(count = 0;count<(count2*10);count++)
				bstar[count].movestar(speed*0.001f);
			for(count = 0;count<(count2);count++)
				mstar[count].movestar(speed*0.001f);
			for(count = 0;count<(count2/10);count++)
				fstar[count].movestar(speed*0.001f);
		}
		loadstarimages();
		//Sleep(1000);
		QueryPerformanceFrequency(&timerfreq);
		QueryPerformanceCounter(&starttime);
		while(PeekMessage(&msg,NULL,WM_MOUSEMOVE,WM_MOUSEMOVE,PM_REMOVE) != 0)
		{
		}
		
		SetTimer(hwnd,1,25,NULL);
		while(GetMessage(&msg,NULL,0,0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			KillTimer(hwnd,1);
			delete [] bstar;
			delete [] fstar;
			delete [] mstar;
			destroystarimages();
			engine.DestroyImageObject(imagereg);
			engine.ShutDown();	
		ShowCursor(true);
		SystemParametersInfo(SPI_SETSCREENSAVERRUNNING,false,NULL,0);
	}
	if(saverstart == start_set)
	{
		MSG msg;
		char appname[6];
		loadregistry();
		strcpy(appname,"stars");
		WNDCLASS wc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProcset;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
		wc.hCursor = LoadCursor(NULL,IDC_ARROW);
		wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = appname;
		RegisterClass(&wc);
		hwnd = CreateWindowEx(WS_EX_TOPMOST,appname,"Stars Setup",WS_OVERLAPPEDWINDOW ,CW_USEDEFAULT,CW_USEDEFAULT,350,300,NULL,NULL,hinstance,NULL);
		//createbuttons();
		ShowWindow(hwnd,SW_SHOW);
		SetFocus(hwnd);
		UpdateWindow(hwnd);
		setbuttons();
		while(GetMessage(&msg,NULL,0,0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	
	}
	ReleaseMutex(mutex);
	CloseHandle(mutex);
	return 0;
}



