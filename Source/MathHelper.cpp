#include "MathHelper.h"

// Cộng hai chuỗi số nguyên ở hệ 10
// Input: 2 chuỗi số hệ 10
// Ouput: Chuỗi số tổng ở hệ 10
string sumDecimalString(string numMax, string numMin)
{
	string sum;

	// Cân bằng độ dài 2 chuỗi
	while (numMax.length() < numMin.length())
	{
		numMax.insert(0, "0");
	}
	while (numMin.length() < numMax.length())
	{
		numMin.insert(0, "0");
	}

	int i = numMax.length() - 1;
	sum = numMax;
	int tmp = 0;

	// Cộng từng số từ phải qua trái
	while (i >= 0)
	{
		// tmp đóng vai trò là biến chứa kết quả tổng của 2 số thứ i lẫn biến nhớ
		tmp += numMax[i] + numMin[i] - 2 * '0';
		sum[i] = tmp % 10 + '0';
		tmp /= 10;
		i--;
	}

	// Nếu cộng hết độ dài 2 chuỗi mà vẫn còn nhớ thì thêm 1 nhớ vào đầu kết quả
	if (tmp > 0)
	{
		sum.insert(0, "1");
	}

	return sum;
}

// Trừ hai chuỗi số nguyên dương ở hệ 10
// Input: 2 chuỗi số hệ 10
//		numMax: Số lớn hơn - không dấu
//		numMin: Số nhỏ hơn - không dấu
// Ouput: Chuỗi số hiệu ở hệ 10
string subDecimalString(string numMax, string numMin)
{
	// Cân bằng độ dài 2 chuỗi (tăng chuỗi numMin)
	while (numMin.length() < numMax.length())
	{
		numMin.insert(0, "0");
	}

	int i = numMax.length() - 1;
	string sub = numMax;
	int tmp = 0;

	// Trừ từng số từ phải qua trái
	while (i >= 0)
	{
		// x là biến chứ kết quả trừ tạm thời
		// tmp là biến nhớ
		int x = numMax[i] - numMin[i] - tmp;

		// Trường hợp có nhớ
		if (x < 0)
		{
			tmp = x + 10;
			sub[i] = tmp + '0';
			tmp = 1;
		}
		// Trường hợp không nhớ
		else
		{
			tmp = x;
			sub[i] = tmp + '0';
			tmp = 0;
		}
		i--;
	}

	// result là biến kết quả
	string result;
	// Lọc bỏ các số '0' ở đầu
	for (i = 0; i < sub.length() && sub[i] == '0'; i++);
	while (i < sub.length())
	{
		result += sub[i];
		i++;
	}

	return result;
}

// Tính lũy thừa của 2 (2^n) ở hệ 10
// Input:
//		n: Số mũ của 2
// Output: Chuỗi số kết quả của 2^n
string powOfTwo(int n)
{
	string pow2 = "1";
	for (int i = 1; i <= n; i++)
	{
		// Mỗi lần tằng mũ là mỗi lần nhân cho 2 nên dùng hàm cộng
		// 2^n = 2^(n-1) * 2 = 2^(n-1) + 2^(n-1) = sum(2^(n-1))
		pow2 = sumDecimalString(pow2, pow2);
	}
	return pow2;
}

// Chia chuỗi số nguyên ở hệ 10 cho 2
// Input:
//		decimal: Chuỗi số nguyên cần chia
// Output: Chuỗi kết quả sau khi chia cho 2
string divTwo(string decimal)
{
	int i = 0;

	// Biến chứa thương
	string quotient;

	// Biến chứa chuỗi số tạm để chuyển sang __int64 chia
	string numTemp;

	while (i < decimal.length())
	{
		// numTemp chứa tối đa 19 ký tự số
		numTemp += decimal[i];
		i++;
		while (i % 18 != 0 && i < decimal.length())
		{
			numTemp += decimal[i];
			i++;
		}

		// Chuyển munTemp sang __int64
		long long quotientTemp = atoll(numTemp.c_str());

		// Thực hiện chia 2
		quotientTemp /= 2;

		// Chuỗi quotient kết quả cập nhật từng phần thương
		quotient += to_string(quotientTemp);

		// Nếu numTemp là 1 chuỗi số lẻ thì cặp nhật dư 1 ở đầu
		if (numTemp[numTemp.length() - 1] % 2)
		{
			numTemp = '1';
		}
		// Ngược lại có 2 trường hợp
		else
		{
			numTemp.clear();
			// Trường hợp 1: 
			// Số tiếp theo của chuỗi cần chia là '0' -> không thể chia,
			//thương tăng thêm '0'
			while (i < decimal.length())
			{
				if (decimal[i] == '0')
				{
					quotient += '0';
				}
				else
				{
					break;
				}
				i++;
			}
			// Trường hợp 2:
			// Số tiếp theo của chuỗi là '1' -> cũng không thể chia,
			//thương tăng thêm '0' và numTemp nhận phần dư 1
			if (decimal[i] == '1')
			{
				quotient += '0';
				numTemp = '1';
				i++;
			}
		}
	}

	return quotient;
}
