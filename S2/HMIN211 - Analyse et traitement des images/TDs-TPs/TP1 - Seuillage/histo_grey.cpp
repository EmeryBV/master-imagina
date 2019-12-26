#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include "ImageBase.h"

using namespace std;

int main(int argc, char **argv) {

  if (argc != 2) {
    cerr << "Usage : <STRING:image.pgm>\n";
    return 1;
  }

  ImagePGM image;
  image.load(argv[1]);

  map<int, int> grey_occurrences;

  for (int i = 0; i < image.height(); ++i){
    for (int j = 0; j < image.width(); ++j){
      grey_occurrences[image[i][j]]++;
    }
  }

  for (auto it = grey_occurrences.begin() ; it != grey_occurrences.end() ; ++it){
    cout << it->first << " " << it->second / (double)(image.height() * image.width()) << "\n";
  }

  return 0;
}
