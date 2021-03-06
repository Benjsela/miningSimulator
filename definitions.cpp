#include "definitions.hpp"
#include<stdio.h>
#include<stdlib.h>
#include<stack>



//

int RaceSet::numTied(){
  int num = 0;
  int len = this->getLength();
  //  return num;
 
  num = 0;
  std::map<Block *,float>::iterator it = optgamma->begin();
  while(it!=optgamma->end()){
    if(it->first->blockID==len){
      num++;
    }
    it++;
  }
  //   printf("numTied=%d\n",num);
  return num;

  
}




int RaceSet::getLength(){

  int max = 0;
  
  /*  return max;*/
  max = 0;
  
  std::map<Block *,float>::iterator it = optgamma->begin();
  while(it != optgamma->end()){
    int b =it->first->blockID; 
    if(max<b){
      max = b;
    }
    it++;
  }
  return max;

  
    
}





int RaceSet::prune(){

  int max = this->getLength();
  
  int i = 0;
  int length = optgamma->size();
   std::map<Block *,float>::iterator it = optgamma->begin();
  for(i=0;i<length;i++){
    int flag = 0;
    it = optgamma->begin();
    while(flag==0 && it!=optgamma->end()){

      if(it->first->blockID<max){

	 optgamma->erase(it->first);
        flag=1;
      }
      else{
         it++;
      }
    }
  }

  return 0;
}



RaceSet::RaceSet(){
  this->size = 0;
  this->optgamma = new std::map <Block *, float>();
}


int RaceSet::add(Block * n, Block * t,float gamma){ 
  std::map<Block *,float>::iterator it = optgamma->begin();
  while(it!=optgamma->end()){
    if(it->first==t){
      optgamma->erase(it);
      optgamma->insert(std::pair <Block *,float> (n,gamma));
      return 0;
    }
    it++;
  }
  optgamma->insert(std::pair <Block *,float> (n,gamma));
  return 0;
}


RaceSet::~RaceSet(){
  this->optgamma->clear();
  delete this->optgamma;
}



Block * RaceSet::choose(){
  float r = ((float)rand())/((float)RAND_MAX);
  
  float total2 = 0;
  std::map<Block *,float>::iterator it = this->optgamma->begin();
  while(it != optgamma->end()){
    total2 = total2 + it->second;
    it++;
  }
  std::map<Block *,float>::iterator it2 = this->optgamma->begin();
  float curr2 = 0;
  while(it2 != optgamma->end()){
    curr2 = curr2 + (it2->second/total2);
    if(r<=curr2){
      /* if(it2->first->blockID==this->getLength()){*/
         return it2->first;
    }
    else{
      it2++;
    }
  }
  return NULL;
}




int Player::mine_block(Chain * chain){  
  this->pchain->mineBlock(this->id); 
  return 0;
}


int Block::count = 0;




Block::Block(int id){
  this->blockID = count;
  this->ID = count;
  this->count++;
  this->playerID = id;
  this->prev = NULL;
  this->next = NULL;
  std::set<Block *> * b = new std::set<Block *>();
  this->branches = b;
}





Block::~Block(){
  this->branches->clear();
  delete this->branches;
}



int Chain::appendBlock(Block * b, Block * target,float gamma){

  if(this->length==0){/*if chain is empty*/
    this->head = b;
    this->tail = b;
    b->prev = NULL;
    this->length++;
   
    b->blockID = 0;
    this->race_set->add(b,NULL,gamma);
  }
  else{

    b->prev = target;

    b->blockID = target->blockID + 1;

    target->next = b;
    target->branches->insert(b);
    this->race_set->add(b,target,gamma);
    if(this->tail==target){
      this->tail=this->tail->next;
      this->length++;	
    }
    b->next = NULL;
  
  }
  return 0;
}



Chain::Chain(){
  this->race = false;
  this->length = 0;
  this->origin = NULL;
  this->head = NULL;
  this->tail = NULL;
  RaceSet * r = new RaceSet();
  this->race_set = r;
}


Chain::~Chain(){
  /*                Block * curr = this->race_set->choose();
   while(curr!=NULL && curr->prev!=NULL){
   if(curr->prev!=NULL){
    curr = curr->prev;
    delete curr->next;
   }
   }
  */
  std::stack<Block *> * myStack = new std::stack<Block *>();
  std::set<Block *> * mySet = NULL;
 
  Block * curr = this->head;
  if(curr!=NULL){
    myStack->push(curr);
    while(!(myStack->empty())){
      curr = myStack->top();
      myStack->pop();
      mySet = curr->branches;
      std::set<Block *>::iterator it = mySet->begin();
      while(it!=mySet->end()){
	myStack->push(*it);
	it++;
      }   
      delete curr;
    }
  }
  delete myStack;
  delete this->race_set;
}



int privateChain::mineBlock(int id){

  Block * b = new Block(id);
  
  if(this->head==NULL){
    this->head = b;
    this->tail = b;
    this->length++;
    if(this->origin!=NULL){
      b->blockID = this->origin->blockID + 1;
    }
    else{
      b->blockID = 0;
    }
  }
  else{
    this->tail->next = b;
    this->tail->branches->insert(b);
    b->prev = this->tail;
    b->blockID = this->tail->blockID + 1;
    this->tail = this->tail->next;
    this->length++;
  }
  return 0; 
}

int privateChain::releaseBlock(){
  



  return 0;
}



int privateChain::clear(){
  if(this->length!=0){
    Block * curr = this->head;
    Block * curr2 = curr->next;
    while(curr!=NULL){
      curr2 = curr->next;
      delete curr;
      curr = curr2;
    }
    this->tail = NULL;
    this->head = NULL;
    this->length=0;
  }
  return 0;
}


Player::~Player(){
    this->pchain->clear();
     delete this->pchain;
  
  //   delete this;
}


Player::Player(float hash, int id){
  this->hash = hash;
  this->id = id;
  privateChain * pc = new privateChain();
  this->pchain = pc;
  this->gamma = 1000;
  this->mined = 0;
}


privateChain::privateChain(){
  this->head = NULL;
  this->tail = NULL;
  this->origin = NULL;
  this->length = 0;
}



privateChain::~privateChain(){
  if(this->length!=0){
    Block * curr = this->head;
    Block * curr2 = curr->next;
    while(curr!=NULL){
      curr2 = curr->next;
      delete curr;
      curr = curr2;
    }
    this->tail = NULL;
    this->head = NULL;
    this->origin = NULL;
    this->length = 0;
  }
  
}








