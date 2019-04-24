#include "BigInt.h"

//////////////////////////////////////////////////////////////////////////////
// Toán tử cộng 
// BigInt + BigInt -> BigInt
BigInt BigInt::operator+(BigInt x)
{
	// Sử dụng thuật toán Half Adder
	BigInt temp1 = *this;
	BigInt temp2 = x;
	while (!((temp1 & temp2).arrBit[0] == 0 && (temp1 & temp2).arrBit[1] == 0))
	{
		temp2 = (temp1 & temp2) << 1;
		temp1 = temp1 ^ x;
		x = temp2;
	}
	return temp1 | temp2;
}

//////////////////////////////////////////////////////////////////////////////
// Toán tử trừ
// BigInt - BigInt -> BigInt
BigInt BigInt::operator-(BigInt x)
{
	// Đảo dấu số bị trừ
	x = ~x + BigInt(2, "1");

	// Trả về kết quả phép cộng
	return (*this + x);
}

//////////////////////////////////////////////////////////////////////////////
// Toán tử nhân
// BigInt * BigInt -> BigInt
BigInt BigInt::operator*(BigInt x)
{
	// Sử dụng thuật toán nhân cải tiến: Booth’s Multiplication Algorithm
	// A, x là tích (xét 128 bit thì x là tích)
	BigInt A;
	bool Q = 0;
	for (int i = 0; i < 128; i++)
	{
		// Nếu 2 bit cuối x và Q là 10
		if ((int)(x.arrBit[1] & 1) == 1 && Q == 0)
		{
			A = A - *this;
		}
		// Nếu bit cuối x và Q là 01
		else if ((int)(x.arrBit[1] & 1) == 0 && Q == 1)
		{
			A = A + *this;
		}

		// Dịch phải [A, x, Q]
		Q = x.arrBit[1] & 1;
		x.arrBit[1] = (unsigned long long)x.arrBit[1] >> 1 | x.arrBit[0] << 63;
		x.arrBit[0] = (unsigned long long)x.arrBit[0] >> 1 | A.arrBit[1] << 63;
		A = A >> 1;
	}
	return x;
}

//////////////////////////////////////////////////////////////////////////////
// Toán tử chia
// BigInt / BigInt -> BigInt
BigInt BigInt::operator/(BigInt x)
{
	// Nếu số chia bằng 0, không thể chia -> quăng ra ngoại lệ
	if (x.arrBit[0] == 0 && x.arrBit[1] == 0)
	{
		throw "Error DivisionByZero";
	}

	// Sử dụng thuật toán chia: Restoring Division Algorithm
	// Thuật toán chia có số âm bị sai nên làm dương hóa 2 số
	// Biến sign dùng để nhớ dấu
	bool sign = 0;

	// Q là số bị chia
	BigInt Q = *this;

	// Nếu số bị chia âm -> đảo dấu
	if (arrBit[0] < 0)
	{
		// Đảo dấu
		Q = ~Q + BigInt(2, "1");

		// Biến nhớ dấu nhận 1 (âm) nếu 1 trong 2 số âm
		if (x.arrBit[0] >= 0)
		{
			sign = 1;
		}
	}
	// Nếu số chia âm -> đảo dấu
	if (x.arrBit[0] < 0)
	{
		// Đảo dấu
		x = ~x + BigInt(2, "1");

		// Biến nhớ dấu nhận 1 (âm) nếu 1 trong 2 số âm
		if (arrBit[0] >= 0)
		{
			sign = 1;
		}
	}

	// Chạy thuật toán Q/x
	// A là phần dư
	// Q là thương
	BigInt A;
	for (int i = 0; i < 128; i++)
	{
		// Dịch trái [A, Q]
		A.arrBit[0] = A.arrBit[0] << 1 | (unsigned long long)A.arrBit[1] >> 63;
		A.arrBit[1] = A.arrBit[1] << 1 | (unsigned long long)Q.arrBit[0] >> 63;
		Q = Q << 1;

		// A = A - x
		A = A - x;

		// Nếu A<0
		if (A.arrBit[0] < 0)
		{
			// Q[0] = 0
			Q = Q & BigInt(10, "-2");
			// A = A + x
			A = A + x;
		}
		else
		{
			// Q[0] = 1
			Q.arrBit[1] = Q.arrBit[1] | 1;
		}
	}

	// Đặt dấu cho kết quả 
	// Nếu kết quả phải là số âm thì đảo bit thêm 1
	if (sign)
	{
		Q = ~Q + BigInt(2, "1");
	}
	return Q;
}

//////////////////////////////////////////////////////////////////////////////
// Toán tử gán bằng
// BigInt = BigInt -> BigInt
BigInt BigInt::operator=(BigInt x)
{
	arrBit[0] = x.arrBit[0];
	arrBit[1] = x.arrBit[1];
	return *this;
}

//////////////////////////////////////////////////////////////////////////////
// Toán tử & (AND)
// BigInt & BigInt -> BigInt
BigInt BigInt::operator&(BigInt x)
{
	BigInt tmp;
	tmp.arrBit[0] = arrBit[0] & x.arrBit[0];
	tmp.arrBit[1] = arrBit[1] & x.arrBit[1];
	return tmp;
}

//////////////////////////////////////////////////////////////////////////////
// Toán tử | (OR)
// BigInt | BigInt -> BigInt
BigInt BigInt::operator|(BigInt x)
{
	BigInt tmp;
	tmp.arrBit[0] = arrBit[0] | x.arrBit[0];
	tmp.arrBit[1] = arrBit[1] | x.arrBit[1];
	return tmp;
}

//////////////////////////////////////////////////////////////////////////////
// Toán tử ^ (XOR)
// BigInt ^ BigInt -> BigInt
BigInt BigInt::operator^(BigInt x)
{
	BigInt tmp;
	tmp.arrBit[0] = arrBit[0] ^ x.arrBit[0];
	tmp.arrBit[1] = arrBit[1] ^ x.arrBit[1];
	return tmp;
}

//////////////////////////////////////////////////////////////////////////////
// Toán tử ~ (NOT)
// ~BigInt -> BigInt
BigInt BigInt::operator~()
{
	BigInt tmp;
	tmp.arrBit[0] = ~arrBit[0];
	tmp.arrBit[1] = ~arrBit[1];
	return tmp;
}

//////////////////////////////////////////////////////////////////////////////
// Lấy bit thứ n
// Input:
//		n: Vị trí cần lấy bit
// Output: Giá trị của bit vị trí n
bool BigInt::getBit(int n)
{
	long long x;
	// Nếu n < 64 dịch bit ở mảng 1
	if (n < 64)
	{
		x = arrBit[1] >> n;
	}
	// Nếu n > 64 dịch bit ở mảng 0
	else
	{
		n -= 64;
		x = arrBit[0] >> n;
	}

	// AND với 1 để lấy bit
	return x & 1;
}

//////////////////////////////////////////////////////////////////////////////
// Đặt bit 1 ở vị trí n
// Input:
//		n: Vị trí cần đặt bit 1
// Output: void - không trả về  - đặt bit 1 đúng vị trí n
void BigInt::setBit1(int n)
{
	// Nếu n nằm ngoài phạm vi 128 bit -> thoát
	if (n < 0 || n > 127)
	{
		return;
	}

	// i là biến nhận độ dịch bit
	int i;
	// Nếu n < 64 dịch đúng bằng n
	if (n < 64)
	{
		i = n;
	}
	// Nếu n >= 64 dịch bằng n - 64, do 1 mảng __int64 chỉ 64 bit
	else
	{
		i = n - 64;
	}

	// x là thứ tự mảng cần phải set bit
	int x = !(n / 64);

	// Gán bit 1 vào vị trí n - ứng với arrBit
	arrBit[x] = ((long long)1 << i) | arrBit[x];
}

//////////////////////////////////////////////////////////////////////////////
// Toán tử >> (dịch phải)
// BigInt >> n -> BigInt
BigInt BigInt::operator >> (int n)
{
	BigInt tmp = *this;

	// Nếu n < 64 xử lý dịch phải ở cả 2 mảng
	if (n < 64)
	{
		// Dịch phải mảng 1 (bit thứ 0 -> 63) và lấy n bit của mảng 0 (bit thứ 64 -> 127)
		tmp.arrBit[1] = (unsigned long long)tmp.arrBit[1] >> n | tmp.arrBit[0] << (64 - n);

		// Dịch phải mảng 0
		tmp.arrBit[0] = tmp.arrBit[0] >> n;
	}
	// Nếu n >= 64 xử lý dịch ở mảng 1, mảng 0 xét trường hợp
	else
	{
		// Dịch (64 - n) bit ở mảng 0 qua mảng 1
		tmp.arrBit[1] = tmp.arrBit[0] >> (n - 64);

		// Nếu mảng 0 âm -> dịch số học -> mảng 1 là 1 dãy bit toàn 1
		// Ngược lại là dương -> dịch luận lý -> mảng 1 là 1 dãy bit toàn 0
		tmp.arrBit[0] = tmp.arrBit[0] < 0 ? -1 : 0;
	}

	return tmp;
}

//////////////////////////////////////////////////////////////////////////////
// Toán tử << (dịch trái)
// BigInt << n -> BigInt
BigInt BigInt::operator << (int n)
{
	BigInt tmp = *this;

	// Nếu n < 64 xử lý dịch trái ở cả 2 mảng
	if (n < 64)
	{
		// Dịch trái mảng 0 (bit thứ 64 -> 127) và lấy n bit của mảng 1 (bit thứ 0 -> 63)
		tmp.arrBit[0] = tmp.arrBit[0] << n | (unsigned long long)tmp.arrBit[1] >> (64 - n);

		// Dịch trái mảng 1
		tmp.arrBit[1] = tmp.arrBit[1] << n;
	}
	// Nếu n >= 64 xử lý dịch trái mảng 0, mảng 1 bằng 0
	else
	{
		// Dịch trái (n - 64) bit từ mảng 1 sang mảng 0
		tmp.arrBit[0] = tmp.arrBit[1] << (n - 64);

		// Mảng 1 là 1 dãy bit toàn 0
		tmp.arrBit[1] = 0;
	}

	return tmp;
}

//////////////////////////////////////////////////////////////////////////////
// Phép xoay phải (ROR)
// BigInt.ror() -> BigInt
BigInt BigInt::ror()
{
	BigInt tmp;

	// Dịch phải 1 mảng 0 và nhận bit thứ 0 từ mảng 1
	tmp.arrBit[0] = (unsigned long long)arrBit[0] >> 1 | arrBit[1] << 63;
	// Dịch phải 1 mảng 1 và nhận bit thứ 64 từ mảng 0
	tmp.arrBit[1] = (unsigned long long)arrBit[1] >> 1 | arrBit[0] << 63;
	return tmp;
}

//////////////////////////////////////////////////////////////////////////////
// Phép xoay trái (ROL)
// BigInt.rol() -> BigInt
BigInt BigInt::rol()
{
	BigInt tmp;

	// Dịch trái 1 mảng 0 và nhận bit thứ 63 từ mảng 1
	tmp.arrBit[0] = arrBit[0] << 1 | (unsigned long long)arrBit[1] >> 63;

	// Dịch trái 1 mảng 1 và nhận bit thứ 127 từ mảng 0
	tmp.arrBit[1] = arrBit[1] << 1 | (unsigned long long)arrBit[0] >> 63;
	return tmp;
}

//////////////////////////////////////////////////////////////////////////////
// Chuyển BigInt sang nhị phân (Hệ 2)
// Output: Chuỗi nhị phân mà BigInt đang lưu trữ
string BigInt::toBinary()
{
	// Biến chuỗi nhị phân kết quả
	string binary;

	int i;
	// Biến vị trí của bit 1 đầu tiên tìm thấy từ trái sang
	bool bitPosN;

	// Tìm vị trí của bit 1 đầu tiên tìm thấy từ trái sang
	for (i = 127; i >= 0; i--)
	{
		bitPosN = getBit(i);
		if (bitPosN)
		{
			break;
		}
	}

	// Chuyển bit vị trí bitPosN vào chuỗi kết quả
	binary += to_string(bitPosN);

	// Chuyển từng bit vào chuỗi kết quả
	for (int j = i - 1; j >= 0; j--)
	{
		binary += to_string(getBit(j));
	}

	// Trường hợp BigInt == 0 thì chuỗi binary rỗng -> trả về "0"
	if (binary.empty())
	{
		return "0";
	}

	return binary;
}

//////////////////////////////////////////////////////////////////////////////
// Chuyển BigInt sang Hexa (hệ 16)
// Output: Chuỗi Hexa sau khi chuyển từ dữ liệu đang lưu trữ
string BigInt::toHexa()
{
	// Biến chuỗi hexa kết quả
	string hexa;
	int i;

	// Tìm vị trí của bit 1 đầu tiên tìm thấy từ trái sang
	for (i = 127; i >= 0; i--)
	{
		if (getBit(i) != 0)
		{
			break;
		}
	}

	// Số hexa ở dạng 10
	int intTmp = 0;
	// Ký tự hexa ở dạng hexa
	string hexaTmp;

	for (int j = 0; j <= i; j++)
	{
		// intTmp là biến chứ giá trị ở hệ 10 của mỗi 4 bit từ phải sang
		intTmp += getBit(j)*pow(2, j % 4);

		// Cứ đủ 4 bit từ phải sang hoặc hết bit
		if (j % 4 == 3 || j == i)
		{
			// Nếu intTmp < 10 thì ký tự hexa là intTemp (0 đến 9)
			if (intTmp < 10)
			{
				hexaTmp = to_string(intTmp);
			}
			// Nếu intTmp >= 10 thì ký tự hexa là intTemp ở dạng A, B,..., F
			else
			{
				hexaTmp = intTmp + 55;
			}
			// Thêm ký tự hexa vào kết quả
			hexa.insert(0, hexaTmp);
			intTmp = 0;
		}
	}

	// Trường hợp BigInt == 0 thì chuỗi hexa rỗng -> trả về "0"
	if (hexa.empty())
	{
		return "0";
	}
	return hexa;
}

//////////////////////////////////////////////////////////////////////////////
// Chuyển BigInt sang Decimal (hệ 10)
// Output: Chuỗi Decimal sau khi chuyển từ dữ liệu đang lưu trữ
string BigInt::toDecimal()
{
	// decimal là chuỗi kết quả, khởi tạo bằng 0
	string decimal = "0";

	// Biến dấu
	int sign = getBit(127);

	int i;
	// Đổi 127 bit từ bit thứ 0 đến bit thứ 126 sang hệ 10
	for (i = 126; i >= 0; i--)
	{
		// Nếu bit thứ i bằng 1 thì cộng vào chuỗi kết quả
		if (getBit(i) == 1)
		{
			decimal = sumDecimalString(decimal, powOfTwo(i));
		}
	}

	// Nếu là số âm thì lấy 2^127 trừ cho chuỗi tổng từ bit 0 đến 126
	//thêm dấu trừ vào đầu chuỗi kết quả 
	if (sign)
	{
		decimal = "-" + subDecimalString(powOfTwo(127), decimal);
	}
	return decimal;
}

//////////////////////////////////////////////////////////////////////////////
// Chuyển BigInt sang 1 hệ bất kỳ
// Input:
//		type: Loại của hệ số với
//			2 là nhị phân (binary)
//			10 là thập phân (decimal)
//			16 là thập lục phân (hexa)
// Output: Chuỗi kết quả ở hệ tương ứng
string BigInt::toNumeralSystem(int type)
{
	switch (type)
	{
	case 2:
		return this->toBinary();
	case 10:
		return this->toDecimal();
	case 16:
		return this->toHexa();
	default:
		break;
	}
	return "";
}

//////////////////////////////////////////////////////////////////////////////
// Nhập chuỗi số nguyên không dấu ở hệ 10 vào BigInt
// Input:
//		strOfNum: Chuỗi số nguyên không dấu ở hệ 10
// Output: Nhập dữ liệu vào 2 mảng của BigInt
void BigInt::inputUnsignedInt(string strOfNum)
{
	int i = 0;
	// Chia lấy dư đến khi nào chuỗi strOfNum bằng 0
	while (strOfNum != "0")
	{
		// Nếu dư 1 ở vị trí i thì gán bit i bằng 1
		if (strOfNum[strOfNum.length() - 1] % 2 == 1)
		{
			setBit1(i);
		}

		// Chia chuỗi cho 2
		strOfNum = divTwo(strOfNum);
		i++;
	}
}

//////////////////////////////////////////////////////////////////////////////
// Nhập chuỗi số nguyên có dấu ở hệ 10 vào BigInt
// Input:
//		strOfNum: Chuỗi số nguyên có dấu ở hệ 10
// Output: Nhập dữ liệu vào 2 mảng của BigInt
void BigInt::inputSignInt(string strOfNum)
{
	// Xóa ký tự dấu âm '-'
	strOfNum.erase(0, 1);
	// Nhập số dương
	inputUnsignedInt(strOfNum);
	// Đảo dấu
	*this = ~(*this);
	// Cộng thêm 1
	*this = *this + BigInt(2, "1");
}

//////////////////////////////////////////////////////////////////////////////
// Nhập chuỗi số nhị phân vào BigInt
// Input:
//		strOfNum: Chuỗi số nhị phân
// Output: Nhập dữ liệu vào 2 mảng của BigInt
void BigInt::inputBinary(string strOfNum)
{
	int len = strOfNum.length() - 1;
	for (int i = len; i >= 0; i--)
	{
		// Nhập từng bit từ chuỗi vào
		if (strOfNum[i] - 48)
		{
			setBit1(len - i);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
// Nhập chuỗi số thập lục phân vào BigInt
// Input:
//		strOfNum: Chuỗi số thập lục phân
// Output: Nhập dữ liệu vào 2 mảng của BigInt
void BigInt::inputHexa(string strOfNum)
{
	for (int i = strOfNum.length() - 1; i >= 0; i--)
	{
		// tmp là biến lưu giá trị ở hệ thập phân của 1 ký tự hexa
		char tmp = strOfNum[i];
		if (strOfNum[i] >= 'A')
		{
			tmp -= 55;
		}

		// Chuyển ký tự thập phân tmp sang nhị phân vào lưu vào
		for (int j = 0; j < 4; j++)
		{
			// Lấy nhị phân của tmp
			char bit = (1 << j) & tmp;

			// Nếu là bit 1 thì lưu vào đúng vị trí trong *this
			if (bit)
			{
				setBit1((strOfNum.length() - i - 1) * 4 + j);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
// Hàm dựng có tham số
// Intput:
//		type: Loại cơ số nhận vào
//		strOfNum: Chuỗi các số ở hệ cơ số tương ứng
// Output: Dữ liệu được nhập vào BigInt
BigInt::BigInt(int type, string strOfNum) : BigInt()
{
	// Gọi hàng dựng mặc định - Đặt BigInt = 0

	int len = strOfNum.length();
	switch (type)
	{
		// Hệ nhị phân
	case 2:
	{
		inputBinary(strOfNum);
		break;
	}
	// Hệ thập phân
	case 10:
	{
		// Nếu số dương -> gọi hàm nhập số dương
		if (strOfNum[0] != '-')
		{
			inputUnsignedInt(strOfNum);
		}
		// Ngược lại số âm -> chuyển sang dạng bù 2
		else
		{
			inputSignInt(strOfNum);
		}
		break;
	}
	// Hệ thập lục phân
	case 16:
	{
		inputHexa(strOfNum);
		break;
	}
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////
// Hàm dựng mặc định 
// Đặt BigInt = 0
BigInt::BigInt()
{
	arrBit[0] = 0;
	arrBit[1] = 0;
}

//////////////////////////////////////////////////////////////////////////////
// Hàm hủy
BigInt::~BigInt()
{
}
