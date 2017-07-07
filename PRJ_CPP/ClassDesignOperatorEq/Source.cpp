#include <iostream>
#include <map>
#include <string>

using namespace std;

class Book {
private:
	double m_price;
	int *m_bookMark;
	int m_num;

public:
	Book() : m_price(0.0), m_bookMark(nullptr), m_num(0) {}
	Book(double price, int *bookMark, int num);
	
	Book &Book::operator=(const Book &b);

	void setBookMark(int, int);
	void display();
};


Book &Book::operator=(const Book &b)
{
	if (this != &b)
	{
		this->m_price = b.m_price;
		this->m_num = b.m_num;

		int *bmTemp = new int[b.m_num];
		for (int i = 0; i < b.m_num; i++)
		{
			bmTemp[i] = b.m_bookMark[i];
		}

		this->m_bookMark = bmTemp;
	}

	return *this;
}


Book::Book(double price, int *bookMark, int num)
	: m_price(price),
	m_num(num)
{
	int *bmTemp = new int[num];
	for (int i = 0; i < num; i++)
	{
		bmTemp[i] = bookMark[i];
	}
	this->m_bookMark = bmTemp;
}

void Book::setBookMark(int page, int index)
{
	if (index >= m_num - 1)
	{
		cout << "Out of bound!" << endl;
	}
	else
	{
		m_bookMark[index] = page;
	}
}

void Book::display()
{
	cout << "Price:" << m_price << endl;
	cout << "bookMarks:";
	for (int i = 0; i < m_num; i++)
	{
		if (i == m_num - 1)
		{
			cout << m_bookMark[i] << endl;
		}
		else
		{
			cout << m_bookMark[i] << "," << endl;
		}
	}
}


int main(void)
{
	int bookMark[] = { 1, 49, 56, 290 };
	Book java, cpp(68.5, bookMark, 4);
	cpp.display();
	java = cpp;
	java.setBookMark(100, 2);
	cpp.display();

	map<string, string> thisMap = { {"mao", "me"} };

	return 0;
}