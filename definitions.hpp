#include<set>
#include<map>
#include<vector>


class Block{
public:
  static int count;
  int playerID;
  int blockID;
  int ID;
  std::set<Block *> * branches;
  Block * next;
  Block * prev;
  Block(int id);
  ~Block();
};


class RaceSet{
public:
  bool r;
  int size;
  std::map <Block *, float> * optgamma;
  Block * choose();
  int add(Block * n, Block * t,float gamma);
  int getLength();
  int numTied();
  int prune();
  RaceSet();
  ~RaceSet();
};



class Chain{
public:
  bool race;
  int length;
  Block * origin;
  Block * head;
  Block * tail;
  RaceSet * race_set;
  int mineBlock(int id);
  int appendBlock(Block * b, Block * target,float gamma);
  Chain();
  ~Chain();
};


class privateChain{
public:
  Block * head;
  Block * tail;
  Block * origin;
  int length;
  privateChain();  
  int mineBlock(int id);
  int clear();
  ~privateChain();


};

  
class Player{
public:
  int id;
  int mined;
  float hash;
  float gamma;
  privateChain * pchain;
  int (*strat)(Player * p, Chain * c);
  int strategy(Chain * c){
    return (*strat)(this,c);    
  }
  int mine_block(Chain * chain);
  Player(float hash, int id);
  ~Player();   
};







