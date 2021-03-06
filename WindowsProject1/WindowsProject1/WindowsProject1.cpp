// WindowsProject1.cpp: 응용 프로그램의 진입점을 정의합니다.
#include "stdafx.h"
#include "WindowsProject1.h"
#include "Commdlg.h"
using namespace std;

#define MAX_LOADSTRING 100

const int MYTEXT = 0x12348;
const int MYTEXT2 = 0x12347;
const int MYCHOICE = 0x12350;

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
HWND mWnd;
BOOL                InitInstance(HINSTANCE, int);
INT_PTR	CALLBACK	Sort_Dlg(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR	CALLBACK	ChoiceReplace_Dlg(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Replace_Dlg(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	FileReplace_Dlg(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // 기본 메시지 루프입니다.
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));


    return RegisterClassExW(&wcex);
}
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
   int midx = GetSystemMetrics(SM_CXSCREEN);
   int midy = GetSystemMetrics(SM_CYSCREEN);
   midx = midx / 2 - 160;
   midy = midy / 2 - 120;

  HWND hWnd = mWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED|WS_MINIMIZEBOX|WS_SYSMENU,
      midx, midy, 360, 240, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
int ReplaceOne(HWND hWnd, TCHAR*, TCHAR*);
int ChoiceReplace(HWND hWnd);
int RandomSort(HWND hWnd);
int ReplaceWithFile(HWND);
int myst = 0, myst2 = 0; //사용자 지정 옵션
int DO = 0; // 그래서 수행하는 지 체크

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	static TCHAR f[100], t[100];
	static bool chk = 0;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case IDM_SORT:
				if (RandomSort(hWnd)) MessageBox(hWnd, _T("완료"), _T("성공"), MB_OK);
				break;
			case IDM_REPLACE:
				ChoiceReplace(hWnd);
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
			RECT rt ;
			GetClientRect(hWnd, &rt);
			TCHAR warn[200]=_T(
				"\n\n 변환할 텍스트파일은 메모장으로 \n \"다른이름으로 저장 ->유니코드\"\n 로 저장되어있어야합니다\n"
				"\n\n\n made by gaelim\n for eeee님") ;
			HFONT hfont = CreateFont(15, 0, 0, 0, 0, 400, 0, 0, 0, 0, 0, 0, 0,_T("consolas"));
			SetBkMode(hdc,TRANSPARENT);
			SelectObject(hdc, hfont);
			DrawText(hdc, warn, _tcslen(warn), &rt, DT_BOTTOM | DT_LEFT);
            EndPaint(hWnd, &ps);
        }
        break;
	case MYTEXT:
		_tcscpy_s(f, sizeof(f),(TCHAR *)lParam); break;
	case MYTEXT2:
		_tcscpy_s(t, sizeof(f),(TCHAR *)lParam); break;
	case MYCHOICE:
		if (((int)lParam) == 1) {
			if (ReplaceOne(hWnd, f, t))  MessageBox(hWnd, _T("완료"), _T("성공"), MB_OK); 
		}
		else if (((int)lParam) == 2) if (ReplaceWithFile(hWnd)) MessageBox(hWnd, _T("완료"), _T("성공"), MB_OK);
		else MessageBox(hWnd, _T("취소"), _T("경고"), MB_OK);
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Sort_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	static bool okbttn = 0;
	HWND bttn = GetDlgItem(hDlg, IDOK);
	switch (message) {
	case WM_INITDIALOG:
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO3);
		myst2=4;
		return (INT_PTR)1;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			DO= GetDlgItemInt(hDlg, IDC_EDIT1, 0,0);
			if (DO <= 0) {
				MessageBox(hDlg, _T("파일 개수 1 이상의 값을 입력해주세요"),
					_T("메시지박스"), MB_OK);
				break;
			}
			okbttn = 0;
			EndDialog(hDlg, 0); break;
		case IDCANCEL:
			EndDialog(hDlg, 0); okbttn = 0; DO = 0; break;
		case IDC_CHECK1:
			myst ^= (1 << 0); break;
		case IDC_CHECK2:
			myst ^= (1 << 1); break;
		case IDC_CHECK3:
			myst ^= (1 << 2); break;
		case IDC_CHECK4:
			myst ^= (1 << 3); break;
		case IDC_CHECK5:
			myst ^= (1 << 4); break;
		case IDC_RADIO1:
			myst2 = 1; EnableWindow(bttn, 1); 
			EnableWindow(GetDlgItem(hDlg, IDC_CHECK3), 0); 
			EnableWindow(GetDlgItem(hDlg, IDC_CHECK4), 0); 
			break;
		case IDC_RADIO2:
			myst2 = 2; EnableWindow(bttn, 1); 
			EnableWindow(GetDlgItem(hDlg, IDC_CHECK3), 0); 
			EnableWindow(GetDlgItem(hDlg, IDC_CHECK4), 0); 
			break;
		case IDC_RADIO3:
			myst2 = 4; EnableWindow(bttn, 1); 
			EnableWindow(GetDlgItem(hDlg, IDC_CHECK3), 1); 
			EnableWindow(GetDlgItem(hDlg, IDC_CHECK4), 1); 
			break;
		}
		break;
	}
	return 0;
}

INT_PTR CALLBACK Replace_Dlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	static TCHAR f[100];
	static TCHAR t[100];
	switch (message) {
	case WM_INITDIALOG:
		return (INT_PTR)1;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			GetDlgItemText(hDlg, IDC_EDIT1, f, 100);
			GetDlgItemText(hDlg, IDC_EDIT2, t, 100);
			PostMessage(mWnd, MYTEXT, sizeof(f),(LPARAM)f);
			PostMessage(mWnd, MYTEXT2, sizeof(t),(LPARAM)t);
			DO = 1;
			EndDialog(hDlg, 0);
			break;
		case IDCANCEL:
			memset(f, 0, sizeof(f)); memset(t, 0, sizeof(t));
			DO = 0;
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}

INT_PTR CALLBACK ChoiceReplace_Dlg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static int my;
	switch (message) {
	case WM_INITDIALOG:
		CheckRadioButton(hWnd, IDC_RADIO4, IDC_RADIO5, IDC_RADIO5);
		my = 2;
		return (INT_PTR)1;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			EndDialog(hWnd, 0);
			PostMessage(mWnd, MYCHOICE, sizeof(my), (LPARAM)my);
			break;
		case IDCANCEL:
			EndDialog(hWnd, 0);
			my = 0;
			PostMessage(mWnd, MYCHOICE,sizeof(my), (LPARAM)my);
			break;
		case IDC_RADIO4: my = 1; break;
		case IDC_RADIO5: my = 2; break;
		}
		break;
	}
	return 0;
}

int RandomSort(HWND hWnd) {
	OPENFILENAME OFN;
	static TCHAR str[1000];
	static TCHAR buf[(int)(5e7)+1];
	TCHAR lpstrFile[1000]=_T("");
	TCHAR filter[] = _T("Text File\0*.txt\0");

	MyRegisterOFN(&OFN, hWnd, filter, lpstrFile);
	int tmp = GetOpenFileName(&OFN);
	if (tmp) {
		myst = 0, myst2 = 0;
		DialogBox(hInst, MAKEINTRESOURCE(IDD_WINDOWSPROJECT1_DIALOG), hWnd, Sort_Dlg);

		memset(buf, 0, sizeof(buf));
		ReadFromFile(OFN.lpstrFile, buf);
		vector<wstring> v;
		myWordCnt(buf, v);
		struct Shuffle s(v, myst, myst2);

		vector<wstring> special; // 특수문자 처리
		if (DO == 0) return 0;
		if (myst & 1 << 4) {
			MessageBox(hWnd, _T("특수문자 패턴파일을 열어주세요"), _T("특수문자 패턴파일"), MB_OK);
			int tmp2 = GetOpenFileName(&OFN);
			if (!tmp2) return 0;
			FILE* fp; 
			_tfopen_s(&fp, OFN.lpstrFile, _T("rb, ccs=UNICODE"));
			while (fgetws(buf, sizeof(buf), fp)) {
				wstring wtmp = wstring(buf);
				int pos = wtmp.find(_T("\r\n"));
				if (pos >= 0) wtmp.erase(pos, 2);
				special.push_back(wtmp);
			}
			fclose(fp);
		}
		s.init(special);

		BROWSEINFO bi;
		memset(&bi, 0, sizeof(bi));
		bi.lpszTitle = _T("폴더 지정하기.");
		bi.ulFlags = BIF_RETURNONLYFSDIRS| BIF_NEWDIALOGSTYLE;
		LPITEMIDLIST pIDL = SHBrowseForFolder(&bi);
		if (pIDL){
			TCHAR dirpath[MAX_PATH]; memset(dirpath, 0, sizeof(dirpath));

			SHGetPathFromIDList(pIDL, dirpath);
			for (int i = 0; i < DO; i++) {
				s.sort();
				TCHAR newfname[MAX_PATH];
				_stprintf_s(newfname, _T("%s\\%d.txt"), dirpath,i);

				FILE* fp;
				_tfopen_s(&fp, newfname, _T("w, ccs=UNICODE"));
				for (int j = 0; j < s.group; j++) {
					if (s.sv[j].rn){
						_ftprintf_s(fp, _T("%s\r\n"), s.sv[j].w.c_str());
					} 
					else _ftprintf_s(fp, _T("%s"), s.sv[j].w.c_str());
				}
				fclose(fp);
			}
		}
		else return 0; 
	}
	else { MessageBox(hWnd, _T("파일열기실패!"), _T("경고"), MB_OK); return 0; }
	return 1;
}

int ChoiceReplace(HWND hWnd) {
	DialogBox(hInst,MAKEINTRESOURCE(IDD_CHOICEREPLACEDLG), hWnd, ChoiceReplace_Dlg);
	return 1;
}

int ReplaceOne(HWND hWnd, TCHAR* f, TCHAR* t){
	OPENFILENAME OFN;
	static TCHAR str[1000];
	static TCHAR buf[(int)(5e7)+1];
	TCHAR lpstrFile[1000]=_T("");
	TCHAR filter[] = _T("Text File\0*.txt\0");

	MyRegisterOFN(&OFN, hWnd, filter, lpstrFile);
	int tmp = 0;
	tmp = GetOpenFileName(&OFN);

	if (tmp) {
		DialogBox(hInst, MAKEINTRESOURCE(IDD_REPLACEDLG), hWnd, Replace_Dlg);

		memset(buf, 0, sizeof(buf));
		ReadFromFile(lpstrFile, buf);
		if (DO == 0) return 0;

		BROWSEINFO bi;
		memset(&bi, 0, sizeof(bi));
		bi.lpszTitle = _T("폴더 지정하기.");
		bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
		LPITEMIDLIST pIDL = SHBrowseForFolder(&bi);
		if (pIDL) {
			TCHAR dirpath[MAX_PATH]; memset(dirpath, 0, sizeof(dirpath));

			SHGetPathFromIDList(pIDL, dirpath);
			TCHAR newfname[MAX_PATH];
			_stprintf_s(newfname, _T("%s\\Replace.txt"), dirpath);

			FILE* fp;
			_tfopen_s(&fp, newfname, _T("w, ccs=UNICODE"));

			wstring tmp = wstring(buf);
			int now = 0;
			int flen = _tcslen(f);
			while (1) {
				int found = tmp.find(f, now);

				if (found < 0) {
					int len = _tcslen(buf);
					for (int i = now; i < len; i++) {
						_ftprintf_s(fp, _T("%c"), tmp[i]);
					}
					break;
				}
				for (int i = now; i < found; i++) {
					_ftprintf_s(fp, _T("%c"), tmp[i]);
				}
				_ftprintf_s(fp, _T("%s"), t);
				now = found + flen;
			}
			fclose(fp);
		}
	}
	else {
		MessageBox(hWnd, _T("파일열기실패!"), _T("경고"), MB_OK); return 0;
	}
	return 1;
}

int ReplaceWithFile(HWND hWnd) {
	OPENFILENAME OFN;
	static TCHAR str[1000];
	static TCHAR buf[(int)(5e7)+1];
	TCHAR lpstrFile[1000]=_T("");
	TCHAR filter[] = _T("Text File\0*.txt\0");
	MyRegisterOFN(&OFN, hWnd, filter, lpstrFile);

	MessageBox(hWnd, _T("바꿀 파일을 선택해주세요"), _T("바꿀 파일을 선택해주세요"), MB_OK);
	int chk = GetOpenFileName(&OFN);
	if (chk) {
		ReadFromFile(lpstrFile, buf);
		MessageBox(hWnd, _T("패턴 파일을 선택해주세요"), _T("패턴 파일을 선택해주세요"), MB_OK);
		wstring str = wstring(buf);

		int chk2 = GetOpenFileName(&OFN);
		if (!chk2) return 0;

		FILE* fp;
		_tfopen_s(&fp, OFN.lpstrFile, _T("rb, ccs=UNICODE"));

		static TCHAR f[200], t[200], tmp[200];
		memset(tmp, 0, sizeof(tmp));
		int st = 2;

		while (fgetws(tmp, sizeof(tmp),fp)) {

			if (tmp[0] == tmp[1] && tmp[1] == _T('/')) continue;
			memset(f, 0, sizeof(f)); 
			memset(t, 0, sizeof(t));

			wstring tmpw = wstring(tmp);
			reverse(tmpw.begin(), tmpw.end());
			int pos = tmpw.find(_T("=="));
			if (pos < 0) continue;
			wstring tmpf = tmpw.substr(pos);

			tmpf.erase(0, 2);
			while (tmpf[0] == _T(' '))  tmpf.erase(0, 1);
			reverse(tmpf.begin(), tmpf.end());
			while (tmpf[0] == _T(' '))  tmpf.erase(0, 1);

			reverse(tmpw.begin(), tmpw.end());
			pos=tmpw.find(_T("=="));
			if (pos < 0) continue;
			wstring tmpt = tmpw.substr(pos);

			tmpt.erase(0, 2);
			while (tmpt[0] == _T(' '))  tmpt.erase(0, 1);
			reverse(tmpt.begin(), tmpt.end());
			while (tmpt[0] == _T(' '))  tmpt.erase(0, 1);
			reverse(tmpt.begin(), tmpt.end());

			int nlposf = tmpf.find(_T("\r\n"));
			int nlpost = tmpt.find(_T("\r\n"));
			if (nlposf >= 0) tmpf.erase(nlposf, 2);
			if (nlpost >= 0) tmpt.erase(nlpost, 2);

			int flen = tmpf.length(), tlen = tmpt.length();
			_tcscpy_s(t, sizeof(t), tmpt.c_str());
			_tcscpy_s(f, sizeof(f), tmpf.c_str());

			int len = str.length(), now = 0;
			while (1) {
				int pos = str.find(f, now);

				if (pos < 0) break;
				str.erase(pos, flen);
				str.insert(pos, t);
				now = pos + tlen;
			}

			memset(tmp, 0, sizeof(tmp));
		}
		fclose(fp);
		BROWSEINFO bi;
		memset(&bi, 0, sizeof(bi));
		bi.lpszTitle = _T("폴더 지정하기.");
		bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
		LPITEMIDLIST pIDL = SHBrowseForFolder(&bi);
		if (pIDL) {
			TCHAR dirpath[MAX_PATH]; memset(dirpath, 0, sizeof(dirpath));

			SHGetPathFromIDList(pIDL, dirpath);
			TCHAR newfname[MAX_PATH];
			_stprintf_s(newfname, _T("%s\\Replace.txt"), dirpath);

			FILE* fp;
			_tfopen_s(&fp, newfname, _T("w, ccs=UNICODE"));
			_ftprintf_s(fp, _T("%s"), str.c_str());
			fclose(fp);
		}
		else return 0;
	}
	else {
		MessageBox(hWnd, _T("파일열기실패!"), _T("경고"), MB_OK); return 0;
	}
	return 1;
}


/* 내 첫번쨰 작업 478  line . 함수 replace With File
if (st == 2){
	if (tmp[0] == _T('[')) {
		st = 1;
		memset(f, 0, sizeof(f)), memset(t, 0, sizeof(t));
		fgetws(f, sizeof(f), fp); 
		fgetws(t, sizeof(t), fp);

		wstring tmpf = wstring(f);
		wstring tmpt = wstring(t);
		
		int nlposf = tmpf.find(_T("\r\n"));
		int nlpost = tmpt.find(_T("\r\n"));
		if (nlposf >= 0) tmpf.erase(nlposf, 2);
		if (nlpost >= 0) tmpt.erase(nlpost, 2);

		int flen = tmpf.length(), tlen = tmpt.length();
		_tcscpy_s(t, sizeof(t), tmpt.c_str());
		_tcscpy_s(f, sizeof(f), tmpf.c_str());

		int len = str.length(), now = 0;
		while (1) {
			int pos = str.find(f, now);

			if (pos < 0) break;
			str.erase(pos, flen);
			str.insert(pos, t);
			now = pos + tlen;
		}
	}
}
else if (st == 1) if(tmp[0] == _T(']')) st=2;
*/
