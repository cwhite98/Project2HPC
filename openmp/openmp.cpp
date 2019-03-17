#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <map>
#include <string>
#include <cctype>
#include <algorithm>
#include <cstdio>
#include <omp.h>
#include <ctime>
using namespace std;

class WordCounter {
public:
  int value;
  WordCounter() : value(0) {}

  void operator++(int) { value++; }
};

typedef struct frecuency {
  int frecuency;
  string doc_id;
  string title;
} frecuency;

ostream &operator<<(ostream &st, WordCounter &wc) {
  return st << wc.value;
}

map<string, map<string, frecuency> > table;

map<string, WordCounter> wordCount(const char *input) {
  map<string, WordCounter> counter;
  char* stateptr;
  char *tok = strtok_r((char *)input, " ", &stateptr);
  
  while (tok != NULL) {
    counter[tok]++;
    //  printf("%s\n", tok);
    tok = strtok_r(NULL, " ", &stateptr);
  }
  return counter;
}

int run(const string path) {
  cout << path << endl;
  ifstream ip(path);
  if (!ip.is_open()) {
    cout << "ERROR: File open" << '\n';
    return 0;
  }
  string header;
  getline(ip, header);
#pragma omp parallel
  {
    string index =   "";
    string id =      "";
    string title =   "";
    string content = "";

    while (ip.good()) {
    #pragma omp critical
    {
      getline(ip, index,   '\t');
      getline(ip, id,      '\t');
      getline(ip, title,   '\t');
      getline(ip, content, '\n');
    }
    
    map<string, WordCounter> counter = wordCount(content.c_str());
    map<string, WordCounter, less<string> >::iterator it;
    
    for (it = counter.begin(); it != counter.end(); it++) {
      struct frecuency f;
      f.frecuency = (*it).second.value;
      //f.doc_id = id;
      f.title = title;
      #pragma omp critical
      {
	table[(*it).first][id] = f;
      }
    }
    }
  }
  ip.close();
  return 0;
}

int read() {
  string search;
  cout << "Please enter a word to search (press / to exit): ";
  while (cin >> search) {
    if (search == "/")
      break;
    
    transform(search.begin(), search.end(), search.begin(), ::tolower);
    map<string, frecuency> docs = table[search];
    map<string, frecuency, less<string> >::iterator i;
    int suma = 0;
    multimap<int, frecuency, greater<int> > sorted;
    
    for (i = docs.begin(); i != docs.end(); i++) {
      (*i).second.doc_id = (*i).first;
      sorted.insert(pair<int, frecuency>((*i).second.frecuency, (*i).second));
      suma += (*i).second.frecuency;
    }
    
    multimap<int, frecuency>::iterator s;
    int j = 0;
    for(s = sorted.begin(); s != sorted.end(); s++) {
      if(j < 10) {
	cout << (*s).first
	     << "   "
	     << (*s).second.doc_id
	     << "   "
	     << (*s).second.title
	     << endl;
	j++;
      } else {
	break;
      }
    }
    cout << "The word " << search << " is " << suma << " times in all news" << endl;
    cout << "Please enter a word to search (press / to exit): ";
  }
  return 0;
}

int main(int argc, char *argv[]) {
  cout << "holi" << endl;
  //    const int nt = omp_get_max_threads();
  //printf("%d",nt);
  //    cout << endl;
  string files[3];
  //files = {"1.csv", "2.csv", "3.csv"};
  files[0]="../datasets/1.csv";
  files[1]="../datasets/2.csv";
  files[2]="../datasets/3.csv";
  unsigned t0, t1;
  t0 = clock();
  for (int i = 0; i < 3; ++i) {
    run(files[i]);
  }
  t1 = clock();
  double time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "Execution Time building inverted index = " << time << endl;
  read();
  return 0;
}
