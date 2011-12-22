#ifndef SUITE_AND_TEST_LIST_WRAPPER_H
#define SUITE_AND_TEST_LIST_WRAPPER_H

#include "unittest.h"
#include "linkedlist.h"

typedef struct _test_suite_element_t {
	ListNode_t link;
	test_suite_t * suitep;
	ListNode_t test_list_head;
}test_suite_element_t;

typedef struct _test_element_t {
	ListNode_t link;
	test_results_t  test;
}test_element_t;

test_suite_element_t * create_test_suite_element();
test_element_t * create_test_element();

#endif
