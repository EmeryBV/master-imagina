#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <limits>

using namespace std;

void print_transactions(const vector<vector<int> >& transactions)
{
	for (size_t i = 0; i < transactions.size(); ++i)
	{
		for (size_t j = 0; j < transactions[i].size(); ++j)
		{
			cout << transactions[i][j] << " ";
		}
		cout << "\n";
	}
}

// int apriori-gen

int apriori(vector<vector<int> >& transactions, int theta)
{
	for (int i = 2; ; ++i)
	{
		for (vector<int> transaction : transactions)
		{
			for (int& candidate : transaction)
			{
				
			}
		}
	}
}

int main(int argc, char const *argv[])
{
	int i = 0;
	int minsup = numeric_limits<int>::max();
	int maxsup = numeric_limits<int>::min();
	
	string line;
	vector<vector<int> > transactions;

	while (getline(cin, line))
	{
		int candidate;
		vector<int> candidates;
		stringstream line_stream(line);

		while (line_stream >> candidate)
		{
			if (minsup > candidate)
				minsup = candidate;

			if (maxsup < candidate)
				maxsup = candidate;
			
			candidates.push_back(candidate);
		}

		transactions.push_back(candidates);
		
		i++;
	}

	cout << "minsup : " << minsup << ", maxsup : " << maxsup << "\n";
	print_transactions(transactions);

 	return 0;
}
