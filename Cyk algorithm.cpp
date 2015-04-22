// Cyk algorithm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <utility>
#include <iostream>
using namespace std;

const int NMAX = 50;

map<string, vector<char> > reverseCNF;
vector<tuple<char /*letter*/, tuple<int, int, int> /*leftSon*/, tuple<int, int, int> /*rightSon*/> > Cyk[NMAX][NMAX];
ifstream in("cyk.txt");
ofstream out("cykRes.txt");
string word;

void Decode(int x, int y, int cellElem)
{
	if (x == 0 || y == 0)
	{
		out << "\"" << word[cellElem - 1] << "\"";
		return;
	}
	int lx, ly, li, rx, ry, ri;
	lx = get<0>(get<1>(Cyk[x][y][cellElem]));
	ly = get<1>(get<1>(Cyk[x][y][cellElem]));
	li = get<2>(get<1>(Cyk[x][y][cellElem]));

	rx = get<0>(get<2>(Cyk[x][y][cellElem]));
	ry = get<1>(get<2>(Cyk[x][y][cellElem]));
	ri = get<2>(get<2>(Cyk[x][y][cellElem]));

	char letter = get<0>(Cyk[x][y][cellElem]);

	//out << letter << '\n';
	out << "\"" << letter << "\" : {\n";
	Decode(lx, ly, li);
	out << ",\n";
	Decode(rx, ry, ri);
	out << "\n}";
}

int _tmain(int argc, _TCHAR* argv[])
{


	string to;
	char from;
	in >> word;
	int N = static_cast<int>(word.size());
	while (in >> from >> to)
	{
		reverseCNF[to].push_back(from);
	}


	for (int wordIndex = 1; wordIndex <= N; wordIndex++)
	{
		string letter = "";
		letter += word[wordIndex - 1];

		for (int revState = 0; revState < reverseCNF[letter].size(); revState++)
		{
			Cyk[N][wordIndex].push_back(make_tuple(reverseCNF[letter][revState], make_tuple(0, 0, wordIndex), make_tuple(0, 0, wordIndex)));
		}
	}


	for (int row = N - 1; row >= 1; row--)
	{
		for (int column = 1; column <= row; column++)
		{
			for (int level = 1; row + level <= N; level++)
			{
				for (int cellElem = 0; cellElem < Cyk[row + level][column].size(); cellElem++)
				{
					//for every neighbour
					for (int neighbourElem = 0; neighbourElem < Cyk[N - level + 1][column + (N - level + 1 - row)].size(); neighbourElem++)
					{
						string concLetters = "";
						char letter1 = get<0>(Cyk[row + level][column][cellElem]);
						char letter2 = get<0>(Cyk[N - level + 1][column + (N - level + 1 - row)][neighbourElem]);
						concLetters += letter1;
						concLetters += letter2;

						for (int revState = 0; revState < reverseCNF[concLetters].size(); revState++)
						{
							Cyk[row][column].push_back(make_tuple(reverseCNF[concLetters][revState], make_tuple(row + level, column, cellElem), make_tuple(N - level + 1, column + (N - level + 1 - row), neighbourElem)));

						}
					}
				}
			}
		}

	}

	for (int cellElem = 0; cellElem < Cyk[1][1].size(); cellElem++)
	{
		char letter1 = get<0>(Cyk[1][1][cellElem]);
		if (letter1 == 'S')
		{
			out << "Possible!\n";
			out << "\"S\":{ ";


			int lx, ly, li, rx, ry, ri;
			lx = get<0>(get<1>(Cyk[1][1][cellElem]));
			ly = get<1>(get<1>(Cyk[1][1][cellElem]));
			li = get<2>(get<1>(Cyk[1][1][cellElem]));

			rx = get<0>(get<2>(Cyk[1][1][cellElem]));
			ry = get<1>(get<2>(Cyk[1][1][cellElem]));
			ri = get<2>(get<2>(Cyk[1][1][cellElem]));

			Decode(lx, ly, li);
			out << ",";
			Decode(rx, ry, ri);
			out << "}\n\n\n";

			break;
		}
	}

	for (int row = 1; row <= N; row++)
	{
		for (int column = 1; column <= N; column++)
		{
			out << "< ";
			for (int cellElement = 0; cellElement < Cyk[row][column].size(); cellElement++)
				out << "{" << get<0>(Cyk[row][column][cellElement]) << ", "
				<< get<0>(get<1>(Cyk[row][column][cellElement])) << ":" << get<1>(get<1>(Cyk[row][column][cellElement])) << ":" << get<2>(get<1>(Cyk[row][column][cellElement])) << ", "
				<< get<0>(get<2>(Cyk[row][column][cellElement])) << ":" << get<1>(get<2>(Cyk[row][column][cellElement])) << ":" << get<2>(get<2>(Cyk[row][column][cellElement])) << "}";
			out << "> ";
		}
		out << endl;
	}

	return 0;
}