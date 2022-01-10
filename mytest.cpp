#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <random>
#include "hash.h"
enum RANDOM {NONE,UNIFORM,NORMAL};

default_random_engine defEngine;
std::binomial_distribution<int> m_normdist_f(DISKMIN,DISKMAX);
std::uniform_int_distribution<int> m_unidist_f(DISKMIN,DISKMAX);


class Tester{ // Tester class to implement test functions

    private:

    hash_fn  hash;
    int filename_size;
    HashTable *test_table;
    vector<File> container;
    vector<unsigned int> hashes;


    std::string gen_random(const int len)
    {
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        std::string tmp_s;
        tmp_s.reserve(len);

        for (int i = 0; i < len; ++i)
            tmp_s += alphanum[rand()%63];

        return tmp_s;
}
    void set_Fsize(int x){filename_size = x;}
    void create_table(unsigned int size)
    {
        test_table = new HashTable(size,hash);
    }
    void delete_table()
    {
        container.clear();
        hashes.clear();
        test_table->~HashTable();
        test_table = nullptr;
        
    }
    
    void insert_elements(unsigned int count,RANDOM type,bool non_col=false,bool verbose=false)
    {
        if(test_table==nullptr) create_table(count);

        string filename;
        unsigned int key;
        
        for(unsigned int i=0;i<count;i++)
        {
            if(non_col || hashes.size()==0 )filename = gen_random(filename_size + (rand()%5));
            else filename = hashes[0];

            if(non_col)
            {
                key = hash(filename);
                for(auto j=0;j<hashes.size();j++)
                {
                    if(hashes[j]==key)
                    {
                        filename = gen_random(filename_size + (rand()%5));
                        key = hash(filename);
                        j=-1;
                    }
                }
            }
            
            if(type==UNIFORM)
            {
                File temp(filename,m_unidist_f(defEngine));
                if(test_table->insert(temp)) {
                    hashes.push_back(key);
                    container.push_back(temp);
                    if(verbose) cout<<key<<"\tInserted\t"<<temp<<"\n";
                    
                }
                else if(verbose)cout<<key<<"\tInsertion Failed\t"<<temp<<"\n";
            }
            else if(type==NORMAL)
            {
                File temp(filename,m_normdist_f(defEngine));
                if(test_table->insert(temp)) {
                    hashes.push_back(key);
                    container.push_back(temp);
                    if(verbose) cout<<key<<"\tInserted\t"<<temp<<"\n";
                    
                }
                else if(verbose)cout<<key<<"\tInsertion Failed\t"<<temp<<"\n";
            }
        }

    }
    void remove_elements(unsigned int count,bool verbose=false)
    {
        for(auto i=0;i<count;i++)
        {
            int j = rand()%container.size();
            test_table->remove(container[j]);
            container.erase(container.begin()+j);
        }
    }

    public:

    Tester(hash_fn h)
    {
        hash=h;
        filename_size;
        test_table = nullptr;

    }

    void test_insertion_1(bool is_noncol, bool verbose)
    {
        cout<<"\n*******\tTEST INSERTION 1 ON HASHTABLE\t*******\n\n";

        set_Fsize(5);
        int insertion_size = rand()%47;

        cout<<"\n\tInserting "<<insertion_size<<" Uniformly distributed files\n\n";
        create_table(insertion_size);
        insert_elements(insertion_size,UNIFORM,is_noncol,verbose);
        delete_table();

        cout<<"\n\tInserting "<<insertion_size<<" Normally distributed files\n\n";
        create_table(insertion_size);
        insert_elements(insertion_size,NORMAL,is_noncol,verbose);
        delete_table();
        cout<<"*******\tINSERTION TEST SUCCESSFUL\t*******\n\n";
    }

    void test_insertion_2(bool is_noncol, bool verbose)
    {
        cout<<"\n*******\tTEST INSERTION 1 ON HASHTABLE\t*******\n\n";
        set_Fsize(7);
        int insertion_size = 50 + (rand()%MINPRIME);
        cout<<"\n\tInserting "<<insertion_size<<" Uniformly distributed files\n\n";
        create_table(MINPRIME);
        insert_elements(insertion_size,UNIFORM,is_noncol,verbose);
        delete_table();

        cout<<"\n\tInserting "<<insertion_size<<" Normally distributed files\n\n";
        create_table(MINPRIME);
        insert_elements(insertion_size,NORMAL,is_noncol,verbose);
        delete_table();
        cout<<"*******\tINSERTION TEST SUCCESSFUL\t*******\n\n";
    }

    void test_insertion_3()
    {
        cout<<"\n*******\tTEST INSERTION 3 ON HASHTABLE\t*******\n\n";
        set_Fsize(13);
        HashTable table(MAXPRIME,hash);
        
        string filename;
        for(int i=0;i<MAXPRIME+10;i++)
        {
            filename = gen_random(4);
            File temp(filename, DISKMIN +rand()%DISKMIN);
            table.insert(temp);
        }
        cout<<"\n*******\tINSERTION TEST SUCCESSFUL\t*******\n\n";

    }
    
    void test_deletion_1(bool is_noncol,bool verbose)
    {
        cout<<"\n*******\tTEST DELETION 1 ON HASHTABLE\t*******\n\n";

        set_Fsize(5);
        int insertion_size = rand()%47;
        int deletion_size = rand()%insertion_size;

        cout<<"\n\tInserting "<<insertion_size<<" Uniformly distributed files and Deleting "<<deletion_size<<" files\n\n";
        create_table(insertion_size);
        insert_elements(insertion_size,UNIFORM,is_noncol,verbose);
        remove_elements(deletion_size);
        delete_table();

        cout<<"\n\tInserting "<<insertion_size<<" NOrmally distributed files and Deleting "<<deletion_size<<" files\n\n";
        create_table(insertion_size);
        insert_elements(insertion_size,NORMAL,is_noncol,verbose);
        remove_elements(deletion_size);
        delete_table();
        cout<<"*******\tINSERTION TEST SUCCESSFUL\t*******\n\n";

    }

    void test_deletion_2(bool is_noncol,bool verbose)
    {
        cout<<"\n*******\tTEST DELETION 1 ON HASHTABLE\t*******\n\n";

        set_Fsize(5);
        int insertion_size = 50 + (rand()%MINPRIME);
        int deletion_size = rand()%insertion_size;

        cout<<"\n\tInserting "<<insertion_size<<" Uniformly distributed files and Deleting "<<deletion_size<<" files\n\n";
        create_table(MINPRIME);
        insert_elements(insertion_size,UNIFORM,is_noncol,verbose);
        remove_elements(deletion_size);
        delete_table();

        cout<<"\n\tInserting "<<insertion_size<<" NOrmally distributed files and Deleting "<<deletion_size<<" files\n\n";
        create_table(MINPRIME);
        insert_elements(insertion_size,NORMAL,is_noncol,verbose);
        remove_elements(deletion_size);
        delete_table();
        cout<<"*******\tINSERTION TEST SUCCESSFUL\t*******\n\n";

    }

    void test_empty_del()
    {
        cout<<"*******\tTESTING DELETION ON EMPTY TABLE\t*******\n\n";
        HashTable test(MINPRIME,hash);
        File temp(gen_random(4),DISKMAX-100);
        if(test.remove(temp)==false)
            cout<<"*******\t\tTEST SUCCESS\t\t*******\n\n";
        else cout<<"*******\t\tTEST FAILED\t\t*******\n\n";
    }
    

};


unsigned int hashCode(const string str) {
   unsigned int val = 0 ;
   const unsigned int thirtyThree = 33 ;  // magic number from textbook
   for (unsigned int i = 0 ; i < str.length(); i++)
      val = val * thirtyThree + str[i] ;
   return val ;
}

int main()
{
    
    Tester T(hashCode);
    srand(time(NULL));

    T.test_insertion_1(true,false);//Insert few non colliding keys without rehashing
    T.test_insertion_1(false,false);//Insert few colliding keys without rehashing
    T.test_insertion_2(false,false);//Insert few non colliding keys with rehashing
    T.test_insertion_2(false,false);//Insert few colliding keys with rehashing
    T.test_insertion_3();//Insert more than MAXPRIME files
    T.test_deletion_1(true,false);
    T.test_deletion_1(false,false);
    T.test_deletion_2(true,false);
    T.test_deletion_2(false,false);
    T.test_empty_del();//Delete when table is empty
    //Delete few keys without rehashing
    //Delete few keys with rehashing
    //Test GetFile

    return 0;
}