#include <ctype.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>


typedef struct{
    char *key;
    int index;
    struct *node next
}node;

node* head=NULL;

void insert (char *key){

    node *new;
    new=malloc(sizeof(node));

    if(new=NULL){
        break;
    }
    new->key= ;
    new->index= ;
    new->next=NULL;
    new->next=head;
    head=new;
}

struct wordlist{            //создаем стракт wordlist
    char* key;              //ключ (указатель на шифруемое слово)
    int value;              //индекс шифруемого слова для хранения в хэш-таблице
    struct wordlist *next;  //создание указателя на следующий стракт

}wordlist;

struct wordlist *table[TABLE_SIZE];        //создаем таблицу данных типа wordlist размера TABLE_SIZE

int hash_func (char *key){            //создаем хэш-функцию, которая преобразовывает содержимое переменной указателя *key в индекс и возвращает индекс
int h=0;  //индекс обрабатываемого ключа, изначально равен 0
char *p;  //дополнительный указатель для обработки ключа

for(p=key;p!='\0';p++){  //цикл перебора символов ключа (начальное значение указателя *p приравнивается к значению переменной по указателю *key), цикл продолжается до окончания строки \0, на каждой итерации положение указателя *p меняется на 1.
    h=h*TABLE_MUL+(int)*p; //рекурсивная формула хэш-функции
}
    return h%TABLE_SIZE;  //возврат значения h либо остатка от деления на TABLE_SIZE, если значенние h превышает размер таблицы.
}

void table_inc (struct wordlist**table){   //функция инициализации таблицы (обнуления указателей), в роли параметра указатель на указатель на таблицу

    for (int i=0; i<TABLE_SIZE;i++){ //перебор i от 0 до размера таблицы
        table[i]=NULL;              //обнуление i-ого указателя
    }
}

int table_add (char*key){          //функция добавления данных в хеш таблицу, в качестве параментров указатель на указатель на таблицу, ключ, индекс ключа

    struct wordlist**table;
    struct wordlist *node;  //создаение указателя на запись в таблице
    int tab_index=hash_func(key);  //обработка ключа, получение индекса
    node=malloc(sizeof(*node));//получение памяти для записи в таблице
    if(node!=NULL){ //присвоение значений переменным в стракте
        node->key=key; //ключ
        node->value=tab_index;//индекс
        node->next=table[tab_index]; //создание указателя на указатель
        table[tab_index]=node; //приравнивание указателя к указателю "node" на выделенную область памяти
    }
    return 0;
}

struct wordlist *search(struct wordlist**table, char*key){  //поиск слова в таблице по индексу

struct wordlist *node;

int tab_index=hash_func(key);
for(node=table[tab_index]; node!=NULL;node=node->next){
    if (node->key==0){
        return node;
    }}
return NULL;
}

void table_del(struct wordlist **table, char*key){
    struct wordlist *p,*prev=NULL;
    int tab_index=hash_func(key);
    for(p=table[tab_index];p!=NULL;p=p->next){
        if(p->key==0){
            if (prev==NULL){
                table[tab_index]=p->next;}
            else{
                    prev->next=p->next;
                    free(p);
                    return;

            }
        }
        prev=p;
    }
}


typedef struct node{
    char *key;
    int index;
    struct *node next
}Node;

void insert (char *key){
    Node *head;
    Node *new;
    new=malloc(sizeof(node));

    if(new==NULL){
        break;
    }
    new->key= ;
    new->index= ;
    new->next=NULL;
    new->next=head;
    head=new;
}



char *trie_lookup(struct trie *root, char *key)
{
struct trie *node, *list;
for (list = root; *key != '\0'; key++) {
for (node = list; node != NULL;
node = node->sibling)
{
if (node->ch == *key)
break;
}
if (node != NULL)
list = node->child;
else
return NULL;
}
/* Check: Node must be a leaf node! */
return node->value;
}

struct trie *trie_insert(struct trie *root,
char *key, char *value)
{
struct trie *node, *parent, *list;
parent = NULL;
list = root;
for (; *key != '\0'; key++) {
/* Lookup sibling node */
for (node = list; node != NULL;
node = node->sibling)
{
if (node->ch == *key)
break;
}
if (node == NULL) {
/* Node not found. Add new node */
node = trie_create();
node->ch = *key;
node->sibling = list;
if (parent != NULL)
parent->child = node;
else
root = node;
list = NULL;
} else {
/* Node found. Move to next level */
list = node->child;
}
parent = node;
}
/* Update value in leaf */
if (node->value != NULL)
free(node->value);
node->value = strdup(value);
return root;
}