#include <iostream>
#include <cstdio>
#include <sstream>
#include "Image.h"

using namespace std;

int main(int argc, char** argv) {

	if (argc < 2) {
		std::cerr << "Usage: <grey/color>\n";
		return 1;
	}

	string filetype(argv[1]);

	bool is_grey = (filetype == "grey");
	bool is_color = (filetype == "color");

	if (!is_grey && !is_color) {
		std::cerr << "Erreur : le programme ne peut prendre que 'grey' ou 'color' comme argument\n";
		return 1;
	}

	string tmp_name = tmpnam(nullptr);
	ofstream tmp_file(tmp_name);
	
	string line;

	while (getline(cin, line)) {
		tmp_file << line << "\n";
	}

	tmp_file.close();

	if (is_grey) {

		string command("gnuplot -p -e \"plot '" + tmp_name + "' using 2 title 'grey' with lines\"");
		system(command.c_str());

	} else {

		string command("gnuplot -p -e \"plot '" + tmp_name + "' using 2 title 'red' lt rgb \\\"red\\\" with lines, '" + tmp_name + "' using 3 title 'green' lt rgb \\\"green\\\" with lines, '" + tmp_name + "' using 4 title 'blue' lt rgb \\\"blue\\\" with lines\"");
		system(command.c_str());

	}

	remove(tmp_name.c_str());

  return 0;
}