// CMSC 341 - Fall 2021 - Project 4
#include "file.h"

File::File(string name, unsigned int diskBlock) {
    if ((diskBlock >= DISKMIN && diskBlock <= DISKMAX) ||
        (diskBlock == 0 && name == "DELETED")){
        // this is a normal or a DELETED object
        m_name = name;
        m_diskBlock = diskBlock;
    }
    else{
        // this is the empty object
        m_name = "";
        m_diskBlock = 0;
    }
}
string File::key() const {
    return m_name;
}
unsigned int File::diskBlock() const{
    return m_diskBlock;
}
void File::setKey(string key) {
    m_name = key;
}
void File::setDiskBlock(unsigned int diskBlock) {
    m_diskBlock = diskBlock;
}
// Overloaded insertion operator.  Prints file's name (key),
// and disk block pointer. This is a friend function in File class.
ostream& operator<<(ostream& sout, const File &file ) {
    if (!file.m_name.empty())
        sout << file.m_name << " (disk block " << file.m_diskBlock << ")";
    else
        sout << "";
  return sout;
}
// Overloaded equality operator. 
// To test inequality we may negate the results of this operator.
bool operator==(const File& lhs, const File& rhs){
    return ((lhs.m_name == rhs.m_name) && (lhs.m_diskBlock == rhs.m_diskBlock));
}
// Overloaded assignment operator
const File& File::operator=(const File& rhs){
    if (this != &rhs){
        m_name = rhs.m_name;
        m_diskBlock = rhs.m_diskBlock;
    }
    return *this;
}