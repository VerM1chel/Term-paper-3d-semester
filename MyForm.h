#pragma once
#include "Detail.h"
#include <algorithm>
#include <math.h>
#include <msclr/marshal_cppstd.h>  //для конвертации System::String^ в std::string
#include <iomanip>//нужно для setw, который задает ширину отображения поля для следующего элемента в потоке
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
	string comment4 = " ", string comment5 = " ") {//передаем бюджет и вектор	
	//comment2: в случае процессора "AMD" может встречаться и в других комплектующих
	//если у нас не процессор, то приравнивание к " " не сломает логику
	double raznica, predRaznica = 999999999999;
	int frequency_power, j, k, c = 0, quantity_of_proper_details = 0, quantity_of_DDR = mem::get_Quantity_of_DDR();
	string name, type;
	if (quantity_of_DDR == 1 && comment == "Soc-AM4" || comment == "Soc-1200")//минимальное кол-во планок в MB равно 2, 
		quantity_of_DDR = 2; //поэтому на время поиска MB мы минимальное кол-во требуемых планок ставим на 2

	for (int i = 0; i < v.size(); ++i) {
		j = k = 0;

		double cost = v[i].getCost();
		name = v[i].getName(); //запоминаем название, чтобы не вызывать постоянно метод getName	

		if (comment == "DDR IV") {//если сейчас рассматриваются планки памяти
			if (comment3 == "KiTof2" && name.find("KiTof2") == -1)//если был выбран тип "KiTof2", 
				continue;//то пропускаем все планки, в чьем имени нет "KiTof2"
			else if (comment3 == "Одиночные" && name.find("KiTof2") != -1)
				continue;//если тип планок "Одиночные", то пропускаем все планки "KiTof2"
			if (comment4 != " " && name.find(comment4+"Gb") == -1)//если было выбрано кол-во Гб в планке  
				continue;//то если в рассматриваемой планке нет именно такого кол-ва Гб, то мы её пропускаем
		}
		if (comment2 == "SSD") {//если пользователь ввел нужное ему кол-во Гб в SSD, то пропускаем все, что не подходит
			if(comment3 == "120Gb" && name.find("120Gb") == -1)
				continue;
			if (comment3 == "240Gb" && name.find("240Gb") == -1)
				continue;
			if (comment3 == "500Gb" && name.find("500Gb") == -1)
				continue;
		}
		if (comment2 == "HDD") {//если пользователь ввел нужное ему кол-во Гб в SSD, то пропускаем все, что не подходит
			if (comment3 == "1Tb" && name.find("1Tb") == -1)
				continue;
			if (comment3 == "2Tb" && name.find("2Tb") == -1)
				continue;
			if (comment3 == "4Tb" && name.find("4Tb") == -1)
				continue;
		}
		if (budget <= 400 && comment == "ATX" || comment == "MicroATX")//если комп дешевый и мы выбираем корпус
			if (name.find("W ") == -1)//выбираем только корпуса со встроенным блоком питания (если в названии есть W (ватты)) (существенная экономия)
				continue;//все остальные корпуса пропускаем
		if (comment == "Процессор" && comment2 == "Socket-1200" && comment3 == "Internal")//если видеокарта встроенная
			if (name.find("F ") != -1)//то пропускаем все процы, которые БЕЗ встроенной видеокарты (для интелов это так)
				continue;
		if (comment == "Процессор" && comment2 == "Socket-AM4" && comment3 == "Internal")//если видеокарта встроенная
			if (name.find("G ") == -1)//ищем, (...G), т.е. со АМДшный проц со встроенной видеокартой
				continue;//если ...G нет, то этот проц нам не подходит, ищем следующий
		if (comment == "Процессор" && orientZena < 60 && name.find("Процессор BOX") == -1)
			continue;//для дешевых конфигураций, чтобы сэкономить, можно купить процессор со встроенным кулером
		if (comment == "Socket-AM4" || comment == "DDR IV" || comment == "БП " || comment == "HDD " || (comment == "Universal" && comment2 != " ")) {
			if (comment2.find("X ") != -1 && comment == "Universal" && v[i].getCost() < 18)//Если в названии процессора AMD есть X, то кулер нужен помощнее
				continue;
			if (comment == "DDR IV")//ищем соответствующие параметры для текущей детали
				j = name.find("MHz ");
			if (comment == "БП ")
				j = name.find("W ");
			if (comment == "HDD ") {
				j = name.find("RPM ");
				if (j == -1)//если у нас кол-во оборотов написано маленькими буквами
					j = name.find("rpm ");//то ищем маленькие
			}
			k = j;
			if (j == -1)//если у нас в прайс-листе есть какая-то деталь у которой не указаны все необходимые нам параметры
				continue;//то мы её просто пропускаем
			if (j != 0)
				while (isdigit(name[j - 1]))//считываем все число, стоящее перед MHz или W
					--j;
			frequency_power = atoi(name.substr(j, k - j + 1).c_str()); //преобразуем строку в число (находим частоту DDR)
			if (budget > 600) {
				if ((comment == "DDR IV" && frequency_power < 3000) //если комп от 600 баксов, то частота DDR минимум 3000
					|| (comment == "БП " && (frequency_power < 600 && name.find("Bronze") == -1))
					|| comment == "HDD" && frequency_power < 7200 && comment != "Universal") //если комп от 600 баксов, то частота DDR минимум 3000 и мощность БП не меньше 600 Вт (и БП должне быть с бронзовым сертификатом), для кулера мы делаем проверку выше
					continue;//(просто "дешёвая" конфигурация будет всегда дешевле, чем бюджет, поэтому и 800, а не 600)
			}	//если не выполняется условие, то переходим к следующей DDR
		}

		if (comment2 == "Socket-AM4" && comment == "Процессор" && name.find("AMD Ryzen") != -1) {
			int	j1 = name.find("Socket-");
			int k1 = j1;
			while (name[k1] != ' ')
				++k1;
			string forComment2 = name.substr(j1, k1 - j1 + 1).c_str();
			comment5 = forComment2;//для процессора 5-й коммент не используется, так что можно использовать его в исключиетльных ситуациях
		}

		if (comment == "Universal" || (name.find(comment) != -1 && name.find(comment2) != -1) || (comment=="Процессор" && comment5 != " ")) {//если у нас полное совпадение по желаемым параметрам или кулер типа "Universal" (которому мы всегда рады), то идём дальше
			if (comment == "Soc-AM4" || comment == "Soc-1200") { //если мы ищем MB
				k = name.find("DDR4");
				if (k == -1)
					continue;
				while (!isdigit(name[k])) //пока не дойдем до кол-ва планок памяти в MB идем к началу строки (там может быть 4хDDR или 4DDR)
					--k;
				j = k;
				while (isdigit(name[j])) //проходим к началу числа, равного максимальному кол-ву DDR в MB (вдруг в прайс-листе будет 16xDDR)
					--j;
				//++j; //увеличиваем на единицу, так как цикл while дополнительно отнял 1
				if (atoi(name.substr(j + 1, k - j).c_str()) < quantity_of_DDR) //если в названии MB максимальное кол-во планок меньше желаемого
					continue; //то такие MB мы пропускаем			
			}

			if (comment2 == "1" && (name.find("DDR IV 4Gb PC") != -1 || name.find("KiTof2") != -1)) //одна планка памяти на 4 Гб 
				continue; //это слишком мало, т.е. нужно чтобы DDR была на 8+ Гб. Также если мы собираем только с 1 планкой памяти, то нельзя KiTof2, т.к. это набор из 2-х планок		
		
			if (comment2 == "2" && c == 0 && name.find("KiTof2") != -1) {//если у нас KiTof2, т.е. набор из 2-х планок, то мы увеличиваем цену (которая была для ОДНОЙ планки) в 2 раза
				orientZena *= 2;
				c = 1;
			}
			else if (c == 1) {
				orientZena /= 2; //если у нас не набор, то мы возвращаем цену в изначальное состояние
				c = 0;
			}

			if (comment2 == "4" && c == 0 && name.find("KiTof2") != -1) {//если у нас KiTof2, т.е. набор из 2-х планок, то мы увеличиваем цену (которая была для ОДНОЙ планки) в 2 раза
				orientZena *= 2;
				c = 1;
			}
			else if (c == 1) {
				orientZena /= 2;
				c = 0;
			}
			++quantity_of_proper_details; //переменная нужна для того, чтобы узнать, у нас нет заданных деталей или бюджета не хватает
			raznica = orientZena - v[i].getCost();
			if (cost <= orientZena && raznica < predRaznica) {
				//для кулера типа "Universal" данный тип может и не быть указанным в названии, но когда мы считывали кулеры, 
				//то мы считывали только кулеры этого типа, 
				//т.е. если кулер прошел все проверки, то мы его спокойно пропускаем
				//если тип детали совпадает с желаемым и цена текущего экземпляра меньше и максимально близко к ориентеровочной, то запоминаем его

				Det = v[i];//запоминаем этот экземпляр в отдельную переменную
				if (raznica == 0) //если мы нашли полное совпадение по желаемой цене				
					return comment;//то дальше нет смысла искать			
				predRaznica = raznica; //запоминаем прошлую разницу, чтобы допускать только те детали, цена которых более близкая к требуемой
			}
		}
	}	
	if (predRaznica == 999999999999 && quantity_of_proper_details == 0) {
		if (comment == "Процессор")
			type = "\"Процессор\"";
		if (comment == "Universal")
			type = "\"Кулер\"";
		if (comment == "Soc-1200" || comment == "Soc-AM4")
			type = "\"Материнская плата\"";
		if (comment == "DDR IV")
			type = "\"Оперативная память\"";
		if (comment == "AMD Radeon" || comment == "GeForce" || comment == "Internal")
			type = "\"Видеокарта\"";
		if (comment2 == "SSD" && (comment == "SATA" || comment == "M.2"))
			type = "\"SSD\"";
		if (comment2 == "HDD" && (comment == "SATA" || comment == "M.2"))
			type = "\"HDD\"";
		if (comment == "ATX" || comment == "MicroATX")
			type = "\"Корпус\"";
		if (comment == "БП ")
			type = "\"Блок питания\"";
		type = "Комплектующих типа " + type + " с указанными параметрами нет в прайс-листе";
		MessageBox::Show(gcnew String(type.c_str()), "Сообщение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		string ret = "return";
		return ret;
	}
	else if (predRaznica == 999999999999 && quantity_of_proper_details > 0) {
		if (comment == "Процессор")
			type = "\"Процессор\"";
		if (comment == "Universal")
			type = "\"Кулер\"";
		if (comment == "Soc-1200" || comment == "Soc-AM4")
			type = "\"Материнская плата\"";
		if (comment == "DDR IV")
			type = "\"Оперативная память\"";
		if (comment == "AMD Radeon" || comment == "GeForce" || comment == "Internal")
			type = "\"Видеокарта\"";
		if (comment2 == "SSD" && (comment == "SATA" || comment == "M.2"))
			type = "\"SSD\"";
		if (comment2 == "HDD" && (comment == "SATA" || comment == "M.2"))
			type = "\"HDD\"";
		if (comment == "ATX" || comment == "MicroATX")
			type = "\"Корпус\"";
		if (comment == "БП ")
			type = "\"Блок питания\"";
		type = "На комплектующие типа " + type + " с выбранными параметрами не хватает бюджета";
		MessageBox::Show(gcnew String(type.c_str()), "Сообщение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		string ret = "return";
		return ret;
	}

	return comment;
}


template <class T>
void create(vector<T>& myVector, ifstream& file, string type, int nr) {//указываем параметры и, отправляя их в конструктор с параметрами создаём новый экземпляр класса	

	file.clear();
	file.seekg(0);
	int begin, end;
	string line, NameForVector, CostForVector, ifItsServer;
	int i, j, number, frequency, z, y, Y=0, firstRaz = 1, firstRaz2 = 1;
	while (getline(file, line)) {
		if (line.find("Socket-1151") != -1 || line.find("Soc-1151") != -1)//устаревшие сокеты процессора не считываем
			continue;
		if (line.find("HDD") != -1  && (line.find("500Gb") != -1 || line.find("500 Gb") != -1))//HDD на 500Гб нам не нужно, 
			continue;//т.к. лучше доплатить 3$ и купить HDD на 1Тб (почему такая маленькая разница в цене и такая большая разница в )		

		for (i = 0; i < line.length(); ++i) {//проходим считанную строку
			i = line.find("\"", i + 1);//если в строке были дюймы, то visual studio считывал не так как нужно, добавляя доп. кавычки
			if (i == -1)//если в строке не осталось кавычек(или их не было)
				break;//то выходим из цикла
			if (!isdigit(line[i - 1]) || line[i - 1] != ';') //найдя кавычку в строке, мы смотрим, есть ли спереди число (тогда это дюйм)
				line.erase(i, 1);//если спереди числа нет или ; (начала ячейки), то это "ошибочные" кавычки, и мы их удаляем								
				/*line.erase(line.find(";\"", 1));
				line.erase(line.find("\";", 1));*/
		}


		if (line.find("Сервер") != -1 || line.find("сервер") != -1) {//если мы дошли до серверных компелктующих				
			z = y = 0;
			while (line[z]) {//пока не пройдем всю строку
				if (line[z] == ';' && line[z + 1] == ';' && firstRaz) {//если у нас текст только в одном столбце, то текущая строка это тип комплектующих
					firstRaz = 0;
					++y;//запоминаем кол-во ';'
				}
				++z;
			}
			Y = y;//по количеству пустых столбцов (; идущих друг за другом) поймём, когда мы дошли до следующей ключевой строки					
			getline(file, line);//считываем следующую строку
			do {
				y = 0;
				z = 1;
				while (line[z]) {//пока не пройдем всю строку
					if (line[z - 1] == ';' && line[z] != ';' && z < line.size() && firstRaz2) {
						begin = z;
						firstRaz2 = 0;
					}
					if (line[z] == ';' && line[z + 1] == ';')//если у нас текст только в одном столбце, то текущая строка это тип комплектующих
						++y;
					++z;
				}
				if (y < Y && line[begin] < 'A' || line[begin] > 'я')//если это тип комплектующей
					//и не подпункт НЕ такое как это "03 - Корзины для серверных корпусов", и не является буквой русского алфавита как, например "LGA 3647"
					getline(file, line);//то пока не дойдем до следующей такой строки мы считываем (пропускаем) все строки
				else break;
			} while (1);
		}	

		i = 0;
		j = 0;
		number = 0;
		if (type == ";Кулер " && line.find("Universal") != -1)
			firstRaz2 = 1;
		if (type != ";Кулер " || (type == ";Кулер " && firstRaz2 == 1))//из кулеров считываем только типа "Universal"(в названии это слово может и не присутствовать, поэтому считываем весь блок стоящий после НАЗВАНИЯ БЛОКА)
			if (line.find(type) != -1) { //если мы нашли введённую подстроку, указывающую на тип детали
				i = j = 0;
				begin = line.find(';');	//2-й столбец в строке
				while (line[begin + 1] == ';')//у нас могут быть пустые ячейки, 
					++begin;//поэтому мы пропускаем лишние ';'
				end = line.find(';', begin + 1);	//2-й НЕПУСТОЙ столбец в строке
				NameForVector = line.substr(begin + 1, end - 1 - begin);//substr считывает от begin (end-1-begin) символов
				//Символы, начиная с начала 2-го столбца и до его окончания  -- это название комплектующей
			
			}
			else continue;
		else continue;

		for (i = line.length(); i >= 0; --i) { //идём к ячейке с ценой ( ; обозначает конец ячейки)
			if (line[i] == ';')
				++number;
			if (number >= nr)
				break;
			if (line[i] == ';' && i != j) //если текущий символ это новая встреча ';' то запоминаем его позицию
				j = i;
		}
		while (line[i] == ';')
			++i; //откат к в сторону конца списка (из-за пустых ячеек i мог зайти дальше чем было нужно)
		if (isdigit(line[i])) //если мы прошли всю ячейку после названия и нашли там число
			CostForVector = line.substr(i, i - j + 1);
		else continue;
		//то это цена (c_str преобразует string в char*)		
		myVector.push_back(T(NameForVector, atof(CostForVector.c_str()))); //после чего помещаем новый экземпляр в конец вектора
	}
}




int processor(CPU& proc, double budget, string comment) {	
	if ((comment == "Internal" || budget < 400) && ((proc.getName().find("Socket-1200") != -1 && proc.getName().find("F ") == -1)
		|| (proc.getName().find("Socket-AM4") != -1 && proc.getName().find("G ") != -1)) && proc.getName().find("Процессор BOX") != -1)
		return 2;//Для дешевых компов, если процессор и со встроенной графикой, и со встроенным кулером, то возвращаем 2
	else if ((comment == "Internal" || budget < 400) && ((proc.getName().find("Socket-1200") != -1 && proc.getName().find("F ") == -1)
		|| (proc.getName().find("Socket-AM4") != -1 && proc.getName().find("G ") != -1)))
		return 1;//Для дешевых компов, если процессор со встроенной графикой, то возвращаем 1
	//встроенные куллер или видеокарта только если комп дешевый или пользователь намеренно выбрал "Internal"
	else return 0;
}

template <class T>
void vectorToCmbBx(vector<T>& v, System::Windows::Forms::ComboBox^ comboBox) {
	if (v.empty())//если в из прайс-листа не удалось считать комплектующую данного типа, то в comboBox нечего заносить
		return;
	else 
		for (int i = 0; i < v.size(); ++i) //если же считали вектор, то заносим его в соответствующий comboBox		
			comboBox->Items->Add(gcnew String(v[i].getName().c_str()));	//преобразуем имя в winforms-строку и добавляем эту строку в соответствующий comboBox
}

template <class T>
void printNameAndCost(System::Windows::Forms::ComboBox^ comboBox, System::Windows::Forms::TextBox^ txtBox, T& det, int i = -1) {//для детали, выбранной программой
	if (i == -1)//если программа ещё не составляла конфигурацию
		i = comboBox->SelectedIndex;

	if (comboBox->Items->Count == 0) {//если в из прайс-листа не удалось считать комплектующую данного типа, то comboBox пуст
		comboBox->Text = gcnew String("В прайс-листе не найдено компелктующих данного типа"); //выводим уведомление в соответствующем комбобоксе
		comboBox->Show();
	}
	else {//если данный comboBox не пуст
		if (det.getName() == "empty name") {
			comboBox->Text = comboBox->Items[0]->ToString();
			comboBox->Show();			
		}
		else {
			comboBox->Text = comboBox->Items[i]->ToString();//то выводим название выбранной комплектующей
			comboBox->Show();
			txtBox->Text = det.getCost().ToString();//и её цену
			txtBox->Show();		
		}
	}
}


template <class T>
void OutputCostByDetailByChoosenDetail(vector<T>& v, System::Windows::Forms::ComboBox^ cmbbx, System::Windows::Forms::TextBox^ txtbx){//когда деталь выбрал пользователь
	int i = cmbbx->SelectedIndex; //получаем индекс выбранного элемента
	if (i == -1) //если выбирать нечего
		return;//то выходим из функции
	else if (i >= 0) {
		txtbx->Text = v[i].getCost().ToString(); //и присваиваем текстбоксу, соответствующему цене данного комбобокса цену за данную деталь
		txtbx->Show();//и выводим присвоенное значение
	}
}


static vector<CPU> vCPU; //объявляем глобальные переменные, чтобы в любом методе нашей форму они были доступны
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
	/// Сводка для MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
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
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
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
			this->label1->Text = L"Введите бюджет клиента";
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
			this->label2->Text = L"Цена=";
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
			this->label3->Text = L"Наценка=";
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
			this->label4->Text = L"Введите наценку";
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
			this->label6->Text = L"Socket-1200 или Socket-AM4";
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
			this->label5->Text = L"Количество планок";
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
			this->label7->Text = L"AMD Radeon или GeForce";
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
			this->button1_Make_conf->Text = L"Составить конфигурацию";
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
			this->label8->Text = L"Процессор";
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
			this->label9->Text = L"Видеокарта";
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
			this->label10->Text = L"Память";
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
			this->label11->Text = L"Мат. плата";
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
			this->label12->Text = L"Куллер";
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
			this->label13->Text = L"Диск SSD";
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
			this->label14->Text = L"Диск HDD";
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
			this->label15->Text = L"Корпус";
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
			this->label16->Text = L"БП";
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
			this->label17->Text = L"Названия комплектующих";
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
			this->label18->Text = L"Цена";
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
			this->button_Sort_by_cost->Text = L"Отсортировать по цене";
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
			this->button_Sort_by_name->Text = L"Отсортировать по алфавиту";
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
			this->button_Sort_by_id->Text = L"Отсортировать по ID";
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
			this->label19->Text = L"Цена без наценки";
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
			this->label20->Text = L"Итоговая цена";
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
			this->buttonConfigInExcel->Text = L"Сохранить конфигурацию в Excel";
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
			this->radioButton7->Text = L"Неважно";
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
			this->label26->Text = L"Количество Гб в SSD";
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
			this->radioButton6->Text = L"Неважно";
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
			this->label25->Text = L"Количество Тб в HDD";
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
			this->label24->Text = L"Необязательные параметры";
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
			this->radioButton1->Text = L"Неважно";
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
			this->label23->Text = L"Тип планок";
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
			this->radioButtonOdinochnyePlanki->Text = L"Одиночные";
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
			this->radioButton2->Text = L"Неважно";
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
			this->label22->Text = L"Количество Гб в одной планке";
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
		String^ Commentary; //для вывода на экран
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
			if (radioButtonOdinochnyePlanki->Checked) { Commentary = "Одиночные"; return Commentary; }
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
		string line, type;//показывает, данные о какой детали мы сейчас будем считывать
		
		try {
			inFile.open("Price.csv");
			if (!inFile.is_open())
				throw exception();
		}
		catch (Exception^ ex) {
			MessageBox::Show(this, "Файл для чтения не был открыт\n"+ex, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			exit(-1);			
		}

		try {
			outFile.open("Готовые конфигурации.xls", ofstream::app);
			if (!outFile.is_open())
				throw exception();
		}
		catch (Exception^ ex) {
			MessageBox::Show(this, "Файл для записи не был открыт"+ex, "Сообщение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			exit(-1);
		}	
		
		outFile << "Название комплектующей" << "\t" << "Цена $" << endl;
		outFile << endl;
		
		outFile.setf(ios::right);
		outFile.setf(ios::fixed);
		outFile.precision(3);
		

		int number = 0;
		getline(inFile, line, '!');
		for (int i = line.size(); i >= 0; --i) {//ищем в какой ячейке по счету находится цена
			//(все ячейки в строке разделены ; )
			if (line[i] == ';')	//каждый раз, как мы находим ; означает, что сейчас мы перейдем в очередную(предыдущую) ячейку
				++number;
			if (i == line.find(";Цена")) //если мы дошли до ячейки с ценой
				break;		//то дело сделано		
		}

		type = "Процессор ";
		vCPU.push_back(CPU("None", 0));//в самом начале вектора будет элемент "None" с ценой равной 0. Это нужно на случай, если комплектующая не нужна
		create(vCPU, inFile, type, number);//то это цена (c_str преобразует string в char*)	
		inFile.clear();
		inFile.seekg(0);
		Detail::setStaticIndexToZero();//зануляем статическую переменную (indexStatic), чтобы для нового вектора начать счет с начала

		type = ";MB ";
		vMB.push_back(MB("None", 0));//в самом начале вектора будет элемент "None" с ценой равной 0. Это нужно на случай, если комплектующая не нужна
		create(vMB, inFile, type, number);
		inFile.clear();
		inFile.seekg(0);
		Detail::setStaticIndexToZero();//зануляем статическую переменную (indexStatic), чтобы для нового вектора начать счет с начала;

		type = ";Кулер ";
		vCooler.push_back(Cooler("None", 0));//в самом начале вектора будет элемент "None" с ценой равной 0. Это нужно на случай, если комплектующая не нужна
		create(vCooler, inFile, type, number);
		inFile.clear();
		inFile.seekg(0);
		Detail::setStaticIndexToZero();//зануляем статическую переменную (indexStatic), чтобы для нового вектора начать счет с начала

		type = "DDR IV";
		vMem.push_back(mem("None", 0));//в самом начале вектора будет элемент "None" с ценой равной 0. Это нужно на случай, если комплектующая не нужна
		create(vMem, inFile, type, number);
		inFile.clear();
		inFile.seekg(0);
		Detail::setStaticIndexToZero();//зануляем статическую переменную (indexStatic), чтобы для нового вектора начать счет с начала

		type = "Видеокарта ";
		vGPU.push_back(GPU("None", 0));//в самом начале вектора будет элемент "None" с ценой равной 0. Это нужно на случай, если комплектующая не нужна
		create(vGPU, inFile, type, number);
		inFile.clear();
		inFile.seekg(0);
		Detail::setStaticIndexToZero();//зануляем статическую переменную (indexStatic), чтобы для нового вектора начать счет с начала

		type = ";SSD ";
		vSSD.push_back(ssd("None", 0));//в самом начале вектора будет элемент "None" с ценой равной 0. Это нужно на случай, если комплектующая не нужна
		create(vSSD, inFile, type, number);
		inFile.clear();
		inFile.seekg(0);
		Detail::setStaticIndexToZero();//зануляем статическую переменную (indexStatic), чтобы для нового вектора начать счет с начала
		
		type = ";HDD 3.5";
		vHDD.push_back(hdd("None", 0));//в самом начале вектора будет элемент "None" с ценой равной 0. Это нужно на случай, если комплектующая не нужна
		create(vHDD, inFile, type, number);
		inFile.clear();
		inFile.seekg(0);
		Detail::setStaticIndexToZero();//зануляем статическую переменную (indexStatic), чтобы для нового вектора начать счет с начала
		
		type = "Корпус ";
		vCase.push_back(Case("None", 0));//в самом начале вектора будет элемент "None" с ценой равной 0. Это нужно на случай, если комплектующая не нужна
		create(vCase, inFile, type, number);
		inFile.clear();
		inFile.seekg(0);
		Detail::setStaticIndexToZero();//зануляем статическую переменную (indexStatic), чтобы для нового вектора начать счет с начала

		type = ";БП ";
		vPSU.push_back(PSU("None", 0));//в самом начале вектора будет элемент "None" с ценой равной 0. Это нужно на случай, если комплектующая не нужна
		create(vPSU, inFile, type, number);
		inFile.clear();
		inFile.seekg(0);
		Detail::setStaticIndexToZero();//зануляем статическую переменную (indexStatic), чтобы для нового вектора начать счет с начала

			inFile.close();//закрытие файла

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
		//кнопка "Составить конфигурацию"
		
		string zena = msclr::interop::marshal_as< std::string >(textBoxCost->Text);//проверка на то, является ли введенная цена или наценка положительным числом
		string nazenka = msclr::interop::marshal_as< std::string >(textBoxNacenka->Text);
		
		for (int m = 0; m < zena.length(); ++m) {
			if (m == 0 && zena[m] == '-')
				continue;
			if (zena[m] == '.')
				zena[m] = ',';
			if (!isdigit(zena[m]) && zena[m] != ',' || zena[0] == '-' && zena.find(1, '-')!=-1) {
				MessageBox::Show(this, "Цена должна быть числом\n", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}
		}
		for (int m = 0; m < nazenka.length(); ++m) {
			if (m == 0 && nazenka[m] == '-')
				continue;
			if (nazenka[m] == '.')
				nazenka[m] = ',';
			if (!isdigit(nazenka[m]) && nazenka[m] != ',' || nazenka[0] == '-' && nazenka.find(1,'-')!=-1) {
				MessageBox::Show(this, "Наценка должна быть числом\n", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}
		}

		if (zena[0] == '-') {
			MessageBox::Show(this, "Цена не должна быть отрицательной\n", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		if (nazenka[0] == '-') {
			MessageBox::Show(this, "Наценка не должна быть отрицательной\n", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}

		textBoxCost->Text = gcnew String(zena.c_str());
		textBoxNacenka->Text = gcnew String(nazenka.c_str());


		try {
			outFile.open("Готовые конфигурации.xls", ofstream::app);
			if (!outFile.is_open())
				throw exception();
		}
		catch (Exception^ ex) {
			MessageBox::Show(this, "Файл для записи не был открыт" + ex, "Сообщение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			exit(-1);
		}

		double Gr_Cost; //введенная цена
		double Gr_nacenka; //введенная наценка
		do {
			if (textBoxCost->Text != "" && textBoxNacenka->Text != "") {//если пользователь не забыл указать цену и наценку
				Gr_Cost = System::Convert::ToDouble(textBoxCost->Text);//то тогда присваиваем значения переменным "цена" и "наценка"
				Gr_nacenka = System::Convert::ToDouble(textBoxNacenka->Text);
			}
			if (textBoxCost->Text == "" || textBoxNacenka->Text == "" || !(radioButtonSocket1200->Checked || radioButtonSocketAM4->Checked)
				|| !(radioButton_1_plankaDDR->Checked || radioButton_2_plankiDDR->Checked || radioButton_4_plankiDDR->Checked) ||
				!(radioButtonAMDRadeon->Checked || radioButtonGeForce->Checked || radioButtonInternal->Checked)) {//если мы не ввели всё, что надо ввести,
				MessageBox::Show(this, "Пожалуйста, введите данные", "Сообщение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				//то тогда выводим уведомление и просим ввести данные			
				return; //и закрываем метод, вызываемый при нажатии кнопки "Составить конфигурацию"
			}		
			else break;	//если все введено, то программа выполняется дальше
		} while (1);

		if (CPU1.getName() != "empty name") {//если мы делаем конфигурацию НЕ в первый раз, 
			CPU1.~CPU();//то чистим уже существующие выбранные детали
			MB1.~MB();
			Cooler1.~Cooler();
			DDR1.~mem();
			GPU1.~GPU();
			SSD1.~ssd();
			HDD1.~hdd();
			Case1.~Case();
			PSU1.~PSU();

			CPU1.setName("empty name");//после вызова деструкторов у нас уничтожается и стринговый Name, поэтому всесто пустой строки
			MB1.setName("empty name");//нужно присвоить "empty name", т.к. на этом завязана часть логики программы
			Cooler1.setName("empty name");
			DDR1.setName("empty name");
			GPU1.setName("empty name");
			SSD1.setName("empty name");
			HDD1.setName("empty name");
			Case1.setName("empty name");
			PSU1.setName("empty name");
		}
		
		if (radioButton_1_plankaDDR->Checked) txtBoxQuantityOfDDR->Text = "1"; //в зависимости от выбранного кол-ва планок памяти вывод соответствующего числа
		if (radioButton_2_plankiDDR->Checked) txtBoxQuantityOfDDR->Text = "2";
		if (radioButton_4_plankiDDR->Checked) txtBoxQuantityOfDDR->Text = "4";

		cout << "Count: " << Detail::getCount() << endl;

		budget_bez_nacenki = Gr_Cost;
		nacenka = Gr_nacenka;

		budget = budget_bez_nacenki - nacenka;

		string comment, comment2;

		comment = "Процессор";
		comment2 = msclr::interop::marshal_as< std::string >(Sockets());//выбираем тип процессора (Socket-1200 or Socket-AM4)
		//для этого мы преобразуем значение типа String^, возвращаемое функцией Sockets(), 
		//(которое возвращает введенную в форме строку типа System::String), в тип string
		string typeOfVideocard = msclr::interop::marshal_as<std::string>(Kursach::MyForm::videocard());
		if ("return" == ForConf(budget, budget * 0.20, vCPU, CPU1, comment, comment2, typeOfVideocard))
			return;
		
		if (processor(CPU1, budget, typeOfVideocard) > 0 && budget > 400){//если проц со встроенное графикой или кулером (или, и то, и другое)
		//и если конфигурация не из самых дешевых, то часть бюджета, которую мы бы потратили на GPU или кулер мы тратим на CPU
			if (processor(CPU1, budget, typeOfVideocard) == 1)  //если процессор со встроенной графикой
				if ("return" == ForConf(budget, budget * (0.20 + 0.34), vCPU, CPU1, comment, comment2, "Internal"))
					return;
			else if (processor(CPU1, budget, typeOfVideocard) == 2) //если процессор и со встроенной графикой, и со встроенным кулером
				if ("return" == ForConf(budget, budget * (0.20 + 0.34 + 0.03), vCPU, CPU1, comment, comment2, "Internal"))
					return;				
		}
		else if (processor(CPU1, budget, typeOfVideocard) > 0 && budget < 400) {//если конфигурация слишком дешевая, 
			//то с GPU мы половину тратим на CPU, а другую половину потратим на матплату 
			if (processor(CPU1, budget, typeOfVideocard) == 1)  //если процессор со встроенной графикой
				if ("return" == ForConf(budget, budget * (0.20 + 0.34 * 0.60 - 0.013), vCPU, CPU1, comment, comment2, "Internal"))// -0.013 это пойдет на DDR
					return;
			if (processor(CPU1, budget, typeOfVideocard) == 2) //если процессор и со встроенной графикой, и со встроенным кулером
				if ("return" == ForConf(budget, budget * (0.20 + 0.34 * 0.6 + 0.03 - 0.013), vCPU, CPU1, comment, comment2, "Internal"))
					return;
		}

		if (CPU1.getName().find("Процессор BOX") == -1 || budget > 400)//если у нас процессор без встроенного кулера
			if ("return" == ForConf(budget, budget * 0.03, vCooler, Cooler1, "Universal", CPU1.getName()))
				return;

		if (CPU1.getName()== "empty name") {//если бюджета не хватает на основные детали, без которых комп не будет работать
			MessageBox::Show(this, "При заданном бюджете невозможно составить выбранную конфигурацию CPU", "Сообщение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			//то выводим сообщение
			return;//и пусть пользователь изменит что-либо и нажмет на кнопку ещё раз
		}

		string quantity = msclr::interop::marshal_as< std::string >(planki_quantity());	
		//выбираем максимальное кол-во планок памяти в MB			 
		if (quantity == "1") 
			mem::set_Quantity_of_DDR(1);
		else if (quantity == "2")
			mem::set_Quantity_of_DDR(2);
		else
			mem::set_Quantity_of_DDR(4);	

		txtBoxQuantityOfDDR->Text = gcnew String(quantity.c_str());//выводим кол-во планок в соответствующий текстбокс

		
		if (comment2 == "Socket-AM4") {//если процессор АМДшный, то выбираем плату под него		
			if (processor(CPU1, budget, typeOfVideocard) > 0 && budget < 400) {//если проц со встроенное графикой или кулером (или, и то, и другое)
				//и если конфигурация ДЕШЕВАЯ, то половину из части бюджета, которую мы бы потратили на GPU или кулер мы тратим на CPU
				//и другую половину тратим на MB
				if ("return" == ForConf(budget, budget * (0.12 + 0.33 * 0.4), vMB, MB1, comment = "Soc-AM4"))
					return;
			}
			else if ("return" == ForConf(budget, budget * 0.12, vMB, MB1, comment = "Soc-AM4"))
				return;
		}
		else {  //аналогично и с Интеловским процессором			
			if (processor(CPU1, budget, typeOfVideocard) > 0 && budget < 400) {
				if ("return" == ForConf(budget, budget * (0.12 + 0.33 * 0.4), vMB, MB1, comment = "Soc-1200"))
					return;
			}
			else if ("return" == ForConf(budget, budget * 0.12, vMB, MB1, comment = "Soc-1200"))
				return;
		}

		if (MB1.getName() == "empty name") {//если бюджета не хватает на основные детали, без которых комп не будет работать
			MessageBox::Show(this, "При заданном бюджете невозможно составить выбранную конфигурацию MB", "Сообщение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			//то выводим сообщение
			return;//и пусть пользователь изменит что-либо и нажмет на кнопку ещё раз
		}
			   	
		string typePlanok = msclr::interop::marshal_as< std::string >(planki_type());
		string plankiGb = msclr::interop::marshal_as< std::string >(planki_Gb());
		string hdd_Tb = msclr::interop::marshal_as< std::string >(HDD_Tb());
		string ssd_Gb = msclr::interop::marshal_as< std::string >(SSD_Gb());
		
		if (MB1.getName().find("DDR4") != -1)
			if (quantity == "1") { //выбираем планки DDR IV (DDR III уже устарела)	
				mem::set_Quantity_of_DDR(1); //передаём кол-во планок оперативной памяти
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
				if ("return" == ForConf(budget, budget * 0.0575, vMem, DDR1, "DDR IV", quantity, typePlanok, plankiGb))//если 2 планки памяти, то цену режем в 2 раза, чтобы вписаться в бюджет		
					return;
			}
			else {
				mem::set_Quantity_of_DDR(4);
				if (budget < 400) {
					if ("return" == ForConf(budget, budget * (0.02875 * 1.5), vMem, DDR1, "DDR IV", quantity, typePlanok, plankiGb))
						return;
				}
				if ("return" == ForConf(budget, budget * 0.02875, vMem, DDR1, "DDR IV", quantity, typePlanok, plankiGb))	//если 4 планки памяти, то цену режем в 4 раза, чтобы вписаться в бюджет			
					return;
			}
		if (DDR1.getName() == "empty name") {//если бюджета не хватает на основные детали, без которых комп не будет работать
			MessageBox::Show(this, "При заданном бюджете невозможно составить выбранную конфигурацию DDR", "Сообщение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			//то выводим сообщение
			return;//и пусть пользователь изменит что-либо и нажмет на кнопку ещё раз
		}
		if (DDR1.getName().find("KiTof2") != -1) //если у нас в конфигурации в названии оперативки есть "KiTof2", то это пара планок
			quantity = to_string(atoi(quantity.c_str()) / 2);//, т.е. делим кол-во на 2 (уже есть проверка на KiTof и 1 планку памяти)
		if (atoi(quantity.c_str()) == 1) txtBoxQuantityOfDDR->Text = "1"; //в зависимости от выбранного кол-ва планок памяти вывод соответствующего числа
		if (atoi(quantity.c_str()) == 2) txtBoxQuantityOfDDR->Text = "2";
		if (atoi(quantity.c_str()) == 4) txtBoxQuantityOfDDR->Text = "4";

		//AMD Radeon or GeForce
		//выбираем тип видеокарты
		if (processor(CPU1, budget, typeOfVideocard) == 0)//если комп НЕ слишком дешевый и процессор БЕЗ встроенной графики
		{//если дешевый комп, то должен выбраться процессор со встроенной графикой, тогда если у нас НЕ такой проц, то выбираем видеокарту
			comment = typeOfVideocard;
			if (comment == "AMD Radeon" || comment == "GeForce" || comment == "Internal") {
				if (CPU1.getName().find("Процессор BOX") != -1) {//если проц со встроенным кулером
					if ("return" == ForConf(budget, budget * (0.33 + 0.03), vGPU, GPU1, comment))//то часть бюджета для кулера тратим на видеокарту
						return;
				}
				else if ("return" == ForConf(budget, budget * 0.33, vGPU, GPU1, comment))
					return;
			}
		}

	
			
		if (MB1.getName().find("SATA")) //в зависимости от материнки выбираем способ подключения к ней SSD и HDD
			comment = "SATA";
		else comment = "M.2";
		if (budget >= 400) {	//если комп дешевый, то SSD может отсутствовать
			if ("return" == ForConf(budget, budget * 0.07, vSSD, SSD1, comment, "SSD", ssd_Gb))//в зависимости от comment выбираем передаём в функцию способ подключения (SATA или M.2)
				return;
			if ("return" == ForConf(budget, budget * 0.06, vHDD, HDD1, comment, "HDD", hdd_Tb))
				return;
		}
		else if ("return" == ForConf(budget, budget * (0.06 + 0.07) * 1.1, vHDD, HDD1, comment, "HDD", hdd_Tb))//в дешевых компах SSD можно проигнорировать
			return;//тогда часть бюджета на SSD отдаем на HDD
		
		if (HDD1.getName() == "empty name") {//если бюджета не хватает на основные детали, без которых комп не будет работать
			MessageBox::Show(this, "При заданном бюджете невозможно составить выбранную конфигурацию HDD", "Сообщение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			//то выводим сообщение
			return;//и пусть пользователь изменит что-либо и нажмет на кнопку ещё раз
		}

		if (MB1.getName().find("ATX")) //В зависимости от материнской платы выбираем размер корпуса
			comment = "ATX";
		else comment = "MicroATX";

		if (budget > 400) {//если комп не совсем дешевый
			if ("return" == ForConf(budget, budget * 0.07, vCase, Case1, comment))//то выбираем корпус без блока питания в комплекте
				return;
		}
		else if ("return" == ForConf(budget, budget * (0.07 + 0.07), vCase, Case1, comment))//иначе с БП (часть бюджета для БП отдаем на корпус)
			return;
		if (Case1.getName() == "empty name") {//если бюджета не хватает на основные детали, без которых комп не будет работать
			MessageBox::Show(this, "При заданном бюджете невозможно составить выбранную конфигурацию Case", "Сообщение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			//то выводим сообщение
			return;//и пусть пользователь изменит что-либо и нажмет на кнопку ещё раз
		}


		if (Case1.getName().find("W ") == -1)//если корпус без БП в комплекте
			if ("return" == ForConf(budget, budget * 0.07, vPSU, PSU1, "БП "))//то подбираем блок питания
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
		totalWithoutNacenka = CPU1.getCost() + Cooler1.getCost() + MB1.getCost() + colichestvo * DDR1.getCost()//складываем цены всех комплектующих конфигурации
			+ GPU1.getCost() + HDD1.getCost() + SSD1.getCost() + Case1.getCost() + PSU1.getCost();
		total = totalWithoutNacenka + nacenka;

	
		txtBox_cost_bez_nacenki->Text = totalWithoutNacenka.ToString();//цена без наценки
		txtBox_cost_bez_nacenki->Show();
		txtBox_total_Cost->Text = total.ToString();//итоговая цена
		txtBox_total_Cost->Show();

		outFile.close();
	};



	private: System::Void ComboBoxCPU_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {//когда выбираем другой процессор из списка
		int i = comboBoxCPU->SelectedIndex;

		static CPU last;
		if (last.getCost() == 0 && CPU1.getCost() != 0) //если мы только что создали переменную last и у нас уже есть выбранная программой комплектующая
			last = CPU1;
		else if (last.getCost() == 0 && CPU1.getCost() == 0)//если мы только что создали переменную last, но 
			//данный элемент ещё не задействовался программой при составлении конфигурации
			last = vCPU[0];//то присваиваем этой переменной элемент "None"

			OutputCostByDetailByChoosenDetail(vCPU, comboBoxCPU, textBoxCPUcost);

			totalWithoutNacenka = round(totalWithoutNacenka * 100) / 100;//убираем погрешность дробных чисел	
			totalWithoutNacenka = totalWithoutNacenka - last.getCost() + vCPU[i].getCost(); //когда меняется цена комплектующей, то должна меняться и цена итоговая

			txtBox_cost_bez_nacenki->Text = totalWithoutNacenka.ToString();//цену без наценки в строку
			txtBox_cost_bez_nacenki->Show();//и выводим её

			total = totalWithoutNacenka + nacenka;//затем рассчитываем итоговую стоимость
			txtBox_total_Cost->Text = total.ToString();//преобразуем её в строку
			txtBox_total_Cost->Show();//и выводим

			last = vCPU[i];//запоминаем последнюю выбранную деталь		
	}

	private: System::Void ComboBoxCooler_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		int i = comboBoxCooler->SelectedIndex;

		static Cooler last;
		if (last.getCost() == 0 && Cooler1.getCost() != 0) //если мы только что создали переменную last и у нас уже есть выбранная программой комплектующая
			last = Cooler1;
		else if (last.getCost() == 0 && Cooler1.getCost() == 0)//если мы только что создали переменную last, но 
			//данный элемент ещё не задействовался программой при составлении конфигурации
			last = vCooler[0];//то присваиваем этой переменной элемент "None"

		OutputCostByDetailByChoosenDetail(vCooler, comboBoxCooler, textBoxCoolerCost);

		totalWithoutNacenka = round(totalWithoutNacenka * 100) / 100;//убираем погрешность дробных чисел	
		totalWithoutNacenka = totalWithoutNacenka - last.getCost() + vCooler[i].getCost(); //когда меняется цена комплектующей, то должна меняться и цена итоговая

		txtBox_cost_bez_nacenki->Text = totalWithoutNacenka.ToString();//цену без наценки в строку
		txtBox_cost_bez_nacenki->Show();//и выводим её

		total = totalWithoutNacenka + nacenka;//затем рассчитываем итоговую стоимость
		txtBox_total_Cost->Text = total.ToString();//преобразуем её в строку
		txtBox_total_Cost->Show();//и выводим

		last = vCooler[i];//запоминаем последнюю выбранную деталь		
	}

	private: System::Void ComboBoxMB_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		int i = comboBoxMB->SelectedIndex;

		static MB last;
		if (last.getCost() == 0 && MB1.getCost() != 0)//если мы только что создали переменную last и у нас уже есть выбранная программой комплектующая
			last = MB1;
		else if (last.getCost() == 0 && MB1.getCost() == 0)//если мы только что создали переменную last, но 
					//данный элемент ещё не задействовался программой при составлении конфигурации
			last = vMB[0];//то присваиваем этой переменной элемент "None"

		OutputCostByDetailByChoosenDetail(vMB, comboBoxMB, textBoxMBcost);

		totalWithoutNacenka = round(totalWithoutNacenka * 100) / 100;//убираем погрешность дробных чисел	
		totalWithoutNacenka = totalWithoutNacenka - last.getCost() + vMB[i].getCost(); //когда меняется цена комплектующей, то должна меняться и цена итоговая

		txtBox_cost_bez_nacenki->Text = totalWithoutNacenka.ToString();//цену без наценки в строку
		txtBox_cost_bez_nacenki->Show();//и выводим её

		total = totalWithoutNacenka + nacenka;//затем рассчитываем итоговую стоимость
		txtBox_total_Cost->Text = total.ToString();//преобразуем её в строку
		txtBox_total_Cost->Show();//и выводим

		last = vMB[i];//запоминаем последнюю выбранную деталь		
	}

	private: System::Void ComboBoxMem_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		int i = comboBoxMem->SelectedIndex;
		if (i == -1)
			return;
		if(txtBoxQuantityOfDDR->Text == "")
			txtBoxQuantityOfDDR->Text = "1";
		static double lastCost;//тут мы используем lastCost, а не last(последняя деталь), т.к. хоть это и более затратно в плане ресурсов,
		//но может быть такая ситуация, что прошлое кол-во планок отличалось от теперешнего количества
		if (lastCost == 0 && DDR1.getCost() != 0)//если мы только что создали переменную last и у нас уже есть выбранная программой комплектующая
			lastCost = mem::get_Quantity_of_DDR() * DDR1.getCost();//запоминаем цену последней выбранной планки памяти, умноженную на кол-во планок
		else if (lastCost == 0 && DDR1.getCost() == 0)//если мы только что создали переменную last, но 
					//данный элемент ещё не задействовался программой при составлении конфигурации
			lastCost = vMem[0].getCost();//то присваиваем этой переменной элемент "None"
		
		OutputCostByDetailByChoosenDetail(vMem, comboBoxMem, textBoxMemCost);

		totalWithoutNacenka = round(totalWithoutNacenka * 100) / 100;//убираем погрешность дробных чисел	
		totalWithoutNacenka = totalWithoutNacenka - lastCost + mem::get_Quantity_of_DDR() * vMem[i].getCost(); //когда меняется цена комплектующей, то должна меняться и цена итоговая
			//умножаем стоимость планок памяти на количество планок

		txtBox_cost_bez_nacenki->Text = totalWithoutNacenka.ToString();//цену без наценки в строку
		txtBox_cost_bez_nacenki->Show();//и выводим её

		total = totalWithoutNacenka + nacenka;//затем рассчитываем итоговую стоимость
		txtBox_total_Cost->Text = total.ToString();//преобразуем её в строку
		txtBox_total_Cost->Show();//и выводим

		lastCost = mem::get_Quantity_of_DDR() * vMem[i].getCost();//запоминаем последнюю выбранную деталь		
	}

	private: System::Void ComboBoxGPU_SelectedIndexChanged(System::Object ^ sender, System::EventArgs ^ e) {
		int i = comboBoxGPU->SelectedIndex;

		static GPU last;
		if (last.getCost() == 0 && GPU1.getCost() != 0)//если мы только что создали переменную last и у нас уже есть выбранная программой комплектующая
			last = GPU1;
		else if (last.getCost() == 0 && CPU1.getCost() == 0)//если мы только что создали переменную last, но 
					//данный элемент ещё не задействовался программой при составлении конфигурации
			last = vGPU[0];//то присваиваем этой переменной элемент "None"

		OutputCostByDetailByChoosenDetail(vGPU, comboBoxGPU, textBoxGPUcost);

		totalWithoutNacenka = round(totalWithoutNacenka * 100) / 100;//убираем погрешность дробных чисел	
		totalWithoutNacenka = totalWithoutNacenka - last.getCost() + vGPU[i].getCost(); //когда меняется цена комплектующей, то должна меняться и цена итоговая

		txtBox_cost_bez_nacenki->Text = totalWithoutNacenka.ToString();//цену без наценки в строку
		txtBox_cost_bez_nacenki->Show();//и выводим её

		total = totalWithoutNacenka + nacenka;//затем рассчитываем итоговую стоимость
		txtBox_total_Cost->Text = total.ToString();//преобразуем её в строку
		txtBox_total_Cost->Show();//и выводим

		last = vGPU[i];//запоминаем последнюю выбранную деталь		
	}

	private: System::Void ComboBoxSSD_SelectedIndexChanged(System::Object ^ sender, System::EventArgs ^ e) {
		int i = comboBoxSSD->SelectedIndex;

		static ssd last;
		if (last.getCost() == 0 && SSD1.getCost() != 0)//если мы только что создали переменную last и у нас уже есть выбранная программой комплектующая
			last = SSD1;
		else if (last.getCost() == 0 && SSD1.getCost() == 0)//если мы только что создали переменную last, но 
					//данный элемент ещё не задействовался программой при составлении конфигурации
			last = vSSD[0];//то присваиваем этой переменной элемент "None"

		OutputCostByDetailByChoosenDetail(vSSD, comboBoxSSD, textBoxSSDcost);

		totalWithoutNacenka = round(totalWithoutNacenka * 100) / 100;//убираем погрешность дробных чисел	
		totalWithoutNacenka = totalWithoutNacenka - last.getCost() + vSSD[i].getCost(); //когда меняется цена комплектующей, то должна меняться и цена итоговая

		txtBox_cost_bez_nacenki->Text = totalWithoutNacenka.ToString();//цену без наценки в строку
		txtBox_cost_bez_nacenki->Show();//и выводим её

		total = totalWithoutNacenka + nacenka;//затем рассчитываем итоговую стоимость
		txtBox_total_Cost->Text = total.ToString();//преобразуем её в строку
		txtBox_total_Cost->Show();//и выводим

		last = vSSD[i];//запоминаем последнюю выбранную деталь		
	}

	private: System::Void ComboBoxHDD_SelectedIndexChanged(System::Object ^ sender, System::EventArgs ^ e) {
		int i = comboBoxHDD->SelectedIndex;

		static hdd last;
		if (last.getCost() == 0 && HDD1.getCost() != 0)//если мы только что создали переменную last и у нас уже есть выбранная программой комплектующая
			last = HDD1;
		else if (last.getCost() == 0 && HDD1.getCost() == 0)//если мы только что создали переменную last, но 
			//данный элемент ещё не задействовался программой при составлении конфигурации
			last = vHDD[0];//то присваиваем этой переменной элемент "None"

		OutputCostByDetailByChoosenDetail(vHDD, comboBoxHDD, textBoxHDDcost);

		totalWithoutNacenka = round(totalWithoutNacenka * 100) / 100;//убираем погрешность дробных чисел	
		totalWithoutNacenka = totalWithoutNacenka - last.getCost() + vHDD[i].getCost(); //когда меняется цена комплектующей, то должна меняться и цена итоговая
		
		txtBox_cost_bez_nacenki->Text = totalWithoutNacenka.ToString();//цену без наценки в строку
		txtBox_cost_bez_nacenki->Show();//и выводим её

		total = totalWithoutNacenka + nacenka;//затем рассчитываем итоговую стоимость
		txtBox_total_Cost->Text = total.ToString();//преобразуем её в строку
		txtBox_total_Cost->Show();//и выводим

		last = vHDD[i];//запоминаем последнюю выбранную деталь		
	}

			 static double gg;
			 static double ez;
			 static double noobs;

	private: System::Void ComboBoxCase_SelectedIndexChanged(System::Object ^ sender, System::EventArgs ^ e) {
		int i = comboBoxCase->SelectedIndex;

		static Case last;
		if (last.getCost() == 0 && Case1.getCost() != 0) //если мы только что создали переменную last и у нас уже есть выбранная программой комплектующая
			last = Case1;
		else if (last.getCost() == 0 && Case1.getCost() == 0)//если мы только что создали переменную last, но 
			//данный элемент ещё не задействовался программой при составлении конфигурации
			last = vCase[0];//то присваиваем этой переменной элемент "None"

		OutputCostByDetailByChoosenDetail(vCase, comboBoxCase, textBoxCaseCost);

		totalWithoutNacenka = round(totalWithoutNacenka * 100) / 100;//убираем погрешность дробных чисел		
		totalWithoutNacenka = totalWithoutNacenka - last.getCost() + vCase[i].getCost(); //когда меняется цена комплектующей, то должна меняться и цена итоговая
		txtBox_cost_bez_nacenki->Text = totalWithoutNacenka.ToString();//цену без наценки в строку
		txtBox_cost_bez_nacenki->Show();//и выводим её

		total = totalWithoutNacenka + nacenka;//затем рассчитываем итоговую стоимость
		txtBox_total_Cost->Text = total.ToString();//преобразуем её в строку
		txtBox_total_Cost->Show();//и выводим

		last = vCase[i];//запоминаем последнюю выбранную деталь		
	}

	private: System::Void ComboBoxPSU_SelectedIndexChanged(System::Object ^ sender, System::EventArgs ^ e) {
		int i = comboBoxPSU->SelectedIndex;

		static PSU last;
		if (last.getCost() == 0 && PSU1.getCost() != 0) //если мы только что создали переменную last и у нас уже есть выбранная программой комплектующая
			last = PSU1;
		else if (last.getCost() == 0 && PSU1.getCost() == 0)//если мы только что создали переменную last, но 
			//данный элемент ещё не задействовался программой при составлении конфигурации
			last = vPSU[0];//то присваиваем этой переменной элемент "None"

		OutputCostByDetailByChoosenDetail(vPSU, comboBoxPSU, textBoxPSUcost);

		totalWithoutNacenka = round(totalWithoutNacenka * 100) / 100;//убираем погрешность дробных чисел	
		totalWithoutNacenka = totalWithoutNacenka - last.getCost() + vPSU[i].getCost(); //когда меняется цена комплектующей, то должна меняться и цена итоговая
		
		txtBox_cost_bez_nacenki->Text = totalWithoutNacenka.ToString();//цену без наценки в строку
		txtBox_cost_bez_nacenki->Show();//и выводим её

		total = totalWithoutNacenka + nacenka;//затем рассчитываем итоговую стоимость
		txtBox_total_Cost->Text = total.ToString();//преобразуем её в строку
		txtBox_total_Cost->Show();//и выводим

		last = vPSU[i];//запоминаем последнюю выбранную деталь			
	}
	

	private: System::Void Button_Sort_by_name_Click(System::Object^ sender, System::EventArgs^ e) {//сортировка по имени
		sortByName(vCPU, comboBoxCPU);
		sortByName(vMB, comboBoxMB);
		sortByName(vCooler, comboBoxCooler);
		sortByName(vMem, comboBoxMem);
		sortByName(vGPU, comboBoxGPU);
		sortByName(vSSD, comboBoxSSD);
		sortByName(vHDD, comboBoxHDD);
		sortByName(vCase, comboBoxCase);
		sortByName(vPSU, comboBoxPSU);	

		sortByIndex(vCPU, comboBoxCPU);//после сортировки индексы перепутались, а для корректной работы программы нужно, чтобы они шли по порядку
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
	
	private: System::Void Button_Sort_by_cost_Click(System::Object^ sender, System::EventArgs^ e) {//сортировка по цене
		sortByCost(vCPU);
		sortByCost(vMB);
		sortByCost(vCooler);
		sortByCost(vMem);
		sortByCost(vGPU);
		sortByCost(vSSD);
		sortByCost(vHDD);
		sortByCost(vCase);
		sortByCost(vPSU);

		sortByIndex(vCPU, comboBoxCPU);//после сортировки индексы перепутались, а для корректной работы программы нужно, чтобы они шли по порядку
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
	private: System::Void Button_Sort_by_id_Click(System::Object^ sender, System::EventArgs^ e) {//сортировка по Id (порядку считывания с прайс-листа)
		sortById(vCPU, comboBoxCPU);
		sortById(vMB, comboBoxMB);
		sortById(vCooler, comboBoxCooler);
		sortById(vMem, comboBoxMem);
		sortById(vGPU, comboBoxGPU);
		sortById(vSSD, comboBoxSSD);
		sortById(vHDD, comboBoxHDD);
		sortById(vCase, comboBoxCase);
		sortById(vPSU, comboBoxPSU);

		sortByIndex(vCPU, comboBoxCPU);//после сортировки индексы перепутались, а для корректной работы программы нужно, чтобы они шли по порядку
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

private: System::Void ButtonConfigInExcel_Click(System::Object^ sender, System::EventArgs^ e) {//когда мы нажимаем кнопку "Сохранить конфигурацию в Excel"
	try {
		outFile.open("Готовые конфигурации.xls", ofstream::app);
		if (!outFile.is_open())
			throw exception();
	}
	catch (Exception^ ex) {
		MessageBox::Show(this, "Файл для записи не был открыт" + ex, "Сообщение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		exit(-1);
	}

	//выводим название выбранной комплектующей
	outFile  << msclr::interop::marshal_as< std::string >(comboBoxCPU->Text) 
	 << "\t" << msclr::interop::marshal_as< std::string >(textBoxCPUcost->Text) << endl;//выводим цену выбранной комплектующей

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
	outFile << "Цена без наценки" << "\t";
	outFile << msclr::interop::marshal_as< std::string >(txtBox_cost_bez_nacenki->Text)  << endl; //выводим итоговую стоимость без учета наценки
	outFile << "Цена с наценкой" << "\t";
	outFile << msclr::interop::marshal_as< std::string >(txtBox_total_Cost->Text)  << endl;//выводим итоговую стоимость с наценкой
	outFile << endl;//чтобы следующая конфигурация шла на некотором удалении от предыдущей
	outFile << endl;
	outFile << endl;
}

	private: System::Void TxtBoxQuantityOfDDR_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		static string lastString;
		lastString = msclr::interop::marshal_as< std::string >(txtBoxQuantityOfDDR->Text);//запоминаем последнюю считанную строку

		if (txtBoxQuantityOfDDR->Text == "")
			return;//если это просто удаление через Backspace, то ничего не делаем

		if (txtBoxQuantityOfDDR->Text == "1")//считываем введенное кол-во планок 
			mem::set_Quantity_of_DDR(1);//и присваиваем соответствующее значение переменной содержащую кол-во планок
		else	if (txtBoxQuantityOfDDR->Text == "2")
			mem::set_Quantity_of_DDR(2);
		else if (txtBoxQuantityOfDDR->Text == "4")
			mem::set_Quantity_of_DDR(4);
		else MessageBox::Show(this, "Введите 1, 2 или 4", "Сообщение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		if (DDR1.getName() != "empty name") {
			ComboBoxMem_SelectedIndexChanged(sender, e);//вызываем эту функцию, чтобы поменялась цена (реализация выше по программе)			
			textBoxMemCost->Text = (mem::get_Quantity_of_DDR() *	atof(msclr::interop::marshal_as< string >(textBoxMemCost->Text).c_str())).ToString();
			//цену из текстбокса про стоимость планок памяти умножаем на кол-во планок памяти
		}
	}
};
}




		 
