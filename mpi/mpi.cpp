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
#include <ctime>
using namespace std;

class WordCounter {
public:
  int value;
  WordCounter() : value(0) {}

  void operator++(int) { value++; }
};

typedef struct frequency {
  int frequency;
  string doc_id;
  string title;
} frequency;

ostream &operator<<(ostream &st, WordCounter &wc) {
  return st << wc.value;
}

//Maps where all word freq are stored
map<string, map<string,frequency> > table0;
map<string, map<string, frequency> > table1;
map<string, map<string, frequency> > table2;

//Maps where top 10 of the word are stored
multimap<int, frequency, greater<int> > sorted0;
multimap<int, frequency, greater<int> > sorted1;
multimap<int, frequency, greater<int> > sorted2;

//Total top 10
multimap<int, frequency, greater<int> > table;

MPI_Comm comm = MPI_COMM_WORLD;
MPI_Status status;

int range;
int sumaTotal;

map<string, WordCounter> wordCount(const char *input) {
  map<string, WordCounter> counter;

  char *stateptr;
  char *tok = strtok_r((char *)input, " ", &stateptr);

  while (tok != NULL) {
    counter[tok]++;
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
    string index = "";
    string id = "";
    string title = "";
    string content = "";

    while (ip.good()) {

      #pragma omp critical
      {
        getline(ip, index, '\t');
        getline(ip, id, '\t');
        getline(ip, title, '\t');
        getline(ip, content, '\n');
      }

      map<string, WordCounter> counter = wordCount(content.c_str());
      map<string, WordCounter, less<string>>::iterator it;

      for (it = counter.begin(); it != counter.end(); it++) {
        struct frequency f;
        f.frequency = (*it).second.value;
        f.doc_id = id;
        f.title = title;

        if (range == 0) {
          #pragma omp critical
          {
            table0[(*it).first][id] = f;
          }
        } else if (range == 1) {
          #pragma omp critical
          {
            table1[(*it).first][id] = f;
          }
        } else if (range == 2) {
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

int sendTop10(multimap<int, frequency, greater<int> > sorted) {
  int j = 0;
  int freq = 0;
  string sortId = "";
  string sortTitle = "";
  multimap<int, frequency>::iterator s;
  for (s = sorted.begin(); s != sorted.end(); s++) {
    if (j < 10) {
      freq = (*s).first;
      sortId = (*s).second.doc_id;
      char sortedId[sortId.length() + 1];
      strcpy(sortedId, sortId.c_str());

      sortTitle = (*s).second.title;
      char sortedTitle[sortTitle.length() + 1];
      strcpy(sortedTitle, sortTitle.c_str());
      MPI_Send(&freq, 1, MPI_INT, 0, 0, comm);
      MPI_Send(&sortedId, 256, MPI_CHAR, 0, 0, comm);
      MPI_Send(&sortedTitle, 256, MPI_CHAR, 0, 0, comm);
    } else {
      break;
    }
    j++;
  }
  return 0;
}

int read(string search) {
  map<string, frequency> docs;
  if (range == 0) {
    cout << "RANK 0" << endl;
    docs = table0[search];
  } else if (range == 1) {
    cout << "RANK 1" << endl;
    docs = table1[search];
  } else if (range == 2) {
    cout << "RANK 2" << endl;
    docs = table2[search];
  }

  map<string, frequency, less<string>>::iterator i;
  int suma = 0;

  for (i = docs.begin(); i != docs.end(); i++) {
    if (range == 0) {
      sorted0.insert(pair<int, frequency>((*i).second.frequency, (*i).second));
    }
    else if (range == 1) {
      sorted1.insert(pair<int, frequency>((*i).second.frequency, (*i).second));
    }
    else if (range == 2) {
      sorted2.insert(pair<int, frequency>((*i).second.frequency, (*i).second));
    }
    suma += (*i).second.frequency;
  }
  sumaTotal += suma;

  if (range == 1) {
    sendTop10(sorted1);
    MPI_Send(&suma, 1, MPI_INT, 0, 0, comm);
  } else if (range == 2) {
    sendTop10(sorted2);
    MPI_Send(&suma, 1, MPI_INT, 0, 0, comm);
  }
  return 0;
}

int getTop10(multimap<int, frequency, greater<int> > table, int source){
  for (int i = 0; i < 10; i++) {
    int freq;
    char id[256];
    char title[256];
    MPI_Recv(&freq, 1, MPI_INT, source, 0, comm, &status);
    MPI_Recv(&id, 256, MPI_CHAR, source, 0, comm, &status);
    MPI_Recv(&title, 256, MPI_CHAR, source, 0, comm, &status);
    struct frequency f;
    f.frequency = freq;
    f.doc_id = id;
    f.title = title;
    table.insert(pair<int, frequency>(freq, f));
  }
  return 0;
}

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);
  int size, nameLen;
  char name[MPI_MAX_PROCESSOR_NAME];
  MPI_Comm_rank(comm, &range);
  MPI_Get_processor_name(name, &nameLen);
  MPI_Comm_size(comm, &size);
  
  cout << "Hello world from rank " << range << " running on " << name << endl;

  char *files[3];
  string file0 = "1.csv";
  string file1 = "2.csv";
  string file2 = "3.csv";
  files[0] = (char *)file0.c_str();
  files[1] = (char *)file1.c_str();
  files[2] = (char *)file2.c_str();

  if (range == 0) {
    cout << "MPI World Size = " << size << " processes" << endl;
  }
  unsigned t0, t1;
  t0 = clock();
  if (range == 0) {
    run(files[0]);
  } else if (range == 1) {
    run(files[1]);
  } else if (range == 2) {
    run(files[2]);
  }
  t1 = clock();
  double time = (double(t1 - t0) / CLOCKS_PER_SEC);
  cout << "Execution Time building inverted index = " << time << endl;

  // Wait until all processes are done.
  int len = 256;
  bool work = true;
  while (work) {
    if (range == 0) {
      string search;
      char word[search.length() + 1];
      cout << "Please enter a word to search (press / to exit): ";
      while (cin >> search) {
        if (search == "/") {
          work = false;
          strcpy(word, search.c_str());
          MPI_Send(&word, len, MPI_CHAR, 1, 0, comm);
          MPI_Send(&word, len, MPI_CHAR, 2, 0, comm);
          break;
        }
        transform(search.begin(), search.end(), search.begin(), ::tolower);
        strcpy(word, search.c_str());
        MPI_Send(&word, len, MPI_CHAR, 1, 0, comm);
        MPI_Send(&word, len, MPI_CHAR, 2, 0, comm);
        read(search);

        // Store top 10 of all sorted top 10.
        multimap<int, frequency>::iterator it;
        int j = 0;
        for (it = sorted0.begin(); it != sorted0.end(); it++) {
          if (j < 10) {
            table.insert(pair<int, frequency>((*it).first, (*it).second));
          } else {
            break;
          }
          j++;
        }
        getTop10(table, 1);
        int suma1;
        MPI_Recv(&suma1, 1, MPI_INT, 1, 0, comm, &status);
        sumaTotal += suma1;
        getTop10(table, 2);
        int suma2;
        MPI_Recv(&suma2, 1, MPI_INT, 2, 0, comm, &status);
        sumaTotal += suma2;
        j = 0;
        for (it = table.begin(); it != table.end(); it++) {
          if (j < 10) {
            cout << (*it).second.frequency << " "
                 << (*it).second.doc_id << " "
                 << (*it).second.title << endl;
          } else break;
          j++;
        }
        cout << "The word " << search << " is " << sumaTotal << " times in all news" << endl;
        sumaTotal = 0;
        // Clean maps
        sorted0.clear();
        table.clear();
        cout << "Please enter a word to search (press / to exit): ";
      }
      work = false;
    } else if (range == 1) {
      char inMsg[len];
      MPI_Recv(&inMsg, len, MPI_CHAR, 0, 0, comm, &status);
      //cout << "EL MENSAJE QUE LLEGO A 1 ES: " << inMsg << endl;
      if (strcmp(inMsg, "/") == 0) {
        break;
      }
      string search(inMsg);
      read(search);
      sorted1.clear();
    } else if (range == 2) {
      char inMsg[len];
      MPI_Recv(&inMsg, len, MPI_CHAR, 0, 0, comm, &status);
      //cout << "EL MENSAJE QUE LLEGO A 2 ES: " << inMsg << endl;
      if (strcmp(inMsg, "/") == 0) {
        break;
      }
      string search(inMsg);
      read(search);
      sorted2.clear();
    }
  }
  MPI_Finalize();
  return 0;
}
