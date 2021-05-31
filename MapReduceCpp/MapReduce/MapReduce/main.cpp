/*
                            FRE 7831 - Homework #3
                            ----------------------
 Deniz Kural / Net id: dk3703
 
 MapReduce Algorithm
 
 */


#include <iostream>
#include <thread>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;


vector<string> WordList;
map<string, int> ResultTable;

const int no_of_threads = 3;

class Parser {
public:
    void operator()(char delimiter,string str)
    {
        string word;
        istringstream wordStream(str);
        while (getline(wordStream, word, delimiter))
            WordList.push_back(word);
    }
};

class Counter
{
public:
    void operator()(map<string, int> *pResultTable, int start,int input_size)
    {
        while (WordList.size() != input_size);
        for (int i = start; i < start + input_size/no_of_threads; i++)
        {
            int count = (*pResultTable)[string(WordList[i])];
            count = count + 1;
            (*pResultTable)[string(WordList[i])] = count;
        }
    }
};

class WordCounter
{
private:
    map<string, int> ResultTables[no_of_threads];
public:
    void Map(string str)
    {
        thread parser(Parser(), ' ',str);
        parser.join();
        
    }
    void Reduce(int input_size)
    {
        int increment= floor(input_size/no_of_threads);
        thread counter1(Counter(), &ResultTables[0], 0,input_size);
        thread counter2(Counter(), &ResultTables[1], increment,input_size);
        thread counter3(Counter(), &ResultTables[2], increment*2,input_size);
        
        counter1.join();
        counter2.join();
        counter3.join();

        for (int i = 0; i < no_of_threads; i++)
        {
            for (map<string, int>::iterator itr = ResultTables[i].begin();
                itr != ResultTables[i].end(); itr++)
                ResultTable[itr->first] += itr->second;
        }
    }
};

int main()
{
    string line;
    ifstream fin;
    ofstream fout("output.txt");
    fin.open("4727.txt");
    getline(fin,line);
    cout<<line<<endl;

    std::for_each(line.begin(), line.end(), [](char & c) {
        c = ::tolower(c);
    });
    cout<<line<<endl;
    
    WordCounter wordcounter;

    wordcounter.Map(line);
    int word_size= WordList.size();
    cout<<"Total world in the given text:"<<word_size<<endl;
    wordcounter.Reduce(word_size);

    for (map<string, int>::iterator itr = ResultTable.begin(); itr != ResultTable.end(); itr++){
        cout << itr->first << "    " << itr->second << endl;
        fout << itr->first << "    " << itr->second << endl;
    }
    
    fin.close();
    fout.close();
    return 0;
}


