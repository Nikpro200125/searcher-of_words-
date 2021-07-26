#include <fstream>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <future>
#include <algorithm>
#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/algorithm/string.hpp>
#include "Structure.cpp"

using namespace std;

bool search(vector<string>&, string&, boost::container::flat_set<string>&, vector<pair<int, int>>&, boost::container::flat_map<string, vector<pair<int, int>>>&);
void r(const vector<vector<char>>&, string&, const int&, const int&, Structure&, vector<vector<bool>>&, boost::container::flat_set<string>&, vector<pair<int, int>>&, boost::container::flat_map<string, vector<pair<int, int>>>&);
void getNeighbors(const vector<vector<char>>&, const int&, const int&, vector<pair<int, int>>&, vector<vector<bool>>&);

int main() {
		SetConsoleOutputCP(1251);
		SetConsoleCP(1251);
		ifstream in("dictionary.txt");
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

		boost::container::flat_map<string, vector<pair<int, int>>> k;
		vector<future<boost::container::flat_map<string, vector<pair<int,int>>>>> futures;
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 5; j++)
				futures.emplace_back(async(launch::async, [&matrix, i, j, &dictionary]() {
					vector<pair<int, int>> kk;
					kk.push_back({ i,j });
					boost::container::flat_map<string, vector<pair<int, int>>> kkResult;
					boost::container::flat_set<string> result;
					string s = "";
					vector<vector<bool>> ready(5, vector<bool>(5, false));
					s.push_back(matrix[i][j]);
					ready[i][j] = true;
					r(matrix, s, i, j, dictionary, ready, result, kk, kkResult);
					return kkResult;
					}));

		for (auto& f : futures)k.merge(f.get());
		vector<vector<pair<int,int>>> result;
		for (auto& i : k)result.emplace_back(i.second);
		sort(result.begin(), result.end(), [](const vector<pair<int, int>>& left, const vector<pair<int, int>>& right) {return left.size() > right.size(); });
		cout << result.size() << endl;
		ofstream out("resultAuto.txt");
		for (auto& i : result) {
			for (auto& j : i)
				out << j.first << j.second;
			out << endl;
		}
		out.close();
}

bool search(Structure & dictionary, string& s, boost::container::flat_set<string>& words, vector<pair<int, int>>& kk, boost::container::flat_map<string, vector<pair<int, int>>>& kkResult)
{
	Node* srh = dictionary.checkWord(s);
	if (s.size() == srh->level + 1 && srh->isEndWord)
		if(words.emplace(s).second)kkResult.emplace(s, kk);
	return srh->nextLevelWords == 0;
}

void r(const vector<vector<char>>& matrix, string& str, const int& x, const int& y, Structure& dictionary, vector<vector<bool>>& ready, boost::container::flat_set<string>& words, vector<pair<int, int>>& kk, boost::container::flat_map<string, vector<pair<int, int>>>& kkResult)
{
	if (search(dictionary, str, words, kk, kkResult))return;
	if (str.size() == 25)return;
	vector<pair<int, int>> neighbors;
	getNeighbors(matrix, x, y, neighbors, ready);
	if (neighbors.size() == 0)return;
	for (int i = 0; i < neighbors.size(); i++)
	{
		str.push_back(matrix[neighbors[i].first][neighbors[i].second]);
		ready[neighbors[i].first][neighbors[i].second] = true;
		kk.push_back({ neighbors[i].first, neighbors[i].second });
		r(matrix, str, neighbors[i].first, neighbors[i].second, dictionary, ready, words, kk, kkResult);
		kk.pop_back();
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