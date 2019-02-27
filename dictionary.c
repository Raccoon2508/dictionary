/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */




#include <stdbool.h>
#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define TABLE_SIZE 150000
#define TABLE_MUL 31
#define LENGHT 45

int words=0;
const char* dict;

typedef struct trie
{
    char ch;
    struct trie *sibling;
    struct trie *child;
    bool isword;
    struct trie *prev_ch;
    struct trie *prev_si;
}trie;

trie *trie_create()
{
trie *node;
if ( (node = malloc(sizeof(*node))) == NULL){
    return NULL;
    node->ch = '\0';
    node->sibling = NULL;
    node->child = NULL;
    node->isword=false;
    node->prev_ch=false;
    node->prev_si=false;
}
return node;
}
struct trie *node, *parent, *list, *root=NULL;

bool check(const char* word)
{


 for(list=root;*word!='\0';word++){
     char key=tolower(*word);
     for(node=list;node!=NULL;node=node->sibling){if(node->ch==key){break;}}
     if(node!=NULL){list=node->child;}
     else{return false;}

 }                   //  поиск слова из программы (текста) в словаре
if (node->isword==true){return true;}
else{return false;}
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{

char k;
dict=dictionary; //передача словаря в глобальную переменную для удаления
FILE *fo=fopen(dictionary, "r");
for (k = fgetc(fo); k != EOF; k = fgetc(fo))
         {if (isalpha(k)){
                    parent = NULL;
                    list = root;

                    for (; isalpha(k)||k=='\''; k = fgetc(fo)) {
                        for (node = list; node != NULL; node = node->sibling){
                            if (node->ch == k){break;}
                        }

                    if (node == NULL){
                        node = trie_create();
                        node->ch = k;
                        node->sibling = list;
                        if(list!=NULL){
                            list->prev_si=node;
                        }

                        if (parent != NULL){
                            parent->child = node;
                            node->prev_ch=parent;

                        }
                        else{
                            root = node;}
                            list = NULL;
                    }
                    else {
                        list = node->child;
                    }
                    parent = node;
                    }
node->isword=true;
words++;
}}
 fclose(fo);
if (k==EOF){return true;}
else{return false;}
}
/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // размер словаря
    return words;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
FILE *fo=fopen(dict, "r");
char k;
struct trie *one,*two;

for (k = fgetc(fo); k != EOF; k = fgetc(fo))
    {
        if (isalpha(k))
         {
                    parent = NULL;
                    list = root;

                    for (; isalpha(k)||k=='\''; k = fgetc(fo))
                    {
                        for (node = list; node != NULL; node = node->sibling)
                        {
                            if (node->ch == k){break;}
                        }
                        if(node!=NULL){list = node->child;
                        }

                    }
                    if(node->child==NULL)
                    {
                        while(node->sibling==NULL&&node!=root&&node->prev_si==NULL)
                        {


                            node=node->prev_ch;
                            free(node->child);
                            node->child=NULL;
                        }

                        if(node->sibling!=NULL||node->prev_si!=NULL)
                        {

                            if(node->sibling!=NULL){two=node->sibling; node->sibling=NULL; two->prev_si=NULL;}
                            else{two=node->prev_si; node->prev_si=NULL; two->sibling=NULL;}
                            if(node->prev_ch!=NULL){one=node->prev_ch;
                            one->child=two;}
                            free(node);
                        }
                    }


        }
   }
fclose(fo);
if (k==EOF){return true;}
else{return false;}

}






