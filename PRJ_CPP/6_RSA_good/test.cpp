//#include "HardDiskSerialNum.h"

//#include "RSA.h"

#include <iostream>
using std::cout;
using std::endl;
using std::cin;

/*

void menu()
{//菜单显示函数
	cout << "==========Welcome to use RSA encoder==========" << endl;
	cout << "               e.encrypt            " << endl;
	cout << "               d.decrypt              " << endl;
	cout << "               s.setkey              " << endl;
	cout << "               p.print              " << endl;
	cout << "               q.quit                " << endl;
	cout << "input your choice:" << endl;
}

bool islegal(const string& str)
{//判断输入是否合法
	for (string::const_iterator it = str.begin(); it != str.end(); ++it)
		if (!isalnum(*it))//不是字母数字
			return false;
	return true;
}

bool decode(RSA& rsa)
{//解密
	string str;
	do
	{
		cout << ">Input hexadecimal data:";
		cin >> str;
	} while (cin && str.length()<1);
	if (!cin || islegal(str) == false)
		return false;
	BigInt c(str);

	long t1 = clock();
	BigInt m = rsa.decodeByPr(c);
	
	long t2 = clock();
	cout << "Time:" << (t2 - t1) << "ms." << endl;

	cout << "Secret content:" << c << endl
		<< "Public content:" << m << endl;
	return true;
}

bool encry(RSA& rsa, BigInt& c)
{//加密
	string str;
	do
	{
		cout << ">Input hexadecimal data :";
		cin >> str;
	} while (cin && str.length()<1);
	if (!cin || islegal(str) == false)
		return false;
	BigInt m(str);

	c = rsa.encryptByPu(m);

	cout << "Public content:" << m << endl
		<< "Secret content:" << c << endl;
	return true;
}

void print(RSA& rsa)
{//输出
	cout << rsa << endl;
}

void init(RSA& rsa, int n)
{//初始化

	cout << "Initialize...." << endl;
	long t1 = clock();
	rsa.init(n);
	long t2 = clock();
	cout << "Initialize done." << endl;
	cout << "Time:" << (t2 - t1) / 1000 << "s." << endl;
}

int go()
{//控制函数
	char ch;
	string str;
	RSA rsa;
	BigInt c, m;
	cout << "Input Bits:";
	int n;
	cin >> n;
	init(rsa, n / 2);

	while (true)
	{
		menu();//菜单显示
		cout << ">";
		cin >> str;
		if (!cin)
			return 0;

		if (str.length()<1)
			cout << "Input again" << endl;
		else
		{
			ch = str.at(0);
			switch (ch)
			{
			case 'e':
			case 'E':
				encry(rsa, c);//加密
				break;
			case 'd':
			case 'D':
				decode(rsa);//解密
				break;
			case 's':
			case 'S':
				go();//重新开始初始
				break;
			case 'p':
			case 'P':
				print(rsa);//输出公私钥信息
				break;
			case 'q':
			case 'Q':
				return 0;
			}
		}
	}
}

*/


int main(void)
{
	//HardDiskSN hardDiskSN;
	//std::string res;
	//hardDiskSN.GetFirstHardDiskSN(res);
	//std::cout << res << std::endl;

	//go();
	std::cout << "Hello:" << std::endl;
	
	return 0;

}