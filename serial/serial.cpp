#include <iostream>
#include <fstream>

#include <sstream>
#include <map>
#include <string>
 
using namespace std;

//Headers
int wordCount(const char * input);
 
class WordCounter {
public:
    int value;
    WordCounter() : value( 0 ) {}
     
    void operator++ (int) { value++; }
};
 
 
ostream& operator<<(ostream& st, WordCounter& wc) {
    return st << wc.value;
}
 
int main() {
    const string path = "/Users/ronald/Documents/Project2HPC/serial/salida.csv";
    ifstream ip(path);
    if(!ip.is_open()) {
        cout << "ERROR: File open" << '\n';
        return 0;
    }

    string header;
    string index;
    string id;
    string title;
    string content;
    getline(ip, header);
    
    //while(ip.good()) {
        getline(ip, index, '\t');
        getline(ip, id,     '\t');
        getline(ip, title,  '\t');
        getline(ip, content,'\n');
        wordCount(content.c_str());
    //}

    ip.close();
    return 0;
}

int wordCount(const char * input) {
    map<string, WordCounter> counter;
    
    char * tok = strtok((char *) input, " ");

    while (tok != NULL) {
        counter[tok]++;
        printf ("%s\n", tok);
        tok = strtok (NULL, " ");
    }
    
    /**map< string, WordCounter,less<string> >::iterator it;
 
    for ( it  = counter.begin();
          it != counter.end();
          it++ )
    {
        cout << (*it).first
             << ", "
             << (*it).second
             << endl;
    }*/
    return 0; 
}