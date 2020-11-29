/* Вариант 27:
 *
 * Пляшущие человечки.
 * На тайном собрании глав преступного мира города
 * Лондона председатель собрания профессор Мориарти
 * постановил: отныне вся переписка между преступниками
 * должна вестись тайнописью. В качестве стандарта
 * были выбраны "пляшущие человечки", шифр, в котором
 * каждой букве латинского алфавита соответствует
 * хитроумный значок. Реализовать многопоточное при-
 * ложение, шифрующее исходный текст (в качестве ключа
 * используется кодовая таблица, устанавливающая одноз-
 * начное соответствие между каждой буквой и каким-нибудь
 * числом). Каждый поток шифрует свои кусочки текста.
 * При решении использовать парадигму портфеля задач.
 *
 * ФИО: Шалаева Марина Андреевна
 * Группа: БПИ191
 */

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <ctime>
#include <algorithm>
#include <omp.h>

 // Имя входного файла.
std::string input;
// Имя выходного файла.
std::string output;
// Имя выходного файла для букв и их числовых значений.
std::string output_alphabet;

// Переменная для считанного из файла текста.
std::string text;
// Строковый массив для записи закодированных переменных.
std::string* encoded_text;
// Словарь (карта) для хранения строчных букв латинского алфавита и их численных значений.
std::map<char, short> alphabet;
// Вспомогательный массив, индексы элементов которого служат значениями букв.
short numbers[90] = { 0 };

/// <summary>
/// Функция, дополняющая название файла до
/// полного пути к нему.
/// </summary>
void addingFullPath() {

	if (input.find("files\\input\\") == std::string::npos)
		input = "files\\input\\" + input;
	if (output.find("files\\output\\") == std::string::npos)
		output = "files\\output\\" + output;
	if (output_alphabet.find("files\\output_alphabet\\") == std::string::npos)
		output_alphabet = "files\\output_alphabet\\" + output_alphabet;
}

/// <summary>
/// Фукция для пересоздания выходного
/// файла, на случай если тот уже создан.
/// </summary>
void createNewOutputFile() {
	std::ofstream out;
	out.open(output, std::ios::out);
	out << "";
	out.close();
}

/// <summary>
/// Функция для заполнения файла
/// данными из словаря (карты).
/// </summary>
void fillAlphabetFiles() {

	std::ofstream out;
	out.open(output_alphabet, std::ios::out);

	out << "--------\n";
	for (auto& item : alphabet) {
		out << " " << item.first << " | " << item.second 
			<< "\n--------\n";
	}
	out.close();
}

/// <summary>
/// Функция для заполнения словаря (карты)
/// строчными буквами латинского алфавита
/// (ключи) и случайными числами от 10
/// до 99 (значения).
/// </summary>
void fillAlphabet() {

	for (size_t i = 0; i < 26; i++) {

		srand((unsigned int)time(nullptr));

		int index_of_numbers_array = rand() % 90;

		if (numbers[index_of_numbers_array] == 0) {
			alphabet[(char)(i + 97)] = index_of_numbers_array + 10;
			numbers[index_of_numbers_array] = 1;
		}
		else
			i--;
	}
	fillAlphabetFiles();
}

/// <summary>
/// Функция для считывания входных
/// данных из файла.
/// </summary>
void readFromFile() {
	std::ifstream in;
	in.open(input, std::ios::in);

	if (!in.is_open()) {
		std::cout << "Opening of the file failed!\n";
	}
	else {
		char x;
		text = "";
		while ((x = in.get()) != EOF) {
			text += tolower(x);
		}
	}
	in.close();
}

/// <summary>
/// Функция для кодирования текста
/// с помощью нескольких потоков
/// с использованием OpenMP.
/// </summary>
void encode() {

#pragma omp parallel for

	for (int i = 0; i < text.size(); ++i) {

		char letter;
		std::map<char, short>::iterator it;
		letter = text[i];
		it = alphabet.find(letter);

		if (it == alphabet.end())
			encoded_text[i] = (char)(letter);
		else
			encoded_text[i] = std::to_string(it->second);
	}
}

/// <summary>
/// Функция для распределения задач по
/// различным потокам.
/// </summary>
void launchPortfolio() {

	/* Инициализируем динамический массив,
	 * в который мы запишем закодированные
	 * буквы, используя соответствующие
	 * индексы. */
	encoded_text = new std::string[text.size()];

	encode();

	std::ofstream out;
	out.open(output, std::ios::app);
	// Запись задокированного текста в файл.
	for (int i = 0; i < text.size(); ++i) {
		out << encoded_text[i];
	}
	out.close();

	// Удаление динамического массива.
	delete[]encoded_text;
}

/// <summary>
/// Функция, проверяющая валидность входных
/// параметров и присваивающая необходимые
/// значения полям, если входные параметры
/// верные.
/// </summary>
/// <param name="argc"> - количество входныз параметров</param>
/// <param name="argv"> - массив входных параметров</param>
void workingWithInputValues(const int& argc, char* argv[]) {

	/* Если количество введенных параметров не равно
	* нужному количеству, сообщаем об этом пользователю
	* и завершаем работу приложения. */
	if (argc != 4) {
		std::cout << "An invalid format of input params!\n"
			"Use the name of input file as the first value,\n"
			"the name of output file as the second one and the\n"
			"name of output file for letters and its codes as\n"
			"the third one.\n";
		exit(0);
	}

	input = argv[1];
	output = argv[2];
	output_alphabet = argv[3];

	addingFullPath();
	createNewOutputFile();
}

int main(int argc, char* argv[]) {

	workingWithInputValues(argc, argv);
	fillAlphabet();
	readFromFile();
	launchPortfolio();

	return 0;
}
