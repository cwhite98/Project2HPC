#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <map>
#include <string>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <cstdio>
#include "mpi.h"

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

    ip.clear();
    ip.seekg(0,ios::beg);
   
    string header;
    getline(ip, header);

    string index;
    string id;
    string title;
    string content;
    //map<string, map<string, frecuency>> table;
    
    //for(int i = 1; i<numLines;i++) {     
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

  MPI_Init(&argc,&argv);
  int rank, size, nameLen;
  char name[MPI_MAX_PROCESSOR_NAME];
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm, &rank);
  MPI_Get_processor_name(name, &nameLen);
  MPI_Comm_size(comm, &size);
  cout << "Hello world from rank" << rank << "running on" << name << endl;
  char* files[3];  
  string file0 = "1.csv";                                                                                                                           
  string file1 = "2.csv";                                                                                                                           
  string file2 = "3.csv";
  files[0]=(char*)file0.c_str();                                                                                                                    
  files[1]=(char*)file1.c_str();                                                                                                                    
  files[2]=(char*)file2.c_str();
if (rank==0) {
    cout << "MPI World Size = " << size << "processes" << endl;
    //}
    cout << "holi" << endl;
    //    const int nt = omp_get_max_threads();
    //printf("%d",nt);
    //    cout << endl;
    //char* files[3];
    // string file0 = "1.csv";
    //string file1 = "2.csv";
    //string file2 = "3.csv";
   //files = {"1.csv", "2.csv", "3.csv"};
   //files[0]=(char*)file0.c_str();
   //files[1]=(char*)file1.c_str();
   //files[2]=(char*)file2.c_str();
   //int a = 1;
   //MPI_Scatter(files,a,MPI_CHAR,files,a,MPI_CHAR,0,comm); 
  }
 if (rank==0){
      run(files[0]);
    } else if (rank==1) {
      run(files[1]);
    } else if (rank==2) {
      run(files[2]);
    }
    if(rank==0){
	read();
    }
     MPI_Finalize();  
     /*
    for (int i = 0; i < 3; ++i) {
      numLines = 0;
      run(files[i]);
      }*/

      //read();
    return 0;
}
