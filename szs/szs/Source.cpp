#include <fstream>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <future>
#include <algorithm>
#include <boost/container/flat_set.hpp>
#include <boost/algorithm/string.hpp>
#include "Structure.cpp"

using namespace std;

bool search(vector<string>&, string&, boost::container::flat_set<string>&);
void r(const vector<vector<char>>&, string&, const int&, const int&, Structure&, vector<vector<bool>>&, boost::container::flat_set<string>&);
void getNeighbors(const vector<vector<char>>&, const int&, const int&, vector<pair<int, int>>&, vector<vector<bool>>&);

int main() {
		SetConsoleOutputCP(1251);
		SetConsoleCP(1251);
		ifstream in("dic2.txt");
		cout << "Dictionary opened - " << boolalpha << in.is_open() << endl;
		Structure dictionary = Structure();
		string s;
		while (!in.eof())
		{
			in >> s;
			//dictionary.emplace(s);
			dictionary.putWord(s);
		}

		cout << "Dictionary loaded - " << dictionary.getSize() << endl;
		in.close();

		//input matrix
		in.open("input.txt");
		getline(in, s);
		cout << s;
		vector<vector<char>> matrix(5, vector<char>(5));
		int x = 0;
		for (auto& i : matrix)
			for (auto& j : i)
				j = s[x++];


		vector<future<boost::container::flat_set<string>>> futures;
		boost::container::flat_set<string> words;
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 5; j++)
				futures.emplace_back(async(launch::async, [&matrix, i, j, &dictionary]() {
					boost::container::flat_set<string> result;
					string s = "";
					vector<vector<bool>> ready(5, vector<bool>(5, false));
					s.push_back(matrix[i][j]);
					ready[i][j] = true;
					r(matrix, s, i, j, dictionary, ready, result);
					return result;
					}));

		for (auto& f : futures)words.merge(f.get());
		vector<string> result;
		for (auto& i : words)result.emplace_back(i);
		sort(result.begin(), result.end(), [](const string& left, const string& right) {return left.size() > right.size(); });
		cout << result.size() << endl;
		ofstream out("result.txt");
		for (auto& i : result)out << i << endl;
		out.close();
}

bool search(Structure & dictionary, string& s, boost::container::flat_set<string>& words)
{
	Node* srh = dictionary.checkWord(s);
	if (s.size() == srh->level + 1 && srh->isEndWord)
		words.emplace(s);
	return srh->nextLevelWords == 0;
}

void r(const vector<vector<char>>& matrix, string& str, const int& x, const int& y, Structure& dictionary, vector<vector<bool>>& ready, boost::container::flat_set<string>& words)
{
	if (search(dictionary, str, words))return;
	if (str.size() == 25)return;
	vector<pair<int, int>> neighbors;
	getNeighbors(matrix, x, y, neighbors, ready);
	if (neighbors.size() == 0)return;
	for (int i = 0; i < neighbors.size(); i++)
	{
		str.push_back(matrix[neighbors[i].first][neighbors[i].second]);
		ready[neighbors[i].first][neighbors[i].second] = true;
		r(matrix, str, neighbors[i].first, neighbors[i].second, dictionary, ready, words);
		str.resize(str.size() - 1);
		ready[neighbors[i].first][neighbors[i].second] = false;
	}
}

void getNeighbors(const vector<vector<char>>& matrix, const int& x, const int& y, vector<pair<int, int>>& neighbors, vector<vector<bool>>& ready)
{
	if (x == 0 || y == 0 || x == 4 || y == 4)
	{
		if (x == 0 && y == 0)
		{
			if (ready[0][1] == false)neighbors.push_back(pair<int, int>{0, 1});
			if (ready[1][0] == false)neighbors.push_back(pair<int, int>{1, 0});
			if (ready[1][1] == false)neighbors.push_back(pair<int, int>{1, 1});
		}
		else if (x == 4 && y == 4)
		{
			if (ready[3][4] == false)neighbors.push_back(pair<int, int>{3, 4});
			if (ready[4][3] == false)neighbors.push_back(pair<int, int>{4, 3});
			if (ready[3][3] == false)neighbors.push_back(pair<int, int>{3, 3});
		}
		else if (x == 4 && y == 0)
		{
			if (ready[3][0] == false)neighbors.push_back(pair<int, int>{3, 0});
			if (ready[4][1] == false)neighbors.push_back(pair<int, int>{4, 1});
			if (ready[3][1] == false)neighbors.push_back(pair<int, int>{3, 1});
		}
		else if (x == 0 && y == 4)
		{
			if (ready[0][3] == false)neighbors.push_back(pair<int, int>{0, 3});
			if (ready[1][4] == false)neighbors.push_back(pair<int, int>{1, 4});
			if (ready[1][3] == false)neighbors.push_back(pair<int, int>{1, 3});
		}
		else if (x == 0)
		{
			if (ready[0][y + 1] == false)neighbors.push_back(pair<int, int>{0, y + 1});
			if (ready[0][y - 1] == false)neighbors.push_back(pair<int, int>{0, y - 1});
			if (ready[1][y] == false)neighbors.push_back(pair<int, int>{1, y});
			if (ready[1][y + 1] == false)neighbors.push_back(pair<int, int>{1, y + 1});
			if (ready[1][y - 1] == false)neighbors.push_back(pair<int, int>{1, y - 1});
		}
		else if (x == 4)
		{
			if (ready[4][y + 1] == false)neighbors.push_back(pair<int, int>{4, y + 1});
			if (ready[4][y - 1] == false)neighbors.push_back(pair<int, int>{4, y - 1});
			if (ready[3][y] == false)neighbors.push_back(pair<int, int>{3, y});
			if (ready[3][y + 1] == false)neighbors.push_back(pair<int, int>{3, y + 1});
			if (ready[3][y - 1] == false)neighbors.push_back(pair<int, int>{3, y - 1});
		}
		else if (y == 0)
		{
			if (ready[x - 1][0] == false)neighbors.push_back(pair<int, int>{x - 1, 0});
			if (ready[x + 1][0] == false)neighbors.push_back(pair<int, int>{x + 1, 0});
			if (ready[x][1] == false)neighbors.push_back(pair<int, int>{x, 1});
			if (ready[x + 1][1] == false)neighbors.push_back(pair<int, int>{x + 1, 1});
			if (ready[x - 1][1] == false)neighbors.push_back(pair<int, int>{x - 1, 1});
		}
		else if (y == 4)
		{
			if (ready[x - 1][4] == false)neighbors.push_back(pair<int, int>{x - 1, 4});
			if (ready[x + 1][4] == false)neighbors.push_back(pair<int, int>{x + 1, 4});
			if (ready[x][3] == false)neighbors.push_back(pair<int, int>{x, 3});
			if (ready[x + 1][3] == false)neighbors.push_back(pair<int, int>{x + 1, 3});
			if (ready[x - 1][3] == false)neighbors.push_back(pair<int, int>{x - 1, 3});
		}
		else cout << "Troubles...neighbors";
	}
	else
	{
		if (ready[x + 1][y] == false)neighbors.push_back(pair<int, int>{x + 1, y});
		if (ready[x - 1][y] == false)neighbors.push_back(pair<int, int>{x - 1, y});
		if (ready[x][y + 1] == false)neighbors.push_back(pair<int, int>{x, y + 1});
		if (ready[x][y - 1] == false)neighbors.push_back(pair<int, int>{x, y - 1});
		if (ready[x + 1][y - 1] == false)neighbors.push_back(pair<int, int>{x + 1, y - 1});
		if (ready[x + 1][y + 1] == false)neighbors.push_back(pair<int, int>{x + 1, y + 1});
		if (ready[x - 1][y - 1] == false)neighbors.push_back(pair<int, int>{x - 1, y - 1});
		if (ready[x - 1][y + 1] == false)neighbors.push_back(pair<int, int>{x - 1, y + 1});
	}
}