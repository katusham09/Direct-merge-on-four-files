#pragma once
#include <iostream>
#include <fstream>
#include <ios>
#include <random>
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::string;
using std::cout;
using std::endl;

bool notEmptyFile(fstream& file)
{
	if (file.peek() == std::ifstream::traits_type::eof())
	{
		return false;
	}
	return true;
}

bool createFileWithRandomNumbers(const std::string& fileName, const int numbersCount, const int maxNumberValue)
{
	fstream file;
	file.open(fileName, fstream::out);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist(0, maxNumberValue);
	for (int i = 0; i < numbersCount; i++)
	{
		file <<  int(dist(gen)) << " ";
	}

	if (!file.is_open())
	{
		return false;
	}
	else
	{
		file.close();
		return true;
	}
}

void partitioningPhase(fstream& file, fstream *arrFilesToWrite, int p)
{
	int readableElement;
	file >> readableElement;

	int fileNumber = 0;
	while (file)
	{
		for (int i = 0; file && (i < p); i++)
		{
			arrFilesToWrite[fileNumber] << readableElement << " ";
			file >> readableElement;
		}
		fileNumber = 1 - fileNumber;
	}
}

int mergePhase(fstream *arrFilesToRead, fstream* arrFilesToWrite, int p)
{
	int arrReadableElement[2];
	arrFilesToRead[0] >> arrReadableElement[0];
	arrFilesToRead[1] >> arrReadableElement[1];
	int fileNumber = 0;
	while (arrFilesToRead[0] && arrFilesToRead[1])
	{
		int i = 0, j = 0;
		while ((arrFilesToRead[0] && arrFilesToRead[1]) && i < p && j < p)
		{
			if (arrReadableElement[0] < arrReadableElement[1])
			{
				arrFilesToWrite[fileNumber] << arrReadableElement[0] << " ";
				arrFilesToRead[0] >> arrReadableElement[0];
				i++;
			}
			else
			{
				arrFilesToWrite[fileNumber] << arrReadableElement[1] << " ";
				arrFilesToRead[1] >> arrReadableElement[1];
				j++;
			}
		}

		while (arrFilesToRead[0] && (i < p))
		{
			arrFilesToWrite[fileNumber] << arrReadableElement[0] << " ";
			arrFilesToRead[0] >> arrReadableElement[0];
			i++;
		}

		while (arrFilesToRead[1] && (j < p))
		{
			arrFilesToWrite[fileNumber] << arrReadableElement[1] << " ";
			arrFilesToRead[1] >> arrReadableElement[1];
			j++;
		}
		fileNumber = 1 - fileNumber;
	}

	while (arrFilesToRead[0])
	{
		arrFilesToWrite[fileNumber] << arrReadableElement[0] << " ";
		arrFilesToRead[0] >> arrReadableElement[0];
	}
	while (arrFilesToRead[1])
	{
		arrFilesToWrite[fileNumber] << arrReadableElement[1] << " ";
		arrFilesToRead[1] >> arrReadableElement[1];
	}
	return 0;
}

int sort(const std::string& fileName)
{
	int p = 1;
	fstream file(fileName, fstream::in);
	fstream arrFilesToWrite[2] = {
		fstream("a.txt", fstream::out),
		fstream("b.txt", fstream::out),
	};

	for (int i = 0; i < 2; i++)
	{
		if (!file.is_open() || !arrFilesToWrite[i].is_open())
		{
			return -1;
		}
	}

	partitioningPhase(file, arrFilesToWrite, p);

	bool isSecondFileNotEmpty = notEmptyFile(arrFilesToWrite[1]);

	for (int i = 0; i < 2; i++)
	{
		arrFilesToWrite[i].close();
	}
	file.close();

	while(isSecondFileNotEmpty)
	{
			arrFilesToWrite[0].open("a.txt", fstream::in);
			arrFilesToWrite[1].open("b.txt", fstream::in);

			fstream arrFilesToRead[2] = {
				fstream("c.txt", fstream::out),
				fstream("d.txt", fstream::out),
			};

			mergePhase(arrFilesToWrite, arrFilesToRead, p);

			for (int i = 0; i < 2; i++)
			{
				arrFilesToWrite[i].close();
				arrFilesToRead[i].close();
			}

			p = 2 * p;

			arrFilesToRead[0].open("c.txt", fstream::in);
			arrFilesToRead[1].open("d.txt", fstream::in);

			arrFilesToWrite[0].open("a.txt", fstream::out);
			arrFilesToWrite[1].open("b.txt", fstream::out);

			mergePhase(arrFilesToRead, arrFilesToWrite, p);


			isSecondFileNotEmpty = notEmptyFile(arrFilesToWrite[1]);

			for (int i = 0; i < 2; i++)
			{
				arrFilesToWrite[i].close();
				arrFilesToRead[i].close();
			}
			p = 2 * p;
	}
}

bool isFileContainsSortedArray(const std::string& fileName)
{
	fstream file(fileName, fstream::in);
	if (!file.is_open())
	{
		return -1;
	}
	int a, b;
	file >> a;
	while (file >> b)
	{
		if (a > b)
		{
			return false;
		}
		a = b;
	}
	return true;

}

int createAndSortFile(const std::string& fileName, const int numbersCount, const int maxNumberValue)
{
	if (!createFileWithRandomNumbers(fileName, numbersCount, maxNumberValue)) {
		return -1;
	}

	sort(fileName); //Вызов вашей функции сортировки

	if (!isFileContainsSortedArray("a.txt")) {
		return -2;
	}

	return 1;
}