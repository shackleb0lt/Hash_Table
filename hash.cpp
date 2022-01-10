// CMSC 341 - Fall 2021 - Project 4
#include "hash.h"
#include "math.h"

HashTable::HashTable(unsigned size, hash_fn hash){

    m_hash = hash;
    m_rehash_step = -1;

    m_newTable = TABLE1;
    m_size1 = 0;
    m_numDeleted1 = 0;

    if(size>=MINPRIME && size<=MAXPRIME && isPrime(size)) m_capacity1 = size;
    else m_capacity1 = findNextPrime(size);

    m_table1 = new File[m_capacity1];

    for(int i=0;i<m_capacity1;i++) m_table1[i] = EMPTY;
    
    m_capacity2 = 0;
    m_size2 = 0;
    m_numDeleted2 = 0;
    m_table2 = nullptr;
}

HashTable::~HashTable(){
}

File HashTable::getFile(string name, unsigned int diskBlock){

    File target;
    target.setKey(name);
    target.setDiskBlock(diskBlock);

    File *old_tab;
    File *active_tab;

    unsigned int old_cap, old_sz;
    unsigned int active_cap, active_sz;
    
    get_param(active_tab, old_tab, active_cap, old_cap, active_sz, old_sz);

    unsigned int index;

    if(get_pos(active_tab,active_cap,name,index,target))
        return active_tab[index];

    else if(old_tab!=nullptr && get_pos(old_tab,old_cap,name,index,target))
        return old_tab[index];
    return EMPTY;
}

bool HashTable::insert(File file)
{
    if(file==EMPTY || file==DELETED) return false;
   
    File *old_tab;
    File *active_tab;

    bool flag = false;
    
    unsigned int index;
    unsigned int old_cap,old_sz;
    unsigned int active_cap,active_sz;

    get_param(active_tab,old_tab,active_cap,old_cap,active_sz,old_sz);
    
    if(active_sz==active_cap && active_cap==MAXPRIME) {
        // cout<<"Max_size reached\n";
        return false;
    }

    else if(old_tab!=nullptr && get_pos(old_tab,old_cap,file.key(),index,file)) flag=false;

    else if(get_pos(active_tab,active_cap,file.key(),index,file)==false)
    {
        // if(index>=0 && index<active_cap)
        // {
            active_tab[index]=file;
            if(m_newTable == TABLE1) m_size1++;
            else m_size2++;
            flag = true;
        // }
    }
    
    if(lambda(m_newTable)<0.5 && old_tab==nullptr) return true;
    
    if(lambda(m_newTable)>=0.5 && active_cap==MAXPRIME && old_tab==nullptr) return true;
    
    if(lambda(m_newTable)>=0.5 && m_rehash_step==-1 && old_tab==nullptr)
    {
        m_rehash_step=0;
        if(m_newTable==TABLE1)
        {
            m_size2 = 0 ;
            m_numDeleted2 = 0;
            m_capacity2 = findNextPrime((m_size1-m_numDeleted1)*4);
            m_table2 = new File[m_capacity2];
            m_newTable = TABLE2;
        } 
        else
        {
            m_size1 = 0 ;
            m_numDeleted1 = 0;
            m_capacity1 = findNextPrime((m_size2-m_numDeleted2)*4);
            m_table1 = new File[m_capacity1];
            m_newTable = TABLE1;
            
        }
    }

    transfer();
    
    return flag;
}

bool HashTable::remove(File file)
{
    if(file==EMPTY || file==DELETED) return false;
    
    File *old_tab;
    File *active_tab;
    
    bool flag = false;
    
    unsigned int index;
    unsigned int old_cap,old_sz;
    unsigned int active_cap,active_sz;

    get_param(active_tab,old_tab,active_cap,old_cap,active_sz,old_sz);
    
    
    if(old_tab!=nullptr && get_pos(old_tab,old_cap,file.key(),index,file))
    {
        old_tab[index] = DELETED;
        flag=true;
    }
    
    else if(get_pos(active_tab,active_cap,file.key(),index,file))
    {
        active_tab[index] = DELETED;
        if(m_newTable == TABLE1) m_numDeleted1++;
        else m_numDeleted2++;
        flag=true;
    }

    if(deletedRatio(m_newTable)<0.8 && old_tab==nullptr) return true;

    if(deletedRatio(m_newTable)>=0.8 && m_rehash_step==-1 && old_tab==nullptr)
    {
        m_rehash_step=0;
        if(m_newTable==TABLE1)
        {
            m_size2 = 0 ;
            m_numDeleted2 = 0;
            m_capacity2 = findNextPrime((m_size1-m_numDeleted1)*4);
            m_table2 = new File[m_capacity2];
            m_newTable = TABLE2;
        } 
        else
        {
            m_size1 = 0 ;
            m_numDeleted1 = 0;
            m_capacity1 = findNextPrime((m_size2-m_numDeleted2)*4);
            m_table1 = new File[m_capacity1];
            m_newTable = TABLE1;
        }
    }

    transfer();

    return flag;
}

float HashTable::lambda(TABLENAME tablename) const {
    
    if(tablename==TABLE1) return ((float)m_size1/(float)m_capacity1);

    return ((float)m_size2/(float)m_capacity2);
}

float HashTable::deletedRatio(TABLENAME tableName) const {

    if(tableName==TABLE1) return ((float)m_numDeleted1/(float)m_size1);
    return ((float)m_numDeleted2/(float)m_size2);
}

void HashTable::dump() const {
    cout << "Dump for table 1: " << endl;
    if (m_table1 != nullptr)
        for (int i = 0; i < m_capacity1; i++) {
            cout << "[" << i << "] : " << m_table1[i] << endl;
        }
    cout << "Dump for table 2: " << endl;
    if (m_table2 != nullptr)
        for (int i = 0; i < m_capacity2; i++) {
            cout << "[" << i << "] : " << m_table2[i] << endl;
        }
}

bool HashTable::isPrime(int number){
    bool result = true;
    for (int i = 2; (i*i) <= number ; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

int HashTable::findNextPrime(int current){
    //we always stay within the range [MINPRIME-MAXPRIME]
    //the smallest prime starts at MINPRIME
    if (current < MINPRIME) current = MINPRIME-1;
    for (int i=current; i<MAXPRIME; i++) { 
        for (int j=2; j*j<=i; j++) {
            if (i % j == 0) 
                break;
            else if (j+1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    //if a user tries to go over MAXPRIME
    return MAXPRIME;
}

void HashTable::get_param(File * &active_table,File * &old_table,unsigned int &active_cap,unsigned int &old_cap,unsigned int &active_sz,unsigned int &old_sz)
{
    if(m_newTable==TABLE1)
    {
        active_table = m_table1;
        old_table = m_table2;
        active_cap = m_capacity1;
        old_cap = m_capacity2;
        active_sz = m_size1;
        old_sz = m_size2;
    }
    else
    {
        active_table = m_table2;
        old_table = m_table1;
        active_cap = m_capacity2;
        old_cap = m_capacity1;
        active_sz = m_size2;
        old_sz = m_size1;
    }

}

void HashTable::transfer()
{    
    File *old_tab;
    File *active_tab;
    
    unsigned int index;
    unsigned int old_cap,old_sz;
    unsigned int active_cap,active_sz;

    get_param(active_tab,old_tab,active_cap,old_cap,active_sz,old_sz);

    unsigned int l = (old_cap/4)*m_rehash_step;
    unsigned int r = l + old_cap/4;
    if(m_rehash_step==3) r=old_cap;

    for(int i=l;i<r;i++)
    {
        if (old_tab[i]==EMPTY || old_tab[i]==DELETED) continue;

        get_pos(active_tab,active_cap,old_tab[i].key(),index);

        active_tab[index] = old_tab[i];
        if(active_tab==m_table1)m_size1++;  
        else m_size2++;
    }
    
    m_rehash_step++;

    if(m_rehash_step==4)
    {
        m_rehash_step=-1;
        if(m_newTable==TABLE1)
        {
            delete[] m_table2;
            m_table2=nullptr;
            m_capacity2=0;
            m_size2=0;
            m_numDeleted2=0;
        }
        if(m_newTable==TABLE2)
        {
            delete[] m_table1;
            m_table1=nullptr;
            m_capacity1=0;
            m_size1=0;
            m_numDeleted1=0;
        }
    }
}

bool HashTable::get_pos(File *active_table,unsigned int capacity,string filename,unsigned int &index, File target)
{
    index = m_hash(filename)%capacity;
    
    unsigned int i=0;
    unsigned int pb_index=index;

    while (!(active_table[pb_index]==EMPTY))
    {
        if(!(target==EMPTY) && (active_table[pb_index]==target))
        {
            index=pb_index;
            return true;
        }
        i++;
        pb_index = (index +(i*i))%capacity;
        
    }
    index = pb_index;
    return false;   
}