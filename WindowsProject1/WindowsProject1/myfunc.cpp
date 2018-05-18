#include "myfunc.h"
#include "stdafx.h"
#include <vector>
// 내가 사용한 함수 
using namespace std;

void MyRegisterOFN(OPENFILENAME *OFN, HWND hWnd,TCHAR filter[], TCHAR lpstrFile[]) {

	memset(OFN, 0, sizeof(OPENFILENAME));
	OFN->lStructSize = sizeof(OPENFILENAME);
	OFN->hwndOwner = hWnd;
	OFN->lpstrFilter = filter;
	OFN->lpstrFile = lpstrFile;
	OFN->nMaxFile = 1000;
	OFN->lpstrInitialDir = _T(".");
}

void ReadFromFile(TCHAR filepath[], TCHAR buf[]) {

	HANDLE hFile;
	DWORD size = 5e7;
	hFile = CreateFile(
		filepath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0
	);
	ReadFile(hFile, buf, (size) * sizeof(TCHAR), &size, NULL);
	buf[size] = 0;
	CloseHandle(hFile);
}

void myWordCnt(TCHAR s[], vector<wstring>& v){
	bool st = 1;
	int sz=0;
	if (s[0]==_T(' ')) st=0;

	//wstring tmp;
	wstring tmp;
	int slen = _tcslen(s);

	for (int i = 1; i < slen; i++) {
		if (s[i] == _T(' ') || s[i]==_T('\r') || s[i]==_T('\n') || s[i]==_T('.') || s[i]==_T(',')) {
			if (st ==1) {
				st = 0;
				tmp += s[i];
				v.push_back(tmp);
				sz++;
				tmp.clear();
			}
			else v[sz-1] += s[i];
		}
		else {
			st = 1;
			tmp += s[i];
		}
	}
	v.push_back(tmp);
	return;
}

