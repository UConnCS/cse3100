#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
 * struct for list nodes
 * DO NOT MODIFY
 */
typedef struct node {
   int         value;
   struct node *next;
} node;

/*
 * Given pointers to the first nodes of two singly linked lists,
 * return 1 if the two lists have a non-empty intersection (i.e.,
 * share any nodes) and 0 otherwise
 */
int have_intersection(node* head1, node* head2) {
    node *p1 = head1;
    node *p2 = head2;
    
    while (p1 != NULL) {
        p2 = head2;
        while (p2 != NULL) {
            if (p1 == p2) {
                return 1;
            }
            p2 = p2->next;
        }
        p1 = p1->next;
    }

    return 0;
}


/*
 * Given pointers to the first nodes of two singly linked lists,
 * return a pointer to the first shared node if the two lists have
 * a non-empty intersection and NULL otherwise
 */
node* find_intersection(node* head1, node* head2)
{
    node *p1 = head1;
    node *p2 = head2;
    
    while (p1 != NULL) {
        p2 = head2;
        while (p2 != NULL) {
            if (p1 == p2)
                return p1;

            p2 = p2->next;
        }
        p1 = p1->next;
    }

    return 0;
}



// STUDY BUT DO NOT MODIFY CODE BELOW

#define MAX 100

/*
 * Given a pointer to the head node of an acyclic list,
 * print the values stored in the nodes.
 */

void tests_print_list(node *head)
{
   int counter = 0;

   while( head )
   {
      printf( "%d%s", head->value, (head->next ? "->" : "") );
      head = head->next;
      counter++;
      if(counter > MAX) {
         printf( "..." );
         break;
      }
   }
   printf( "\n" );

   return;
}

/*
 * Tests for have_intersection() function
 */

void have_intersection_test1(void)
{
    node nodes[20] = { { 0,NULL}, { 1,NULL}, { 2,NULL}, { 3,NULL}, { 4,NULL},
                       { 5,NULL}, { 6,NULL}, { 7,NULL}, { 8,NULL}, { 9,NULL},
                       { 0,NULL}, { 1,NULL}, { 2,NULL}, { 3,NULL}, { 4,NULL},
                       { 5,NULL}, { 6,NULL}, { 7,NULL}, { 8,NULL}, { 9,NULL} };

    nodes[0].next = &nodes[1];
    nodes[1].next = &nodes[2];
    nodes[2].next = &nodes[3];
    nodes[4].next = &nodes[2];

    printf( "The lists intersect. have_intersection() says: %s\n",
            have_intersection(&nodes[0], &nodes[4]) ? "intersect" : "do not intersect" );
}

void have_intersection_test2(void)
{
    node nodes[20] = { { 0,NULL}, { 1,NULL}, { 2,NULL}, { 3,NULL}, { 4,NULL},
                       { 5,NULL}, { 6,NULL}, { 7,NULL}, { 8,NULL}, { 9,NULL},
                       { 0,NULL}, { 1,NULL}, { 2,NULL}, { 3,NULL}, { 4,NULL},
                       { 5,NULL}, { 6,NULL}, { 7,NULL}, { 8,NULL}, { 9,NULL} };

    nodes[0].next = &nodes[1];
    nodes[1].next = &nodes[2];
    nodes[2].next = &nodes[3];
    nodes[10].next = &nodes[11];
    nodes[11].next = &nodes[12];
    nodes[12].next = &nodes[13];

    printf( "The lists do not intersect. have_intersection() says: %s\n",
            have_intersection(&nodes[0], &nodes[10]) ? "intersect" : "do not intersect" );
}

void have_intersection_test3(void)
{
    node nodes[20] = { { 0,NULL}, { 1,NULL}, { 2,NULL}, { 3,NULL}, { 4,NULL},
                       { 5,NULL}, { 6,NULL}, { 7,NULL}, { 8,NULL}, { 9,NULL},
                       { 0,NULL}, { 1,NULL}, { 2,NULL}, { 3,NULL}, { 4,NULL},
                       { 5,NULL}, { 6,NULL}, { 7,NULL}, { 8,NULL}, { 9,NULL} };

    nodes[0].next = &nodes[1];
    nodes[1].next = &nodes[2];
    nodes[2].next = &nodes[3];
    nodes[3].next = &nodes[4];

    printf( "The lists intersect. have_intersection() says: %s\n",
            have_intersection(&nodes[0], &nodes[2]) ? "intersect" : "do not intersect" );
}

void have_intersection_test4(void)
{
    node nodes[20] = { { 0,NULL}, { 1,NULL}, { 2,NULL}, { 3,NULL}, { 4,NULL},
                       { 5,NULL}, { 6,NULL}, { 7,NULL}, { 8,NULL}, { 9,NULL},
                       { 0,NULL}, { 1,NULL}, { 2,NULL}, { 3,NULL}, { 4,NULL},
                       { 5,NULL}, { 6,NULL}, { 7,NULL}, { 8,NULL}, { 9,NULL} };

    nodes[0].next = &nodes[1];
    nodes[1].next = &nodes[2];
    nodes[2].next = &nodes[3];
    nodes[3].next = &nodes[4];

    printf( "The lists do not intersect. have_intersection() says: %s\n",
            have_intersection(&nodes[0], NULL) ? "intersect" : "do not intersect" );
}

void have_intersection_test5(void)
{
    node nodes[20] = { { 0,NULL}, { 1,NULL}, { 2,NULL}, { 3,NULL}, { 4,NULL},
                       { 5,NULL}, { 6,NULL}, { 7,NULL}, { 8,NULL}, { 9,NULL},
                       { 0,NULL}, { 1,NULL}, { 2,NULL}, { 3,NULL}, { 4,NULL},
                       { 5,NULL}, { 6,NULL}, { 7,NULL}, { 8,NULL}, { 9,NULL}  };

    nodes[0].next = &nodes[1];
    nodes[1].next = &nodes[2];
    nodes[2].next = &nodes[3];
    nodes[3].next = &nodes[4];
    nodes[4].next = &nodes[5];
    nodes[5].next = &nodes[6];
    nodes[6].next = &nodes[7];
    nodes[7].next = &nodes[8];
    nodes[8].next = &nodes[9];
    nodes[10].next = &nodes[11];
    nodes[11].next = &nodes[12];
    nodes[12].next = &nodes[13];
    nodes[13].next = &nodes[14];
    nodes[14].next = &nodes[15];
    nodes[15].next = &nodes[16];
    nodes[16].next = &nodes[17];
    nodes[17].next = &nodes[18];
    nodes[18].next = &nodes[9];

    printf( "The lists intersect. have_intersection() says: %s\n",
            have_intersection(&nodes[0], &nodes[10]) ? "intersect" : "do not intersect" );
}

void have_intersection_test6(void)
{

    printf( "The lists do not intersect. have_intersection() says: %s\n",
            have_intersection(NULL, NULL) ? "intersect" : "do not intersect" );
}



/*
 * Tests for find_intersection() function
 */

void find_intersection_test1(void)
{
    node nodes[20] = { { 0,NULL}, { 1,NULL}, { 2,NULL}, { 3,NULL}, { 4,NULL},
                       { 5,NULL}, { 6,NULL}, { 7,NULL}, { 8,NULL}, { 9,NULL},
                       { 0,NULL}, { 1,NULL}, { 2,NULL}, { 3,NULL}, { 4,NULL},
                       { 5,NULL}, { 6,NULL}, { 7,NULL}, { 8,NULL}, { 9,NULL} };

    nodes[0].next = &nodes[1];
    nodes[1].next = &nodes[2];
    nodes[2].next = &nodes[3];
    nodes[4].next = &nodes[2];

    tests_print_list( find_intersection(&nodes[0], &nodes[4]) );
}

void find_intersection_test2(void)
{
    node nodes[20] = { { 0,NULL}, { 1,NULL}, { 2,NULL}, { 3,NULL}, { 4,NULL},
                       { 5,NULL}, { 6,NULL}, { 7,NULL}, { 8,NULL}, { 9,NULL},
                       { 0,NULL}, { 1,NULL}, { 2,NULL}, { 3,NULL}, { 4,NULL},
                       { 5,NULL}, { 6,NULL}, { 7,NULL}, { 8,NULL}, { 9,NULL} };

    nodes[0].next = &nodes[1];
    nodes[1].next = &nodes[2];
    nodes[2].next = &nodes[3];
    nodes[10].next = &nodes[11];
    nodes[11].next = &nodes[12];
    nodes[12].next = &nodes[13];

    tests_print_list( find_intersection(&nodes[0], &nodes[10]) );
}

void find_intersection_test3(void)
{
    node nodes[20] = { { 0,NULL}, { 1,NULL}, { 2,NULL}, { 3,NULL}, { 4,NULL},
                       { 5,NULL}, { 6,NULL}, { 7,NULL}, { 8,NULL}, { 9,NULL},
                       { 0,NULL}, { 1,NULL}, { 2,NULL}, { 3,NULL}, { 4,NULL},
                       { 5,NULL}, { 6,NULL}, { 7,NULL}, { 8,NULL}, { 9,NULL} };

    nodes[0].next = &nodes[1];
    nodes[1].next = &nodes[2];
    nodes[2].next = &nodes[3];
    nodes[3].next = &nodes[4];

    tests_print_list( find_intersection(&nodes[0], &nodes[2]) );
}

void find_intersection_test4(void)
{
    node nodes[20] = { { 0,NULL}, { 1,NULL}, { 2,NULL}, { 3,NULL}, { 4,NULL},
                       { 5,NULL}, { 6,NULL}, { 7,NULL}, { 8,NULL}, { 9,NULL},
                       { 0,NULL}, { 1,NULL}, { 2,NULL}, { 3,NULL}, { 4,NULL},
                       { 5,NULL}, { 6,NULL}, { 7,NULL}, { 8,NULL}, { 9,NULL} };

    nodes[0].next = &nodes[1];
    nodes[1].next = &nodes[2];
    nodes[2].next = &nodes[3];
    nodes[3].next = &nodes[4];

    tests_print_list( find_intersection(&nodes[0], NULL) );
}

void find_intersection_test5(void)
{
    node nodes[20] = { { 0,NULL}, { 1,NULL}, { 2,NULL}, { 3,NULL}, { 4,NULL},
                       { 5,NULL}, { 6,NULL}, { 7,NULL}, { 8,NULL}, { 9,NULL},
                       { 0,NULL}, { 1,NULL}, { 2,NULL}, { 3,NULL}, { 4,NULL},
                       { 5,NULL}, { 6,NULL}, { 7,NULL}, { 8,NULL}, { 9,NULL}  };

    nodes[0].next = &nodes[1];
    nodes[1].next = &nodes[2];
    nodes[2].next = &nodes[3];
    nodes[3].next = &nodes[4];
    nodes[4].next = &nodes[5];
    nodes[5].next = &nodes[6];
    nodes[6].next = &nodes[7];
    nodes[7].next = &nodes[8];
    nodes[8].next = &nodes[9];
    nodes[10].next = &nodes[11];
    nodes[11].next = &nodes[12];
    nodes[12].next = &nodes[13];
    nodes[13].next = &nodes[14];
    nodes[14].next = &nodes[15];
    nodes[15].next = &nodes[16];
    nodes[16].next = &nodes[17];
    nodes[17].next = &nodes[18];
    nodes[18].next = &nodes[9];

    tests_print_list( find_intersection(&nodes[0], &nodes[10]) );
}

void find_intersection_test6(void)
{
    tests_print_list( find_intersection(NULL, NULL) );
}



int main(void) {

    have_intersection_test1();
    have_intersection_test2();
    have_intersection_test3();
    have_intersection_test4();
    have_intersection_test5();
    have_intersection_test6();

    find_intersection_test1();
    find_intersection_test2();
    find_intersection_test3();
    find_intersection_test4();
    find_intersection_test5();
    find_intersection_test6();

    return 0;
}

