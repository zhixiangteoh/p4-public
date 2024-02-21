#include "List.hpp"
#include "unit_test_framework.hpp"

using namespace std;

// Helpers
template <typename T>
bool are_lists_equal(List<T> &list1, List<T> &list2);
void create_list_int(List<int> &target);
void create_list_string(List<string> &target);
void create_singleton_list_int(List<int> &target);
void create_singleton_list_string(List<string> &target);
// Debugging helper
template <typename T>
void print_list(List<T> &list);

TEST(test_ctor_empty) {
    // Test empty ctor and .empty()
    List<int> list_int;
    ASSERT_TRUE(list_int.empty());
    List<double> list_double;
    ASSERT_TRUE(list_double.empty());

    List<string> list_string;
    ASSERT_TRUE(list_string.empty());

    List<bool> list_bool;
    ASSERT_TRUE(list_bool.empty());
}

TEST(test_copy_ctor) {
    // int
    List<int> list_int;
    create_list_int(list_int);
    List<int> list_int_copy = list_int;
    ASSERT_TRUE(are_lists_equal(list_int_copy, list_int));

    // string
    List<string> list_string;
    create_list_string(list_string);
    List<string> list_string_copy = list_string;
    ASSERT_TRUE(are_lists_equal(list_string_copy, list_string));
}

TEST(test_op_assignment) {
    // int
    List<int> list_int;
    create_list_int(list_int);
    List<int> list_int_assigned;
    ASSERT_FALSE(are_lists_equal(list_int_assigned, list_int));

    list_int_assigned = list_int;
    ASSERT_TRUE(are_lists_equal(list_int_assigned, list_int));

    // string
    List<string> list_string;
    create_list_string(list_string);
    List<string> list_string_assigned;
    ASSERT_FALSE(are_lists_equal(list_string_assigned, list_string));

    list_string_assigned = list_string;
    ASSERT_TRUE(are_lists_equal(list_string_assigned, list_string));
}

TEST(test_op_assignment_self) {
    List<int> list_int;
    create_list_int(list_int);
    int orig_size = list_int.size();
    List<int> *list_ptr = &list_int;
    list_int = *list_ptr;

    // no error if made it to here
    ASSERT_EQUAL(list_int.size(), orig_size);
}

TEST(test_insert_size_front_back) {
    List<int> list_fb;
    list_fb.push_front(1);
    list_fb.push_back(2);
    ASSERT_EQUAL(list_fb.size(), 2);
    ASSERT_EQUAL(list_fb.front(), 1);
    ASSERT_EQUAL(list_fb.back(), 2);
}

TEST(test_pop_front_single_elem) {
    // int
    List<int> singleton_int;
    create_singleton_list_int(singleton_int);
    singleton_int.pop_front();
    // list is now empty
    ASSERT_TRUE(singleton_int.empty());

    // string
    List<string> singleton_string;
    create_singleton_list_string(singleton_string);
    singleton_string.pop_front();
    // list is now empty
    ASSERT_TRUE(singleton_string.empty());
}

TEST(test_pop_back_single_elem) {
    // int
    List<int> singleton_int;
    create_singleton_list_int(singleton_int);
    singleton_int.pop_back();
    // list is now empty
    ASSERT_TRUE(singleton_int.empty());

    // string
    List<string> singleton_string;
    create_singleton_list_string(singleton_string);
    singleton_string.pop_back();
    // list is now empty
    ASSERT_TRUE(singleton_string.empty());
}

TEST(test_pop_clear) {
    List<int> test_list;
    test_list.push_back(1);
    test_list.push_back(2);
    test_list.push_back(3);
    test_list.push_back(4);

    // Testing pop_front()
    test_list.pop_front();
    ASSERT_EQUAL(test_list.front(), 2);
    ASSERT_EQUAL(test_list.size(), 3);

    // Testing pop_back()
    test_list.pop_back();
    ASSERT_EQUAL(test_list.back(), 3);
    ASSERT_EQUAL(test_list.size(), 2);

    // Testing clear() and empty()
    test_list.clear();
    ASSERT_EQUAL(test_list.size(), 0);
    ASSERT_TRUE(test_list.empty());
}

TEST(test_Iterator_begin_op_dereference) {
    List<int> list_int;
    create_list_int(list_int);
    ASSERT_EQUAL(*list_int.begin(), 1);
}

TEST(test_Iterator_op_prefix_increment_decrement_equality) {
    List<int> list_int;
    create_list_int(list_int);
    List<int>::Iterator it;
    // test assignment (value by value assign)
    it = list_int.begin();
    ASSERT_EQUAL(*it, 1);

    // test increment
    List<int>::Iterator it_second = ++it;
    ASSERT_EQUAL(*it_second, 2);
    ASSERT_EQUAL(*it, 2);
    // test equality
    ASSERT_TRUE(it_second == it);
    // test inequality
    ASSERT_TRUE(list_int.begin() != it);

    // test decrement
    List<int>::Iterator it_begin = --it;
    ASSERT_EQUAL(*it_begin, 1);
    ASSERT_EQUAL(*it, 1);
    // test equality
    ASSERT_TRUE(it_begin == it);
    // test inequality
    ASSERT_TRUE(it_second != it);
}

TEST(test_end) {
    List<int> list_int;
    create_list_int(list_int);
    List<int>::Iterator past_the_end;
    ASSERT_EQUAL(list_int.end(), past_the_end);
}

TEST(test_erase) {
    List<int> list_int;
    create_list_int(list_int);
    List<int>::Iterator it_second = ++list_int.begin();

    list_int.erase(it_second);
    List<int> list_int_two;
    list_int_two.push_back(1);
    list_int_two.push_back(3);
    ASSERT_EQUAL(list_int.size(), 2);
    ASSERT_TRUE(are_lists_equal(list_int, list_int_two));

    // erase from list of two
    it_second = ++list_int.begin();
    list_int.erase(it_second);
    List<int> list_int_one;
    list_int_one.push_back(1);
    ASSERT_EQUAL(list_int.size(), 1);
    ASSERT_TRUE(are_lists_equal(list_int, list_int_one));
}

TEST(test_erase_single_element) {
    List<int> singleton_int;
    create_singleton_list_int(singleton_int);
    List<int>::Iterator it = singleton_int.begin();

    singleton_int.erase(it);
    List<int> empty_int;
    ASSERT_TRUE(singleton_int.empty());
    ASSERT_TRUE(are_lists_equal(singleton_int, empty_int));
}

TEST(test_insert) {
    // insert to empty list
    List<int> list_int;
    list_int.insert(list_int.begin(), 3);
    ASSERT_EQUAL(list_int.size(), 1);

    // insert to list of one element
    list_int.insert(list_int.begin(), 1);
    ASSERT_EQUAL(list_int.size(), 2);

    // insert to list of two elements
    List<int>::Iterator it_third = ++list_int.begin();

    list_int.insert(it_third, 2);
    List<int> list_int_three;
    create_list_int(list_int_three);
    ASSERT_EQUAL(list_int.size(), 3);
    ASSERT_TRUE(are_lists_equal(list_int, list_int_three));
}

TEST_MAIN()

// Helpers implementation
template <typename T>
bool are_lists_equal(List<T> &list1, List<T> &list2) {
    if (list1.size() != list2.size()) {
        return false;
    }
    for (typename List<T>::Iterator it1 = list1.begin(), it2 = list2.begin();
         it1 != list1.end() && it2 != list2.end(); ++it1, ++it2) {
        if (*it1 != *it2) {
            return false;
        }
    }
    return true;
}

void create_list_int(List<int> &target) {
    target.push_back(1);
    target.push_back(2);
    target.push_back(3);
}

void create_list_string(List<string> &target) {
    string s1 = "hello world", s2 = "goodbye world", s3 = "project 4 is wack";
    target.push_back(s1);
    target.push_back(s2);
    target.push_back(s3);
}

void create_singleton_list_int(List<int> &target) {
    target.push_back(3);
}

void create_singleton_list_string(List<string> &target) {
    string s1 = "hello world";
    target.push_back(s1);
}

template <typename T>
void print_list(List<T> &list) {
    for (typename List<T>::Iterator it = list.begin(); it != list.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
}
