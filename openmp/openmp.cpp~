#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <map>
#include <string>
#include <cctype>
#include <algorithm>
#include <omp.h>
#include <cstdio>

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

int numLines = 0;

map<string, WordCounter> wordCount(const char *input) {
    map<string, WordCounter> counter;

    char *tok = strtok((char *)input, " ");

    while (tok != NULL) {
        counter[tok]++;
        //printf("%s\n", tok);
        tok = strtok(NULL, " ");
    }

    /**map< string, WordCounter,less<string> >::iterator it;
 
    for ( it  = counter.begin(); it != counter.end(); it++ ) {
        cout << (*it).first
             << ", "
             << (*it).second
             << endl;
    }*/
    return counter;
}

int run(const string path) {
    //const string path = "/Users/camilawhite/Documents/Universidad/Semestre7/TopicosTelematica/apps/Project2HPC/serialersidad/
    cout << path << endl;
    ifstream ip(path);
    if (!ip.is_open()) {
        cout << "ERROR: File open" << '\n';
        return 0;
    }
    string unused;
    while (getline(ip,unused)) {                                                                                                                 

      ++numLines;                                                                                                                                

    }     
 
    ip.seekg(0,ios::beg);
   
    string header;
    string index;
    string id;
    string title;
    string content;
    getline(ip, header);
    //map<string, map<string, frecuency>> table;
    
    cout << numLines << endl;
   for(int i = 1; i<numLines;i++)
  
{     
        getline(ip, index, '\t');
        getline(ip, id, '\t');
        getline(ip, title, '\t');
        getline(ip, content, '\n');

        map<string, WordCounter> counter = wordCount(content.c_str());
        map<string, WordCounter, less<string> >::iterator it;

        for (it = counter.begin(); it != counter.end(); it++) {
            struct frecuency f;
            f.frecuency = (*it).second.value;
            f.doc_id = id;
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

        for (i = docs.begin(); i != docs.end(); i++) {

            suma += (*i).second.frecuency;

	    /*  cout << (*i).second.frecuency
                 << "   "
                 << (*i).second.doc_id
                 << "   "
                 << (*i).second.title
                 << endl;
	    */
        }

        cout << "The word " << search << " is " << suma << " times in all news" << endl;
        cout << "Please enter a word to search: ";
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
    files[0]="1.csv";
    files[1]="2.csv";
    files[2]="3.csv";

    for (int i = 0; i < 3; ++i) {
      numLines = 0;
      run(files[i]);
    }

    read();
    return 0;
}
