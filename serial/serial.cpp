#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <map>
#include <string>
#include <cctype>
#include <algorithm>
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
    char *tok = strtok((char *)input, " ");
    while (tok != NULL) {
        counter[tok]++;
        //printf("%s\n", tok);
        tok = strtok(NULL, " ");
    }
    return counter;
}

int run(const string path) {
    ifstream ip(path);
    if (!ip.is_open()) {
        cout << "ERROR: File open" << '\n';
        return 0;
    }
    string header, index, id, title, content;
    getline(ip, header);

    while (ip.good()) {
        getline(ip, index, '\t');
        getline(ip, id, '\t');
        getline(ip, title, '\t');
        getline(ip, content, '\n');

        map<string, WordCounter> counter = wordCount(content.c_str());
        map<string, WordCounter, less<string> >::iterator it;

        for (it = counter.begin(); it != counter.end(); it++) {
            struct frecuency f;
            f.frecuency = (*it).second.value;
            //f.doc_id = id;
            f.title = title;

            table[(*it).first][id] = f;
        }
    }
    ip.close();
    return 0;
}

int read() {
    string search;
    cout << "Please enter a word to search: ";
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
        for(s = sorted.begin(); s != sorted.end(); s++) {
            cout << (*s).first
            << "   "
            << (*s).second.doc_id
            << "   "
            << (*s).second.title
            << endl;
        }
        cout << "The word " << search << " is " << suma << " times in all news" << endl;
        cout << "Please enter a word to search: ";
    }
    return 0;
}

int main(int argc, char *argv[]) {
    string files[3] = {"1.csv", "2.csv", "3.csv"};
    unsigned t0, t1;
    t0 = clock();
    for (int i = 0; i < 3; i++) {
        run(files[i]);
    }
    t1 = clock();
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Execution Time building inverted index = " << time << endl;
    read();
}