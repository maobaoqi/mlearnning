#pragma once
#include"BigInt.h"
class Rsa
{
public:
	Rsa();
	~Rsa();
	void init(unsigned int n);

	friend void test();
public:
	BigInt encryptByPu(const BigInt& m);
	BigInt decodeByPr(const BigInt& c);

	BigInt encryptByPr(const BigInt& m);
	BigInt decodeByPu(const BigInt& m);
private:
	BigInt createOddNum(unsigned int n);
	bool isPrime(const BigInt& a, const unsigned int k);
	BigInt createPrime(unsigned int n, int it_cout);
	void createExp(const BigInt& ou);
	BigInt createRandomSmallThan(const BigInt& a);
	friend ostream& operator <<(ostream& out, const Rsa& rsa)
	{
		out << "N:" << rsa.N << "\n";
		out << "p:" << rsa._p << "\n";
		out << "q:" << rsa._q << "\n";
		out << "e:" << rsa.e << "\n";
		out << "d:" << rsa._d;
		return out;
	}
public:
	BigInt e, N;
private:
	BigInt _d;
	BigInt _p, _q;
	BigInt _ol;
};