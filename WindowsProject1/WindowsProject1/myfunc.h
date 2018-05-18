#pragma once
#include  <commdlg.h>
#include <tchar.h>
#include <time.h>
using namespace std;

void MyRegisterOFN(OPENFILENAME *OFN, HWND hWnd, TCHAR filter[], TCHAR lpstrFile[]);
void ReadFromFile(TCHAR[], TCHAR[]);
void myWordCnt(TCHAR[],vector<wstring>& );

struct Shuffle {
	struct Word {
		int line, th;
		bool fcom, bcom, rn;

		wstring w;
		Word(wstring s, int l, int t, bool a, bool b, bool rn) : w(s), line(l), th(t), fcom(a), bcom(b) , rn(rn){}
		Word() {
			w = _T("");
			line = th = 0;
			fcom = bcom = rn = 0;
		}
	};
	int op1, op2, group;
	vector<bool> chk;
	vector<Word> sv;
	vector<pair<int, int>>  linesector;

	int ret(int a) { // 그룹 단어 갯수 리턴
		if (a == 2) return 3; // 옵션이 2일 경우, 3단어 그룹이다.
		else if (a == 1) return 2;// 옵션이 1일경우 2단어 그룹이다.
	}

	Shuffle(vector<wstring>& v, int option1, int option2) : op1(option1), op2(option2){
		sv.resize(v.size());

		vector<wstring> tmp;
		int l=0, th=0; // l : 라인, th : 해당 줄의 몇 번째 단어인가.
		bool fcom = 0, bcom = 0;
		int cnt = 0; // fcom : front comma? , bcom : before comma? cnt : 그룹 단어 갯수 세기 
		group = 0;

		for (int i = 0; i < v.size(); i++) {  //v 는 (,) (.) (스페이스) (\n) 단위로 들어옴. 스페이스 빼곤 다 wstring에 포함시킴

			cnt++;
			int nowlen = v[i].length();

			if ((int)(v[i].find(_T('.'))) >= 0 || (int)(v[i].find(_T(','))) >= 0) bcom = 1; //점 앞단어

			int pos = v[i].find(_T("\r\n"));
			if (pos >= 0) {
				v[i].erase(pos, 2); // 현재 단어에 개행이 있따면, 개행은 잠깐 삭제해놓고 변수 저장
				reverse(v[i].begin(), v[i].end());
				while (v[i][0] == ' ') v[i].erase(0, 1);
				reverse(v[i].begin(), v[i].end());
				v[i] += L" ";
			}
			if (th == 0 && cnt==1) linesector.push_back({group,0}); //라인 섹터, 줄 시작일 때 first => 현재 그룹 단위 카운트

			if (option2 & 3) {  // 2, 3 단어
				if (cnt < ret(option2) && pos <0) { //단어가 2~3 세어졌거나, 개행이 있을경우.
					tmp.push_back(v[i]); //tmp에 v[i], 몇번쨰 줄, 몇번쨰 단어인지. 단어반복자 일단 저장해놓음
				}
				else { //개행이거나 그룹 단위를 다 채웠을 때

					tmp.push_back(v[i]);
					Word add(_T(""), l, th, 0, 0, pos>=0); // Word initialize
					for (int j = 0; j < tmp.size(); j++) add.w += tmp[j];
					
					sv[group]=add; // 단어

					tmp.clear();
					cnt = 0;
					th++; //th 는 줄에서 몇번쨰 단어인지
					if (pos >= 0) { // 현재 단어에 개행이 있ㄷㅏ면 
						l++, th = 0;
						linesector[linesector.size() - 1].second = group;
					}
					group++;//, group은 단어 단위 반복자(i) 와 같은 개념.
				}
			}
			else{ // 1단어 단위씩 끊기.
				sv[group]={v[i], l, th, fcom, bcom, (pos >= 0)};
				bcom = 0, fcom = 0, th++, cnt=0;
				if (pos >= 0) { // 현재 단어에 개행이 있ㄷㅏ면 
					l++, th = 0;
					linesector[linesector.size() - 1].second = group;
				}
				group++;
			}

			if ((int)v[i].find(_T('.'))>=0 || (int)v[i].find(_T(','))>=0) fcom= 1; //점 뒤단어
		}

		if (option2 & 3) { // 두 세개 단어일 때 개행도 아니고 카운트도 다 못채웠을떄 tmp 벡터가 남아있다. 그 짜투리 다 더해서 넣어줌
			if (tmp.size() >= 1) {
				Word add(_T(""), l, th, 0, 0, 0); // Word initialize
				for (int i = 0; i < tmp.size(); i++) add.w += tmp[i];
				sv[group]=add;
				group++;
			}
		}
		linesector[linesector.size() - 1].second = group;
	};

	void init(vector<wstring>& s) {
		chk.resize(sv.size());
		fill(chk.begin(), chk.end(), 1);
		int n = s.size();

		for (int i = 0; i < group; i++) {
			if (op1 & 2) if (sv[i].th == 0) chk[i] = 0;
			if (op1 & 4) if (sv[i].bcom)    chk[i] = 0; // 점 앞단어
			if (op1 & 8) if (sv[i].fcom)   chk[i] = 0; // 점 뒤 단어.
			if (op1 & 16) {
				for (int j = 0; j < n; j++) {
					int tmp;
					while ((tmp=sv[i].w.find(s[j]))>=0) sv[i].w.erase(tmp,s[j].length());
				}
			}
		}
	};

	void sort() {
		srand(time(NULL));
		if (op1 & 1) {  // 만약 같은줄 섞기를 한다면, line sector 정보를 이용한다
			for (int i = 0; i < group; i++) {
				if (!chk[i]) continue;

				pair<int, int> nowsector = linesector[sv[i].line]; // start ~ end. 
				int j = rand() % (nowsector.second - nowsector.first + 1);
				j += nowsector.first;
				if (chk[j]) {
					swap(sv[i], sv[j]);
					swap(sv[i].rn, sv[j].rn);
				};
			};
		}
		else {
			for (int i = 0; i <group; i++) {
				if (!chk[i]) continue;

				int j = rand() % group;
				if (chk[j]) {
					swap(sv[i], sv[j]);
					swap(sv[i].rn, sv[j].rn);
				}
			}
		}
	}
};