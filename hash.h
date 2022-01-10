// CMSC 341 - Fall 2021 - Project 4
#ifndef _HASH_H
#define _HASH_H
#include <string>
#include <iostream>
#include "file.h"

using namespace std;

#define EMPTY File("",0)
#define DELETED File("DELETED", 0)
#define DELETEDKEY "DELETED"

enum TABLENAME {TABLE1, TABLE2};

const int MINPRIME = 101;   // Min size for hash table
const int MAXPRIME = 99991; // Max size for hash table

typedef unsigned int (*hash_fn)(string);
class Grader;
class Tester;
class HashTable {
    public:
    friend class Grader;
    friend class Tester;
    // Constructor,  requires table size and hash function.
    HashTable(unsigned size, hash_fn hash);
    // Destructor
    ~HashTable();
    // Returns table size; set by constructor
    unsigned int tableSize(TABLENAME tableName) const 
    { return ((tableName == TABLE1) ? m_capacity1 : m_capacity2); }
    // Returns number of entries in the table
    unsigned int numEntries(TABLENAME tableName) const 
    { return ((tableName == TABLE1) ? m_size1 : m_size2); }
    // Returns Load factor
    float lambda(TABLENAME tableName) const;
    // Returns the ratio of deleted slots
    float deletedRatio(TABLENAME tableName) const;
    bool insert(File file);
    bool remove(File file);
    File getFile(string name, unsigned int diskBlock);
    void dump() const;
    private:
    hash_fn         m_hash;         // hash function
    TABLENAME       m_newTable;     // the latest table created

    unsigned int    m_capacity1;     // hash table size
    unsigned int    m_size1;         // current number of entries in TABLE1
                                     // m_size1 includes deleted entries 
    unsigned int    m_numDeleted1;   // number of deleted entries in TABLE1
    File*           m_table1;        // hash table pointer to TABLE1

    unsigned int    m_capacity2;     // hash table size
    unsigned int    m_size2;         // current number of entries in TABLE2
                                     // m_size2 includes deleted entries
    unsigned int    m_numDeleted2;   // number of deleted entries in TABLE2
    File*           m_table2;        // hash table pointer to TABLE2

    int             m_rehash_step;   // Current rehash step, 
                                     // shows how much transfer is done

    //private helper functions
    bool isPrime(int number);
    int findNextPrime(int current);

    /******************************************
    * Private function declarations go here! *
    ******************************************/

   void get_param(File * &active_table,File * &old_table,unsigned int &active_cap,unsigned  int &old_cap,unsigned int &active_sz,unsigned int &old_sz);
   void transfer();
   bool get_pos(File *active_table,unsigned int capacity, string filename,unsigned int &index, File target=EMPTY);
};
#endif