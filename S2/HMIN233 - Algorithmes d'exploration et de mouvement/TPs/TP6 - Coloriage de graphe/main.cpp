#include <iostream> 
#include <vector>
#include <limits>

using namespace std;

void SA_move(vector<vector<size_t> >& voisins, vector<size_t>& init_config) {
	
	int best_cost = numeric_limits<int>::infinity();
	
	vector<size_t> best_config = init_config;
	vector<size_t> config = init_config;

	size_t i = 0; 
	bool accepted = false;

	while (i < min_neighbors(voisins)) {
		current_config = generate_neighbors(voisins);

		if (cost(current_config) <= cost(config) || random()) {
			accepted = true;
		}
		if (cost(current_config) < cost(best_config)) {
			best_config = current_config;
			best_cost = cost(current_config);
		}
		i++;
	}

	if (accepted)
		return best_config;
	else 
		return config;
}

int main(int argc, char const *argv[]) {
	
	string ignore;
	size_t nb_sommets = 0, nb_aretes = 0;

	cin >> ignore >> ignore >> nb_sommets >> nb_aretes;

	size_t sommet = 0, voisin = 0;
	vector<vector<size_t> > voisins(nb_sommets + 1);
	vector<size_t> colors(nb_sommets + 1);

	cout << "sommets, aretes : " << nb_sommets << ", " << nb_aretes << "\n";

	while (cin >> ignore >> sommet >> voisin) {
		voisins[sommet].push_back(voisin);
		voisins[voisin].push_back(sommet);
	}

	for (size_t i = 0; i < voisins.size(); ++i) {
		cout << i << " : ";
		for (size_t j = 0; j < voisins[i].size(); ++j) {
			cout << voisins[i][j] << " ";
		}
		cout << "\n";
	}

	for (size_t i = 1; i < colors.size(); ++i) {
		colors[i] = 1 + rand() % 15;
		cout << colors[i] << " ";
	}
	cout << "\n";

	return 0;
}