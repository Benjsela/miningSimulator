#include "engine.hpp"
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>


/*sets up and plays a game and returns fraction mined by each player*/
float * game0(int num_players, int num_rounds,float * powers,int (*strategy[])(Player * p, Chain * c)){

  Chain * chain = new Chain();
  Block * gen = new Block(-1);
  chain->appendBlock(gen,NULL,1);
  //  Player * list[] =  {p1,p2,p3};
  Player ** list = new  Player*[num_players];
  for(int i=0;i<num_players;i++){
    list[i] = new Player(powers[i],i+1);
    list[i]->pchain->origin = gen;
    list[i]->strat = strategy[i];
  }
  
  int total = rounds(num_players,num_rounds,list,chain);
  float * results = new float[num_players];
  for(int i=0;i<num_players;i++){
    results[i] = (((float)(list[i]->mined))/((float)total));
  }
  for(int j=0;j<num_players;j++){
    delete (list[j]); 
  }
  
  delete [] list;
  
  delete chain;  
  return results;
}

/*2 selfish miners, first selfish miner size is fixed and second selfish miner size is varied*/
int oneDimPlot(int num_rounds, float resolution, float a){
  int (*h)(Player * p, Chain * c) = honestMine;
  int (*s)(Player * p, Chain * c) = selfish;
  int (*s2)(Player * p, Chain * c) = pub_n;
  float b = 0;
  float honest = 1-a-b;
  float inc = .01;
  
   std::ofstream myfile;
   myfile.open("test2.txt");
  for(int i=0;i<50;i++){
     printf("i=%d\r",i);
     std::cout.flush();
     b = b+inc;
     honest = 1-a-b;
     int (*strategy[])(Player * p, Chain * c) = {h,s,s};
     float sizes[3] = {honest,a,b};
     float * results = game0(3,num_rounds,sizes,strategy);
     float rewards = results[1];
     int rewards2 = (int)(1000*rewards);
     rewards = ((float)rewards2)/((float)1000);
     std::ostringstream ss;
     std::ostringstream ss2;
    ss << b;
    std::string s(ss.str());
    ss2 << rewards;
    std::string s2(ss2.str());
    std::string k = s+","+s2;
    myfile << k+"\n";
  }
  
  return 0;
}

int twoDimPlot(int num_rounds){
  int (*h)(Player * p, Chain * c) = honestMine;
  int (*s)(Player * p, Chain * c) = selfish;
  int (*s2)(Player * p, Chain * c) = pub_n;
  float a=.18;
  float b=.18;
  float honest = 1-a-b;
  float inc = .001;
 
  int (*strategy[])(Player * p, Chain * c) = {h,s,s};

  
  std::ofstream myfile;
  myfile.open("test3.txt");
  for(int i=180;i<330;i++){
    float percent = (100*((float)(i-180))/((float)150));
    printf("%f%%\r",percent);
    std::cout.flush();
    a = .18; 
    for(int j=180;j<330;j++){
     
      honest = 1-a-b;
      float sizes[3] = {honest,a,b};
      float * results = game0(3,num_rounds,sizes,strategy);
      float rewards = results[1];
      float rewards2 = results[2];
      // printf("rewards = %f\n",rewards);
      //  printf("rewards2 = %f\n",rewards2);
      // int r = (int)(1000*rewards);
      // int r2 = (int)(1000*rewards2);
      // rewards = ((float)r)/((float)1000);
      // rewards2 = ((float)r2)/((float)1000);
      
      // printf("r=%f\n",rewards);
      // printf("r2=%f\n",rewards2);
      
      if(rewards>a && rewards2>b){
	rewards = 1;
      }
      else if(rewards>a && rewards2<b){
	rewards = 2;
      }
      else if(rewards<a && rewards2>b){
	rewards = 3;
      }
      else{
	rewards = 0;
      }
      
      delete [] results;
      
      std::ostringstream ss;
      std::ostringstream ss2;
      ss<<rewards;
      std::string s(ss.str());
      std::string k = s+",";
      myfile<<k;
      a = a + inc;
    }
    myfile<<"\n";
     b = b+inc;
  }
  

  return 0;

}

int twoDimPlot_v2(int num_rounds){
  int (*h)(Player * p, Chain * c) = honestMine;
  int (*s)(Player * p, Chain * c) = selfish;
  int (*s2)(Player * p, Chain * c) = pub_n;
  float a=.18;
  float b=.18;
  float honest = 1-a-b;
  float inc = .001;
 
  int (*strategy[])(Player * p, Chain * c) = {h,s,s};

  std::ofstream selfish1;
  std::ofstream selfish2;
  selfish1.open("s1.txt");
  selfish2.open("s2.txt");
 
  for(int i=180;i<330;i++){
    float percent = (100*((float)(i-180))/((float)150));
    printf("%f%%\r",percent);
    std::cout.flush();
    a = .18; 
    for(int j=180;j<330;j++){
     
      honest = 1-a-b;
      float sizes[3] = {honest,a,b};
      float * results = game0(3,num_rounds,sizes,strategy);
      float rewards = results[1];
      float rewards2 = results[2];
      // printf("rewards = %f\n",rewards);
      //  printf("rewards2 = %f\n",rewards2);
      // int r = (int)(1000*rewards);
      // int r2 = (int)(1000*rewards2);
      // rewards = ((float)r)/((float)1000);
      // rewards2 = ((float)r2)/((float)1000);
      
      // printf("r=%f\n",rewards);
      // printf("r2=%f\n",rewards2);

      std::ostringstream aa;
      std::ostringstream aa2;
      aa<<rewards2;
      std::string k1(aa.str());
      std::string k2 = k1+",";
      selfish2<<k2;
     
      
      delete [] results;
      
      std::ostringstream ss;
      std::ostringstream ss2;
      ss<<rewards;
      std::string s(ss.str());
      std::string k = s+",";
      selfish1<<k;
      a = a + inc;
    }
    selfish1<<"\n";
    selfish2<<"\n";
    b = b+inc;
  }
  return 0;
}




int PNE(){



  return 0;
}






int main(){

  int num_players = 3;
  int num_rounds = 5000;
  float a = .25;
  float b = .25;
  float honest = 1-a-b;
  float sizes[3] = {honest,a,b};
  int (*h)(Player * p, Chain * c) = honestMine;
  int (*s)(Player * p, Chain * c) = selfish;
  int (*ss)(Player * p, Chain * c) = pub_n;
  a=2;
  //  f([a](int n, Player * p, Chain * c)->int {return pub_n(a,p,c);});
  
  // int (*ss2)(Player * p, Chain * c) = (*ss)(2,_1,_1);
  int (*strategy[])(Player * p, Chain * c) = {h,s,s};
  float * results = game0(num_players,num_rounds,sizes,strategy);
  printf("h1: %f\n",results[0]);

}





