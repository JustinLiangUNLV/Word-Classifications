#include "hashMap.cpp"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

int main()
{

    int t; // amount of test cases
    cin >> t; // read into temp variable t
    cin.ignore(); // ignore after that

    for(int i = 1; i <= t; i++) // while it is reading all test cases
    {
        vector<string> category; // create a vector for the categories of each test case
        hashMap<string, string> keys; // create a hash map for keywords and return the category name
        hashMap<string, int> occurences; // hash map to tally up how many times the abstract had a keyword relating to the specific category
        hashMap<string, bool> duplicates; // hash map to hold duplicates and has default value of false
        hashMap<string, int> categoryMax; // holds 'p' for each category and that is what it is compared to
        string n; // name of category
        string keyWords; // temp variable to hold keywords within a category
        int c; // amount of categories
        int w; // amount of words within each category
        int p; // has to be exactly p in order for the abstract to fit this category
        
        cout << "Test Case " << i << endl; // output test case number
        
        cin >> c; // read in the amount of categories
        for(int i = 0; i < c; i++) // for loop the categories
        {
            cin >> n >> w >> p; // populate temp variables of name of category, amount of keywords, and categoryMax to see if abstract can be categorized
            category.push_back(n); // push the name of category into a vector
            categoryMax[n] = p; // populate each categories 'p'
            for(int j = 0; j < w; j++) // another for loop to populate keywords
            {
                cin >> keyWords; // read keywords
                for(int i = 0; i < keyWords.length(); i++) // for loop to lowercase everthing
                {
                    if(keyWords[i] >= 'A' && keyWords[i] <= 'Z') // while the characters within string are capital
                    {
                        keyWords[i] = keyWords[i] + 32; // lowercase them
                    }
                }
                keys[keyWords] = n; // push the keywords into the hash map that holds keywords
            }
        }

        cin .ignore();
        string abstract; // create a temp string to read the entire abstract
        while(getline(cin, abstract) && !abstract.empty()) // while reading each line and it doesn't hit a blank
        {
            stringstream abstractObject(abstract); // create a stringstream object called abstractObject
            string word; // temp word variable
            while(abstractObject >> word) // from the entire line read each word
            {
                for(int i = 0; i < word.length(); i++) // for loop again to lowercase each word being read
                {
                    if(word[i] >= 'A' && word[i] <= 'Z')
                    {
                        word[i] = word[i] + 32;
                    }
                }
                if(keys[word] != "") // if we find that word within our keys hashmap
                {
                    if(!duplicates[word]) // we check it is a duplicate
                    {
                        string tempCategory = keys[word]; // create a temp string variable to hold the category that was returned
                        occurences[tempCategory]++; // push that string into another hashmap called occurences to tally up the amount of keywords it has found, exlcuding duplicates
                        duplicates[word] = true; // change that specific word to have a value of true, this avoids duplicates
                    }
                }
            }
        }

        bool classified = false; // abstract classification is initially false
        for (const auto& categoryName : category)  // loop through category 
        {
            int count = occurences[categoryName]; // return how many times that category has tallied up itself
            if (count == categoryMax[categoryName]) // if that count is equal to the category's 'p'
            {
                cout << categoryName << endl; // output the name of it
                classified = true; // change this boolean to true
            }
        }

        if(!classified) // if it fails to classify the abstract as anything
        {
            cout << "SQF Problem" << endl; // output SQF Problem
        }
        
    }
    return 0;
    
}