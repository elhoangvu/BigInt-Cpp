#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <string>
#include <fstream>
using namespace std;

#define MAX_NUM "170141183460469231731687303715884105727"

string randBinary()
{
	int n = rand() % 128 + 1;
	string kq = "";
	for (int i = 0; i < n; i++)
	{
		int m = rand() % 2;
		kq.push_back(m + '0');
	}
	return kq;

}
char ChuyenKiTuSangHex(int m)
{
	// m<10 thi chuyen sang ki tu thi cong them 48
	if (m < 10)
		return m + 48;
	//m>=10 thi cong them 55 cho ki tu In
	return m + 55;
}
string randHexa()
{
	// Chi co 32 hoac 33 chu so thoi
	int n = rand() % 32 + 1;
	string kq = "";
	for (int i = 0; i < n; i++)
	{
		int m = rand() % 16;
		kq.push_back(ChuyenKiTuSangHex(m));
	}
	return kq;
}
int SoSanh2Chuoi(string a, string b)
{
	if (a[0] == '-')
		a.erase(0, 1);
	if (b[0] == '-')
		b.erase(0, 1);
	if (a.length()>b.length())
		return 1;
	if (a.length() < b.length())
		return -1;
	for (int i = 0; i < a.length(); i++)
	{
		if (a[i]>b[i])
			return 1;
		if (a[i] < b[i])
			return -1;
	}
	return 0;
}
string randDecimal(string max)
{
	string kq = "";
	do
	{
		kq = "";
		int n = rand() % 2;
		if (n == 1)
			kq.push_back('-');
		n = rand() % (max.length()) + 1;
		for (int i = 0; i < n; i++)
		{
			int m = rand() % 10;
			kq.push_back(m + 48);
		}
		// xoa cac so 0 dau   vd:  000000112
		while (kq[1] == '0'&&kq.length()>1)
			kq.erase(1, 1);
		// lo co trung hop" -00000000000000000000"
		if (kq[0] == '-')
		{
			if (kq.length() == 1)
				kq = "0";
		}
		if (kq[0] == '0' && kq.length() != 1)
		{
			kq.erase(0, 1);
		}
	} while (SoSanh2Chuoi(kq, max) >= 1);
	return kq;
}

string randNum(int type)
{
	switch (type)
	{
	case 2:
		return randBinary();
	case 10:
		return randDecimal(MAX_NUM);
	case 16:
		return randHexa();
	}
	return "";
}

void randFileTest(ostream& os, int n)
{
	for (int i = 0; i < n; i++)
	{
		string num1, num2;
		int type1 = rand() % 3;

		switch (type1)
		{
		case 0:
			os << 2 << " ";
			num1 = randNum(2);
			num2 = randNum(2);
			break;
		case 1:
			os << 10 << " ";
			num1 = randNum(10);
			num2 = randNum(10);
			break;
		case 2:
			os << 16 << " ";
			num1 = randNum(16);
			num2 = randNum(16);
			break;
		}

		int type2 = rand() % 7;
		if (type2 != 6)
		{
			switch (type2)
			{
			case 0:
				os << 2 << " ";
				break;
			case 1:
				os << 10 << " ";
				break;
			case 2:
				os << 16 << " ";
				break;
			case 3:
				os << '~' << " ";
				break;
			case 4:
				os << "ror" << " ";
				break;
			case 5:
				os << "rol" << " ";
				break;
			default:
				os << "10 ";
				break;
			}
			os << num1;
		}
		else
		{
			os << num1 << " ";
			int type3 = rand() % 9;
			switch (type3)
			{
			case 0:
				os << "+ " << num2;
				break;
			case 1:
				os << "- " << num2;
				break;
			case 2:
				os << "* " << num2;
				break;
			case 3:
				os << "/ " << num2;
				break;
			case 4:
				os << "& " << num2;
				break;
			case 5:
				os << "^ " << num2;
				break;
			case 6:
				os << "| " << num2;
				break;
			case 7:
				os << "<< " << rand() % 128 + 1;
				break;
			case 8:
				os << ">> " << rand() % 128 + 1;
				break;
			}
			
		}
		if (i != n - 1)
		{
			os << endl;
		}
	}

}

void main()
{

	fstream f;
	f.open("OUTPUT.TXT", ios::out);
	srand(time(NULL));

	randFileTest(f, 1000);

	f.close();
	/*cout << randBinary() << endl;
	cout << randHexa() << endl;
	cout << randDecimal("12345678987654321") << endl;*/
	system("pause");
}