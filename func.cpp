#include <stdio.h>
#include <stdlib.h>
#include<set>
#include<map>
#include<vector>
#include<iostream>
#include<fstream>
#include<iostream>


class Block{
public:
  static int count;
  int playerID;
  int blockID;
  int ID;
  Block * next;
  Block * prev;
  bool operator<(const Block & b);
  Block(int id);
};


class RaceSet{
public:
  bool r;
  int size;
  Block ** options;
  float prop [3];
  float prop2 [3];
  std::map <Block *, float> * optgamma;
  std::set<Block *> opts;
  Block * choose();
  int add(Block * n, Block * t,float gamma);
  int getLength();
  int numTied();
  int prune();
  RaceSet();
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
};

class Player{
public:
  int id;
  int mined;
  float hash;
  float gamma;
  Chain * chain;
  int (*strat)(Player * p, Chain * c);
  int strategy(Chain * c){
    return (*strat)(this,c);
    
  }
  int mine_block(Chain * chain);
  Player(float hash, int id);
   
};


bool Block::operator<(const Block & b1){
  return this->ID<b1.ID;
}

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
  int i = 0;
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

  
  for(i = 0;i<3;i++){
    if(this->options[i]->blockID>max){
      max = this->options[i]->blockID;
    }   
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
  this->options = new Block*[3];
  this->optgamma = new std::map <Block *, float>();
}


int RaceSet::add(Block * n, Block * t,float gamma){
  int i = 0;
  Block * curr=NULL;
  
 
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






Block * RaceSet::choose(){
  float r = ((float)rand())/((float)RAND_MAX);
  float total = 0;
  float curr = 0;
  int i = 0;
  Block * b = NULL;
  
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
  this->chain->mineBlock(this->id); 
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


int Chain::mineBlock(int id){

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

    b->prev = this->tail;
    b->blockID = this->tail->blockID + 1;
    this->tail = this->tail->next;
    this->length++;

  }
  return 0; 
}


Player::Player(float hash, int id){
  this->hash = hash;
  this->id = id;
  Chain * c = new Chain();
  this->chain = c;
  this->gamma = 1000;
  this->mined = 0;
}









int releaseBlock(Player * p, Chain * c){
  Block * n = p->chain->head;
  p->chain->head = p->chain->head->next;
  p->chain->length--;
  if(p->chain->length==0){
    p->chain->head=NULL;
    p->chain->tail=NULL;
  }
  c->appendBlock(n,n->prev,p->gamma);
  
  return 0;
}



/*cases: 
  1)
  2)
  3)
  4)
  5)            */
int selfish(Player * p, Chain * c){
   Block * f = p->chain->origin;
   int len = p->chain->length + f->blockID;
   int comp = c->race_set->getLength();
   int pub_len = p->chain->origin->blockID;
   //   printf("p->id = %d\n",p->id);
   // printf("publen = %d\n",pub_len);
   // printf("comp=%d\n",comp);
   if((len<comp||((p->chain->length==0) && ((p->chain->origin->playerID)!=(p->id))))){
     /*give up*/
     // printf("p: %d, h1\n",p->id);

     p->chain->origin = c->race_set->choose();
     p->chain->head = NULL;
     p->chain->tail = NULL;
     p->chain->length = 0;
     return 0;
   }
   else if(len==comp && len!=0 && p->chain->length!=0){/*race*/
     // printf("p: %d, h2\n",p->id);
     Block * b = p->chain->tail;
     Block * n = p->chain->head;
     
     n->prev = p->chain->origin;
     p->chain->length = 0;
     p->chain->head = NULL;
     p->chain->tail = NULL;
     p->chain->origin = b; 
     c->appendBlock(b,b->prev,p->gamma);

     return 1;
     /*release - len should equal 1 in this case*/
   }
   else if(c->race_set->numTied()>1 && len==(comp+1)){/*break tie by releasing*/
     // printf("p: %d, h3\n",p->id);
     
     Block * n = p->chain->tail;
     // printf("blockID released = %d\n",n->blockID);
     Block * n2 = p->chain->head;
     n2->prev = p->chain->origin;
     c->appendBlock(n,n->prev,p->gamma);
     p->chain->origin = n;
     p->chain->length=0;
     p->chain->tail=NULL;
     p->chain->head = NULL;
     // printf("got here\n");
     return 1;
   }
   else if(len==(comp+1) && pub_len<comp){/*release both blocks*/
     // printf("p: %d, h4\n",p->id);
      Block * n = p->chain->tail;
      Block * s = p->chain->head;
      s->prev = p->chain->origin;
      c->appendBlock(n,n->prev,p->gamma);
     
      p->chain->length=0;
      p->chain->tail=NULL;
      p->chain->head=NULL;
      p->chain->origin = n;

      return 1;
   }
   else if((len-comp)>=2 && pub_len<comp){/*publish 1 block - */
     // printf("p: %d, h5\n",p->id);
     Block * n = p->chain->head;
     // printf("blockID released = %d\n",n->blockID);
     p->chain->head = p->chain->head->next;
     p->chain->origin = n;
     p->chain->length--;
     c->appendBlock(n,f,p->gamma); 
     return 1;
   }
   else{/*do nothing*/
     // printf("do nothingllll\n");
     return 0;
   }
   
}




int honestMine(Player * p, Chain * c){
  
  int len = p->chain->length;
  if(len==0){
    p->chain->origin = c->race_set->choose();
    // printf("Honest ID chosen = %d\n",p->chain->origin->blockID);
    // printf("HonestMinenumTied = %d\n",c->race_set->numTied());
    /*do nothing*/
  
    return 0;
  }
  else{
  
    Chain * pChain = p->chain;
    Block * t = pChain->head;
    Block * target = pChain->origin;

    pChain->origin = pChain->head;
    pChain->head = pChain->head->next;  
    pChain->length--;
    c->appendBlock(t,target,p->gamma);
  
    return 1;
  }
  /**/
}



int states(Player ** players, int num_players, Chain * chain){
  int done = 1;
  Player * p;
     int i = 0;
  while(done!=0){
     done = 0;
  
     for(i=0;i<num_players;i++){
       p = players[i];
       done = done +  p->strategy(chain);
      
     }
     //  printf("done = %d\n",done);
  }
  
  chain->race_set->prune();
  // printf("chain is pruned\n");
  for(i=0;i<num_players;i++){
    p = players[i];
    p->strategy(chain);
  }
  // printf("done choosing\n");
  return 0;
}



int rounds(int num_players, int rounds,Player ** players ,Chain * chain){
  Player * p1 = players[0];
  Player * p2 = players[1];
  float h1 = p1->hash;
  float h2 = p2->hash;
  int i = 0;
  float vec[]={.1,.1,.7,.7,.7,.8,.8,.1,.1,.1};
  int interval = rounds/100;
   printf("interval = %d\n",interval);
  for(i=0;i<rounds;i++){
    if(interval==0||i%interval==0){
      printf("#");
      std::cout.flush(); 
    }
    int j = 0;
    float total = 0;
    Player * p;
    float r = ((float)rand())/((float)RAND_MAX);
    
    //r=vec[i];
  
    while(j<num_players){
      total = total + players[j]->hash;
      if(r<total){
        p = players[j]; 
	p->mine_block(chain);
	p->strategy(chain);
	j = num_players + 1;
      }
      j++;
    }
    states(players,num_players,chain); 
  }
  printf("\n");
  int total = 0;
  Block * curr = chain->race_set->choose();
  while(curr!=NULL){
    for(int i=0;i<num_players;i++){
      if(curr->playerID==i+1){
	players[i]->mined++;
        total++;
	//	printf("BlockID = %d, p%d\n",curr->blockID,players[i]->id);
      }
    }
    curr = curr->prev;
  }


   return total;
}




/*sets up and plays a game and returns fraction mined by each player*/
float * game0(int num_players, int num_rounds,float * powers,int (*strategy[])(Player * p, Chain * c)){

  Chain * chain = new Chain();
  Block * gen = new Block(-1);
  chain->appendBlock(gen,NULL,1); 
  Player ** list = new Player*[num_players];
  for(int i=0;i<num_players;i++){
    list[i] = new Player(powers[i],i+1);
    list[i]->chain->origin = gen;
    list[i]->strat = strategy[i];
  }
  
  int total = rounds(num_players,num_rounds,list,chain);
  float * results = new float[num_players];
  for(int i=0;i<num_players;i++){
    results[i] = ((float)list[i]->mined)/((float)total);
  }
  
  return results;
}

/*2 selfish miners, first selfish miner size is fixed and second selfish miner size is varied*/
int oneDimPlot(int num_rounds, float resolution, float a){
  int (*h)(Player * p, Chain * c) = honestMine;
  int (*s)(Player * p, Chain * c) = selfish;
  float b = 0;
  float honest = 1-a-b;
  float inc = .05;
  for(int i=0;i<30;i++){
    b = b+inc;
    honest = 1-a-b;
    int (*strategy[])(Player * p, Chain * c) = {h,s,s};
    float sizes[3] = {honest,a,b};
    game0(3,10000,sizes,strategy);
  }


}


int game1(){
  Block * gen = new Block(-1);
  Chain * chain = new Chain();
  chain->appendBlock(gen,NULL,1); 
  Player ** list = new Player*[3];
  float power[] = {.5,.25,.25};
  for(int i=0;i<3;i++){
     list[i] = new Player(power[i],i+1);
  }
  int (*h)(Player * p, Chain * c) = honestMine;
  int (*s)(Player * p, Chain * c) = selfish;
  
  list[0]->strat = h;
  list[1]->strat = s;
  list[2]->strat = s;
  int num_rounds = 10;
  int num_players = 3;

  std::ofstream myfile;
  myfile.open("test1.txt");
  

  for(int i=0;i<5;i++){
    for(int j=0;j<5;j++){
      int total = rounds(num_players,num_rounds,list,chain);
     
      for(int k=0;k<num_players;k++){


      }

    }
    myfile << "\n";
  }
   
  return 0;
}



  

int game2(){
  Chain * chain = new Chain();
  Player * p1 = new Player(.55,1);
  Player * p2 = new Player(.25,2);
  Player * p3 = new Player(.23,3);
  Player * p4 = new Player(.125,4);
  Player * p5 = new Player(.125,5);
  Player * p6 = new Player(.125,6);
  Block * gen = new Block(-1);
  chain->appendBlock(gen,NULL,1); 
  p1->chain->origin = gen;
  p2->chain->origin = gen;
  p3->chain->origin = gen;
  p4->chain->origin = gen;
  p5->chain->origin = gen;
  p6->chain->origin = gen;
  // p2->gamma=1;
  // p3->gamma=1;
  std::ofstream myfile;
  myfile.open("sdfdf.txt");
  myfile << "sdfd";
  myfile.close();
  
  int (*h)(Player * p, Chain * c) = honestMine;
  int (*s)(Player * p, Chain * c) = selfish;
  p1->strat = h;
  p2->strat = s;
  p3->strat = s;
  p4->strat = s;
  p5->strat = s;
  p6->strat = s;
  
  int num_players = 3;
  int num_rounds = 6000000;
  Player * arr[] = {p1,p2,p3};
  int total =  rounds(num_players,num_rounds,arr,chain);
  Block * curr = chain->race_set->choose();


  float * m = new float[num_players];
  while(curr!=NULL){
    for(int i=0;i<num_players;i++){
      if(curr->playerID==i+1){
	arr[i]->mined++;
	total++;
	//	printf("BlockID = %d, p%d\n",curr->blockID,arr[i]->id);
      }
    }
    curr = curr->prev;
  }

  
  for(int i=0;i<num_players;i++){
    float m1 = ((float)arr[i]->mined)/total;
    printf("p%d: %f\n",(i+1),m1);
  }
  return 0;
}



int game3(){
  Chain * c = new Chain();
  


  return 0;
}

int main(){
  game2();

}



