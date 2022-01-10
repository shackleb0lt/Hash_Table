# Hash Table Implementation

## Objectives

* To implement a hash table.
* Using quadratic probing to manage hash collisions.
* To implement an incremental re-hashing algorithm
* To lazy deletion in hash table

The application starts with a hash table of size MINPRIME. After certain criteria is reached  it will switch to another table larger table and transfer  all data nodes from the current table to the new one incrementally.  That is once the switching happens  it transfers 25% of the nodes in the old table and at every consecutive operation (insert/remove) It continues to transfer 25% more of the data from the old to table to the new table until all data is transferred.

Criteria for re-hashing
* After an insertion, if the load factor becomes greater than 0.5.
* After a deletion, if the number of deleted buckets is more than 80 percent of the total number of occupied buckets

During a rehashing process the deleted buckets will be removed from the system permanently. They will  not be transferred to the new table

Read instructions.pdf for detailed problem statement.

To compile the code,
```bash
make all
```

To run the program,
```bash
./run
```