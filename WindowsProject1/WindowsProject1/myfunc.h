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

	int ret(int a) { // �׷� �ܾ� ���� ����
		if (a == 2) return 3; // �ɼ��� 2�� ���, 3�ܾ� �׷��̴�.
		else if (a == 1) return 2;// �ɼ��� 1�ϰ�� 2�ܾ� �׷��̴�.
	}

	Shuffle(vector<wstring>& v, int option1, int option2) : op1(option1), op2(option2){
		sv.resize(v.size());

		vector<wstring> tmp;
		int l=0, th=0; // l : ����, th : �ش� ���� �� ��° �ܾ��ΰ�.
		bool fcom = 0, bcom = 0;
		int cnt = 0; // fcom : front comma? , bcom : before comma? cnt : �׷� �ܾ� ���� ���� 
		group = 0;

		for (int i = 0; i < v.size(); i++) {  //v �� (,) (.) (�����̽�) (\n) ������ ����. �����̽� ���� �� wstring�� ���Խ�Ŵ

			cnt++;
			int nowlen = v[i].length();

			if ((int)(v[i].find(_T('.'))) >= 0 || (int)(v[i].find(_T(','))) >= 0) bcom = 1; //�� �մܾ�

			int pos = v[i].find(_T("\r\n"));
			if (pos >= 0) {
				v[i].erase(pos, 2); // ���� �ܾ ������ �ֵ���, ������ ��� �����س��� ���� ����
				reverse(v[i].begin(), v[i].end());
				while (v[i][0] == ' ') v[i].erase(0, 1);
				reverse(v[i].begin(), v[i].end());
				v[i] += L" ";
			}
			if (th == 0 && cnt==1) linesector.push_back({group,0}); //���� ����, �� ������ �� first => ���� �׷� ���� ī��Ʈ

			if (option2 & 3) {  // 2, 3 �ܾ�
				if (cnt < ret(option2) && pos <0) { //�ܾ 2~3 �������ų�, ������ �������.
					tmp.push_back(v[i]); //tmp�� v[i], ����� ��, ����� �ܾ�����. �ܾ�ݺ��� �ϴ� �����س���
				}
				else { //�����̰ų� �׷� ������ �� ä���� ��

					tmp.push_back(v[i]);
					Word add(_T(""), l, th, 0, 0, pos>=0); // Word initialize
					for (int j = 0; j < tmp.size(); j++) add.w += tmp[j];
					
					sv[group]=add; // �ܾ�

					tmp.clear();
					cnt = 0;
					th++; //th �� �ٿ��� ����� �ܾ�����
					if (pos >= 0) { // ���� �ܾ ������ �֤����� 
						l++, th = 0;
						linesector[linesector.size() - 1].second = group;
					}
					group++;//, group�� �ܾ� ���� �ݺ���(i) �� ���� ����.
				}
			}
			else{ // 1�ܾ� ������ ����.
				sv[group]={v[i], l, th, fcom, bcom, (pos >= 0)};
				bcom = 0, fcom = 0, th++, cnt=0;
				if (pos >= 0) { // ���� �ܾ ������ �֤����� 
					l++, th = 0;
					linesector[linesector.size() - 1].second = group;
				}
				group++;
			}

			if ((int)v[i].find(_T('.'))>=0 || (int)v[i].find(_T(','))>=0) fcom= 1; //�� �ڴܾ�
		}

		if (option2 & 3) { // �� ���� �ܾ��� �� ���൵ �ƴϰ� ī��Ʈ�� �� ��ä������ tmp ���Ͱ� �����ִ�. �� ¥���� �� ���ؼ� �־���
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
			if (op1 & 4) if (sv[i].bcom)    chk[i] = 0; // �� �մܾ�
			if (op1 & 8) if (sv[i].fcom)   chk[i] = 0; // �� �� �ܾ�.
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
		if (op1 & 1) {  // ���� ������ ���⸦ �Ѵٸ�, line sector ������ �̿��Ѵ�
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