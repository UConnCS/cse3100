typedef struct node {
   int         value;
   struct node *next;
} node;

node* ll_reverse(node *head)
{
    if (head == NULL)
        return NULL;
        
    if (head->next == NULL)
        return head;
        
    node* target = ll_reverse(head->next);
    head->next->next = head;
    head->next = NULL;
    
    return target;
}
