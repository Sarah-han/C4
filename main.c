#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define NUM_LETTER ((int)26)


typedef enum {NO=0, YES=1} boolean;



typedef struct node {      
  char letter; 
  struct node* children[NUM_LETTER]; 
  boolean hasChildren;   
  long unsigned int count;    
  boolean end;  //end of word 
} node;



typedef struct trie {  
  int maxLengthW; 
  node* children[NUM_LETTER];       
  boolean empty;     
  node* current; 
  char* word;
} trie;



node* initialize_node(node* n,char letter) {        
  n->letter = letter;  
  n->count = 0;   
  n->end = NO;   
  n->hasChildren = NO;  
  for (int i=0; i<NUM_LETTER; ++i) { 
      (n->children)[i] = NULL;      
}       
 return n;
}




node* create_node(char letter) {
      
   node* n;      
  if (!(n=(node*)malloc(sizeof(node)))){
      printf("can't allocating memory. exit\n");  
      exit(1);
}     
   return initialize_node(n,letter);
}


trie* createTrie(void) {   

 trie* t;
  if (!(t=(trie*)malloc(sizeof(trie)))) {
      printf("can't allocating memory. exit \n");  
      exit(1);
}
 //initialize the trie 
 for (int i=0; i<NUM_LETTER; ++i) {  
       t->children[i]=NULL;  
}             
  t->current=NULL; 
      t->empty=YES;  
      t->maxLengthW=0; 
       return t;
}


void endWord(trie* root) { 
  if (root->current == NULL)     
      return;     
  root->current->count++;      
  root->current->end = YES;     
  root->current=NULL;
   }
  boolean is_empty(trie* root) {     
  return root->empty;
}


int readLetter(trie* root, int c) {   
  int lengthW=0; 
  if(!isalpha(c)) { 
	if(c == ' ' || c == '\t' || c == '\n')
           endWord(root);     
      return lengthW;       
    }      
  lengthW++;   
  c=tolower(c); 
  int index= c-'a'; 
  if (root->current==NULL) { // new word from the root
         if (root->children[index] == NULL)   
            root->children[index] = create_node(c);    
            root->current = root->children[index]; 
            root->empty=NO; 
       } 
else { 
       root->current->hasChildren = YES;  
       if (root->current->children[index] == NULL) 
           root->current->children[index] = create_node(c);  
           root->current=root->current->children[index];  
      }       
 return lengthW;
}



trie* enterTxt() {
  int c;  
  int lengthW;  
  trie* root; 
  root = createTrie(); 
  while( EOF!=(c=getchar()) ) {       
        lengthW=readLetter(root,c); 
         if (lengthW>root->maxLengthW)  
            root->maxLengthW=lengthW;
        }    
 //allocate the word       
free(root->word);   
   if (!(root->word=(char*)malloc(1+sizeof(char)*(root->maxLengthW))) ){
	printf("can't allocating memory. exit \n");  
        exit(1);
	}        
 return root;
}


boolean needR(int argc, char* argv[]) {    

       if ( (argc == 2) && (argv[1][0]=='r' || argv[1][0]=='R') )  
           return YES;   
    if (argc == 1)   
            return NO;
    printf("\nBad command line arguments.\n\n");    
    exit(1);
return NO; 
}



void printR(trie* root) { 
   static int p=0;  
   node* current;  
   root->word[p++]=root->current->letter;  
   if (root->current->hasChildren) {   
       for (int i=NUM_LETTER-1;i>=0; --i) {  
            if (root->current->children[i] == NULL)  
                continue;  
             current = root->current;       
             root->current = root->current->children[i];  
             printR(root); 
             root->current = current; 
               } 
       }
 else {    
        if (root->current->end) {
            root->word[p]='\0';   
            printf("%s\t%ld\n",root->word,root->current->count);
        }
   --p;      
   return;       
 }      
  if (root->current->end) {   
      root->word[p]='\0'; 
      printf("%s\t%ld\n",root->word,root->current->count);  
    }      
  --p;
}



void printTrieR(trie* root) {      
   if (root == NULL)              
       return;       
  if (is_empty(root))        
       return;       
  for (int i=NUM_LETTER-1; i>=0; --i) { 
        if (root->children[i] == NULL)  
            continue; 
       root->current = root->children[i];  
       printR(root); 
     }
}


void print(trie* root) { 
   static int p=0;   
   node* current;  
   root->word[p++]=root->current->letter; 
   if (root->current->end) {  
       root->word[p]='\0';  
       printf("%s\t%ld\n",root->word,root->current->count); 
     }     
   if (root->current->hasChildren) {  
       for(int i=0; i<NUM_LETTER; ++i) {    
           if (root->current->children[i] == NULL)  
                continue;
         current = root->current;
         root->current = root->current->children[i]; 
         print(root);  
         root->current = current;   
       }       
 }
 else {             
      --p;      
      return;  
      }      
  --p;
}

void printTrie(trie* root) {    
 if (root == NULL)      
     return;    
 if (is_empty(root)) 
     return; 
for (int i=0; i<NUM_LETTER; ++i) {      
	if (root->children[i] == NULL)      
            continue;      
      root->current = root->children[i]; 
      print(root);    
    }
}






void freeNode(node* n) {     
  if (n==NULL) {  
      return;  
  }    
 if (0==(n->hasChildren)) {  
     free(n);    
    } 
else {       
     for (int i=0; i<NUM_LETTER; ++i) {   
          freeNode((n->children)[i]);            
        } 
free(n);     
   }    
  return;
}


void freeToTrie(trie* t) {  
if (t == NULL)   
     return; 
 for(int i=0;i<NUM_LETTER; ++i) {   
     freeNode(t->children[i]);  
    }  
free(t->word);
free(t);
}

int main(int argc, char* argv[]) { 
   trie* root;       
   boolean r = needR(argc,argv);
    root = enterTxt();  
    if (r)  {     
            printTrieR(root);
         }
  else           
    printTrie(root);
    freeToTrie(root);   
     return 0;
}
