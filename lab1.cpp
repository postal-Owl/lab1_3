// 10 Вариант. 
// Объект исследования: Растения.
// Примеры классов: Дерево (tree), Кустарник (flower / цветы), Кактус (cactus)
// Общие параметры: Название.
// Параметры отдельных классов:
//		Дерево - возраст (long int);
//		Цветы - месяца цветения (vector<int>);		(В задании упоминается "перечисляемый тип")
//		Кактус - длина колючек (float).
//


#include <iostream>
#include <vector>
#include <string>
#include "nlohmann/json.hpp"
#include <fstream>
#include <algorithm>

using json = nlohmann::json;

using namespace std;

class plants {
private:
	string name;

public:
	
	plants() : name("") {}

	explicit plants(string const& new_name) : name(new_name) {}

	string get_name() const { return this->name; }

	void change_name(string const& new_name) { this->name = new_name; }
	
	virtual void print_plant() {};
	virtual string get_type() { return "plant"; };

	virtual ~plants() = default; // Виртуальный деструктор
};

class tree : public plants {
private:
	long int age;

public:

	tree() { this->age = 0; }

	explicit tree(string const& new_name) { this->change_name(new_name); this->age = 0; }

	explicit tree(long int new_age) { this->age = new_age; }

	tree(string const& new_name, long int new_age) {
		this->change_name(new_name);
		this->age = new_age;
	}

	long int get_age() const { return this->age; };

	void change_age(long int new_age) { this->age = new_age; }

	void print_plant() override {
		cout << "Tree: \t\t" << this->get_name() << endl;
		cout << "Age:  \t\t\t" << this->get_age() << endl;
	}

	string get_type() final { return "tree"; };
};

class flower : public plants {
private:
	vector<int> months;

public:

	flower() = default;

	explicit flower(string const& new_name) { this->change_name(new_name); }

	explicit flower(vector<int> const& new_dates) { this->months = new_dates; }

	flower(string const& new_name, vector<int> const& new_dates) {
		this->change_name(new_name);
		this->months = new_dates;
	}

	vector<int> get_months() const { return this->months; };

	void change_months(vector<int> const& new_dates) { this->months = new_dates; }

	void add_date_to_months(int new_date) { this->months.push_back(new_date); }

	void print_plant() override {
		const bool vertical = false;	// Вертикальный вывод массива или горизонтальный
		cout << "flower:\t\t" << this->get_name() << endl;
		cout << "flowering mounth:" << (vertical ? "\n" : "\t");
		for (int i = 0; i < this->get_months().size(); i++)
			cout << (vertical ? "\t\t\t" : "") << this->get_months()[i] << (vertical ? "\n" : " ");
		cout << endl;
	}

	string get_type() final { return "flower"; };
};

class cactus : public plants {
private:
	float lenght;

public:

	cactus() { this->lenght = 0; }

	explicit cactus(string const& new_name) { this->change_name(new_name); this->lenght = 0; }

	explicit cactus(float new_len) { this->lenght = new_len; }

	cactus(string const& new_name, float new_len) {
		this->change_name(new_name);
		this->lenght = new_len;
	}

	float get_lenght() const { return this->lenght; };

	void change_lenght(float new_len) { this->lenght = new_len; }

	void print_plant() override {
		cout << "Cactus:\t\t" << this->get_name() << endl;
		cout << "Length: \t\t" << this->get_lenght() << endl;		// не стилизовано
	}

	string get_type() final { return "cactus"; };
};

void print(vector<plants*> garden) {
	for (int i = 0; i < garden.size(); i++) {
		cout << i << ". ";
		garden[i]->print_plant();
		cout << endl;
	}
	cout << "Всего растений в саду: " << garden.size() << endl;		// можно усложнить, чтобы выводило кол-во каждого типа растений
}

template <typename T>
void add_obj(vector<plants*>& garden, T new_plant) {				// добавление объекта в сад
	if (new_plant.at("type").get<string>() == "tree") {
		auto* plant = new tree;
		plant->change_name(new_plant.at("name").get<string>());
		plant->change_age(new_plant.at("age").get<long int>());
		garden.push_back(plant);
		return;
	}

	if (new_plant.at("type").get<string>() == "flower") {
		auto* plant = new flower;
		plant->change_name(new_plant.at("name").get<string>());
		plant->change_months(new_plant.at("mounths"));
		garden.push_back(plant);
		return;
	}

	if (new_plant.at("type").get<string>() == "cactus") {
		auto* plant = new cactus;
		plant->change_name(new_plant.at("name").get<string>());
		plant->change_lenght(new_plant.at("lenght").get<float>());
		garden.push_back(plant);
		return;
	}

	cout << "В списке присутствует неизвестный тип." << endl;
	return;
}

int main()
{
	setlocale(LC_ALL, "ru");

	ifstream file("garden.json");
	if (!file.is_open()) {
		cout << "Не удалось открыть файл garden.json" << endl;
		return 1;
	}

	vector<plants*> garden;

	json data;
	file >> data;

	// заполнение вектора garden объектами, хранящимися в json файле
	for (auto& i : data)
		add_obj(garden, i);

	bool exit = false;

	while (!exit) {

		cout << endl;
		cout << "1. Добавить объект." << endl;
		cout << "2. Удалить объект." << endl;
		cout << "3. Вывести список." << endl;
		cout << "4. Выход.\n" << endl;

		int mode = 0;
		cout << "Выберите действие: ";
		cin >> mode;
		cout << endl;

		string new_type;

		switch (mode) {
		case 1:
			cout << "Введите тип объекта [tree, flower, cactus]: ";
			cin >> new_type;

			if (new_type == "tree") {
				auto* plant = new tree;
				cout << "Введите название дерева: ";
				string str_buffer;
				cin >> str_buffer;
				plant->change_name(str_buffer);

				cout << "Введите возраст дерева [0-10^6 лет]: ";
				int int_buffer;

				bool error = true;
				while (error) {
					cin >> int_buffer;
					if (int_buffer >= 0 && int_buffer <= 10000000 && !cin.fail())
						error = false;
					else
					{
						cout << "Некорректное значение.\n";
						cout << "Введите возраст еще раз : ";
						cin.clear();
						cin.ignore(314159, '\n');
					}
				}
				plant->change_age(int_buffer);
				garden.push_back(plant);

				cout << "Дерево успешно добавлено." << endl;
			}																			// добавление дерева
			else if (new_type == "cactus") {
				auto* plant = new cactus;
				cout << "Введите имя кактуса: ";
				string str_buffer;
				cin >> str_buffer;
				plant->change_name(str_buffer);

				cout << "Введите длину колючек [0-50 см]: ";
				float float_buffer;

				bool error = true;
				while (error) {
					cin >> float_buffer;
					if (float_buffer >= 0 && float_buffer <= 50 && !cin.fail())
						error = false;
					else
					{
						cout << "Некорректное значение.\n";
						cout << "Значение длины колючек должно быть вещественным числом в диапозоне от 0 до 50.\n";
						cout << "Введите значение еще раз[0 - 50 см]: ";
						cin.clear();
						cin.ignore(314159, '\n');
					}
				}
				plant->change_lenght(float_buffer);
				garden.push_back(plant);

				cout << "Кактус успешно добавлен." << endl;
			}																				// добавление кактуса
			else if (new_type == "flower") {
				auto* plant = new flower;
				cout << "Введите имя цветка: ";
				string str_buffer;
				cin >> str_buffer;
				plant->change_name(str_buffer);

				cout << "Введите кол-во месяцев, когда цветет цветок [1-12]: ";
				int count_m;

				bool error = true;
				while (error) {
					cin >> count_m;
					if (count_m >= 1 && count_m <= 12 && !cin.fail())
						error = false;
					else
					{
						cout << "Некорректное значение.\n";
						cout << "Кол-во месяцев не должно превышать 12.\n";
						cout << "Введите кол - во еще раз[1 - 12]: ";
						cin.clear();
						cin.ignore(314159, '\n');
					}
				}

				int int_buffer;

				for (int i = 0; i < count_m; i++) {
					cout << "Введите номер месяца [1-12]: ";

					bool error = true;
					while (error) {
						cin >> int_buffer;
						if (int_buffer >= 1 && int_buffer <= 12 && !cin.fail()) {
							// Проверка на наличие месяца в списке
							bool check_month = false;
							for (int j = 0; j < plant->get_months().size(); j++) {
								if (plant->get_months()[j] == int_buffer) {
									cout << "Такой месяц уже есть в списке." << endl;
									cout << "Повторите ввод.\n";
									cout << "Enter: ";
									check_month = true;
									break;
								}
							}
							if (check_month)
								continue;
							error = false;
						}
						else
						{
							// Проверка на валидность значений
							cout << "Некорректное значение.\n";
							cout << "Номер месяца должен находиться в диапозоне от 1 до 12.\n";
							cout << "Enter: ";
							cin.clear();
							cin.ignore(314159, '\n');
						}
					}

					plant->add_date_to_months(int_buffer);
					cout << "Месяц успешно добавлен.\n";
				}

				garden.push_back(plant);

				cout << "Цветок успешно добавлен." << endl;
			}																				// добавление цветка
			else {
				cout << "Тип объекта выбран некорректно." << endl;
			}

			break;

		case 2:
			cout << "Введите тип объекта [tree, flower, cactus]: ";
			cin >> new_type;

			if (new_type == "tree") {
				cout << "Выберите параметр [type, name, age]: ";
				string param;
				cin >> param;

				if (param == "type") {
					cout << "Удалить все деревья из сада?\n1. Да\n2. Нет\n";
					cout << "Enter: ";
					int choose;
					cin >> choose;
					if (choose == 1) {
						bool fixed_del = false;		// флаг фиксирует удаление
						int count_del = 0;			// счетчик кол-ва элементов, которые нужно будет удалить

						for (int i = 0; i < garden.size() - count_del; i++) {	// смотрим на все элементы, не считая тех, что были перемещены в конец списка на удаление
							if (auto* t = dynamic_cast<tree*>(garden[i])) {		// пытаемся создать элемент с классом tree, если успешно, то идем дальше
								if (t->get_type() == "tree") {					// предположительно это излишняя проверка.
									rotate(garden.begin() + i, garden.begin() + i + 1, garden.end());	// перемещаем в конец списка элемент, который подлежит удалению
									fixed_del = true;
									count_del++;
									i--;
								}
							}
						}

						for (int d = 0; d < count_del; d++)		// удаление элементов из конца списка
							garden.pop_back();

						if (fixed_del)
							cout << "Все деревья были удалены.\n";
						else
							cout << "Деревья в списке отсутсвовали.\n";
						break;
					}
				}

				if (param == "name") {
					cout << "Введите название дерева: ";
					string choose;
					cin >> choose;

					// аналогичная часть кода, что и при удалении всех деревьев	//
					bool fixed_del = false;
					int count_del = 0;

					for (int i = 0; i < garden.size() - count_del; i++) {
						if (auto* t = dynamic_cast<tree*>(garden[i])) {
							if (t->get_name() == choose) {
								rotate(garden.begin() + i, garden.begin() + i + 1, garden.end());
								fixed_del = true;
								count_del++;
								i--;
							}
						}
					}

					for (int d = 0; d < count_del; d++)
						garden.pop_back();
					/////////////////////////////////////////////////////////////

					if (fixed_del)
						cout << "Все деревья с названием " << choose << " были удалены.\n";
					else
						cout << "Деревья с таким названием не найдены.\n";
					break;
				}

				if (param == "age") {
					cout << "Введите возраст дерева: ";
					int choose;
					cin >> choose;

					cout << "Удалить деревья, которые:\n";
					cout << "1. Соответствуют этому возрасту.\n";
					cout << "2. Младше этого возраста.\n";
					cout << "3. Старше этого возраста.\n";
					cout << "Enter: ";
					int choose_2;
					cin >> choose_2;

					// удаление по условию //
					bool fixed_del = false;
					int count_del = 0;

					for (int i = 0; i < garden.size() - count_del; i++) {
						if (auto* t = dynamic_cast<tree*>(garden[i])) {
							if (((t->get_age() == choose) && (choose_2 == 1)) ||
								((t->get_age() <= choose) && (choose_2 == 2)) ||
								((t->get_age() >= choose) && (choose_2 == 3))) {
								rotate(garden.begin() + i, garden.begin() + i + 1, garden.end());
								fixed_del = true;
								count_del++;
								i--;
							}
						}
					}

					for (int d = 0; d < count_del; d++)
						garden.pop_back();
					/////////////////////////

					if (fixed_del) {
						cout << "Все деревья, чей возраст ";
						cout << ((choose_2 == 1) ? "соответствовал " : ((choose_2 == 2) ? "был младше " : "был старше "));
						cout << choose << ", были удалены.\n";
					}
					else
						cout << "Деревья с подходящим возрастом не найдены.\n";
					break;
				}
			}
			else if (new_type == "cactus") {
				cout << "Выберите параметр [type, name, len]: ";
				string param;
				cin >> param;

				if (param == "type") {
					cout << "Удалить все кактусы из сада?\n1. Да\n2. Нет\n";
					cout << "Enter: ";
					int choose;
					cin >> choose;
					if (choose == 1) {
						// удаление по условию //
						bool fixed_del = false;
						int count_del = 0;

						for (int i = 0; i < garden.size() - count_del; i++) {
							if (auto* c = dynamic_cast<cactus*>(garden[i])) {
								if (c->get_type() == "cactus") {
									rotate(garden.begin() + i, garden.begin() + i + 1, garden.end());
									fixed_del = true;
									count_del++;
									i--;
								}
							}
						}

						for (int d = 0; d < count_del; d++)
							garden.pop_back();
						/////////////////////////

						if (fixed_del)
							cout << "Все кактусы были удалены.\n";
						else
							cout << "Кактусы в списке отсутсвовали.\n";
						break;
					}
				}

				if (param == "name") {
					cout << "Введите имя кактуса: ";
					string choose;
					cin >> choose;

					// удаление по условию //
					bool fixed_del = false;
					int count_del = 0;

					for (int i = 0; i < garden.size() - count_del; i++) {
						if (auto* c = dynamic_cast<cactus*>(garden[i])) {
							if (c->get_name() == choose) {
								rotate(garden.begin() + i, garden.begin() + i + 1, garden.end());
								fixed_del = true;
								count_del++;
								i--;
							}
						}
					}

					for (int d = 0; d < count_del; d++)
						garden.pop_back();
					/////////////////////////

					if (fixed_del)
						cout << "Все кактусы с именами " << choose << " были удалены.\n";
					else
						cout << "Кактусов с таким именем не найдено.\n";
					break;
				}

				if (param == "len") {
					cout << "Введите длину колючек: ";
					int choose;
					cin >> choose;

					cout << "Удалить кактусы, у которых колючки:\n";
					cout << "1. Равны заданной длинне.\n";
					cout << "2. Меньше заданной длинны.\n";
					cout << "3. Больше заданной длинны.\n";
					cout << "Enter: ";
					int choose_2;
					cin >> choose_2;

					// удаление по условию //
					bool fixed_del = false;
					int count_del = 0;

					for (int i = 0; i < garden.size() - count_del; i++) {
						if (auto* c = dynamic_cast<cactus*>(garden[i])) {
							if (((c->get_lenght() == choose) && (choose_2 == 1)) ||
								((c->get_lenght() <= choose) && (choose_2 == 2)) ||
								((c->get_lenght() >= choose) && (choose_2 == 3))) {
								rotate(garden.begin() + i, garden.begin() + i + 1, garden.end());
								fixed_del = true;
								count_del++;
								i--;
							}
						}
					}

					for (int d = 0; d < count_del; d++)
						garden.pop_back();
					/////////////////////////

					if (fixed_del) {
						cout << "Все кактусы, с длинной колючек ";
						cout << ((choose_2 == 1) ? "равной " : ((choose_2 == 2) ? "меньше " : "больше "));
						cout << choose << ", были удалены.\n";
					}
					else
						cout << "Кактусы с подходящими параметрами не найдены.\n";
					break;
				}
			}
			else if (new_type == "flower") {
				cout << "Выберите параметр [type, name, months]: ";
				string param;
				cin >> param;

				if (param == "type") {
					cout << "Удалить все кустарники из сада?\n1. Да\n2. Нет\n";
					cout << "Enter: ";
					int choose;
					cin >> choose;
					if (choose == 1) {
						// удаление по условию //
						bool fixed_del = false;
						int count_del = 0;

						for (int i = 0; i < garden.size() - count_del; i++) {
							if (auto* f = dynamic_cast<flower*>(garden[i])) {
								if (f->get_type() == "flower") {
									rotate(garden.begin() + i, garden.begin() + i + 1, garden.end());
									fixed_del = true;
									count_del++;
									i--;
								}
							}
						}

						for (int d = 0; d < count_del; d++)
							garden.pop_back();
						/////////////////////////

						if (fixed_del)
							cout << "Все кустарники были удалены.\n";
						else
							cout << "Кустарники в списке отсутсвовали.\n";
						break;
					}
				}

				if (param == "name") {
					cout << "Введите название цветка: ";
					string choose;
					cin >> choose;

					// удаление по условию //
					bool fixed_del = false;
					int count_del = 0;

					for (int i = 0; i < garden.size() - count_del; i++) {
						if (auto* f = dynamic_cast<flower*>(garden[i])) {
							if (f->get_name() == choose) {
								rotate(garden.begin() + i, garden.begin() + i + 1, garden.end());
								fixed_del = true;
								count_del++;
								i--;
							}
						}
					}

					for (int d = 0; d < count_del; d++)
						garden.pop_back();
					/////////////////////////

					if (fixed_del)
						cout << "Все цветки с названием " << choose << " были удалены.\n";
					else
						cout << "Цветки с таким названием не найдены.\n";
					break;
				}

				if (param == "months") {
					cout << "Укажите месяца [1-12].\n";
					cout << "Если кустарник цветет хотя бы в один из указанных месяцов, то он будет удален из сада.\n";
					cout << "(Для выхода введите 0)\n";

					vector<int> choose;

					for (int i = 0; i < 12; i++) {
						cout << "Enter: ";
						int int_buffer = 0;
						cin >> int_buffer;
						if (int_buffer == 0)
							break;
						if (int_buffer >= 1 && int_buffer <= 12 && !cin.fail())
							choose.push_back(int_buffer);
						else if (int_buffer < 0 || int_buffer > 12 || cin.fail()) {
							cout << "Некорректно введенный месяц.\n";
							i--;
							cin.clear();
							cin.ignore(314159, '\n');
							continue;
						}

					}

					// удаление по условию //
					bool fixed_del = false;
					int count_del = 0;

					for (int i = 0; i < garden.size() - count_del; i++) {			// проход по саду
						if (auto f = dynamic_cast<flower*>(garden[i])) {			// если цветок, то делаем проверку
							bool del_fix = false;
							for (int j = 0; j < choose.size(); j++) {				// проходим по заданному массиву
								for (int m = 0; m < f->get_months().size(); m++) {	// проходим по массиву внутри цветка
									if (f->get_months()[m] == choose[j]) {
										del_fix = true;								// если есть совпадение, зафиксировать и выйти из цикла
										break;
									}
								}
								if (del_fix) {
									rotate(garden.begin() + i, garden.begin() + i + 1, garden.end());	// перемещение удаляемых элементов в конец вектора
									fixed_del = true;
									count_del++;
									i--;
									break;
								}
							}
						}
					}

					for (int d = 0; d < count_del; d++)
						garden.pop_back();											// удаление
					/////////////////////////

					if (fixed_del)
						cout << "Все цветы, которые цвели в указанные месяца, были удалены. ";
					else
						cout << "Деревья с подходящим возрастом не найдены.\n";
					break;
				}
			}
			else {
				cout << "Тип объекта выбран некорректно." << endl;
			}

			break;

		case 3:
			print(garden);
			break;

		default:
			exit = true;
		}
	}

	for (auto& i : garden)
		delete i;
	garden.clear();
	file.close();

	return 0;
}
