#include <fstream>
#include "BigInt.h"
using namespace std;

// Hàm tách chuỗi ký tự (1 line trong input.txt)
//thành tối đa 4 chuỗi con thành phần
void distributeString(string strSource, string* element, int& n);

// argv[1]: input file path
// argv[2]: output file path
int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cout << "Sai cu phap!" << endl;
		return 0;
	}

	fstream fIn, fOut;
	fIn.open(argv[1], ios::in);
	fOut.open(argv[2], ios::out);

	string element[4];
	while (!fIn.eof())
	{
		string lineTemp;
		int nElement;

		// Đọc cả dòng vào lineTemp
		getline(fIn, lineTemp);

		// Tách lineTemp thành tối đa 4 chuỗi phần tử element
		//nElement đếm số lượng chuỗi
		distributeString(lineTemp, element, nElement);

		// radix tương ứng element[0] là chỉ cơ số đang xử lý
		int radix = atoi(element[0].c_str());

		//////////////////////////////////////////////////////////////////
		// Toán tử 1 ngôi (cần 1 chuỗi số để xử lý) - eElement == 3
		//
		if (nElement == 3)
		{
			// Tạo BigInt thứ 1: a
			BigInt a(radix, element[2]);

			// Nếu phần tử tiếp theo: element[1] là 2, 10, 16 
			//và số lượng chuỗi là 3 -> đổi cơ số
			if (element[1] == "2" && nElement == 3)
			{
				fOut << a.toBinary();
			}
			else if (element[1] == "10" && nElement == 3)
			{
				fOut << a.toDecimal();
			}
			else if (element[1] == "16" && nElement == 3)
			{
				fOut << a.toHexa();
			}

			// Nếu phần tử tiếp theo: element[1] là ror, rol, ~ -> phép xoay, not
			//đọc thêm 1 chuỗi số và thực hiện xoay, ghi xuống file ở hệ tương ứng
			else if (element[1] == "ror")
			{
				a = a.ror();
				fOut << a.toNumeralSystem(radix);
			}
			else if (element[1] == "rol")
			{
				a = a.rol();
				fOut << a.toNumeralSystem(radix);
			}
			else if (element[1] == "~")
			{
				a = ~a;
				fOut << a.toNumeralSystem(radix);
			}
		}

		//////////////////////////////////////////////////////////////////
		// Toán tử 2 ngôi (cần 2 chuỗi số xử lý)
		//khi đó element[3] là chuỗi số thứ 2: b
		//hoặc đối với phép dịch là 1 số nguyên: n
		else
		{
			// BigInt thứ 1: a
			BigInt a(radix, element[1]);

			// Nếu là toán tử dịch 
			//phần tử tiếp theo element[3] là 1 số nguyên - n - số của phép dịch
			//đọc toán tử, n, thực hiện dịch 
			//và ghi xuống file theo cơ số tương ứng
			if (element[2] == "<<")
			{
				int n = atoi(element[3].c_str());
				a = a << n;
				fOut << a.toNumeralSystem(radix);
			}
			else if (element[2] == ">>")
			{
				int n = atoi(element[3].c_str());
				a = a >> n;
				fOut << a.toNumeralSystem(radix);
			}


			// Ngược lại là toán tử +, -, *, /, |, ^, &
			//phần tử cuối cùng là 1 chuỗi số
			//đọc toán tử, chuỗi số thứ 2, thực hiên phép tính
			//và ghi xuống file theo hệ số tương ứng
			else
			{
				// Biến nhận toán tử +, -, *, /, |, ^, &
				char _operator = element[2].c_str()[0];

				// Số BigInt thứ 2: b, nhập chuỗi số elemen[3]
				BigInt b(radix, element[3]);

				switch (_operator)
				{
				case '+':
					// Phép cộng
					fOut << (a + b).toNumeralSystem(radix);
					break;
				case '-':
					// Phép trừ
					fOut << (a - b).toNumeralSystem(radix);
					break;
				case '*':
					// Phép nhân
					fOut << (a * b).toNumeralSystem(radix);
					break;
				case '/':
					// Phép chia
					fOut << (a / b).toNumeralSystem(radix);
					break;
				case '&':
					// Phép AND
					fOut << (a&b).toNumeralSystem(radix);
					break;
				case '|':
					// Phép OR
					fOut << (a | b).toNumeralSystem(radix);
					break;
				case '^':
					// Phép XOR
					fOut << (a^b).toNumeralSystem(radix);
					break;
				default:
					break;
				}
			}
		}
		// Nếu không phải dòng cuối cùng thì xuống dòng
		if (!fIn.eof())
		{
			fOut << endl;
		}
	}

	// Đóng file
	fIn.close();
	fOut.close();

	system("pause");
	return 0;
}


// Hàm tách chuỗi ký tự (1 line trong input.txt)
//thành tối đa 4 chuỗi con thành phần
// Input: 
//		strSource: Chuỗi nguồn cần tách
//		*element: Mảng chứ các chuỗi sau khi tách
//		n: Số lượng chuỗi con tách được
// Output: Mảng *element chứa các chuỗi con đã tách và lưu vào
//n là số lượng chuỗi con
void distributeString(string strSource, string *element, int &n)
{
	// Xóa khoảng trắng dư ở cuối dãy
	//tránh gặp lỗi đối với file input không hợp lệ
	while (strSource[strSource.length() - 1] == ' ')
	{
		strSource.erase(strSource.length() - 1, 1);
	}

	// n là biến đếm số lượng chuỗi con
	n = 0;

	// Làm trống chuỗi đầu
	element[0].clear();

	for (int i = 0; i < strSource.length(); i++)
	{
		// Nếu gặp khoảng trắng -> hết 1 chuỗi -> tăng đếm chuyển sang
		//chuỗi tiếp theo
		if (strSource[i] == ' ')
		{
			n++;
			i++;
			// Làm trống chuỗi tiếp theo trước khi nhận dữ liệu
			element[n].clear();
		}

		// Tách dữ liệu vào chuỗi thứ n
		element[n] += strSource[i];
	}
	n++;
}