#include<stdio.h>
#include<stdlib.h>
#include "strats.hpp"



int release(){
  return 0;
}



int publish_n(int n, Player * p, Chain * c){
  Block * f = p->pchain->origin;
  int len = p->pchain->length+f->blockID;
  int comp = c->race_set->getLength();
  int pub_len = p->pchain->origin->blockID;
  if(len<comp||(p->pchain->length==0)&&(p->pchain->origin->playerID!=p->id)){
    //give up
    p->pchain->clear();
    p->pchain->origin = c->race_set->choose();
    return 0;
  }
  else if(p->pchain->length>n){
    //release first block
    Block * n = p->pchain->head;
    Block * t = p->pchain->origin;
    n->prev = t;
    c->appendBlock(n,n->prev,p->gamma);
    return 1;
  }
 
  return 1;  
}


int pub_n(Player * p, Chain * c){
  Block * f = p->pchain->origin;
  int len = p->pchain->length+f->blockID;
  if(p->pchain->length>2){
    Block * n = p->pchain->head;
    Block * t = p->pchain->origin;
    p->pchain->head = n->next;
    p->pchain->origin = n;
    n->prev = t;
    n->branches->erase(n->next);
    p->pchain->length--;
    n->prev->branches->insert(n);
    c->appendBlock(n,n->prev,p->gamma);
    return 1;
  }
  else{
    int k = selfish(p,c);
    return k;
  }


}






int selfish(Player * p, Chain * c){
   Block * f = p->pchain->origin;
   int len = p->pchain->length + f->blockID;
   int comp = c->race_set->getLength();
   int pub_len = p->pchain->origin->blockID;
   //   printf("p->id = %d\n",p->id);
   // printf("publen = %d\n",pub_len);
   // printf("comp=%d\n",comp);
   if((len<comp||((p->pchain->length==0) && ((p->pchain->origin->playerID)!=(p->id))))){
     /*give up*/
     p->pchain->clear();
     p->pchain->origin = c->race_set->choose();
     return 0;
   }
   else if(len==comp && len!=0 && p->pchain->length!=0){/*race*/
     // printf("p: %d, h2\n",p->id);
     Block * b = p->pchain->tail;
     Block * n = p->pchain->head;
     
     n->prev = p->pchain->origin;
     p->pchain->origin->branches->insert(n);
     p->pchain->length = 0;
     p->pchain->head = NULL;
     p->pchain->tail = NULL;
     p->pchain->origin = b; 
     c->appendBlock(b,b->prev,p->gamma);

     return 1;
     /*release - len should equal 1 in this case*/
   }
   else if(c->race_set->numTied()>1 && len==(comp+1)){/*break tie by releasing*/
     // printf("p: %d, h3\n",p->id);
     
     Block * n = p->pchain->tail;
     // printf("blockID released = %d\n",n->blockID);
     Block * n2 = p->pchain->head;
     n2->prev = p->pchain->origin;
     p->pchain->origin->branches->insert(n2);
     c->appendBlock(n,n->prev,p->gamma);
     p->pchain->origin = n;
     p->pchain->length=0;
     p->pchain->tail=NULL;
     p->pchain->head = NULL;
     // printf("got here\n");
     return 1;
   }
   else if(len==(comp+1) && pub_len<comp){/*release both blocks*/
     // printf("p: %d, h4\n",p->id);
      Block * n = p->pchain->tail;
      Block * s = p->pchain->head;
      s->prev = p->pchain->origin;
      p->pchain->origin->branches->insert(s);
      c->appendBlock(n,n->prev,p->gamma);
     
      p->pchain->length=0;
      p->pchain->tail=NULL;
      p->pchain->head=NULL;
      p->pchain->origin = n;

      return 1;
   }
   else if((len-comp)>=2 && pub_len<comp){/*publish 1 block - */
     // printf("p: %d, h5\n",p->id);
     Block * n = p->pchain->head;
     // printf("blockID released = %d\n",n->blockID);
     p->pchain->head = p->pchain->head->next;
     p->pchain->origin = n;
     n->branches->erase(n->next);
     p->pchain->length--;
     c->appendBlock(n,f,p->gamma); 
     return 1;
   }
   else{/*do nothing*/
     // printf("do nothingllll\n");
     return 0;
   }
   
}



int honestMine(Player * p, Chain * c){
  
  int len = p->pchain->length;
  if(len==0){
    p->pchain->origin = c->race_set->choose();
    // printf("Honest ID chosen = %d\n",p->chain->origin->blockID);
    // printf("HonestMinenumTied = %d\n",c->race_set->numTied());
    /*do nothing*/
  
    return 0;
  }
  else{
    //    printf("sdffdn\n");
    privateChain * pChain = p->pchain;
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



