// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"


TBitField::TBitField(int len)
{
	if (len < 1)
		throw "len_is_L_one!";
	BitLen = len;
	MemLen = ((BitLen - 1) / sizeof(TELEM) * 8) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.GetLength();
	MemLen = bf.GetLength();
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0)
		throw "GetMemIndex: out_of_bottom_border!";
	if (n >= BitLen)
		throw "GetMemIndex: out_of_upper_border!";
	return  n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0)
		throw "GetMemMask: out_of_bottom_border!";
	if (n >= BitLen)
		throw "GetMemMask: out_of_upper_border!";
	TELEM Mask = 1;
	Mask = Mask << (sizeof(TELEM) * 8 - n % (sizeof(TELEM) * 8) - 1);
	return Mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0)
		throw "SetBit: out_of_bottom_border!";
	if (n >= BitLen)
		throw "SetBit: out_of_upper_border!";
	TELEM Mask = GetMemMask(n);
	//Mask = Mask << (sizeof(TELEM) * 8 - (n % (sizeof(TELEM) * 8)) - 1);
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | Mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0)
		throw "ClrBit: out_of_bottom_border!";
	if (n >= BitLen)
		throw "ClrBit: out_of_upper_border!";
	TELEM Mask = 1;
	Mask = Mask << (sizeof(TELEM) * 8 - (n % (sizeof(TELEM) * 8)) - 1);
	Mask = (~Mask);
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & Mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0)
		"GetBit: out_of_bottom_border!";
	if (n >= BitLen)
		throw "GetBit: out_of_upper_border!";
	TELEM Mask = 1;
	Mask = Mask << (sizeof(TELEM) * 8 - (n % (sizeof(TELEM) * 8)) - 1);

	return ((pMem[GetMemIndex(n)] & Mask) >> (sizeof(TELEM) * 8 - n % (sizeof(TELEM) * 8) - 1));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (*this == bf)
		return *this;
	else {
		BitLen = bf.BitLen;
		if (MemLen != bf.MemLen) {
			MemLen = bf.MemLen;
			delete[] pMem;
			pMem = new TELEM[MemLen];
		}
		for (int i = 0; i < BitLen; i++)
			pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (BitLen == bf.GetLength())
	{
		for (int i = 0; i < BitLen; i++)
		{
			if (GetBit(i) != bf.GetBit(i))
				return 0;
		}
		return 1;
	}
	return 0;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	int MaxLen, MinLen;
	TBitField result(1);
	if (BitLen < bf.GetLength()) {
		MaxLen = bf.GetLength();
		MinLen = BitLen;
		result = bf;
	}
	else {
		MaxLen = BitLen;
		MinLen = bf.GetLength();
		result = *this;
	}
	for (int i = 0; i < MinLen; i++)
		if ((GetBit(i) == 1) || (bf.GetBit(i) == 1))
			result.SetBit(i);
	return result;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	int MaxLen, MinLen;
	if (BitLen < bf.GetLength()) {
		MaxLen = bf.GetLength();
		MinLen = BitLen;
	}
	else {
		MaxLen = BitLen;
		MinLen = bf.GetLength();
	}
	TBitField result(MaxLen);
	for (int i = 0; i < MinLen; i++)
		if ((GetBit(i) == 1) && (bf.GetBit(i) == 1))
			result.SetBit(i);
	return result;
}

TBitField TBitField::operator~(void) // отрицание
{
	for (int i = 0; i < BitLen; i++) {
		if (GetBit(i) == 1)
			ClrBit(i);
		else
			SetBit(i);
	}
	return *this;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	int tmp;
	for (int i = 0; i < bf.GetLength(); i++)
	{
		istr >> tmp;
		if (tmp == 1)
			bf.SetBit(i);
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int i = 0; i < bf.GetLength(); i++)
	{
		ostr << bf.GetBit(i);
	}
	ostr << "\n";
	return ostr;
}
