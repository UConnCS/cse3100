typedef struct node {
   int         value;
   struct node *next;
} node;

int ll_has_cycle(node *head)
{
    if (head == NULL || head->next == NULL)
        return 0;
        
    node* hare = head->next->next;
    node* turtoise = head->next;
    
    while (1) {
        if (hare->next == NULL || hare->next->next == NULL)
            return 0;
        
        hare = hare->next->next;
        turtoise = turtoise->next;
        
        if (hare == turtoise)
            return 1;
    }
}
