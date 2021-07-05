#pragma once
#include "Detail.h"
#include <algorithm>
#include <math.h>
#include <msclr/marshal_cppstd.h>  //��� ����������� System::String^ � std::string
#include <iomanip>//����� ��� setw, ������� ������ ������ ����������� ���� ��� ���������� �������� � ������
using namespace std;
using namespace System;

template <class T>
void sortByName(vector<T>& v, System::Windows::Forms::ComboBox^ comboBox);
template <class T>
void sortByCost(vector<T>& v);
template <class T>
void sortById(vector<T>& v, System::Windows::Forms::ComboBox^ comboBox);
template <class T>
void sortByIndex(vector<T>& v, System::Windows::Forms::ComboBox^ comboBox);
template <class T>
string ForConf(double budget, double orientZena, vector<T>& v, T& Det, string comment, string comment2, string comment3,
	string comment4, string comment5);
template <class T>
void create(vector<T>& myVector, ifstream& file, string type, int nr);
int processor(CPU& proc, double budget, string comment);
template <class T>
void vectorToCmbBx(vector<T>& v, System::Windows::Forms::ComboBox^ comboBox);
template <class T>
void printNameAndCost(System::Windows::Forms::ComboBox^ comboBox, System::Windows::Forms::TextBox^ txtBox, T& det, int i);
template <class T>
void OutputCostByDetailByChoosenDetail(vector<T>& v, System::Windows::Forms::ComboBox^ cmbbx, System::Windows::Forms::TextBox^ txtbx);


template <class T>
void sortByName(vector<T>& v, System::Windows::Forms::ComboBox^ comboBox) {
	if (v.empty()) {
		cout << "vector is empty" << endl;
		return;
	}
	else {
		sort(v.begin(), v.end(), [](T& firstDetail, T& secondDetail) {
			return	firstDetail.getName() < secondDetail.getName();
			});
	}
}

template <class T>
void sortByCost(vector<T>& v) {
	if (v.empty()) {
		cout << "vector is empty" << endl;
		return;
	}
	else {
		sort(v.begin(), v.end(), [](T& firstDetail, T& secondDetail) {
			return	firstDetail.getCost() < secondDetail.getCost();
			});
	}
}

template <class T>
void sortById(vector<T>& v, System::Windows::Forms::ComboBox^ comboBox) {
	if (v.empty()) {
		cout << "vector is empty" << endl;
		return;
	}
	else {
		sort(v.begin(), v.end(), [](T& firstDetail, T& secondDetail) {
			return	firstDetail.getId() < secondDetail.getId();
			});
	}
}

template <class T>
void sortByIndex(vector<T>& v, System::Windows::Forms::ComboBox^ comboBox) {
	if (v.empty()) {
		cout << "vector is empty" << endl;
		return;
	}
	else {
		for (int i = 0; i < v.size(); ++i)
			v[i].setIndex(i);
	}
}


template <class T>
string ForConf(double budget, double orientZena, vector<T>& v, T& Det, string comment, string comment2 = " ", string comment3 = " ",
	string comment4 = " ", string comment5 = " ") {//�������� ������ � ������	
	//comment2: � ������ ���������� "AMD" ����� ����������� � � ������ �������������
	//���� � ��� �� ���������, �� ������������� � " " �� ������� ������
	double raznica, predRaznica = 999999999999;
	int frequency_power, j, k, c = 0, quantity_of_proper_details = 0, quantity_of_DDR = mem::get_Quantity_of_DDR();
	string name, type;
	if (quantity_of_DDR == 1 && comment == "Soc-AM4" || comment == "Soc-1200")//����������� ���-�� ������ � MB ����� 2, 
		quantity_of_DDR = 2; //������� �� ����� ������ MB �� ����������� ���-�� ��������� ������ ������ �� 2

	for (int i = 0; i < v.size(); ++i) {
		j = k = 0;

		double cost = v[i].getCost();
		name = v[i].getName(); //���������� ��������, ����� �� �������� ��������� ����� getName	

		if (comment == "DDR IV") {//���� ������ ��������������� ������ ������
			if (comment3 == "KiTof2" && name.find("KiTof2") == -1)//���� ��� ������ ��� "KiTof2", 
				continue;//�� ���������� ��� ������, � ���� ����� ��� "KiTof2"
			else if (comment3 == "���������" && name.find("KiTof2") != -1)
				continue;//���� ��� ������ "���������", �� ���������� ��� ������ "KiTof2"
			if (comment4 != " " && name.find(comment4+"Gb") == -1)//���� ���� ������� ���-�� �� � ������  
				continue;//�� ���� � ��������������� ������ ��� ������ ������ ���-�� ��, �� �� � ����������
		}
		if (comment2 == "SSD") {//���� ������������ ���� ������ ��� ���-�� �� � SSD, �� ���������� ���, ��� �� ��������
			if(comment3 == "120Gb" && name.find("120Gb") == -1)
				continue;
			if (comment3 == "240Gb" && name.find("240Gb") == -1)
				continue;
			if (comment3 == "500Gb" && name.find("500Gb") == -1)
				continue;
		}
		if (comment2 == "HDD") {//���� ������������ ���� ������ ��� ���-�� �� � SSD, �� ���������� ���, ��� �� ��������
			if (comment3 == "1Tb" && name.find("1Tb") == -1)
				continue;
			if (comment3 == "2Tb" && name.find("2Tb") == -1)
				continue;
			if (comment3 == "4Tb" && name.find("4Tb") == -1)
				continue;
		}
		if (budget <= 400 && comment == "ATX" || comment == "MicroATX")//���� ���� ������� � �� �������� ������
			if (name.find("W ") == -1)//�������� ������ ������� �� ���������� ������ ������� (���� � �������� ���� W (�����)) (������������ ��������)
				continue;//��� ��������� ������� ����������
		if (comment == "���������" && comment2 == "Socket-1200" && comment3 == "Internal")//���� ���������� ����������
			if (name.find("F ") != -1)//�� ���������� ��� �����, ������� ��� ���������� ���������� (��� ������� ��� ���)
				continue;
		if (comment == "���������" && comment2 == "Socket-AM4" && comment3 == "Internal")//���� ���������� ����������
			if (name.find("G ") == -1)//����, (...G), �.�. �� ������� ���� �� ���������� �����������
				continue;//���� ...G ���, �� ���� ���� ��� �� ��������, ���� ���������
		if (comment == "���������" && orientZena < 60 && name.find("��������� BOX") == -1)
			continue;//��� ������� ������������, ����� ����������, ����� ������ ��������� �� ���������� �������
		if (comment == "Socket-AM4" || comment == "DDR IV" || comment == "�� " || comment == "HDD " || (comment == "Universal" && comment2 != " ")) {
			if (comment2.find("X ") != -1 && comment == "Universal" && v[i].getCost() < 18)//���� � �������� ���������� AMD ���� X, �� ����� ����� ��������
				continue;
			if (comment == "DDR IV")//���� ��������������� ��������� ��� ������� ������
				j = name.find("MHz ");
			if (comment == "�� ")
				j = name.find("W ");
			if (comment == "HDD ") {
				j = name.find("RPM ");
				if (j == -1)//���� � ��� ���-�� �������� �������� ���������� �������
					j = name.find("rpm ");//�� ���� ���������
			}
			k = j;
			if (j == -1)//���� � ��� � �����-����� ���� �����-�� ������ � ������� �� ������� ��� ����������� ��� ���������
				continue;//�� �� � ������ ����������
			if (j != 0)
				while (isdigit(name[j - 1]))//��������� ��� �����, ������� ����� MHz ��� W
					--j;
			frequency_power = atoi(name.substr(j, k - j + 1).c_str()); //����������� ������ � ����� (������� ������� DDR)
			if (budget > 600) {
				if ((comment == "DDR IV" && frequency_power < 3000) //���� ���� �� 600 ������, �� ������� DDR ������� 3000
					|| (comment == "�� " && (frequency_power < 600 && name.find("Bronze") == -1))
					|| comment == "HDD" && frequency_power < 7200 && comment != "Universal") //���� ���� �� 600 ������, �� ������� DDR ������� 3000 � �������� �� �� ������ 600 �� (� �� ������ ���� � ��������� ������������), ��� ������ �� ������ �������� ����
					continue;//(������ "�������" ������������ ����� ������ �������, ��� ������, ������� � 800, � �� 600)
			}	//���� �� ����������� �������, �� ��������� � ��������� DDR
		}

		if (comment2 == "Socket-AM4" && comment == "���������" && name.find("AMD Ryzen") != -1) {
			int	j1 = name.find("Socket-");
			int k1 = j1;
			while (name[k1] != ' ')
				++k1;
			string forComment2 = name.substr(j1, k1 - j1 + 1).c_str();
			comment5 = forComment2;//��� ���������� 5-� ������� �� ������������, ��� ��� ����� ������������ ��� � �������������� ���������
		}

		if (comment == "Universal" || (name.find(comment) != -1 && name.find(comment2) != -1) || (comment=="���������" && comment5 != " ")) {//���� � ��� ������ ���������� �� �������� ���������� ��� ����� ���� "Universal" (�������� �� ������ ����), �� ��� ������
			if (comment == "Soc-AM4" || comment == "Soc-1200") { //���� �� ���� MB
				k = name.find("DDR4");
				if (k == -1)
					continue;
				while (!isdigit(name[k])) //���� �� ������ �� ���-�� ������ ������ � MB ���� � ������ ������ (��� ����� ���� 4�DDR ��� 4DDR)
					--k;
				j = k;
				while (isdigit(name[j])) //�������� � ������ �����, ������� ������������� ���-�� DDR � MB (����� � �����-����� ����� 16xDDR)
					--j;
				//++j; //����������� �� �������, ��� ��� ���� while ������������� ����� 1
				if (atoi(name.substr(j + 1, k - j).c_str()) < quantity_of_DDR) //���� � �������� MB ������������ ���-�� ������ ������ ���������
					continue; //�� ����� MB �� ����������			
			}

			if (comment2 == "1" && (name.find("DDR IV 4Gb PC") != -1 || name.find("KiTof2") != -1)) //���� ������ ������ �� 4 �� 
				continue; //��� ������� ����, �.�. ����� ����� DDR ���� �� 8+ ��. ����� ���� �� �������� ������ � 1 ������� ������, �� ������ KiTof2, �.�. ��� ����� �� 2-� ������		
		
			if (comment2 == "2" && c == 0 && name.find("KiTof2") != -1) {//���� � ��� KiTof2, �.�. ����� �� 2-� ������, �� �� ����������� ���� (������� ���� ��� ����� ������) � 2 ����
				orientZena *= 2;
				c = 1;
			}
			else if (c == 1) {
				orientZena /= 2; //���� � ��� �� �����, �� �� ���������� ���� � ����������� ���������
				c = 0;
			}

			if (comment2 == "4" && c == 0 && name.find("KiTof2") != -1) {//���� � ��� KiTof2, �.�. ����� �� 2-� ������, �� �� ����������� ���� (������� ���� ��� ����� ������) � 2 ����
				orientZena *= 2;
				c = 1;
			}
			else if (c == 1) {
				orientZena /= 2;
				c = 0;
			}
			++quantity_of_proper_details; //���������� ����� ��� ����, ����� ������, � ��� ��� �������� ������� ��� ������� �� �������
			raznica = orientZena - v[i].getCost();
			if (cost <= orientZena && raznica < predRaznica) {
				//��� ������ ���� "Universal" ������ ��� ����� � �� ���� ��������� � ��������, �� ����� �� ��������� ������, 
				//�� �� ��������� ������ ������ ����� ����, 
				//�.�. ���� ����� ������ ��� ��������, �� �� ��� �������� ����������
				//���� ��� ������ ��������� � �������� � ���� �������� ���������� ������ � ����������� ������ � ���������������, �� ���������� ���

				Det = v[i];//���������� ���� ��������� � ��������� ����������
				if (raznica == 0) //���� �� ����� ������ ���������� �� �������� ����				
					return comment;//�� ������ ��� ������ ������			
				predRaznica = raznica; //���������� ������� �������, ����� ��������� ������ �� ������, ���� ������� ����� ������� � ���������
			}
		}
	}	
	if (predRaznica == 999999999999 && quantity_of_proper_details == 0) {
		if (comment == "���������")
			type = "\"���������\"";
		if (comment == "Universal")
			type = "\"�����\"";
		if (comment == "Soc-1200" || comment == "Soc-AM4")
			type = "\"����������� �����\"";
		if (comment == "DDR IV")
			type = "\"����������� ������\"";
		if (comment == "AMD Radeon" || comment == "GeForce" || comment == "Internal")
			type = "\"����������\"";
		if (comment2 == "SSD" && (comment == "SATA" || comment == "M.2"))
			type = "\"SSD\"";
		if (comment2 == "HDD" && (comment == "SATA" || comment == "M.2"))
			type = "\"HDD\"";
		if (comment == "ATX" || comment == "MicroATX")
			type = "\"������\"";
		if (comment == "�� ")
			type = "\"���� �������\"";
		type = "������������� ���� " + type + " � ���������� ����������� ��� � �����-�����";
		MessageBox::Show(gcnew String(type.c_str()), "���������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		string ret = "return";
		return ret;
	}
	else if (predRaznica == 999999999999 && quantity_of_proper_details > 0) {
		if (comment == "���������")
			type = "\"���������\"";
		if (comment == "Universal")
			type = "\"�����\"";
		if (comment == "Soc-1200" || comment == "Soc-AM4")
			type = "\"����������� �����\"";
		if (comment == "DDR IV")
			type = "\"����������� ������\"";
		if (comment == "AMD Radeon" || comment == "GeForce" || comment == "Internal")
			type = "\"����������\"";
		if (comment2 == "SSD" && (comment == "SATA" || comment == "M.2"))
			type = "\"SSD\"";
		if (comment2 == "HDD" && (comment == "SATA" || comment == "M.2"))
			type = "\"HDD\"";
		if (comment == "ATX" || comment == "MicroATX")
			type = "\"������\"";
		if (comment == "�� ")
			type = "\"���� �������\"";
		type = "�� ������������� ���� " + type + " � ���������� ����������� �� ������� �������";
		MessageBox::Show(gcnew String(type.c_str()), "���������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		string ret = "return";
		return ret;
	}

	return comment;
}


template <class T>
void create(vector<T>& myVector, ifstream& file, string type, int nr) {//��������� ��������� �, ��������� �� � ����������� � ����������� ������ ����� ��������� ������	

	file.clear();
	file.seekg(0);
	int begin, end;
	string line, NameForVector, CostForVector, ifItsServer;
	int i, j, number, frequency, z, y, Y=0, firstRaz = 1, firstRaz2 = 1;
	while (getline(file, line)) {
		if (line.find("Socket-1151") != -1 || line.find("Soc-1151") != -1)//���������� ������ ���������� �� ���������
			continue;
		if (line.find("HDD") != -1  && (line.find("500Gb") != -1 || line.find("500 Gb") != -1))//HDD �� 500�� ��� �� �����, 
			continue;//�.�. ����� ��������� 3$ � ������ HDD �� 1�� (������ ����� ��������� ������� � ���� � ����� ������� ������� � )		

		for (i = 0; i < line.length(); ++i) {//�������� ��������� ������
			i = line.find("\"", i + 1);//���� � ������ ���� �����, �� visual studio �������� �� ��� ��� �����, �������� ���. �������
			if (i == -1)//���� � ������ �� �������� �������(��� �� �� ����)
				break;//�� ������� �� �����
			if (!isdigit(line[i - 1]) || line[i - 1] != ';') //����� ������� � ������, �� �������, ���� �� ������� ����� (����� ��� ����)
				line.erase(i, 1);//���� ������� ����� ��� ��� ; (������ ������), �� ��� "���������" �������, � �� �� �������								
				/*line.erase(line.find(";\"", 1));
				line.erase(line.find("\";", 1));*/
		}


		if (line.find("������") != -1 || line.find("������") != -1) {//���� �� ����� �� ��������� �������������				
			z = y = 0;
			while (line[z]) {//���� �� ������� ��� ������
				if (line[z] == ';' && line[z + 1] == ';' && firstRaz) {//���� � ��� ����� ������ � ����� �������, �� ������� ������ ��� ��� �������������
					firstRaz = 0;
					++y;//���������� ���-�� ';'
				}
				++z;
			}
			Y = y;//�� ���������� ������ �������� (; ������ ���� �� ������) �����, ����� �� ����� �� ��������� �������� ������					
			getline(file, line);//��������� ��������� ������
			do {
				y = 0;
				z = 1;
				while (line[z]) {//���� �� ������� ��� ������
					if (line[z - 1] == ';' && line[z] != ';' && z < line.size() && firstRaz2) {
						begin = z;
						firstRaz2 = 0;
					}
					if (line[z] == ';' && line[z + 1] == ';')//���� � ��� ����� ������ � ����� �������, �� ������� ������ ��� ��� �������������
						++y;
					++z;
				}
				if (y < Y && line[begin] < 'A' || line[begin] > '�')//���� ��� ��� �������������
					//� �� �������� �� ����� ��� ��� "03 - ������� ��� ��������� ��������", � �� �������� ������ �������� �������� ���, �������� "LGA 3647"
					getline(file, line);//�� ���� �� ������ �� ��������� ����� ������ �� ��������� (����������) ��� ������
				else break;
			} while (1);
		}	

		i = 0;
		j = 0;
		number = 0;
		if (type == ";����� " && line.find("Universal") != -1)
			firstRaz2 = 1;
		if (type != ";����� " || (type == ";����� " && firstRaz2 == 1))//�� ������� ��������� ������ ���� "Universal"(� �������� ��� ����� ����� � �� ��������������, ������� ��������� ���� ���� ������� ����� �������� �����)
			if (line.find(type) != -1) { //���� �� ����� �������� ���������, ����������� �� ��� ������
				i = j = 0;
				begin = line.find(';');	//2-� ������� � ������
				while (line[begin + 1] == ';')//� ��� ����� ���� ������ ������, 
					++begin;//������� �� ���������� ������ ';'
				end = line.find(';', begin + 1);	//2-� �������� ������� � ������
				NameForVector = line.substr(begin + 1, end - 1 - begin);//substr ��������� �� begin (end-1-begin) ��������
				//�������, ������� � ������ 2-�� ������� � �� ��� ���������  -- ��� �������� �������������
			
			}
			else continue;
		else continue;

		for (i = line.length(); i >= 0; --i) { //��� � ������ � ����� ( ; ���������� ����� ������)
			if (line[i] == ';')
				++number;
			if (number >= nr)
				break;
			if (line[i] == ';' && i != j) //���� ������� ������ ��� ����� ������� ';' �� ���������� ��� �������
				j = i;
		}
		while (line[i] == ';')
			++i; //����� � � ������� ����� ������ (��-�� ������ ����� i ��� ����� ������ ��� ���� �����)
		if (isdigit(line[i])) //���� �� ������ ��� ������ ����� �������� � ����� ��� �����
			CostForVector = line.substr(i, i - j + 1);
		else continue;
		//�� ��� ���� (c_str ����������� string � char*)		
		myVector.push_back(T(NameForVector, atof(CostForVector.c_str()))); //����� ���� �������� ����� ��������� � ����� �������
	}
}




int processor(CPU& proc, double budget, string comment) {	
	if ((comment == "Internal" || budget < 400) && ((proc.getName().find("Socket-1200") != -1 && proc.getName().find("F ") == -1)
		|| (proc.getName().find("Socket-AM4") != -1 && proc.getName().find("G ") != -1)) && proc.getName().find("��������� BOX") != -1)
		return 2;//��� ������� ������, ���� ��������� � �� ���������� ��������, � �� ���������� �������, �� ���������� 2
	else if ((comment == "Internal" || budget < 400) && ((proc.getName().find("Socket-1200") != -1 && proc.getName().find("F ") == -1)
		|| (proc.getName().find("Socket-AM4") != -1 && proc.getName().find("G ") != -1)))
		return 1;//��� ������� ������, ���� ��������� �� ���������� ��������, �� ���������� 1
	//���������� ������ ��� ���������� ������ ���� ���� ������� ��� ������������ ��������� ������ "Internal"
	else return 0;
}

template <class T>
void vectorToCmbBx(vector<T>& v, System::Windows::Forms::ComboBox^ comboBox) {
	if (v.empty())//���� � �� �����-����� �� ������� ������� ������������� ������� ����, �� � comboBox ������ ��������
		return;
	else 
		for (int i = 0; i < v.size(); ++i) //���� �� ������� ������, �� ������� ��� � ��������������� comboBox		
			comboBox->Items->Add(gcnew String(v[i].getName().c_str()));	//����������� ��� � winforms-������ � ��������� ��� ������ � ��������������� comboBox
}

template <class T>
void printNameAndCost(System::Windows::Forms::ComboBox^ comboBox, System::Windows::Forms::TextBox^ txtBox, T& det, int i = -1) {//��� ������, ��������� ����������
	if (i == -1)//���� ��������� ��� �� ���������� ������������
		i = comboBox->SelectedIndex;

	if (comboBox->Items->Count == 0) {//���� � �� �����-����� �� ������� ������� ������������� ������� ����, �� comboBox ����
		comboBox->Text = gcnew String("� �����-����� �� ������� ������������� ������� ����"); //������� ����������� � ��������������� ����������
		comboBox->Show();
	}
	else {//���� ������ comboBox �� ����
		if (det.getName() == "empty name") {
			comboBox->Text = comboBox->Items[0]->ToString();
			comboBox->Show();			
		}
		else {
			comboBox->Text = comboBox->Items[i]->ToString();//�� ������� �������� ��������� �������������
			comboBox->Show();
			txtBox->Text = det.getCost().ToString();//� � ����
			txtBox->Show();		
		}
	}
}


template <class T>
void OutputCostByDetailByChoosenDetail(vector<T>& v, System::Windows::Forms::ComboBox^ cmbbx, System::Windows::Forms::TextBox^ txtbx){//����� ������ ������ ������������
	int i = cmbbx->SelectedIndex; //�������� ������ ���������� ��������
	if (i == -1) //���� �������� ������
		return;//�� ������� �� �������
	else if (i >= 0) {
		txtbx->Text = v[i].getCost().ToString(); //� ����������� ����������, ���������������� ���� ������� ���������� ���� �� ������ ������
		txtbx->Show();//� ������� ����������� ��������
	}
}


static vector<CPU> vCPU; //��������� ���������� ����������, ����� � ����� ������ ����� ����� ��� ���� ��������
static vector<MB> vMB;
static vector<Cooler> vCooler;
static vector<mem> vMem;
static vector<GPU> vGPU;
static vector<ssd> vSSD;
static vector<hdd> vHDD;
static vector<Case> vCase;
static vector<PSU> vPSU;

static CPU CPU1;
static Cooler Cooler1;
static MB MB1;
static mem DDR1;
static GPU GPU1;
static hdd HDD1;
static ssd SSD1;
static Case Case1;
static PSU PSU1;

static double totalWithoutNacenka, total, nacenka, budget, budget_bez_nacenki;
static ifstream inFile;
static ofstream outFile;
//------------------------------------------------------------------------------------------------------------------------------------------------------



namespace Kursach {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// ������ ��� MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: �������� ��� ������������
			//
		}

	protected:
		/// <summary>
		/// ���������� ��� ������������ �������.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ label1;
	protected:
	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::TextBox^ textBoxCost;


	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::GroupBox^ groupBox2;
	private: System::Windows::Forms::TextBox^ textBoxNacenka;


	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::GroupBox^ groupBox3;
	private: System::Windows::Forms::RadioButton^ radioButtonSocketAM4;

	private: System::Windows::Forms::RadioButton^ radioButtonSocket1200;



	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::GroupBox^ groupBox4;
	private: System::Windows::Forms::RadioButton^ radioButton_4_plankiDDR;
	private: System::Windows::Forms::RadioButton^ radioButton_1_plankaDDR;



	private: System::Windows::Forms::RadioButton^ radioButton_2_plankiDDR;




	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::GroupBox^ groupBox5;
	private: System::Windows::Forms::RadioButton^ radioButtonAMDRadeon;
	private: System::Windows::Forms::RadioButton^ radioButtonGeForce;




	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::Button^ button1_Make_conf;
	private: System::Windows::Forms::ComboBox^ comboBoxCPU;

	private:





	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::ComboBox^ comboBoxGPU;

	private:



	private: System::Windows::Forms::Label^ label10;
	private: System::Windows::Forms::ComboBox^ comboBoxMem;

	private:



	private: System::Windows::Forms::Label^ label11;
	private: System::Windows::Forms::ComboBox^ comboBoxMB;

	private:



	private: System::Windows::Forms::Label^ label12;
	private: System::Windows::Forms::ComboBox^ comboBoxCooler;

	private:



	private: System::Windows::Forms::Label^ label13;
	private: System::Windows::Forms::ComboBox^ comboBoxSSD;

	private:



	private: System::Windows::Forms::Label^ label14;
private: System::Windows::Forms::ComboBox^ comboBoxHDD;

	private:



	private: System::Windows::Forms::Label^ label15;
private: System::Windows::Forms::ComboBox^ comboBoxCase;

	private:



	private: System::Windows::Forms::Label^ label16;
private: System::Windows::Forms::ComboBox^ comboBoxPSU;

	private:



	private: System::Windows::Forms::Label^ label17;
	private: System::Windows::Forms::Label^ label18;
	private: System::Windows::Forms::TextBox^ textBoxCPUcost;

	private: System::Windows::Forms::TextBox^ textBoxCoolerCost;
	private: System::Windows::Forms::TextBox^ textBoxMemCost;
	private: System::Windows::Forms::TextBox^ textBoxMBcost;




	private: System::Windows::Forms::TextBox^ textBoxPSUcost;









private: System::Windows::Forms::TextBox^ textBoxCaseCost;
private: System::Windows::Forms::TextBox^ textBoxHDDcost;
private: System::Windows::Forms::TextBox^ textBoxSSDcost;
private: System::Windows::Forms::TextBox^ textBoxGPUcost;
private: System::Windows::Forms::Button^ button_Sort_by_cost;
private: System::Windows::Forms::Button^ button_Sort_by_name;
private: System::Windows::Forms::Button^ button_Sort_by_id;























	private: System::Windows::Forms::Label^ label19;
private: System::Windows::Forms::TextBox^ txtBox_cost_bez_nacenki;


	private: System::Windows::Forms::Label^ label20;
private: System::Windows::Forms::TextBox^ txtBox_total_Cost;


	private: System::Windows::Forms::Label^ label21;
private: System::Windows::Forms::RadioButton^ radioButtonInternal;
private: System::Windows::Forms::Button^ buttonConfigInExcel;
private: System::Windows::Forms::TextBox^ txtBoxQuantityOfDDR;
private: System::Windows::Forms::GroupBox^ groupBox8;
private: System::Windows::Forms::Label^ label24;
private: System::Windows::Forms::GroupBox^ groupBoxTypePlanok;
private: System::Windows::Forms::RadioButton^ radioButton1;
private: System::Windows::Forms::Label^ label23;
private: System::Windows::Forms::RadioButton^ radioButtonKiTof2;
private: System::Windows::Forms::RadioButton^ radioButtonOdinochnyePlanki;
private: System::Windows::Forms::GroupBox^ groupBoxGbInPlanka;
private: System::Windows::Forms::RadioButton^ radioButton2;
private: System::Windows::Forms::RadioButton^ radioButtonDDR32Gb;

private: System::Windows::Forms::RadioButton^ radioButtonDDR16Gb;

private: System::Windows::Forms::RadioButton^ radioButtonDDR4Gb;
private: System::Windows::Forms::RadioButton^ radioButtonDDR8Gb;


private: System::Windows::Forms::Label^ label22;
private: System::Windows::Forms::GroupBox^ groupBox7;
private: System::Windows::Forms::RadioButton^ radioButton7;
private: System::Windows::Forms::RadioButton^ radioButtonSSD500Gb;

private: System::Windows::Forms::RadioButton^ radioButtonSSD120Gb;
private: System::Windows::Forms::RadioButton^ radioButtonSSD240Gb;


private: System::Windows::Forms::Label^ label26;
private: System::Windows::Forms::GroupBox^ groupBox6;
private: System::Windows::Forms::RadioButton^ radioButton6;
private: System::Windows::Forms::RadioButton^ radioButtonHDD4Tb;

private: System::Windows::Forms::RadioButton^ radioButtonHDD1Tb;
private: System::Windows::Forms::RadioButton^ radioButtonHDD2Tb;


private: System::Windows::Forms::Label^ label25;

































	private:
		/// <summary>
		/// ������������ ���������� ������������.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// ��������� ����� ��� ��������� ������������ � �� ��������� 
		/// ���������� ����� ������ � ������� ��������� ����.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->textBoxCost = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->textBoxNacenka = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->radioButtonSocketAM4 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonSocket1200 = (gcnew System::Windows::Forms::RadioButton());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->radioButton_4_plankiDDR = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton_1_plankaDDR = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton_2_plankiDDR = (gcnew System::Windows::Forms::RadioButton());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->radioButtonInternal = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonAMDRadeon = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonGeForce = (gcnew System::Windows::Forms::RadioButton());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->button1_Make_conf = (gcnew System::Windows::Forms::Button());
			this->comboBoxCPU = (gcnew System::Windows::Forms::ComboBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->comboBoxGPU = (gcnew System::Windows::Forms::ComboBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->comboBoxMem = (gcnew System::Windows::Forms::ComboBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->comboBoxMB = (gcnew System::Windows::Forms::ComboBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->comboBoxCooler = (gcnew System::Windows::Forms::ComboBox());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->comboBoxSSD = (gcnew System::Windows::Forms::ComboBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->comboBoxHDD = (gcnew System::Windows::Forms::ComboBox());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->comboBoxCase = (gcnew System::Windows::Forms::ComboBox());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->comboBoxPSU = (gcnew System::Windows::Forms::ComboBox());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->textBoxCPUcost = (gcnew System::Windows::Forms::TextBox());
			this->textBoxCoolerCost = (gcnew System::Windows::Forms::TextBox());
			this->textBoxMemCost = (gcnew System::Windows::Forms::TextBox());
			this->textBoxMBcost = (gcnew System::Windows::Forms::TextBox());
			this->textBoxPSUcost = (gcnew System::Windows::Forms::TextBox());
			this->textBoxCaseCost = (gcnew System::Windows::Forms::TextBox());
			this->textBoxHDDcost = (gcnew System::Windows::Forms::TextBox());
			this->textBoxSSDcost = (gcnew System::Windows::Forms::TextBox());
			this->textBoxGPUcost = (gcnew System::Windows::Forms::TextBox());
			this->button_Sort_by_cost = (gcnew System::Windows::Forms::Button());
			this->button_Sort_by_name = (gcnew System::Windows::Forms::Button());
			this->button_Sort_by_id = (gcnew System::Windows::Forms::Button());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->txtBox_cost_bez_nacenki = (gcnew System::Windows::Forms::TextBox());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->txtBox_total_Cost = (gcnew System::Windows::Forms::TextBox());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->buttonConfigInExcel = (gcnew System::Windows::Forms::Button());
			this->txtBoxQuantityOfDDR = (gcnew System::Windows::Forms::TextBox());
			this->groupBox8 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox7 = (gcnew System::Windows::Forms::GroupBox());
			this->radioButton7 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonSSD500Gb = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonSSD120Gb = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonSSD240Gb = (gcnew System::Windows::Forms::RadioButton());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->groupBox6 = (gcnew System::Windows::Forms::GroupBox());
			this->radioButton6 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonHDD4Tb = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonHDD1Tb = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonHDD2Tb = (gcnew System::Windows::Forms::RadioButton());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->groupBoxTypePlanok = (gcnew System::Windows::Forms::GroupBox());
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->radioButtonKiTof2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonOdinochnyePlanki = (gcnew System::Windows::Forms::RadioButton());
			this->groupBoxGbInPlanka = (gcnew System::Windows::Forms::GroupBox());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonDDR32Gb = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonDDR16Gb = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonDDR4Gb = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonDDR8Gb = (gcnew System::Windows::Forms::RadioButton());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->groupBox5->SuspendLayout();
			this->groupBox8->SuspendLayout();
			this->groupBox7->SuspendLayout();
			this->groupBox6->SuspendLayout();
			this->groupBoxTypePlanok->SuspendLayout();
			this->groupBoxGbInPlanka->SuspendLayout();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label1->Location = System::Drawing::Point(15, 16);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(208, 20);
			this->label1->TabIndex = 0;
			this->label1->Text = L"������� ������ �������";
			this->label1->Click += gcnew System::EventHandler(this, &MyForm::Label1_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->textBoxCost);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Location = System::Drawing::Point(23, 23);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(239, 77);
			this->groupBox1->TabIndex = 1;
			this->groupBox1->TabStop = false;
			// 
			// textBoxCost
			// 
			this->textBoxCost->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxCost->Location = System::Drawing::Point(75, 45);
			this->textBoxCost->Name = L"textBoxCost";
			this->textBoxCost->Size = System::Drawing::Size(97, 22);
			this->textBoxCost->TabIndex = 2;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label2->Location = System::Drawing::Point(15, 45);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(57, 20);
			this->label2->TabIndex = 1;
			this->label2->Text = L"����=";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->textBoxNacenka);
			this->groupBox2->Controls->Add(this->label3);
			this->groupBox2->Controls->Add(this->label4);
			this->groupBox2->Location = System::Drawing::Point(268, 23);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(239, 77);
			this->groupBox2->TabIndex = 2;
			this->groupBox2->TabStop = false;
			// 
			// textBoxNacenka
			// 
			this->textBoxNacenka->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxNacenka->Location = System::Drawing::Point(102, 45);
			this->textBoxNacenka->Name = L"textBoxNacenka";
			this->textBoxNacenka->Size = System::Drawing::Size(100, 22);
			this->textBoxNacenka->TabIndex = 2;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label3->Location = System::Drawing::Point(15, 45);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(83, 20);
			this->label3->TabIndex = 1;
			this->label3->Text = L"�������=";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label4->Location = System::Drawing::Point(15, 16);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(140, 20);
			this->label4->TabIndex = 0;
			this->label4->Text = L"������� �������";
			this->label4->Click += gcnew System::EventHandler(this, &MyForm::Label4_Click);
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->radioButtonSocketAM4);
			this->groupBox3->Controls->Add(this->radioButtonSocket1200);
			this->groupBox3->Controls->Add(this->label6);
			this->groupBox3->Location = System::Drawing::Point(513, 23);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(243, 77);
			this->groupBox3->TabIndex = 3;
			this->groupBox3->TabStop = false;
			// 
			// radioButtonSocketAM4
			// 
			this->radioButtonSocketAM4->AutoSize = true;
			this->radioButtonSocketAM4->Location = System::Drawing::Point(145, 40);
			this->radioButtonSocketAM4->Name = L"radioButtonSocketAM4";
			this->radioButtonSocketAM4->Size = System::Drawing::Size(84, 17);
			this->radioButtonSocketAM4->TabIndex = 2;
			this->radioButtonSocketAM4->TabStop = true;
			this->radioButtonSocketAM4->Text = L"Socket-AM4";
			this->radioButtonSocketAM4->UseVisualStyleBackColor = true;
			// 
			// radioButtonSocket1200
			// 
			this->radioButtonSocket1200->AutoSize = true;
			this->radioButtonSocket1200->Location = System::Drawing::Point(19, 40);
			this->radioButtonSocket1200->Name = L"radioButtonSocket1200";
			this->radioButtonSocket1200->Size = System::Drawing::Size(86, 17);
			this->radioButtonSocket1200->TabIndex = 1;
			this->radioButtonSocket1200->TabStop = true;
			this->radioButtonSocket1200->Text = L"Socket-1200";
			this->radioButtonSocket1200->UseVisualStyleBackColor = true;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label6->Location = System::Drawing::Point(15, 16);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(224, 20);
			this->label6->TabIndex = 0;
			this->label6->Text = L"Socket-1200 ��� Socket-AM4";
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->radioButton_4_plankiDDR);
			this->groupBox4->Controls->Add(this->radioButton_1_plankaDDR);
			this->groupBox4->Controls->Add(this->radioButton_2_plankiDDR);
			this->groupBox4->Controls->Add(this->label5);
			this->groupBox4->Location = System::Drawing::Point(762, 23);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(192, 77);
			this->groupBox4->TabIndex = 4;
			this->groupBox4->TabStop = false;
			// 
			// radioButton_4_plankiDDR
			// 
			this->radioButton_4_plankiDDR->AutoSize = true;
			this->radioButton_4_plankiDDR->Location = System::Drawing::Point(141, 40);
			this->radioButton_4_plankiDDR->Name = L"radioButton_4_plankiDDR";
			this->radioButton_4_plankiDDR->Size = System::Drawing::Size(31, 17);
			this->radioButton_4_plankiDDR->TabIndex = 3;
			this->radioButton_4_plankiDDR->TabStop = true;
			this->radioButton_4_plankiDDR->Text = L"4";
			this->radioButton_4_plankiDDR->UseVisualStyleBackColor = true;
			// 
			// radioButton_1_plankaDDR
			// 
			this->radioButton_1_plankaDDR->AutoSize = true;
			this->radioButton_1_plankaDDR->Location = System::Drawing::Point(19, 40);
			this->radioButton_1_plankaDDR->Name = L"radioButton_1_plankaDDR";
			this->radioButton_1_plankaDDR->Size = System::Drawing::Size(31, 17);
			this->radioButton_1_plankaDDR->TabIndex = 2;
			this->radioButton_1_plankaDDR->TabStop = true;
			this->radioButton_1_plankaDDR->Text = L"1";
			this->radioButton_1_plankaDDR->UseVisualStyleBackColor = true;
			this->radioButton_1_plankaDDR->CheckedChanged += gcnew System::EventHandler(this, &MyForm::RadioButton3_CheckedChanged);
			// 
			// radioButton_2_plankiDDR
			// 
			this->radioButton_2_plankiDDR->AutoSize = true;
			this->radioButton_2_plankiDDR->Location = System::Drawing::Point(78, 40);
			this->radioButton_2_plankiDDR->Name = L"radioButton_2_plankiDDR";
			this->radioButton_2_plankiDDR->Size = System::Drawing::Size(31, 17);
			this->radioButton_2_plankiDDR->TabIndex = 1;
			this->radioButton_2_plankiDDR->TabStop = true;
			this->radioButton_2_plankiDDR->Text = L"2";
			this->radioButton_2_plankiDDR->UseVisualStyleBackColor = true;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label5->Location = System::Drawing::Point(15, 16);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(158, 20);
			this->label5->TabIndex = 0;
			this->label5->Text = L"���������� ������";
			// 
			// groupBox5
			// 
			this->groupBox5->Controls->Add(this->radioButtonInternal);
			this->groupBox5->Controls->Add(this->radioButtonAMDRadeon);
			this->groupBox5->Controls->Add(this->radioButtonGeForce);
			this->groupBox5->Controls->Add(this->label7);
			this->groupBox5->Location = System::Drawing::Point(960, 23);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(243, 77);
			this->groupBox5->TabIndex = 5;
			this->groupBox5->TabStop = false;
			// 
			// radioButtonInternal
			// 
			this->radioButtonInternal->AutoSize = true;
			this->radioButtonInternal->Location = System::Drawing::Point(171, 39);
			this->radioButtonInternal->Name = L"radioButtonInternal";
			this->radioButtonInternal->Size = System::Drawing::Size(60, 17);
			this->radioButtonInternal->TabIndex = 3;
			this->radioButtonInternal->TabStop = true;
			this->radioButtonInternal->Text = L"Internal";
			this->radioButtonInternal->UseVisualStyleBackColor = true;
			// 
			// radioButtonAMDRadeon
			// 
			this->radioButtonAMDRadeon->AutoSize = true;
			this->radioButtonAMDRadeon->Location = System::Drawing::Point(6, 39);
			this->radioButtonAMDRadeon->Name = L"radioButtonAMDRadeon";
			this->radioButtonAMDRadeon->Size = System::Drawing::Size(90, 17);
			this->radioButtonAMDRadeon->TabIndex = 2;
			this->radioButtonAMDRadeon->TabStop = true;
			this->radioButtonAMDRadeon->Text = L"AMD Radeon";
			this->radioButtonAMDRadeon->UseVisualStyleBackColor = true;
			// 
			// radioButtonGeForce
			// 
			this->radioButtonGeForce->AutoSize = true;
			this->radioButtonGeForce->Location = System::Drawing::Point(102, 39);
			this->radioButtonGeForce->Name = L"radioButtonGeForce";
			this->radioButtonGeForce->Size = System::Drawing::Size(66, 17);
			this->radioButtonGeForce->TabIndex = 1;
			this->radioButtonGeForce->TabStop = true;
			this->radioButtonGeForce->Text = L"GeForce";
			this->radioButtonGeForce->UseVisualStyleBackColor = true;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label7->Location = System::Drawing::Point(15, 16);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(205, 20);
			this->label7->TabIndex = 0;
			this->label7->Text = L"AMD Radeon ��� GeForce";
			// 
			// button1_Make_conf
			// 
			this->button1_Make_conf->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button1_Make_conf->ForeColor = System::Drawing::SystemColors::ControlText;
			this->button1_Make_conf->Location = System::Drawing::Point(50, 616);
			this->button1_Make_conf->Name = L"button1_Make_conf";
			this->button1_Make_conf->Size = System::Drawing::Size(256, 62);
			this->button1_Make_conf->TabIndex = 6;
			this->button1_Make_conf->Text = L"��������� ������������";
			this->button1_Make_conf->UseVisualStyleBackColor = true;
			this->button1_Make_conf->Click += gcnew System::EventHandler(this, &MyForm::Button1_Click);
			// 
			// comboBoxCPU
			// 
			this->comboBoxCPU->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::SuggestAppend;
			this->comboBoxCPU->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
			this->comboBoxCPU->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->comboBoxCPU->FormattingEnabled = true;
			this->comboBoxCPU->Location = System::Drawing::Point(126, 238);
			this->comboBoxCPU->Name = L"comboBoxCPU";
			this->comboBoxCPU->Size = System::Drawing::Size(923, 28);
			this->comboBoxCPU->TabIndex = 7;
			this->comboBoxCPU->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::ComboBoxCPU_SelectedIndexChanged);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label8->Location = System::Drawing::Point(16, 238);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(91, 20);
			this->label8->TabIndex = 3;
			this->label8->Text = L"���������";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label9->Location = System::Drawing::Point(16, 401);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(102, 20);
			this->label9->TabIndex = 8;
			this->label9->Text = L"����������";
			// 
			// comboBoxGPU
			// 
			this->comboBoxGPU->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::SuggestAppend;
			this->comboBoxGPU->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
			this->comboBoxGPU->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->comboBoxGPU->FormattingEnabled = true;
			this->comboBoxGPU->Location = System::Drawing::Point(126, 401);
			this->comboBoxGPU->Name = L"comboBoxGPU";
			this->comboBoxGPU->Size = System::Drawing::Size(923, 28);
			this->comboBoxGPU->TabIndex = 9;
			this->comboBoxGPU->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::ComboBoxGPU_SelectedIndexChanged);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label10->Location = System::Drawing::Point(16, 360);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(68, 20);
			this->label10->TabIndex = 10;
			this->label10->Text = L"������";
			// 
			// comboBoxMem
			// 
			this->comboBoxMem->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::SuggestAppend;
			this->comboBoxMem->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
			this->comboBoxMem->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->comboBoxMem->FormattingEnabled = true;
			this->comboBoxMem->Location = System::Drawing::Point(126, 359);
			this->comboBoxMem->Name = L"comboBoxMem";
			this->comboBoxMem->Size = System::Drawing::Size(923, 28);
			this->comboBoxMem->TabIndex = 11;
			this->comboBoxMem->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::ComboBoxMem_SelectedIndexChanged);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label11->Location = System::Drawing::Point(16, 321);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(94, 20);
			this->label11->TabIndex = 12;
			this->label11->Text = L"���. �����";
			// 
			// comboBoxMB
			// 
			this->comboBoxMB->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::SuggestAppend;
			this->comboBoxMB->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
			this->comboBoxMB->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->comboBoxMB->FormattingEnabled = true;
			this->comboBoxMB->Location = System::Drawing::Point(126, 321);
			this->comboBoxMB->Name = L"comboBoxMB";
			this->comboBoxMB->Size = System::Drawing::Size(923, 28);
			this->comboBoxMB->TabIndex = 13;
			this->comboBoxMB->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::ComboBoxMB_SelectedIndexChanged);
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label12->Location = System::Drawing::Point(16, 279);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(64, 20);
			this->label12->TabIndex = 14;
			this->label12->Text = L"������";
			// 
			// comboBoxCooler
			// 
			this->comboBoxCooler->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::SuggestAppend;
			this->comboBoxCooler->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
			this->comboBoxCooler->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->comboBoxCooler->FormattingEnabled = true;
			this->comboBoxCooler->Location = System::Drawing::Point(126, 279);
			this->comboBoxCooler->Name = L"comboBoxCooler";
			this->comboBoxCooler->Size = System::Drawing::Size(923, 28);
			this->comboBoxCooler->TabIndex = 15;
			this->comboBoxCooler->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::ComboBoxCooler_SelectedIndexChanged);
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label13->Location = System::Drawing::Point(16, 442);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(84, 20);
			this->label13->TabIndex = 20;
			this->label13->Text = L"���� SSD";
			// 
			// comboBoxSSD
			// 
			this->comboBoxSSD->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::SuggestAppend;
			this->comboBoxSSD->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
			this->comboBoxSSD->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->comboBoxSSD->FormattingEnabled = true;
			this->comboBoxSSD->Location = System::Drawing::Point(126, 442);
			this->comboBoxSSD->Name = L"comboBoxSSD";
			this->comboBoxSSD->Size = System::Drawing::Size(923, 28);
			this->comboBoxSSD->TabIndex = 21;
			this->comboBoxSSD->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::ComboBoxSSD_SelectedIndexChanged);
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label14->Location = System::Drawing::Point(16, 480);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(86, 20);
			this->label14->TabIndex = 18;
			this->label14->Text = L"���� HDD";
			// 
			// comboBoxHDD
			// 
			this->comboBoxHDD->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::SuggestAppend;
			this->comboBoxHDD->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
			this->comboBoxHDD->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->comboBoxHDD->FormattingEnabled = true;
			this->comboBoxHDD->Location = System::Drawing::Point(126, 480);
			this->comboBoxHDD->Name = L"comboBoxHDD";
			this->comboBoxHDD->Size = System::Drawing::Size(923, 28);
			this->comboBoxHDD->TabIndex = 19;
			this->comboBoxHDD->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::ComboBoxHDD_SelectedIndexChanged);
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label15->Location = System::Drawing::Point(16, 525);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(61, 20);
			this->label15->TabIndex = 16;
			this->label15->Text = L"������";
			// 
			// comboBoxCase
			// 
			this->comboBoxCase->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::SuggestAppend;
			this->comboBoxCase->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
			this->comboBoxCase->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->comboBoxCase->FormattingEnabled = true;
			this->comboBoxCase->Location = System::Drawing::Point(126, 525);
			this->comboBoxCase->Name = L"comboBoxCase";
			this->comboBoxCase->Size = System::Drawing::Size(923, 28);
			this->comboBoxCase->TabIndex = 17;
			this->comboBoxCase->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::ComboBoxCase_SelectedIndexChanged);
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label16->Location = System::Drawing::Point(16, 567);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(32, 20);
			this->label16->TabIndex = 22;
			this->label16->Text = L"��";
			// 
			// comboBoxPSU
			// 
			this->comboBoxPSU->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::SuggestAppend;
			this->comboBoxPSU->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
			this->comboBoxPSU->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->comboBoxPSU->FormattingEnabled = true;
			this->comboBoxPSU->Location = System::Drawing::Point(126, 567);
			this->comboBoxPSU->Name = L"comboBoxPSU";
			this->comboBoxPSU->Size = System::Drawing::Size(923, 28);
			this->comboBoxPSU->TabIndex = 23;
			this->comboBoxPSU->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::ComboBoxPSU_SelectedIndexChanged);
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label17->Location = System::Drawing::Point(158, 209);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(208, 20);
			this->label17->TabIndex = 24;
			this->label17->Text = L"�������� �������������";
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label18->Location = System::Drawing::Point(1130, 211);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(48, 20);
			this->label18->TabIndex = 25;
			this->label18->Text = L"����";
			// 
			// textBoxCPUcost
			// 
			this->textBoxCPUcost->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxCPUcost->Location = System::Drawing::Point(1105, 240);
			this->textBoxCPUcost->Name = L"textBoxCPUcost";
			this->textBoxCPUcost->ReadOnly = true;
			this->textBoxCPUcost->Size = System::Drawing::Size(100, 26);
			this->textBoxCPUcost->TabIndex = 26;
			// 
			// textBoxCoolerCost
			// 
			this->textBoxCoolerCost->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxCoolerCost->Location = System::Drawing::Point(1105, 281);
			this->textBoxCoolerCost->Name = L"textBoxCoolerCost";
			this->textBoxCoolerCost->ReadOnly = true;
			this->textBoxCoolerCost->Size = System::Drawing::Size(100, 26);
			this->textBoxCoolerCost->TabIndex = 27;
			// 
			// textBoxMemCost
			// 
			this->textBoxMemCost->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxMemCost->Location = System::Drawing::Point(1105, 361);
			this->textBoxMemCost->Name = L"textBoxMemCost";
			this->textBoxMemCost->ReadOnly = true;
			this->textBoxMemCost->Size = System::Drawing::Size(100, 26);
			this->textBoxMemCost->TabIndex = 29;
			// 
			// textBoxMBcost
			// 
			this->textBoxMBcost->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxMBcost->Location = System::Drawing::Point(1105, 323);
			this->textBoxMBcost->Name = L"textBoxMBcost";
			this->textBoxMBcost->ReadOnly = true;
			this->textBoxMBcost->Size = System::Drawing::Size(100, 26);
			this->textBoxMBcost->TabIndex = 28;
			// 
			// textBoxPSUcost
			// 
			this->textBoxPSUcost->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxPSUcost->Location = System::Drawing::Point(1105, 569);
			this->textBoxPSUcost->Name = L"textBoxPSUcost";
			this->textBoxPSUcost->ReadOnly = true;
			this->textBoxPSUcost->Size = System::Drawing::Size(100, 26);
			this->textBoxPSUcost->TabIndex = 31;
			// 
			// textBoxCaseCost
			// 
			this->textBoxCaseCost->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxCaseCost->Location = System::Drawing::Point(1105, 527);
			this->textBoxCaseCost->Name = L"textBoxCaseCost";
			this->textBoxCaseCost->ReadOnly = true;
			this->textBoxCaseCost->Size = System::Drawing::Size(100, 26);
			this->textBoxCaseCost->TabIndex = 30;
			// 
			// textBoxHDDcost
			// 
			this->textBoxHDDcost->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxHDDcost->Location = System::Drawing::Point(1105, 482);
			this->textBoxHDDcost->Name = L"textBoxHDDcost";
			this->textBoxHDDcost->ReadOnly = true;
			this->textBoxHDDcost->Size = System::Drawing::Size(100, 26);
			this->textBoxHDDcost->TabIndex = 32;
			// 
			// textBoxSSDcost
			// 
			this->textBoxSSDcost->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxSSDcost->Location = System::Drawing::Point(1105, 444);
			this->textBoxSSDcost->Name = L"textBoxSSDcost";
			this->textBoxSSDcost->ReadOnly = true;
			this->textBoxSSDcost->Size = System::Drawing::Size(100, 26);
			this->textBoxSSDcost->TabIndex = 33;
			// 
			// textBoxGPUcost
			// 
			this->textBoxGPUcost->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxGPUcost->Location = System::Drawing::Point(1105, 403);
			this->textBoxGPUcost->Name = L"textBoxGPUcost";
			this->textBoxGPUcost->ReadOnly = true;
			this->textBoxGPUcost->Size = System::Drawing::Size(100, 26);
			this->textBoxGPUcost->TabIndex = 34;
			// 
			// button_Sort_by_cost
			// 
			this->button_Sort_by_cost->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->button_Sort_by_cost->ForeColor = System::Drawing::SystemColors::ControlText;
			this->button_Sort_by_cost->Location = System::Drawing::Point(692, 684);
			this->button_Sort_by_cost->Name = L"button_Sort_by_cost";
			this->button_Sort_by_cost->Size = System::Drawing::Size(152, 49);
			this->button_Sort_by_cost->TabIndex = 35;
			this->button_Sort_by_cost->Text = L"������������� �� ����";
			this->button_Sort_by_cost->UseVisualStyleBackColor = true;
			this->button_Sort_by_cost->Click += gcnew System::EventHandler(this, &MyForm::Button_Sort_by_cost_Click);
			// 
			// button_Sort_by_name
			// 
			this->button_Sort_by_name->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->button_Sort_by_name->ForeColor = System::Drawing::SystemColors::ControlText;
			this->button_Sort_by_name->Location = System::Drawing::Point(850, 684);
			this->button_Sort_by_name->Name = L"button_Sort_by_name";
			this->button_Sort_by_name->Size = System::Drawing::Size(152, 49);
			this->button_Sort_by_name->TabIndex = 36;
			this->button_Sort_by_name->Text = L"������������� �� ��������";
			this->button_Sort_by_name->UseVisualStyleBackColor = true;
			this->button_Sort_by_name->Click += gcnew System::EventHandler(this, &MyForm::Button_Sort_by_name_Click);
			// 
			// button_Sort_by_id
			// 
			this->button_Sort_by_id->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_Sort_by_id->ForeColor = System::Drawing::SystemColors::ControlText;
			this->button_Sort_by_id->Location = System::Drawing::Point(1008, 684);
			this->button_Sort_by_id->Name = L"button_Sort_by_id";
			this->button_Sort_by_id->Size = System::Drawing::Size(152, 49);
			this->button_Sort_by_id->TabIndex = 37;
			this->button_Sort_by_id->Text = L"������������� �� ID";
			this->button_Sort_by_id->UseVisualStyleBackColor = true;
			this->button_Sort_by_id->Click += gcnew System::EventHandler(this, &MyForm::Button_Sort_by_id_Click);
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label19->Location = System::Drawing::Point(646, 615);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(144, 20);
			this->label19->TabIndex = 38;
			this->label19->Text = L"���� ��� �������";
			// 
			// txtBox_cost_bez_nacenki
			// 
			this->txtBox_cost_bez_nacenki->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->txtBox_cost_bez_nacenki->Location = System::Drawing::Point(796, 615);
			this->txtBox_cost_bez_nacenki->Name = L"txtBox_cost_bez_nacenki";
			this->txtBox_cost_bez_nacenki->ReadOnly = true;
			this->txtBox_cost_bez_nacenki->Size = System::Drawing::Size(100, 26);
			this->txtBox_cost_bez_nacenki->TabIndex = 39;
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label20->Location = System::Drawing::Point(923, 615);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(121, 20);
			this->label20->TabIndex = 38;
			this->label20->Text = L"�������� ����";
			// 
			// txtBox_total_Cost
			// 
			this->txtBox_total_Cost->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->txtBox_total_Cost->Location = System::Drawing::Point(1050, 613);
			this->txtBox_total_Cost->Name = L"txtBox_total_Cost";
			this->txtBox_total_Cost->ReadOnly = true;
			this->txtBox_total_Cost->Size = System::Drawing::Size(100, 26);
			this->txtBox_total_Cost->TabIndex = 40;
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label21->Location = System::Drawing::Point(1055, 359);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(24, 29);
			this->label21->TabIndex = 22;
			this->label21->Text = L"x";
			// 
			// buttonConfigInExcel
			// 
			this->buttonConfigInExcel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->buttonConfigInExcel->ForeColor = System::Drawing::SystemColors::ControlText;
			this->buttonConfigInExcel->Location = System::Drawing::Point(12, 684);
			this->buttonConfigInExcel->Name = L"buttonConfigInExcel";
			this->buttonConfigInExcel->Size = System::Drawing::Size(371, 49);
			this->buttonConfigInExcel->TabIndex = 41;
			this->buttonConfigInExcel->Text = L"��������� ������������ � Excel";
			this->buttonConfigInExcel->UseVisualStyleBackColor = true;
			this->buttonConfigInExcel->Click += gcnew System::EventHandler(this, &MyForm::ButtonConfigInExcel_Click);
			// 
			// txtBoxQuantityOfDDR
			// 
			this->txtBoxQuantityOfDDR->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->txtBoxQuantityOfDDR->Location = System::Drawing::Point(1074, 359);
			this->txtBoxQuantityOfDDR->Name = L"txtBoxQuantityOfDDR";
			this->txtBoxQuantityOfDDR->Size = System::Drawing::Size(22, 29);
			this->txtBoxQuantityOfDDR->TabIndex = 42;
			this->txtBoxQuantityOfDDR->TextChanged += gcnew System::EventHandler(this, &MyForm::TxtBoxQuantityOfDDR_TextChanged);
			// 
			// groupBox8
			// 
			this->groupBox8->Controls->Add(this->groupBox7);
			this->groupBox8->Controls->Add(this->groupBox6);
			this->groupBox8->Controls->Add(this->label24);
			this->groupBox8->Controls->Add(this->groupBoxTypePlanok);
			this->groupBox8->Controls->Add(this->groupBoxGbInPlanka);
			this->groupBox8->Location = System::Drawing::Point(23, 108);
			this->groupBox8->Name = L"groupBox8";
			this->groupBox8->Size = System::Drawing::Size(1157, 100);
			this->groupBox8->TabIndex = 48;
			this->groupBox8->TabStop = false;
			// 
			// groupBox7
			// 
			this->groupBox7->Controls->Add(this->radioButton7);
			this->groupBox7->Controls->Add(this->radioButtonSSD500Gb);
			this->groupBox7->Controls->Add(this->radioButtonSSD120Gb);
			this->groupBox7->Controls->Add(this->radioButtonSSD240Gb);
			this->groupBox7->Controls->Add(this->label26);
			this->groupBox7->Location = System::Drawing::Point(232, 17);
			this->groupBox7->Name = L"groupBox7";
			this->groupBox7->Size = System::Drawing::Size(252, 77);
			this->groupBox7->TabIndex = 50;
			this->groupBox7->TabStop = false;
			// 
			// radioButton7
			// 
			this->radioButton7->AutoSize = true;
			this->radioButton7->Location = System::Drawing::Point(164, 39);
			this->radioButton7->Name = L"radioButton7";
			this->radioButton7->Size = System::Drawing::Size(71, 17);
			this->radioButton7->TabIndex = 4;
			this->radioButton7->TabStop = true;
			this->radioButton7->Text = L"�������";
			this->radioButton7->UseVisualStyleBackColor = true;
			// 
			// radioButtonSSD500Gb
			// 
			this->radioButtonSSD500Gb->AutoSize = true;
			this->radioButtonSSD500Gb->Location = System::Drawing::Point(106, 39);
			this->radioButtonSSD500Gb->Name = L"radioButtonSSD500Gb";
			this->radioButtonSSD500Gb->Size = System::Drawing::Size(43, 17);
			this->radioButtonSSD500Gb->TabIndex = 3;
			this->radioButtonSSD500Gb->TabStop = true;
			this->radioButtonSSD500Gb->Text = L"500";
			this->radioButtonSSD500Gb->UseVisualStyleBackColor = true;
			// 
			// radioButtonSSD120Gb
			// 
			this->radioButtonSSD120Gb->AutoSize = true;
			this->radioButtonSSD120Gb->Location = System::Drawing::Point(6, 39);
			this->radioButtonSSD120Gb->Name = L"radioButtonSSD120Gb";
			this->radioButtonSSD120Gb->Size = System::Drawing::Size(43, 17);
			this->radioButtonSSD120Gb->TabIndex = 2;
			this->radioButtonSSD120Gb->TabStop = true;
			this->radioButtonSSD120Gb->Text = L"120";
			this->radioButtonSSD120Gb->UseVisualStyleBackColor = true;
			// 
			// radioButtonSSD240Gb
			// 
			this->radioButtonSSD240Gb->AutoSize = true;
			this->radioButtonSSD240Gb->Location = System::Drawing::Point(57, 39);
			this->radioButtonSSD240Gb->Name = L"radioButtonSSD240Gb";
			this->radioButtonSSD240Gb->Size = System::Drawing::Size(43, 17);
			this->radioButtonSSD240Gb->TabIndex = 1;
			this->radioButtonSSD240Gb->TabStop = true;
			this->radioButtonSSD240Gb->Text = L"240";
			this->radioButtonSSD240Gb->UseVisualStyleBackColor = true;
			// 
			// label26
			// 
			this->label26->AutoSize = true;
			this->label26->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label26->Location = System::Drawing::Point(15, 16);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(173, 20);
			this->label26->TabIndex = 0;
			this->label26->Text = L"���������� �� � SSD";
			// 
			// groupBox6
			// 
			this->groupBox6->Controls->Add(this->radioButton6);
			this->groupBox6->Controls->Add(this->radioButtonHDD4Tb);
			this->groupBox6->Controls->Add(this->radioButtonHDD1Tb);
			this->groupBox6->Controls->Add(this->radioButtonHDD2Tb);
			this->groupBox6->Controls->Add(this->label25);
			this->groupBox6->Location = System::Drawing::Point(6, 17);
			this->groupBox6->Name = L"groupBox6";
			this->groupBox6->Size = System::Drawing::Size(206, 77);
			this->groupBox6->TabIndex = 49;
			this->groupBox6->TabStop = false;
			// 
			// radioButton6
			// 
			this->radioButton6->AutoSize = true;
			this->radioButton6->Location = System::Drawing::Point(133, 39);
			this->radioButton6->Name = L"radioButton6";
			this->radioButton6->Size = System::Drawing::Size(71, 17);
			this->radioButton6->TabIndex = 4;
			this->radioButton6->TabStop = true;
			this->radioButton6->Text = L"�������";
			this->radioButton6->UseVisualStyleBackColor = true;
			// 
			// radioButtonHDD4Tb
			// 
			this->radioButtonHDD4Tb->AutoSize = true;
			this->radioButtonHDD4Tb->Location = System::Drawing::Point(84, 39);
			this->radioButtonHDD4Tb->Name = L"radioButtonHDD4Tb";
			this->radioButtonHDD4Tb->Size = System::Drawing::Size(31, 17);
			this->radioButtonHDD4Tb->TabIndex = 3;
			this->radioButtonHDD4Tb->TabStop = true;
			this->radioButtonHDD4Tb->Text = L"4";
			this->radioButtonHDD4Tb->UseVisualStyleBackColor = true;
			// 
			// radioButtonHDD1Tb
			// 
			this->radioButtonHDD1Tb->AutoSize = true;
			this->radioButtonHDD1Tb->Location = System::Drawing::Point(6, 39);
			this->radioButtonHDD1Tb->Name = L"radioButtonHDD1Tb";
			this->radioButtonHDD1Tb->Size = System::Drawing::Size(31, 17);
			this->radioButtonHDD1Tb->TabIndex = 2;
			this->radioButtonHDD1Tb->TabStop = true;
			this->radioButtonHDD1Tb->Text = L"1";
			this->radioButtonHDD1Tb->UseVisualStyleBackColor = true;
			// 
			// radioButtonHDD2Tb
			// 
			this->radioButtonHDD2Tb->AutoSize = true;
			this->radioButtonHDD2Tb->Location = System::Drawing::Point(47, 39);
			this->radioButtonHDD2Tb->Name = L"radioButtonHDD2Tb";
			this->radioButtonHDD2Tb->Size = System::Drawing::Size(31, 17);
			this->radioButtonHDD2Tb->TabIndex = 1;
			this->radioButtonHDD2Tb->TabStop = true;
			this->radioButtonHDD2Tb->Text = L"2";
			this->radioButtonHDD2Tb->UseVisualStyleBackColor = true;
			// 
			// label25
			// 
			this->label25->AutoSize = true;
			this->label25->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label25->Location = System::Drawing::Point(15, 16);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(175, 20);
			this->label25->TabIndex = 0;
			this->label25->Text = L"���������� �� � HDD";
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label24->Location = System::Drawing::Point(412, 0);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(229, 20);
			this->label24->TabIndex = 48;
			this->label24->Text = L"�������������� ���������";
			// 
			// groupBoxTypePlanok
			// 
			this->groupBoxTypePlanok->Controls->Add(this->radioButton1);
			this->groupBoxTypePlanok->Controls->Add(this->label23);
			this->groupBoxTypePlanok->Controls->Add(this->radioButtonKiTof2);
			this->groupBoxTypePlanok->Controls->Add(this->radioButtonOdinochnyePlanki);
			this->groupBoxTypePlanok->Location = System::Drawing::Point(520, 17);
			this->groupBoxTypePlanok->Name = L"groupBoxTypePlanok";
			this->groupBoxTypePlanok->Size = System::Drawing::Size(271, 77);
			this->groupBoxTypePlanok->TabIndex = 46;
			this->groupBoxTypePlanok->TabStop = false;
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Location = System::Drawing::Point(189, 39);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(71, 17);
			this->radioButton1->TabIndex = 46;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"�������";
			this->radioButton1->UseVisualStyleBackColor = true;
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label23->Location = System::Drawing::Point(49, 16);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(94, 20);
			this->label23->TabIndex = 0;
			this->label23->Text = L"��� ������";
			// 
			// radioButtonKiTof2
			// 
			this->radioButtonKiTof2->AutoSize = true;
			this->radioButtonKiTof2->Location = System::Drawing::Point(120, 39);
			this->radioButtonKiTof2->Name = L"radioButtonKiTof2";
			this->radioButtonKiTof2->Size = System::Drawing::Size(56, 17);
			this->radioButtonKiTof2->TabIndex = 44;
			this->radioButtonKiTof2->TabStop = true;
			this->radioButtonKiTof2->Text = L"KiTof2";
			this->radioButtonKiTof2->UseVisualStyleBackColor = true;
			// 
			// radioButtonOdinochnyePlanki
			// 
			this->radioButtonOdinochnyePlanki->AutoSize = true;
			this->radioButtonOdinochnyePlanki->Location = System::Drawing::Point(17, 39);
			this->radioButtonOdinochnyePlanki->Name = L"radioButtonOdinochnyePlanki";
			this->radioButtonOdinochnyePlanki->Size = System::Drawing::Size(82, 17);
			this->radioButtonOdinochnyePlanki->TabIndex = 45;
			this->radioButtonOdinochnyePlanki->TabStop = true;
			this->radioButtonOdinochnyePlanki->Text = L"���������";
			this->radioButtonOdinochnyePlanki->UseVisualStyleBackColor = true;
			// 
			// groupBoxGbInPlanka
			// 
			this->groupBoxGbInPlanka->Controls->Add(this->radioButton2);
			this->groupBoxGbInPlanka->Controls->Add(this->radioButtonDDR32Gb);
			this->groupBoxGbInPlanka->Controls->Add(this->radioButtonDDR16Gb);
			this->groupBoxGbInPlanka->Controls->Add(this->radioButtonDDR4Gb);
			this->groupBoxGbInPlanka->Controls->Add(this->radioButtonDDR8Gb);
			this->groupBoxGbInPlanka->Controls->Add(this->label22);
			this->groupBoxGbInPlanka->Location = System::Drawing::Point(797, 17);
			this->groupBoxGbInPlanka->Name = L"groupBoxGbInPlanka";
			this->groupBoxGbInPlanka->Size = System::Drawing::Size(310, 77);
			this->groupBoxGbInPlanka->TabIndex = 43;
			this->groupBoxGbInPlanka->TabStop = false;
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Location = System::Drawing::Point(232, 40);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(71, 17);
			this->radioButton2->TabIndex = 6;
			this->radioButton2->TabStop = true;
			this->radioButton2->Text = L"�������";
			this->radioButton2->UseVisualStyleBackColor = true;
			// 
			// radioButtonDDR32Gb
			// 
			this->radioButtonDDR32Gb->AutoSize = true;
			this->radioButtonDDR32Gb->Location = System::Drawing::Point(180, 39);
			this->radioButtonDDR32Gb->Name = L"radioButtonDDR32Gb";
			this->radioButtonDDR32Gb->Size = System::Drawing::Size(37, 17);
			this->radioButtonDDR32Gb->TabIndex = 4;
			this->radioButtonDDR32Gb->TabStop = true;
			this->radioButtonDDR32Gb->Text = L"32";
			this->radioButtonDDR32Gb->UseVisualStyleBackColor = true;
			// 
			// radioButtonDDR16Gb
			// 
			this->radioButtonDDR16Gb->AutoSize = true;
			this->radioButtonDDR16Gb->Location = System::Drawing::Point(114, 40);
			this->radioButtonDDR16Gb->Name = L"radioButtonDDR16Gb";
			this->radioButtonDDR16Gb->Size = System::Drawing::Size(37, 17);
			this->radioButtonDDR16Gb->TabIndex = 3;
			this->radioButtonDDR16Gb->TabStop = true;
			this->radioButtonDDR16Gb->Text = L"16";
			this->radioButtonDDR16Gb->UseVisualStyleBackColor = true;
			// 
			// radioButtonDDR4Gb
			// 
			this->radioButtonDDR4Gb->AutoSize = true;
			this->radioButtonDDR4Gb->Location = System::Drawing::Point(6, 39);
			this->radioButtonDDR4Gb->Name = L"radioButtonDDR4Gb";
			this->radioButtonDDR4Gb->Size = System::Drawing::Size(31, 17);
			this->radioButtonDDR4Gb->TabIndex = 2;
			this->radioButtonDDR4Gb->TabStop = true;
			this->radioButtonDDR4Gb->Text = L"4";
			this->radioButtonDDR4Gb->UseVisualStyleBackColor = true;
			// 
			// radioButtonDDR8Gb
			// 
			this->radioButtonDDR8Gb->AutoSize = true;
			this->radioButtonDDR8Gb->Location = System::Drawing::Point(57, 40);
			this->radioButtonDDR8Gb->Name = L"radioButtonDDR8Gb";
			this->radioButtonDDR8Gb->Size = System::Drawing::Size(31, 17);
			this->radioButtonDDR8Gb->TabIndex = 1;
			this->radioButtonDDR8Gb->TabStop = true;
			this->radioButtonDDR8Gb->Text = L"8";
			this->radioButtonDDR8Gb->UseVisualStyleBackColor = true;
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label22->Location = System::Drawing::Point(15, 16);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(244, 20);
			this->label22->TabIndex = 0;
			this->label22->Text = L"���������� �� � ����� ������";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1215, 806);
			this->Controls->Add(this->groupBox8);
			this->Controls->Add(this->txtBoxQuantityOfDDR);
			this->Controls->Add(this->buttonConfigInExcel);
			this->Controls->Add(this->txtBox_total_Cost);
			this->Controls->Add(this->txtBox_cost_bez_nacenki);
			this->Controls->Add(this->label20);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->label19);
			this->Controls->Add(this->button_Sort_by_id);
			this->Controls->Add(this->button_Sort_by_name);
			this->Controls->Add(this->button_Sort_by_cost);
			this->Controls->Add(this->textBoxGPUcost);
			this->Controls->Add(this->textBoxSSDcost);
			this->Controls->Add(this->textBoxHDDcost);
			this->Controls->Add(this->textBoxPSUcost);
			this->Controls->Add(this->textBoxCaseCost);
			this->Controls->Add(this->textBoxMemCost);
			this->Controls->Add(this->textBoxMBcost);
			this->Controls->Add(this->textBoxCoolerCost);
			this->Controls->Add(this->textBoxCPUcost);
			this->Controls->Add(this->label18);
			this->Controls->Add(this->label17);
			this->Controls->Add(this->label21);
			this->Controls->Add(this->label16);
			this->Controls->Add(this->comboBoxPSU);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->comboBoxSSD);
			this->Controls->Add(this->label14);
			this->Controls->Add(this->comboBoxHDD);
			this->Controls->Add(this->label15);
			this->Controls->Add(this->comboBoxCase);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->comboBoxCooler);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->comboBoxMB);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->comboBoxMem);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->comboBoxGPU);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->comboBoxCPU);
			this->Controls->Add(this->button1_Make_conf);
			this->Controls->Add(this->groupBox5);
			this->Controls->Add(this->groupBox4);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->groupBox1);
			this->HelpButton = true;
			this->Name = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			this->groupBox5->ResumeLayout(false);
			this->groupBox5->PerformLayout();
			this->groupBox8->ResumeLayout(false);
			this->groupBox8->PerformLayout();
			this->groupBox7->ResumeLayout(false);
			this->groupBox7->PerformLayout();
			this->groupBox6->ResumeLayout(false);
			this->groupBox6->PerformLayout();
			this->groupBoxTypePlanok->ResumeLayout(false);
			this->groupBoxTypePlanok->PerformLayout();
			this->groupBoxGbInPlanka->ResumeLayout(false);
			this->groupBoxGbInPlanka->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion		
		String^ Commentary; //��� ������ �� �����
		String^ Sockets() {
			if (radioButtonSocket1200->Checked) { Commentary = "Socket-1200"; return Commentary; }
			else if (radioButtonSocketAM4->Checked) { Commentary = "Socket-AM4"; return Commentary; }
		}
		String^ planki_quantity() {

			if (radioButton_1_plankaDDR->Checked) { Commentary = "1"; return Commentary; }
			else if (radioButton_2_plankiDDR->Checked) { Commentary = "2"; return Commentary; }
			else if (radioButton_4_plankiDDR->Checked) { Commentary = "4"; return Commentary; }
		}

		String^ planki_type() {
			if (radioButtonOdinochnyePlanki->Checked) { Commentary = "���������"; return Commentary; }
			else if (radioButtonKiTof2->Checked) { Commentary = "KiTof2"; return Commentary; }
			else { Commentary = " "; return Commentary; }
		}

		String^ planki_Gb() {
			if (radioButtonDDR4Gb->Checked) {
				if (radioButtonKiTof2->Checked) {
					Commentary = "8"; return Commentary;
				}
				else {
					Commentary = "4"; return Commentary;
				}
			}
			else if (radioButtonDDR8Gb->Checked) {
				if (radioButtonKiTof2->Checked) {
					Commentary = "16"; return Commentary;
				}
				else {
					Commentary = "8"; return Commentary;
				}
			}		
			else if (radioButtonDDR16Gb->Checked) {
				if (radioButtonKiTof2->Checked) {
					Commentary = "32"; return Commentary;
				}
				else {
					Commentary = "16"; return Commentary;
				}
			}
			else if (radioButtonDDR32Gb->Checked) {
				if (radioButtonKiTof2->Checked) {
					Commentary = "64"; return Commentary;
				}
				else {
					Commentary = "32"; return Commentary;
				}
			}				
			else { Commentary = " "; return Commentary; }
		}
		String^ videocard() {
			if (radioButtonAMDRadeon->Checked) { Commentary = "AMD Radeon"; return Commentary; }
			else if (radioButtonGeForce->Checked) { Commentary = "GeForce"; return Commentary; }
			else if (radioButtonInternal->Checked) { Commentary = "Internal"; return Commentary; }
		}
		String^ SSD_Gb() {
			if (radioButtonSSD120Gb->Checked) { Commentary = "120Gb"; return Commentary; }
			else if (radioButtonSSD240Gb->Checked) { Commentary = "240Gb"; return Commentary; }
			else if (radioButtonSSD500Gb->Checked) { Commentary = "500Gb"; return Commentary; }
			else { Commentary = " "; return Commentary; }
		}
		String^ HDD_Tb() {
			if (radioButtonHDD1Tb->Checked) { Commentary = "1Tb"; return Commentary; }
			else if (radioButtonHDD2Tb->Checked) { Commentary = "2Tb"; return Commentary; }
			else if (radioButtonHDD4Tb->Checked) { Commentary = "4Tb"; return Commentary; }
			else { Commentary = " "; return Commentary; }
		}


	private: System::Void Label1_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void Label4_Click(System::Object^ sender, System::EventArgs^ e) {
	}
private: System::Void RadioButton3_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
}

	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
		setlocale(LC_ALL, "russian");
		double budget, budget_bez_nacenki;
		string line, type;//����������, ������ � ����� ������ �� ������ ����� ���������
		
		try {
			inFile.open("Price.csv");
			if (!inFile.is_open())
				throw exception();
		}
		catch (Exception^ ex) {
			MessageBox::Show(this, "���� ��� ������ �� ��� ������\n"+ex, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			exit(-1);			
		}

		try {
			outFile.open("������� ������������.xls", ofstream::app);
			if (!outFile.is_open())
				throw exception();
		}
		catch (Exception^ ex) {
			MessageBox::Show(this, "���� ��� ������ �� ��� ������"+ex, "���������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			exit(-1);
		}	
		
		outFile << "�������� �������������" << "\t" << "���� $" << endl;
		outFile << endl;
		
		outFile.setf(ios::right);
		outFile.setf(ios::fixed);
		outFile.precision(3);
		

		int number = 0;
		getline(inFile, line, '!');
		for (int i = line.size(); i >= 0; --i) {//���� � ����� ������ �� ����� ��������� ����
			//(��� ������ � ������ ��������� ; )
			if (line[i] == ';')	//������ ���, ��� �� ������� ; ��������, ��� ������ �� �������� � ���������(����������) ������
				++number;
			if (i == line.find(";����")) //���� �� ����� �� ������ � �����
				break;		//�� ���� �������		
		}

		type = "��������� ";
		vCPU.push_back(CPU("None", 0));//� ����� ������ ������� ����� ������� "None" � ����� ������ 0. ��� ����� �� ������, ���� ������������� �� �����
		create(vCPU, inFile, type, number);//�� ��� ���� (c_str ����������� string � char*)	
		inFile.clear();
		inFile.seekg(0);
		Detail::setStaticIndexToZero();//�������� ����������� ���������� (indexStatic), ����� ��� ������ ������� ������ ���� � ������

		type = ";MB ";
		vMB.push_back(MB("None", 0));//� ����� ������ ������� ����� ������� "None" � ����� ������ 0. ��� ����� �� ������, ���� ������������� �� �����
		create(vMB, inFile, type, number);
		inFile.clear();
		inFile.seekg(0);
		Detail::setStaticIndexToZero();//�������� ����������� ���������� (indexStatic), ����� ��� ������ ������� ������ ���� � ������;

		type = ";����� ";
		vCooler.push_back(Cooler("None", 0));//� ����� ������ ������� ����� ������� "None" � ����� ������ 0. ��� ����� �� ������, ���� ������������� �� �����
		create(vCooler, inFile, type, number);
		inFile.clear();
		inFile.seekg(0);
		Detail::setStaticIndexToZero();//�������� ����������� ���������� (indexStatic), ����� ��� ������ ������� ������ ���� � ������

		type = "DDR IV";
		vMem.push_back(mem("None", 0));//� ����� ������ ������� ����� ������� "None" � ����� ������ 0. ��� ����� �� ������, ���� ������������� �� �����
		create(vMem, inFile, type, number);
		inFile.clear();
		inFile.seekg(0);
		Detail::setStaticIndexToZero();//�������� ����������� ���������� (indexStatic), ����� ��� ������ ������� ������ ���� � ������

		type = "���������� ";
		vGPU.push_back(GPU("None", 0));//� ����� ������ ������� ����� ������� "None" � ����� ������ 0. ��� ����� �� ������, ���� ������������� �� �����
		create(vGPU, inFile, type, number);
		inFile.clear();
		inFile.seekg(0);
		Detail::setStaticIndexToZero();//�������� ����������� ���������� (indexStatic), ����� ��� ������ ������� ������ ���� � ������

		type = ";SSD ";
		vSSD.push_back(ssd("None", 0));//� ����� ������ ������� ����� ������� "None" � ����� ������ 0. ��� ����� �� ������, ���� ������������� �� �����
		create(vSSD, inFile, type, number);
		inFile.clear();
		inFile.seekg(0);
		Detail::setStaticIndexToZero();//�������� ����������� ���������� (indexStatic), ����� ��� ������ ������� ������ ���� � ������
		
		type = ";HDD 3.5";
		vHDD.push_back(hdd("None", 0));//� ����� ������ ������� ����� ������� "None" � ����� ������ 0. ��� ����� �� ������, ���� ������������� �� �����
		create(vHDD, inFile, type, number);
		inFile.clear();
		inFile.seekg(0);
		Detail::setStaticIndexToZero();//�������� ����������� ���������� (indexStatic), ����� ��� ������ ������� ������ ���� � ������
		
		type = "������ ";
		vCase.push_back(Case("None", 0));//� ����� ������ ������� ����� ������� "None" � ����� ������ 0. ��� ����� �� ������, ���� ������������� �� �����
		create(vCase, inFile, type, number);
		inFile.clear();
		inFile.seekg(0);
		Detail::setStaticIndexToZero();//�������� ����������� ���������� (indexStatic), ����� ��� ������ ������� ������ ���� � ������

		type = ";�� ";
		vPSU.push_back(PSU("None", 0));//� ����� ������ ������� ����� ������� "None" � ����� ������ 0. ��� ����� �� ������, ���� ������������� �� �����
		create(vPSU, inFile, type, number);
		inFile.clear();
		inFile.seekg(0);
		Detail::setStaticIndexToZero();//�������� ����������� ���������� (indexStatic), ����� ��� ������ ������� ������ ���� � ������

			inFile.close();//�������� �����

		vectorToCmbBx(vCPU, comboBoxCPU);
		vectorToCmbBx(vMB, comboBoxMB);
		vectorToCmbBx(vCooler, comboBoxCooler);
		vectorToCmbBx(vMem, comboBoxMem);
		vectorToCmbBx(vGPU, comboBoxGPU);
		vectorToCmbBx(vSSD, comboBoxSSD);
		vectorToCmbBx(vHDD, comboBoxHDD);
		vectorToCmbBx(vCase, comboBoxCase);
		vectorToCmbBx(vPSU, comboBoxPSU);		
	}


	private: System::Void Button1_Click(System::Object^ sender, System::EventArgs^ e) {
		//������ "��������� ������������"
		
		string zena = msclr::interop::marshal_as< std::string >(textBoxCost->Text);//�������� �� ��, �������� �� ��������� ���� ��� ������� ������������� ������
		string nazenka = msclr::interop::marshal_as< std::string >(textBoxNacenka->Text);
		
		for (int m = 0; m < zena.length(); ++m) {
			if (m == 0 && zena[m] == '-')
				continue;
			if (zena[m] == '.')
				zena[m] = ',';
			if (!isdigit(zena[m]) && zena[m] != ',' || zena[0] == '-' && zena.find(1, '-')!=-1) {
				MessageBox::Show(this, "���� ������ ���� ������\n", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}
		}
		for (int m = 0; m < nazenka.length(); ++m) {
			if (m == 0 && nazenka[m] == '-')
				continue;
			if (nazenka[m] == '.')
				nazenka[m] = ',';
			if (!isdigit(nazenka[m]) && nazenka[m] != ',' || nazenka[0] == '-' && nazenka.find(1,'-')!=-1) {
				MessageBox::Show(this, "������� ������ ���� ������\n", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}
		}

		if (zena[0] == '-') {
			MessageBox::Show(this, "���� �� ������ ���� �������������\n", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		if (nazenka[0] == '-') {
			MessageBox::Show(this, "������� �� ������ ���� �������������\n", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}

		textBoxCost->Text = gcnew String(zena.c_str());
		textBoxNacenka->Text = gcnew String(nazenka.c_str());


		try {
			outFile.open("������� ������������.xls", ofstream::app);
			if (!outFile.is_open())
				throw exception();
		}
		catch (Exception^ ex) {
			MessageBox::Show(this, "���� ��� ������ �� ��� ������" + ex, "���������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			exit(-1);
		}

		double Gr_Cost; //��������� ����
		double Gr_nacenka; //��������� �������
		do {
			if (textBoxCost->Text != "" && textBoxNacenka->Text != "") {//���� ������������ �� ����� ������� ���� � �������
				Gr_Cost = System::Convert::ToDouble(textBoxCost->Text);//�� ����� ����������� �������� ���������� "����" � "�������"
				Gr_nacenka = System::Convert::ToDouble(textBoxNacenka->Text);
			}
			if (textBoxCost->Text == "" || textBoxNacenka->Text == "" || !(radioButtonSocket1200->Checked || radioButtonSocketAM4->Checked)
				|| !(radioButton_1_plankaDDR->Checked || radioButton_2_plankiDDR->Checked || radioButton_4_plankiDDR->Checked) ||
				!(radioButtonAMDRadeon->Checked || radioButtonGeForce->Checked || radioButtonInternal->Checked)) {//���� �� �� ����� ��, ��� ���� ������,
				MessageBox::Show(this, "����������, ������� ������", "���������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				//�� ����� ������� ����������� � ������ ������ ������			
				return; //� ��������� �����, ���������� ��� ������� ������ "��������� ������������"
			}		
			else break;	//���� ��� �������, �� ��������� ����������� ������
		} while (1);

		if (CPU1.getName() != "empty name") {//���� �� ������ ������������ �� � ������ ���, 
			CPU1.~CPU();//�� ������ ��� ������������ ��������� ������
			MB1.~MB();
			Cooler1.~Cooler();
			DDR1.~mem();
			GPU1.~GPU();
			SSD1.~ssd();
			HDD1.~hdd();
			Case1.~Case();
			PSU1.~PSU();

			CPU1.setName("empty name");//����� ������ ������������ � ��� ������������ � ���������� Name, ������� ������ ������ ������
			MB1.setName("empty name");//����� ��������� "empty name", �.�. �� ���� �������� ����� ������ ���������
			Cooler1.setName("empty name");
			DDR1.setName("empty name");
			GPU1.setName("empty name");
			SSD1.setName("empty name");
			HDD1.setName("empty name");
			Case1.setName("empty name");
			PSU1.setName("empty name");
		}
		
		if (radioButton_1_plankaDDR->Checked) txtBoxQuantityOfDDR->Text = "1"; //� ����������� �� ���������� ���-�� ������ ������ ����� ���������������� �����
		if (radioButton_2_plankiDDR->Checked) txtBoxQuantityOfDDR->Text = "2";
		if (radioButton_4_plankiDDR->Checked) txtBoxQuantityOfDDR->Text = "4";

		cout << "Count: " << Detail::getCount() << endl;

		budget_bez_nacenki = Gr_Cost;
		nacenka = Gr_nacenka;

		budget = budget_bez_nacenki - nacenka;

		string comment, comment2;

		comment = "���������";
		comment2 = msclr::interop::marshal_as< std::string >(Sockets());//�������� ��� ���������� (Socket-1200 or Socket-AM4)
		//��� ����� �� ����������� �������� ���� String^, ������������ �������� Sockets(), 
		//(������� ���������� ��������� � ����� ������ ���� System::String), � ��� string
		string typeOfVideocard = msclr::interop::marshal_as<std::string>(Kursach::MyForm::videocard());
		if ("return" == ForConf(budget, budget * 0.20, vCPU, CPU1, comment, comment2, typeOfVideocard))
			return;
		
		if (processor(CPU1, budget, typeOfVideocard) > 0 && budget > 400){//���� ���� �� ���������� �������� ��� ������� (���, � ��, � ������)
		//� ���� ������������ �� �� ����� �������, �� ����� �������, ������� �� �� ��������� �� GPU ��� ����� �� ������ �� CPU
			if (processor(CPU1, budget, typeOfVideocard) == 1)  //���� ��������� �� ���������� ��������
				if ("return" == ForConf(budget, budget * (0.20 + 0.34), vCPU, CPU1, comment, comment2, "Internal"))
					return;
			else if (processor(CPU1, budget, typeOfVideocard) == 2) //���� ��������� � �� ���������� ��������, � �� ���������� �������
				if ("return" == ForConf(budget, budget * (0.20 + 0.34 + 0.03), vCPU, CPU1, comment, comment2, "Internal"))
					return;				
		}
		else if (processor(CPU1, budget, typeOfVideocard) > 0 && budget < 400) {//���� ������������ ������� �������, 
			//�� � GPU �� �������� ������ �� CPU, � ������ �������� �������� �� �������� 
			if (processor(CPU1, budget, typeOfVideocard) == 1)  //���� ��������� �� ���������� ��������
				if ("return" == ForConf(budget, budget * (0.20 + 0.34 * 0.60 - 0.013), vCPU, CPU1, comment, comment2, "Internal"))// -0.013 ��� ������ �� DDR
					return;
			if (processor(CPU1, budget, typeOfVideocard) == 2) //���� ��������� � �� ���������� ��������, � �� ���������� �������
				if ("return" == ForConf(budget, budget * (0.20 + 0.34 * 0.6 + 0.03 - 0.013), vCPU, CPU1, comment, comment2, "Internal"))
					return;
		}

		if (CPU1.getName().find("��������� BOX") == -1 || budget > 400)//���� � ��� ��������� ��� ����������� ������
			if ("return" == ForConf(budget, budget * 0.03, vCooler, Cooler1, "Universal", CPU1.getName()))
				return;

		if (CPU1.getName()== "empty name") {//���� ������� �� ������� �� �������� ������, ��� ������� ���� �� ����� ��������
			MessageBox::Show(this, "��� �������� ������� ���������� ��������� ��������� ������������ CPU", "���������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			//�� ������� ���������
			return;//� ����� ������������ ������� ���-���� � ������ �� ������ ��� ���
		}

		string quantity = msclr::interop::marshal_as< std::string >(planki_quantity());	
		//�������� ������������ ���-�� ������ ������ � MB			 
		if (quantity == "1") 
			mem::set_Quantity_of_DDR(1);
		else if (quantity == "2")
			mem::set_Quantity_of_DDR(2);
		else
			mem::set_Quantity_of_DDR(4);	

		txtBoxQuantityOfDDR->Text = gcnew String(quantity.c_str());//������� ���-�� ������ � ��������������� ���������

		
		if (comment2 == "Socket-AM4") {//���� ��������� �������, �� �������� ����� ��� ����		
			if (processor(CPU1, budget, typeOfVideocard) > 0 && budget < 400) {//���� ���� �� ���������� �������� ��� ������� (���, � ��, � ������)
				//� ���� ������������ �������, �� �������� �� ����� �������, ������� �� �� ��������� �� GPU ��� ����� �� ������ �� CPU
				//� ������ �������� ������ �� MB
				if ("return" == ForConf(budget, budget * (0.12 + 0.33 * 0.4), vMB, MB1, comment = "Soc-AM4"))
					return;
			}
			else if ("return" == ForConf(budget, budget * 0.12, vMB, MB1, comment = "Soc-AM4"))
				return;
		}
		else {  //���������� � � ����������� �����������			
			if (processor(CPU1, budget, typeOfVideocard) > 0 && budget < 400) {
				if ("return" == ForConf(budget, budget * (0.12 + 0.33 * 0.4), vMB, MB1, comment = "Soc-1200"))
					return;
			}
			else if ("return" == ForConf(budget, budget * 0.12, vMB, MB1, comment = "Soc-1200"))
				return;
		}

		if (MB1.getName() == "empty name") {//���� ������� �� ������� �� �������� ������, ��� ������� ���� �� ����� ��������
			MessageBox::Show(this, "��� �������� ������� ���������� ��������� ��������� ������������ MB", "���������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			//�� ������� ���������
			return;//� ����� ������������ ������� ���-���� � ������ �� ������ ��� ���
		}
			   	
		string typePlanok = msclr::interop::marshal_as< std::string >(planki_type());
		string plankiGb = msclr::interop::marshal_as< std::string >(planki_Gb());
		string hdd_Tb = msclr::interop::marshal_as< std::string >(HDD_Tb());
		string ssd_Gb = msclr::interop::marshal_as< std::string >(SSD_Gb());
		
		if (MB1.getName().find("DDR4") != -1)
			if (quantity == "1") { //�������� ������ DDR IV (DDR III ��� ��������)	
				mem::set_Quantity_of_DDR(1); //������� ���-�� ������ ����������� ������
				if (budget < 400) {
					if ("return" == ForConf(budget, budget * (0.12 * 1.5), vMem, DDR1, "DDR IV", quantity, typePlanok, plankiGb))
						return;
				}
				else if ("return" == ForConf(budget, budget * 0.12, vMem, DDR1, "DDR IV", quantity, typePlanok, plankiGb))
					return;
			}
			else if (quantity == "2") {
				mem::set_Quantity_of_DDR(2);
				if (budget < 400) {
					if ("return" == ForConf(budget, budget * (0.0575 * 1.5), vMem, DDR1, "DDR IV", quantity, typePlanok, plankiGb))
						return;
				}
				if ("return" == ForConf(budget, budget * 0.0575, vMem, DDR1, "DDR IV", quantity, typePlanok, plankiGb))//���� 2 ������ ������, �� ���� ����� � 2 ����, ����� ��������� � ������		
					return;
			}
			else {
				mem::set_Quantity_of_DDR(4);
				if (budget < 400) {
					if ("return" == ForConf(budget, budget * (0.02875 * 1.5), vMem, DDR1, "DDR IV", quantity, typePlanok, plankiGb))
						return;
				}
				if ("return" == ForConf(budget, budget * 0.02875, vMem, DDR1, "DDR IV", quantity, typePlanok, plankiGb))	//���� 4 ������ ������, �� ���� ����� � 4 ����, ����� ��������� � ������			
					return;
			}
		if (DDR1.getName() == "empty name") {//���� ������� �� ������� �� �������� ������, ��� ������� ���� �� ����� ��������
			MessageBox::Show(this, "��� �������� ������� ���������� ��������� ��������� ������������ DDR", "���������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			//�� ������� ���������
			return;//� ����� ������������ ������� ���-���� � ������ �� ������ ��� ���
		}
		if (DDR1.getName().find("KiTof2") != -1) //���� � ��� � ������������ � �������� ���������� ���� "KiTof2", �� ��� ���� ������
			quantity = to_string(atoi(quantity.c_str()) / 2);//, �.�. ����� ���-�� �� 2 (��� ���� �������� �� KiTof � 1 ������ ������)
		if (atoi(quantity.c_str()) == 1) txtBoxQuantityOfDDR->Text = "1"; //� ����������� �� ���������� ���-�� ������ ������ ����� ���������������� �����
		if (atoi(quantity.c_str()) == 2) txtBoxQuantityOfDDR->Text = "2";
		if (atoi(quantity.c_str()) == 4) txtBoxQuantityOfDDR->Text = "4";

		//AMD Radeon or GeForce
		//�������� ��� ����������
		if (processor(CPU1, budget, typeOfVideocard) == 0)//���� ���� �� ������� ������� � ��������� ��� ���������� �������
		{//���� ������� ����, �� ������ ��������� ��������� �� ���������� ��������, ����� ���� � ��� �� ����� ����, �� �������� ����������
			comment = typeOfVideocard;
			if (comment == "AMD Radeon" || comment == "GeForce" || comment == "Internal") {
				if (CPU1.getName().find("��������� BOX") != -1) {//���� ���� �� ���������� �������
					if ("return" == ForConf(budget, budget * (0.33 + 0.03), vGPU, GPU1, comment))//�� ����� ������� ��� ������ ������ �� ����������
						return;
				}
				else if ("return" == ForConf(budget, budget * 0.33, vGPU, GPU1, comment))
					return;
			}
		}

	
			
		if (MB1.getName().find("SATA")) //� ����������� �� ��������� �������� ������ ����������� � ��� SSD � HDD
			comment = "SATA";
		else comment = "M.2";
		if (budget >= 400) {	//���� ���� �������, �� SSD ����� �������������
			if ("return" == ForConf(budget, budget * 0.07, vSSD, SSD1, comment, "SSD", ssd_Gb))//� ����������� �� comment �������� ������� � ������� ������ ����������� (SATA ��� M.2)
				return;
			if ("return" == ForConf(budget, budget * 0.06, vHDD, HDD1, comment, "HDD", hdd_Tb))
				return;
		}
		else if ("return" == ForConf(budget, budget * (0.06 + 0.07) * 1.1, vHDD, HDD1, comment, "HDD", hdd_Tb))//� ������� ������ SSD ����� ���������������
			return;//����� ����� ������� �� SSD ������ �� HDD
		
		if (HDD1.getName() == "empty name") {//���� ������� �� ������� �� �������� ������, ��� ������� ���� �� ����� ��������
			MessageBox::Show(this, "��� �������� ������� ���������� ��������� ��������� ������������ HDD", "���������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			//�� ������� ���������
			return;//� ����� ������������ ������� ���-���� � ������ �� ������ ��� ���
		}

		if (MB1.getName().find("ATX")) //� ����������� �� ����������� ����� �������� ������ �������
			comment = "ATX";
		else comment = "MicroATX";

		if (budget > 400) {//���� ���� �� ������ �������
			if ("return" == ForConf(budget, budget * 0.07, vCase, Case1, comment))//�� �������� ������ ��� ����� ������� � ���������
				return;
		}
		else if ("return" == ForConf(budget, budget * (0.07 + 0.07), vCase, Case1, comment))//����� � �� (����� ������� ��� �� ������ �� ������)
			return;
		if (Case1.getName() == "empty name") {//���� ������� �� ������� �� �������� ������, ��� ������� ���� �� ����� ��������
			MessageBox::Show(this, "��� �������� ������� ���������� ��������� ��������� ������������ Case", "���������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			//�� ������� ���������
			return;//� ����� ������������ ������� ���-���� � ������ �� ������ ��� ���
		}


		if (Case1.getName().find("W ") == -1)//���� ������ ��� �� � ���������
			if ("return" == ForConf(budget, budget * 0.07, vPSU, PSU1, "�� "))//�� ��������� ���� �������
				return;
	
			
		printNameAndCost(comboBoxCPU, textBoxCPUcost, CPU1, CPU1.getIndex());
		printNameAndCost(comboBoxMB, textBoxMBcost, MB1, MB1.getIndex());
		printNameAndCost(comboBoxCooler, textBoxCoolerCost, Cooler1, Cooler1.getIndex());
		printNameAndCost(comboBoxMem, textBoxMemCost, DDR1, DDR1.getIndex());
		printNameAndCost(comboBoxGPU, textBoxGPUcost, GPU1, GPU1.getIndex());
		printNameAndCost(comboBoxSSD, textBoxSSDcost, SSD1, SSD1.getIndex());
		printNameAndCost(comboBoxHDD, textBoxHDDcost, HDD1, HDD1.getIndex());
		printNameAndCost(comboBoxCase, textBoxCaseCost, Case1, Case1.getIndex());
		printNameAndCost(comboBoxPSU, textBoxPSUcost, PSU1, PSU1.getIndex());
		
		OutputCostByDetailByChoosenDetail(vCPU, comboBoxCPU, textBoxCPUcost);
		OutputCostByDetailByChoosenDetail(vCooler, comboBoxCooler, textBoxCoolerCost);
		OutputCostByDetailByChoosenDetail(vMB, comboBoxMB, textBoxMBcost);
		OutputCostByDetailByChoosenDetail(vMem, comboBoxMem, textBoxMemCost);
		OutputCostByDetailByChoosenDetail(vGPU, comboBoxGPU, textBoxGPUcost);
		OutputCostByDetailByChoosenDetail(vSSD, comboBoxSSD, textBoxSSDcost);
		OutputCostByDetailByChoosenDetail(vHDD, comboBoxHDD, textBoxHDDcost);
		OutputCostByDetailByChoosenDetail(vCase, comboBoxCase, textBoxCaseCost);
		OutputCostByDetailByChoosenDetail(vPSU, comboBoxPSU, textBoxPSUcost);


		double colichestvo = atoi(quantity.c_str());
		totalWithoutNacenka = CPU1.getCost() + Cooler1.getCost() + MB1.getCost() + colichestvo * DDR1.getCost()//���������� ���� ���� ������������� ������������
			+ GPU1.getCost() + HDD1.getCost() + SSD1.getCost() + Case1.getCost() + PSU1.getCost();
		total = totalWithoutNacenka + nacenka;

	
		txtBox_cost_bez_nacenki->Text = totalWithoutNacenka.ToString();//���� ��� �������
		txtBox_cost_bez_nacenki->Show();
		txtBox_total_Cost->Text = total.ToString();//�������� ����
		txtBox_total_Cost->Show();

		outFile.close();
	};



	private: System::Void ComboBoxCPU_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {//����� �������� ������ ��������� �� ������
		int i = comboBoxCPU->SelectedIndex;

		static CPU last;
		if (last.getCost() == 0 && CPU1.getCost() != 0) //���� �� ������ ��� ������� ���������� last � � ��� ��� ���� ��������� ���������� �������������
			last = CPU1;
		else if (last.getCost() == 0 && CPU1.getCost() == 0)//���� �� ������ ��� ������� ���������� last, �� 
			//������ ������� ��� �� �������������� ���������� ��� ����������� ������������
			last = vCPU[0];//�� ����������� ���� ���������� ������� "None"

			OutputCostByDetailByChoosenDetail(vCPU, comboBoxCPU, textBoxCPUcost);

			totalWithoutNacenka = round(totalWithoutNacenka * 100) / 100;//������� ����������� ������� �����	
			totalWithoutNacenka = totalWithoutNacenka - last.getCost() + vCPU[i].getCost(); //����� �������� ���� �������������, �� ������ �������� � ���� ��������

			txtBox_cost_bez_nacenki->Text = totalWithoutNacenka.ToString();//���� ��� ������� � ������
			txtBox_cost_bez_nacenki->Show();//� ������� �

			total = totalWithoutNacenka + nacenka;//����� ������������ �������� ���������
			txtBox_total_Cost->Text = total.ToString();//����������� � � ������
			txtBox_total_Cost->Show();//� �������

			last = vCPU[i];//���������� ��������� ��������� ������		
	}

	private: System::Void ComboBoxCooler_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		int i = comboBoxCooler->SelectedIndex;

		static Cooler last;
		if (last.getCost() == 0 && Cooler1.getCost() != 0) //���� �� ������ ��� ������� ���������� last � � ��� ��� ���� ��������� ���������� �������������
			last = Cooler1;
		else if (last.getCost() == 0 && Cooler1.getCost() == 0)//���� �� ������ ��� ������� ���������� last, �� 
			//������ ������� ��� �� �������������� ���������� ��� ����������� ������������
			last = vCooler[0];//�� ����������� ���� ���������� ������� "None"

		OutputCostByDetailByChoosenDetail(vCooler, comboBoxCooler, textBoxCoolerCost);

		totalWithoutNacenka = round(totalWithoutNacenka * 100) / 100;//������� ����������� ������� �����	
		totalWithoutNacenka = totalWithoutNacenka - last.getCost() + vCooler[i].getCost(); //����� �������� ���� �������������, �� ������ �������� � ���� ��������

		txtBox_cost_bez_nacenki->Text = totalWithoutNacenka.ToString();//���� ��� ������� � ������
		txtBox_cost_bez_nacenki->Show();//� ������� �

		total = totalWithoutNacenka + nacenka;//����� ������������ �������� ���������
		txtBox_total_Cost->Text = total.ToString();//����������� � � ������
		txtBox_total_Cost->Show();//� �������

		last = vCooler[i];//���������� ��������� ��������� ������		
	}

	private: System::Void ComboBoxMB_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		int i = comboBoxMB->SelectedIndex;

		static MB last;
		if (last.getCost() == 0 && MB1.getCost() != 0)//���� �� ������ ��� ������� ���������� last � � ��� ��� ���� ��������� ���������� �������������
			last = MB1;
		else if (last.getCost() == 0 && MB1.getCost() == 0)//���� �� ������ ��� ������� ���������� last, �� 
					//������ ������� ��� �� �������������� ���������� ��� ����������� ������������
			last = vMB[0];//�� ����������� ���� ���������� ������� "None"

		OutputCostByDetailByChoosenDetail(vMB, comboBoxMB, textBoxMBcost);

		totalWithoutNacenka = round(totalWithoutNacenka * 100) / 100;//������� ����������� ������� �����	
		totalWithoutNacenka = totalWithoutNacenka - last.getCost() + vMB[i].getCost(); //����� �������� ���� �������������, �� ������ �������� � ���� ��������

		txtBox_cost_bez_nacenki->Text = totalWithoutNacenka.ToString();//���� ��� ������� � ������
		txtBox_cost_bez_nacenki->Show();//� ������� �

		total = totalWithoutNacenka + nacenka;//����� ������������ �������� ���������
		txtBox_total_Cost->Text = total.ToString();//����������� � � ������
		txtBox_total_Cost->Show();//� �������

		last = vMB[i];//���������� ��������� ��������� ������		
	}

	private: System::Void ComboBoxMem_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		int i = comboBoxMem->SelectedIndex;
		if (i == -1)
			return;
		if(txtBoxQuantityOfDDR->Text == "")
			txtBoxQuantityOfDDR->Text = "1";
		static double lastCost;//��� �� ���������� lastCost, � �� last(��������� ������), �.�. ���� ��� � ����� �������� � ����� ��������,
		//�� ����� ���� ����� ��������, ��� ������� ���-�� ������ ���������� �� ����������� ����������
		if (lastCost == 0 && DDR1.getCost() != 0)//���� �� ������ ��� ������� ���������� last � � ��� ��� ���� ��������� ���������� �������������
			lastCost = mem::get_Quantity_of_DDR() * DDR1.getCost();//���������� ���� ��������� ��������� ������ ������, ���������� �� ���-�� ������
		else if (lastCost == 0 && DDR1.getCost() == 0)//���� �� ������ ��� ������� ���������� last, �� 
					//������ ������� ��� �� �������������� ���������� ��� ����������� ������������
			lastCost = vMem[0].getCost();//�� ����������� ���� ���������� ������� "None"
		
		OutputCostByDetailByChoosenDetail(vMem, comboBoxMem, textBoxMemCost);

		totalWithoutNacenka = round(totalWithoutNacenka * 100) / 100;//������� ����������� ������� �����	
		totalWithoutNacenka = totalWithoutNacenka - lastCost + mem::get_Quantity_of_DDR() * vMem[i].getCost(); //����� �������� ���� �������������, �� ������ �������� � ���� ��������
			//�������� ��������� ������ ������ �� ���������� ������

		txtBox_cost_bez_nacenki->Text = totalWithoutNacenka.ToString();//���� ��� ������� � ������
		txtBox_cost_bez_nacenki->Show();//� ������� �

		total = totalWithoutNacenka + nacenka;//����� ������������ �������� ���������
		txtBox_total_Cost->Text = total.ToString();//����������� � � ������
		txtBox_total_Cost->Show();//� �������

		lastCost = mem::get_Quantity_of_DDR() * vMem[i].getCost();//���������� ��������� ��������� ������		
	}

	private: System::Void ComboBoxGPU_SelectedIndexChanged(System::Object ^ sender, System::EventArgs ^ e) {
		int i = comboBoxGPU->SelectedIndex;

		static GPU last;
		if (last.getCost() == 0 && GPU1.getCost() != 0)//���� �� ������ ��� ������� ���������� last � � ��� ��� ���� ��������� ���������� �������������
			last = GPU1;
		else if (last.getCost() == 0 && CPU1.getCost() == 0)//���� �� ������ ��� ������� ���������� last, �� 
					//������ ������� ��� �� �������������� ���������� ��� ����������� ������������
			last = vGPU[0];//�� ����������� ���� ���������� ������� "None"

		OutputCostByDetailByChoosenDetail(vGPU, comboBoxGPU, textBoxGPUcost);

		totalWithoutNacenka = round(totalWithoutNacenka * 100) / 100;//������� ����������� ������� �����	
		totalWithoutNacenka = totalWithoutNacenka - last.getCost() + vGPU[i].getCost(); //����� �������� ���� �������������, �� ������ �������� � ���� ��������

		txtBox_cost_bez_nacenki->Text = totalWithoutNacenka.ToString();//���� ��� ������� � ������
		txtBox_cost_bez_nacenki->Show();//� ������� �

		total = totalWithoutNacenka + nacenka;//����� ������������ �������� ���������
		txtBox_total_Cost->Text = total.ToString();//����������� � � ������
		txtBox_total_Cost->Show();//� �������

		last = vGPU[i];//���������� ��������� ��������� ������		
	}

	private: System::Void ComboBoxSSD_SelectedIndexChanged(System::Object ^ sender, System::EventArgs ^ e) {
		int i = comboBoxSSD->SelectedIndex;

		static ssd last;
		if (last.getCost() == 0 && SSD1.getCost() != 0)//���� �� ������ ��� ������� ���������� last � � ��� ��� ���� ��������� ���������� �������������
			last = SSD1;
		else if (last.getCost() == 0 && SSD1.getCost() == 0)//���� �� ������ ��� ������� ���������� last, �� 
					//������ ������� ��� �� �������������� ���������� ��� ����������� ������������
			last = vSSD[0];//�� ����������� ���� ���������� ������� "None"

		OutputCostByDetailByChoosenDetail(vSSD, comboBoxSSD, textBoxSSDcost);

		totalWithoutNacenka = round(totalWithoutNacenka * 100) / 100;//������� ����������� ������� �����	
		totalWithoutNacenka = totalWithoutNacenka - last.getCost() + vSSD[i].getCost(); //����� �������� ���� �������������, �� ������ �������� � ���� ��������

		txtBox_cost_bez_nacenki->Text = totalWithoutNacenka.ToString();//���� ��� ������� � ������
		txtBox_cost_bez_nacenki->Show();//� ������� �

		total = totalWithoutNacenka + nacenka;//����� ������������ �������� ���������
		txtBox_total_Cost->Text = total.ToString();//����������� � � ������
		txtBox_total_Cost->Show();//� �������

		last = vSSD[i];//���������� ��������� ��������� ������		
	}

	private: System::Void ComboBoxHDD_SelectedIndexChanged(System::Object ^ sender, System::EventArgs ^ e) {
		int i = comboBoxHDD->SelectedIndex;

		static hdd last;
		if (last.getCost() == 0 && HDD1.getCost() != 0)//���� �� ������ ��� ������� ���������� last � � ��� ��� ���� ��������� ���������� �������������
			last = HDD1;
		else if (last.getCost() == 0 && HDD1.getCost() == 0)//���� �� ������ ��� ������� ���������� last, �� 
			//������ ������� ��� �� �������������� ���������� ��� ����������� ������������
			last = vHDD[0];//�� ����������� ���� ���������� ������� "None"

		OutputCostByDetailByChoosenDetail(vHDD, comboBoxHDD, textBoxHDDcost);

		totalWithoutNacenka = round(totalWithoutNacenka * 100) / 100;//������� ����������� ������� �����	
		totalWithoutNacenka = totalWithoutNacenka - last.getCost() + vHDD[i].getCost(); //����� �������� ���� �������������, �� ������ �������� � ���� ��������
		
		txtBox_cost_bez_nacenki->Text = totalWithoutNacenka.ToString();//���� ��� ������� � ������
		txtBox_cost_bez_nacenki->Show();//� ������� �

		total = totalWithoutNacenka + nacenka;//����� ������������ �������� ���������
		txtBox_total_Cost->Text = total.ToString();//����������� � � ������
		txtBox_total_Cost->Show();//� �������

		last = vHDD[i];//���������� ��������� ��������� ������		
	}

			 static double gg;
			 static double ez;
			 static double noobs;

	private: System::Void ComboBoxCase_SelectedIndexChanged(System::Object ^ sender, System::EventArgs ^ e) {
		int i = comboBoxCase->SelectedIndex;

		static Case last;
		if (last.getCost() == 0 && Case1.getCost() != 0) //���� �� ������ ��� ������� ���������� last � � ��� ��� ���� ��������� ���������� �������������
			last = Case1;
		else if (last.getCost() == 0 && Case1.getCost() == 0)//���� �� ������ ��� ������� ���������� last, �� 
			//������ ������� ��� �� �������������� ���������� ��� ����������� ������������
			last = vCase[0];//�� ����������� ���� ���������� ������� "None"

		OutputCostByDetailByChoosenDetail(vCase, comboBoxCase, textBoxCaseCost);

		totalWithoutNacenka = round(totalWithoutNacenka * 100) / 100;//������� ����������� ������� �����		
		totalWithoutNacenka = totalWithoutNacenka - last.getCost() + vCase[i].getCost(); //����� �������� ���� �������������, �� ������ �������� � ���� ��������
		txtBox_cost_bez_nacenki->Text = totalWithoutNacenka.ToString();//���� ��� ������� � ������
		txtBox_cost_bez_nacenki->Show();//� ������� �

		total = totalWithoutNacenka + nacenka;//����� ������������ �������� ���������
		txtBox_total_Cost->Text = total.ToString();//����������� � � ������
		txtBox_total_Cost->Show();//� �������

		last = vCase[i];//���������� ��������� ��������� ������		
	}

	private: System::Void ComboBoxPSU_SelectedIndexChanged(System::Object ^ sender, System::EventArgs ^ e) {
		int i = comboBoxPSU->SelectedIndex;

		static PSU last;
		if (last.getCost() == 0 && PSU1.getCost() != 0) //���� �� ������ ��� ������� ���������� last � � ��� ��� ���� ��������� ���������� �������������
			last = PSU1;
		else if (last.getCost() == 0 && PSU1.getCost() == 0)//���� �� ������ ��� ������� ���������� last, �� 
			//������ ������� ��� �� �������������� ���������� ��� ����������� ������������
			last = vPSU[0];//�� ����������� ���� ���������� ������� "None"

		OutputCostByDetailByChoosenDetail(vPSU, comboBoxPSU, textBoxPSUcost);

		totalWithoutNacenka = round(totalWithoutNacenka * 100) / 100;//������� ����������� ������� �����	
		totalWithoutNacenka = totalWithoutNacenka - last.getCost() + vPSU[i].getCost(); //����� �������� ���� �������������, �� ������ �������� � ���� ��������
		
		txtBox_cost_bez_nacenki->Text = totalWithoutNacenka.ToString();//���� ��� ������� � ������
		txtBox_cost_bez_nacenki->Show();//� ������� �

		total = totalWithoutNacenka + nacenka;//����� ������������ �������� ���������
		txtBox_total_Cost->Text = total.ToString();//����������� � � ������
		txtBox_total_Cost->Show();//� �������

		last = vPSU[i];//���������� ��������� ��������� ������			
	}
	

	private: System::Void Button_Sort_by_name_Click(System::Object^ sender, System::EventArgs^ e) {//���������� �� �����
		sortByName(vCPU, comboBoxCPU);
		sortByName(vMB, comboBoxMB);
		sortByName(vCooler, comboBoxCooler);
		sortByName(vMem, comboBoxMem);
		sortByName(vGPU, comboBoxGPU);
		sortByName(vSSD, comboBoxSSD);
		sortByName(vHDD, comboBoxHDD);
		sortByName(vCase, comboBoxCase);
		sortByName(vPSU, comboBoxPSU);	

		sortByIndex(vCPU, comboBoxCPU);//����� ���������� ������� ������������, � ��� ���������� ������ ��������� �����, ����� ��� ��� �� �������
		sortByIndex(vMB, comboBoxMB);
		sortByIndex(vCooler, comboBoxCooler);
		sortByIndex(vMem, comboBoxMem);
		sortByIndex(vGPU, comboBoxGPU);
		sortByIndex(vSSD, comboBoxSSD);
		sortByIndex(vHDD, comboBoxHDD);
		sortByIndex(vCase, comboBoxCase);
		sortByIndex(vPSU, comboBoxPSU);

		comboBoxCPU->Items->Clear();
		comboBoxMB->Items->Clear();
		comboBoxCooler->Items->Clear();
		comboBoxMem->Items->Clear();
		comboBoxGPU->Items->Clear();
		comboBoxSSD->Items->Clear();
		comboBoxHDD->Items->Clear();
		comboBoxCase->Items->Clear();
		comboBoxPSU->Items->Clear();

		vectorToCmbBx(vCPU, comboBoxCPU);
		vectorToCmbBx(vMB, comboBoxMB);
		vectorToCmbBx(vCooler, comboBoxCooler);
		vectorToCmbBx(vMem, comboBoxMem);
		vectorToCmbBx(vGPU, comboBoxGPU);
		vectorToCmbBx(vSSD, comboBoxSSD);
		vectorToCmbBx(vHDD, comboBoxHDD);
		vectorToCmbBx(vCase, comboBoxCase);
		vectorToCmbBx(vPSU, comboBoxPSU);
	}
	
	private: System::Void Button_Sort_by_cost_Click(System::Object^ sender, System::EventArgs^ e) {//���������� �� ����
		sortByCost(vCPU);
		sortByCost(vMB);
		sortByCost(vCooler);
		sortByCost(vMem);
		sortByCost(vGPU);
		sortByCost(vSSD);
		sortByCost(vHDD);
		sortByCost(vCase);
		sortByCost(vPSU);

		sortByIndex(vCPU, comboBoxCPU);//����� ���������� ������� ������������, � ��� ���������� ������ ��������� �����, ����� ��� ��� �� �������
		sortByIndex(vMB, comboBoxMB);
		sortByIndex(vCooler, comboBoxCooler);
		sortByIndex(vMem, comboBoxMem);
		sortByIndex(vGPU, comboBoxGPU);
		sortByIndex(vSSD, comboBoxSSD);
		sortByIndex(vHDD, comboBoxHDD);
		sortByIndex(vCase, comboBoxCase);
		sortByIndex(vPSU, comboBoxPSU);

		comboBoxCPU->Items->Clear();
		comboBoxMB->Items->Clear();
		comboBoxCooler->Items->Clear();
		comboBoxMem->Items->Clear();
		comboBoxGPU->Items->Clear();
		comboBoxSSD->Items->Clear();
		comboBoxHDD->Items->Clear();
		comboBoxCase->Items->Clear();
		comboBoxPSU->Items->Clear();

		vectorToCmbBx(vCPU, comboBoxCPU);
		vectorToCmbBx(vMB, comboBoxMB);
		vectorToCmbBx(vCooler, comboBoxCooler);
		vectorToCmbBx(vMem, comboBoxMem);
		vectorToCmbBx(vGPU, comboBoxGPU);
		vectorToCmbBx(vSSD, comboBoxSSD);
		vectorToCmbBx(vHDD, comboBoxHDD);
		vectorToCmbBx(vCase, comboBoxCase);
		vectorToCmbBx(vPSU, comboBoxPSU);
	}
	private: System::Void Button_Sort_by_id_Click(System::Object^ sender, System::EventArgs^ e) {//���������� �� Id (������� ���������� � �����-�����)
		sortById(vCPU, comboBoxCPU);
		sortById(vMB, comboBoxMB);
		sortById(vCooler, comboBoxCooler);
		sortById(vMem, comboBoxMem);
		sortById(vGPU, comboBoxGPU);
		sortById(vSSD, comboBoxSSD);
		sortById(vHDD, comboBoxHDD);
		sortById(vCase, comboBoxCase);
		sortById(vPSU, comboBoxPSU);

		sortByIndex(vCPU, comboBoxCPU);//����� ���������� ������� ������������, � ��� ���������� ������ ��������� �����, ����� ��� ��� �� �������
		sortByIndex(vMB, comboBoxMB);
		sortByIndex(vCooler, comboBoxCooler);
		sortByIndex(vMem, comboBoxMem);
		sortByIndex(vGPU, comboBoxGPU);
		sortByIndex(vSSD, comboBoxSSD);
		sortByIndex(vHDD, comboBoxHDD);
		sortByIndex(vCase, comboBoxCase);
		sortByIndex(vPSU, comboBoxPSU);

		comboBoxCPU->Items->Clear();
		comboBoxMB->Items->Clear();
		comboBoxCooler->Items->Clear();
		comboBoxMem->Items->Clear();
		comboBoxGPU->Items->Clear();
		comboBoxSSD->Items->Clear();
		comboBoxHDD->Items->Clear();
		comboBoxCase->Items->Clear();
		comboBoxPSU->Items->Clear();

		vectorToCmbBx(vCPU, comboBoxCPU);
		vectorToCmbBx(vMB, comboBoxMB);
		vectorToCmbBx(vCooler, comboBoxCooler);
		vectorToCmbBx(vMem, comboBoxMem);
		vectorToCmbBx(vGPU, comboBoxGPU);
		vectorToCmbBx(vSSD, comboBoxSSD);
		vectorToCmbBx(vHDD, comboBoxHDD);
		vectorToCmbBx(vCase, comboBoxCase);
		vectorToCmbBx(vPSU, comboBoxPSU);
	}

private: System::Void ButtonConfigInExcel_Click(System::Object^ sender, System::EventArgs^ e) {//����� �� �������� ������ "��������� ������������ � Excel"
	try {
		outFile.open("������� ������������.xls", ofstream::app);
		if (!outFile.is_open())
			throw exception();
	}
	catch (Exception^ ex) {
		MessageBox::Show(this, "���� ��� ������ �� ��� ������" + ex, "���������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		exit(-1);
	}

	//������� �������� ��������� �������������
	outFile  << msclr::interop::marshal_as< std::string >(comboBoxCPU->Text) 
	 << "\t" << msclr::interop::marshal_as< std::string >(textBoxCPUcost->Text) << endl;//������� ���� ��������� �������������

	outFile << msclr::interop::marshal_as< std::string >(comboBoxMB->Text)
	 << "\t" << msclr::interop::marshal_as< std::string >(textBoxMBcost->Text)  << endl;

	outFile << msclr::interop::marshal_as< std::string >(comboBoxCooler->Text) 
	 << "\t" << msclr::interop::marshal_as< std::string >(textBoxCoolerCost->Text)  << endl;

	outFile << msclr::interop::marshal_as< std::string >(comboBoxMem->Text) 
	 << "\t" << msclr::interop::marshal_as< std::string >(textBoxMemCost->Text) << endl;

	outFile  << msclr::interop::marshal_as< std::string >(comboBoxGPU->Text)
	 << "\t" << msclr::interop::marshal_as< std::string >(textBoxGPUcost->Text)  << endl;

	outFile  << msclr::interop::marshal_as< std::string >(comboBoxSSD->Text)
	 << "\t" << msclr::interop::marshal_as< std::string >(textBoxSSDcost->Text)  << endl;

	outFile << msclr::interop::marshal_as< std::string >(comboBoxHDD->Text) 
	 << "\t" << msclr::interop::marshal_as< std::string >(textBoxHDDcost->Text)  << endl;

	outFile  << msclr::interop::marshal_as< std::string >(comboBoxCase->Text)
	 << "\t" << msclr::interop::marshal_as< std::string >(textBoxCaseCost->Text)  << endl;

	outFile  << msclr::interop::marshal_as< std::string >(comboBoxPSU->Text) 
	 << "\t" << msclr::interop::marshal_as< std::string >(textBoxPSUcost->Text)  << endl;

	outFile << endl;
	outFile << "���� ��� �������" << "\t";
	outFile << msclr::interop::marshal_as< std::string >(txtBox_cost_bez_nacenki->Text)  << endl; //������� �������� ��������� ��� ����� �������
	outFile << "���� � ��������" << "\t";
	outFile << msclr::interop::marshal_as< std::string >(txtBox_total_Cost->Text)  << endl;//������� �������� ��������� � ��������
	outFile << endl;//����� ��������� ������������ ��� �� ��������� �������� �� ����������
	outFile << endl;
	outFile << endl;
}

	private: System::Void TxtBoxQuantityOfDDR_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		static string lastString;
		lastString = msclr::interop::marshal_as< std::string >(txtBoxQuantityOfDDR->Text);//���������� ��������� ��������� ������

		if (txtBoxQuantityOfDDR->Text == "")
			return;//���� ��� ������ �������� ����� Backspace, �� ������ �� ������

		if (txtBoxQuantityOfDDR->Text == "1")//��������� ��������� ���-�� ������ 
			mem::set_Quantity_of_DDR(1);//� ����������� ��������������� �������� ���������� ���������� ���-�� ������
		else	if (txtBoxQuantityOfDDR->Text == "2")
			mem::set_Quantity_of_DDR(2);
		else if (txtBoxQuantityOfDDR->Text == "4")
			mem::set_Quantity_of_DDR(4);
		else MessageBox::Show(this, "������� 1, 2 ��� 4", "���������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		if (DDR1.getName() != "empty name") {
			ComboBoxMem_SelectedIndexChanged(sender, e);//�������� ��� �������, ����� ���������� ���� (���������� ���� �� ���������)			
			textBoxMemCost->Text = (mem::get_Quantity_of_DDR() *	atof(msclr::interop::marshal_as< string >(textBoxMemCost->Text).c_str())).ToString();
			//���� �� ���������� ��� ��������� ������ ������ �������� �� ���-�� ������ ������
		}
	}
};
}




		 
