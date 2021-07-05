#include "Detail.h"

int Detail::Count = 0;
int Detail::indexStatic = 0;

double Detail::getCost() { //геттеры
	return Cost;
};

string Detail::getName() {
	return Name;
}

int Detail::getId() {
	return id;
}

int Detail::getCount() {
	return Count;
}

int Detail::getIndex() {
	return indexInVector;
}

//---------------------------------------------------
void Detail::setCost(double Cost) {
	this->Cost = Cost;
}

void Detail::setName(string Name) {
	this->Name = Name;
}

void Detail::setStaticIndexToZero(){
	Detail::indexStatic = 0;
}

void Detail::setIndex(int indexInVector) {
	this->indexInVector = indexInVector;
}
//---------------------------------------------------
Detail::Detail() { //конструктор по умолчанию
	this->Cost = 0;
	this->Name = "empty name";
	//this->indexInVector = indexStatic++;
	this->id = ++Count;
};

Detail::Detail(string Name, double Cost) { //конструктор с параметрами	
	this->Name = Name;
	this->Cost = Cost;
	this->indexInVector = indexStatic++;
	this->id = ++Count;
};

Detail::Detail(const Detail& other) { //конструктор копирования
	this->Name = other.Name;
	this->Cost = other.Cost;
	this->indexInVector = other.indexInVector;
	this->id = other.id;
};


Detail::~Detail() {		//деструктор		
	this->Name = "empty name";
	this->Cost = 0;
	this->indexInVector = -1;
	this->id = -1;
};


bool Detail:: operator ==(const Detail& other) {
	if (this->Cost == other.Cost)
		return true;
	else return false;
}

bool Detail:: operator !=(const Detail& other) {
	if (!(this->Cost == other.Cost))
		return true;
	else return false;
}

bool Detail:: operator <(const Detail& other) {//не нужны, но пусть будут
	if (this->Cost <= other.Cost && this->id <= other.id && this->Name <= other.Name)
		return true;
	else return false;
}

bool Detail:: operator >(const Detail& other) {
	if (this->Cost >= other.Cost && this->id >= other.id && this->Name >= other.Name)
		return true;
	else return false;
}

ostream& operator<<(ostream& out, const Detail& detail) {
	cout << "Name: " << detail.Name << "\nCost: " << detail.Cost << "\nid: " << detail.id << endl;
	return out;
};

istream& operator>>(istream& in, Detail& detail) {
	in >> detail.Cost;
	in >> detail.Name;
	return in;
};

void mem::set_Quantity_of_DDR(int quantity) { 
	quantity_of_DDR = quantity; 
}

double mem::get_Quantity_of_DDR() {
	return quantity_of_DDR;
}

int mem::quantity_of_DDR = 1;
//---------------------------------------------------






