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
#include <stdio.h>
#include "mpi.h"
#include <omp.h>

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

map<string, map<string, frecuency> > table0;
map<string, map<string, frecuency> > table1;
map<string, map<string, frecuency> > table2;

int range;

map<string, WordCounter> wordCount(const char *input) {
    map<string, WordCounter> counter;

    char* stateptr;
    char *tok = strtok_r((char *)input, " ", &stateptr);

    while (tok != NULL) {
      counter[tok]++;               
      tok = strtok_r(NULL, " ", &stateptr);
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
    
    //map<string, map<string, frecuency>> table;
    
    //for(int i = 1; i<numLines;i++) {     
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
            f.doc_id = id;
            f.title = title;
            
            if(range == 0){
	      #pragma omp critical
	      { 
                table0[(*it).first][id] = f;
              }
            }else if(range == 1){
	      #pragma omp critical
	      { 
                table1[(*it).first][id] = f;
              }
	    }else if(range == 2){
	      #pragma omp critical
	      {
		table2[(*it).first][id] = f;
	      }
	    }
	}   
    }
    
}
    ip.close();
    return 0;
}

int read(string search) {
  map<string, frecuency> docs;
  if(range == 0) {
    cout << "RANK 0" << endl;
    docs = table0[search];
  } else if(range == 1) {
    cout << "RANK 1" <<endl;
    docs = table1[search];
  } else if(range == 2) {
    cout << "RANK 2" <<endl;
    docs = table2[search];
  }
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
  return 0;
}

int main(int argc, char *argv[]) {
  MPI_Init(&argc,&argv);
  int size, nameLen;
  char name[MPI_MAX_PROCESSOR_NAME];
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm, &range);
  MPI_Get_processor_name(name, &nameLen);
  MPI_Comm_size(comm, &size);
  MPI_Status status;
  cout << "Hello world from rank" << range << "running on" << name << endl;
  
  char* files[3];  
  string file0 = "1.csv";                                                        
  string file1 = "2.csv";                                                         
  string file2 = "3.csv";
  files[0]=(char*)file0.c_str();                                                                                             
  files[1]=(char*)file1.c_str();                                                                                                 
  files[2]=(char*)file2.c_str();

  if (range == 0) {
    cout << "MPI World Size = " << size << "processes" << endl;
    cout << "holi" << endl;
  }

  if (range == 0){
    run(files[0]);
  } else if (range == 1) {
    run(files[1]);
  } else if (range == 2) {
    run(files[2]);
  } 
  
  int len = 256;
  bool work = true;
  while(work) {
    if(range == 0) { 
      string search;
      char word[search.length() + 1];
      cout << "Please enter a word to search: ";
      while(cin >> search) { 
	if(search == "/") {
	  work = false;
	  break;
	}
	transform(search.begin(), search.end(), search.begin(), ::tolower);
	strcpy(word, search.c_str());
	MPI_Send(&word, len, MPI_CHAR, 1, 0, comm);
	MPI_Send(&word, len, MPI_CHAR, 2, 0, comm);
	read(search);
	cout << "Please enter a word to search: ";
      }
    } else if (range == 1) {
      char inMsg[len];
      MPI_Recv(&inMsg, len, MPI_CHAR, 0, 0, comm, &status);
      //cout << "EL MENSAJE QUE LLEGO A 1 ES: " << inMsg << endl;
      string search(inMsg);
      read(search);
    } else if(range == 2) {
      char inMsg[len];
      MPI_Recv(&inMsg, len, MPI_CHAR, 0, 0, comm, &status);
      //cout << "EL MENSAJE QUE LLEGO A 2 ES: " << inMsg << endl;
      string search(inMsg);
      read(search);
    }  
  }
  MPI_Finalize();
  return 0;
}
