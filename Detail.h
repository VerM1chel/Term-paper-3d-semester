#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;

class Detail {
protected:
	double Cost;//цена комплектующей
	string Name;//название комплектующей
	int id;//id комплектующей (каким по порядку считана из документа)
	int indexInVector;//индекс комплектующей (как id, только index для каждого вектора свой)
	static int indexStatic;//статическая переменная, которую мы будем присваивать полю index элементов, а также обнулять при переходе на новый вектор
	static int Count;//общее количество считанных комплектующих
public:
	Detail();
	Detail(string Name, double Cost);
	Detail(const Detail& other);

	void setCost(double Cost);
	void setName(string Name);
	void setIndex(int indexInVector);
	static void setStaticIndexToZero();

	double getCost();
	virtual void uslessFunction() = 0;//эта функция нужна лишь для того, чтобы сделать класс Detail абстрактный, поэтому, ниже, в её реализации ничего не пишем
	string getName();
	int getId();
	int getIndex();
	static int getCount();

	bool operator ==(const Detail& other);
	bool operator !=(const Detail& other);
	bool operator <(const Detail& other);
	bool operator >(const Detail& other);
	friend ostream& operator<<(ostream& out, const Detail& detail);
	friend istream& operator>>(istream& in, Detail& detail);

	~Detail();
};




class CPU : public Detail {
public:
	virtual void uslessFunction() override {}
	CPU(string Name, double Cost) : Detail(Name, Cost) {}
	CPU() : Detail() {}
};

//---------------------------------------------------

class MB : public Detail {
public:
	virtual void uslessFunction() override {}
	MB(string Name, double Cost) : Detail(Name, Cost) {}
	MB() : Detail() {}

private:

};

//---------------------------------------------------

class Cooler : public Detail {
public:
	virtual void uslessFunction() override {}
	Cooler(string Name, double Cost) : Detail(Name, Cost) {}
	Cooler() : Detail() {}

private:

};

//---------------------------------------------------

class mem : public Detail {
private:	
	static int quantity_of_DDR;
public:
	virtual void uslessFunction() override {}
	mem(string Name, double Cost) : Detail(Name, Cost) {}
	mem() : Detail() {}
	static void set_Quantity_of_DDR(int quantity);
	static double get_Quantity_of_DDR();
};

//---------------------------------------------------

class GPU : public Detail {
public:
	virtual void uslessFunction() override {}
	GPU(string Name, double Cost) : Detail(Name, Cost) {}
	GPU() : Detail() {}
private:

};


//---------------------------------------------------

class ssd : public Detail {
public:
	virtual void uslessFunction() override {}
	ssd(string Name, double Cost) : Detail(Name, Cost) {}
	ssd() : Detail() {}
private:

};


//---------------------------------------------------

class hdd : public Detail {
public:
	virtual void uslessFunction() override {}
	hdd(string Name, double Cost) : Detail(Name, Cost) {}
	hdd() : Detail() {}
private:

};


//---------------------------------------------------

class Case : public Detail {
public:
	virtual void uslessFunction() override {}
	Case(string Name, double Cost) : Detail(Name, Cost) {}
	Case() : Detail() {}
private:

};


//---------------------------------------------------

class PSU : public Detail {
public:
	virtual void uslessFunction() override {}
	PSU(string Name, double Cost) : Detail(Name, Cost) {}
	PSU() : Detail() {}
private:

};