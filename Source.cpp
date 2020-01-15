#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <iomanip>
#include <Windows.h>
#include <thread>
#include <set>
#include <future>
#include <boost/container/flat_set.hpp>
#include <chrono>

using namespace std;

bool start_with(string&, string&);
bool search(vector<string>&, string&, boost::container::flat_set<string>&);
void r(const vector<vector<char>> &, string&, const int &, const int &, vector<string>&, vector<vector<bool>>&, boost::container::flat_set<string>&);
void getNeighbors(const vector<vector<char>>&, const int&, const int&, vector<pair<int, int>>&, vector<vector<bool>>&);

int main() {
	auto sta = chrono::system_clock::now();
	boost::container::flat_set<string> words;
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	ifstream in("dic.txt");
	ifstream input("input.txt");
	ofstream out("out.txt");
	vector<string> dictionary;
	while (!in.eof())
	{
		string s;
		in >> s;
		dictionary.emplace_back(s);
	}
	in.close();
	vector<vector<char>> matrix(5, vector<char>(5));
	for (auto& i : matrix)
		for (auto& j : i)
			input >> j;
	input.close();

	//////////////////////////////////////////////
	vector<future<boost::container::flat_set<string>>> futures;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			futures.emplace_back(async(launch::deferred | launch::async, [&matrix, i = i, j = j, & dictionary]() {
				boost::container::flat_set<string> result;
				string s = "";
				vector<vector<bool>> ready(5, vector<bool>(5, 0));
				s += matrix[i][j];
				ready[i][j] = true;
				r(matrix, s, i, j, dictionary, ready, result);
				return result;
				}));
		}
	}
	for (auto& f : futures)words.merge(f.get());
	vector<string> s;
	for (auto &  i : words)s.emplace_back(i);
	sort(s.begin(), s.end(), [](const string& left, const string& right) {return left.size() > right.size(); });
	cout << s.size() << endl;
	for (auto i : s)cout << i << endl;
	auto fin = chrono::system_clock::now();
	cout << chrono::duration_cast<std::chrono::milliseconds>(fin - sta).count();
	return 0;
}

bool start_with(string& i, string& s)
{
	if (i.size() > s.size())return false;
	for (int j = 0; j < i.size(); j++)if (i[j] != s[j])return false;
	return true;
}

bool search(vector<string>& dictionary, string& s, boost::container::flat_set<string>& words)
{
	bool start = false, finish = false;
	for (auto & i : dictionary) {
		if (i == s) {
			words.emplace(s);
			return false;
		}
		start = start_with(s, i);
		if (!finish && start)finish = true;
	}
	if (finish)return false;
	else return true;
}

void r(const vector<vector<char>>& matrix, string& str, const int &  x, const int & y, vector<string> & dictionary, vector<vector<bool>>& ready, boost::container::flat_set<string>& words)
{
	if (search(dictionary, str, words))return;
	if (str.size() == 25)return;
	vector<pair<int, int>> neighbors;
	getNeighbors(matrix, x, y, neighbors, ready);
	if (neighbors.size() == 0)return;
	for (int i = 0; i < neighbors.size(); i++)
	{
		str += matrix[neighbors[i].first][neighbors[i].second];
		ready[neighbors[i].first][neighbors[i].second] = true;
		r(matrix, str, neighbors[i].first, neighbors[i].second, dictionary, ready, words);
		str.resize(str.size() - 1);
		ready[neighbors[i].first][neighbors[i].second] = false;
	}
}

void getNeighbors(const vector<vector<char>>& matrix, const int& x , const int& y, vector<pair<int, int>>& neighbors, vector<vector<bool>> & ready)
{
	if (x == 0 || y == 0 || x == 4 || y == 4)
	{
		if (x == 0 && y == 0)
		{
			if(ready[0][1] == false)neighbors.push_back(pair<int, int>{0, 1});
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
			if(ready[0][3] == false)neighbors.push_back(pair<int, int>{0, 3});
			if(ready[1][4] == false)neighbors.push_back(pair<int, int>{1, 4});
			if(ready[1][3] == false)neighbors.push_back(pair<int, int>{1, 3});
		}
		else if (x == 0)
		{
			if(ready[0][y + 1] == false)neighbors.push_back(pair<int, int>{0, y + 1});
			if(ready[0][y - 1] == false)neighbors.push_back(pair<int, int>{0, y - 1});
			if(ready[1][y] == false)neighbors.push_back(pair<int, int>{1, y});
			if(ready[1][y + 1] == false)neighbors.push_back(pair<int, int>{1, y + 1});
			if(ready[1][y - 1] == false)neighbors.push_back(pair<int, int>{1, y - 1});
		}
		else if (x == 4)
		{
			if(ready[4][y + 1] == false)neighbors.push_back(pair<int, int>{4, y + 1});
			if(ready[4][y - 1] == false)neighbors.push_back(pair<int, int>{4, y - 1});
			if(ready[3][y] == false)neighbors.push_back(pair<int, int>{3, y});
			if(ready[3][y + 1] == false)neighbors.push_back(pair<int, int>{3, y + 1});
			if(ready[3][y - 1] == false)neighbors.push_back(pair<int, int>{3, y - 1});
		}
		else if (y == 0)
		{
			if(ready[x - 1][0] == false)neighbors.push_back(pair<int, int>{x - 1, 0});
			if(ready[x + 1][0] == false)neighbors.push_back(pair<int, int>{x + 1, 0});
			if(ready[x][1] == false)neighbors.push_back(pair<int, int>{x, 1});
			if(ready[x + 1][1] == false)neighbors.push_back(pair<int, int>{x + 1, 1});
			if(ready[x - 1][1] == false)neighbors.push_back(pair<int, int>{x - 1, 1});
		}
		else if (y == 4)
		{
			if(ready[x - 1][4] == false)neighbors.push_back(pair<int, int>{x - 1, 4});
			if(ready[x + 1][4] == false)neighbors.push_back(pair<int, int>{x + 1, 4});
			if(ready[x][3] == false)neighbors.push_back(pair<int, int>{x, 3});
			if(ready[x + 1][3] == false)neighbors.push_back(pair<int, int>{x + 1, 3});
			if(ready[x - 1][3] == false)neighbors.push_back(pair<int, int>{x - 1, 3});
		}
		else cout << "Troubles...neighbors";
	}
	else
	{
		if(ready[x + 1][y] == false)neighbors.push_back(pair<int, int>{x + 1, y});
		if(ready[x - 1][y] == false)neighbors.push_back(pair<int, int>{x - 1, y});
		if(ready[x][y + 1] == false)neighbors.push_back(pair<int, int>{x, y + 1});
		if(ready[x][y - 1] == false)neighbors.push_back(pair<int, int>{x, y - 1});
		if(ready[x + 1][y - 1] == false)neighbors.push_back(pair<int, int>{x + 1, y - 1});
		if(ready[x + 1][y + 1] == false)neighbors.push_back(pair<int, int>{x + 1, y + 1});
		if(ready[x - 1][y - 1] == false)neighbors.push_back(pair<int, int>{x - 1, y - 1});
		if(ready[x - 1][y + 1] == false)neighbors.push_back(pair<int, int>{x - 1, y + 1});
	}
}
