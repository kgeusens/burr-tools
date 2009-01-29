#ifndef __DISASSEMBLER_HASHES_H__
#define __DISASSEMBLER_HASHES_H__

class disassemblerNode_c;


/* this is a hashtable that stores nodes */
class nodeHash {

  private:

    unsigned long tab_size;
    unsigned long tab_entries;

    disassemblerNode_c ** tab;

  public:

    nodeHash(void);

    ~nodeHash(void);

    /* delete all nodes and empty table for new usage */
    void clear(bool reset = true);

    /* add a new node  returns true, if the given node has already been
     * in the table, false if the node is inserted
     */
    bool insert(disassemblerNode_c * n);

    /* check, if a node is in the map */
    bool contains(const disassemblerNode_c * n) const;
};



/* this is a hashtable that stores nodes but is also
 * alows traversal of all nodes added at a given point in
 * time, only one such traversal can be active at one time
 * and the nodes are scanned in the reverse order they
 * were added
 */
class countingNodeHash {

  private:

    unsigned long tab_size;
    unsigned long tab_entries;

    struct hashNode {
      disassemblerNode_c * dat;
      hashNode * next;
      hashNode * link;
    };

    hashNode ** tab;
    hashNode * linkStart;

    hashNode * scanPtr;
    bool scanActive;

  public:

    countingNodeHash(void);
    ~countingNodeHash(void);

    /* delete all nodes and empty table for new usage */
    void clear(bool reset = true);

    /* add a new node  returns true, if the given node has already been
     * in the table, false if the node is inserted
     */
    bool insert(disassemblerNode_c * n);

    /* with the following 2 functions it is possible to
     * scan through all nodes that are currently in the
     * hashhable, first you call initScan to start
     * it end then nextScan. This function returns one
     * node after the other until nothing is left and then
     * returns 0.
     * You can add new nodes to the hashtable while a scan
     * is running. The new nodes will not influence a running
     * scan, only the nodes that were present when calling initScan
     * will be returned.
     * The nodes will be returned in the revers order they were inserted
     */
    void initScan(void);
    const disassemblerNode_c * nextScan(void);
};

#endif

