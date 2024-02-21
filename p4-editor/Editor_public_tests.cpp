#include "Editor.hpp"
#include "unit_test_framework.hpp"

TEST(test_new_editor) {
    Editor E;
    ASSERT_TRUE(E.is_at_end());
    ASSERT_TRUE(E.get_row() == 1);

    ASSERT_TRUE(E.get_column() == 0);
    auto str = E.stringify();
    ASSERT_EQUAL(str, "");
}
TEST(test_insert) {
    Editor E;
    E.insert('A');
    E.backward();
    ASSERT_EQUAL(E.data_at_cursor(), 'A');
    auto str = E.stringify();
    ASSERT_EQUAL(str, "A");
}

TEST_MAIN()