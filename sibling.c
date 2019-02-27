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
26
Вставка узла в Trie
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