#include "unit_test_framework.hpp"
#include "Editor.hpp"

TEST(test_new_editor){
  Editor E;
  ASSERT_TRUE(E.is_at_end());
  ASSERT_TRUE(E.get_row() == 1);
  
  ASSERT_TRUE(E.get_column()==0);
  auto str = E.stringify();
  ASSERT_TRUE(str.first == "");
  
  ASSERT_TRUE(str.second == -1);
}
TEST(test_insert){
  Editor E;
  E.insert('A');
  E.backward();
  ASSERT_TRUE(E.data_at_cursor() == 'A');
  auto str = E.stringify();
  ASSERT_TRUE(str.first == "A");
  ASSERT_TRUE(str.second == 0);
}

TEST_MAIN()