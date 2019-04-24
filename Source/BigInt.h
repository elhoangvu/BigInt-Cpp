#pragma once
#include <iostream>
#include <string>
#include "MathHelper.h"
using namespace std;

class BigInt
{
private:
	// Sử dụng mảng 2 phần tử long long (__int64) để lưu dữ liệu
	// Tổng cộng 16 byte ~ 128 bit
	// Thứ tự bit được đọc từ phải sang trái ngược với thứ tự mảng
	// arrBit[0] chứa các bit từ 64 đến 127 từ phải sang
	// arrBit[1] chứa các bit từ 0 đến 63 từ phải sang
	long long arrBit[2];
public:

	/////////////////////////////////////////////////////////////////////
	// Nhóm các toán tử
	//
	// Toán tử + (Cộng)
	BigInt operator + (BigInt x);
	// Toán tử - (Trừ)
	BigInt operator - (BigInt x);
	// Toán tử * (Nhân)
	BigInt operator * (BigInt x);
	// Toán tử / (Chia)
	BigInt operator / (BigInt x);

	// Toán tử = (gán bằng)
	BigInt operator = (BigInt x);
	// Toán tử & (AND)
	BigInt operator & (BigInt x);
	// Toán tử | (OR)
	BigInt operator | (BigInt x);
	// Toán tử ^ (XOR)
	BigInt operator ^ (BigInt x);
	// Toán tử ~ (NOT)
	BigInt operator ~ ();

	// Toán tử >> (Dịch phải)
	BigInt operator >> (int n);
	// Toán tử << (Dịch trái)
	BigInt operator << (int n);

	/////////////////////////////////////////////////////////////////////
	// Nhóm các phương thức
	//
	// Phương thức lấy bit thứ n
	bool getBit(int n);
	// Phương thức gán bit 1 ở vị trí n
	void setBit1(int n);

	// Phương thức xoay phải
	BigInt ror();
	// Phương thức xoay trái
	BigInt rol();

	// Phương thức chuyển ra hệ nhị phân
	string toBinary();
	// Phương thức chuyển ra hệ thập phân
	string toHexa();
	// Phương thức chuyển ra hệ thập lục phân
	string toDecimal();
	// Phương thức chuyển ra một hệ bất kỳ
	string toNumeralSystem(int type);

	// Phương thức nhập vào hệ 10 nguyên không dấu
	void inputUnsignedInt(string strOfNum);
	// Phương thức nhập vào hệ 10 nguyên có dấu
	void inputSignInt(string strOfNum);
	// Phương thức nhập vào hệ 2
	void inputBinary(string strOfNum);
	// Phương thức nhập vào hệ 16
	void inputHexa(string strOfNum);

	// Phương thức dựng tham số
	BigInt(int type, string strOfNum);
	// Phương thức dựng mặc định
	BigInt();
	// Phương thức hủy
	~BigInt();

};


