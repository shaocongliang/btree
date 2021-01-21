#include "btree.h"

#include <gtest/gtest.h>

/*
TEST(btree_test, insert_key_test){
  const std::size_t O = 5; // order of btree
  int a[4] = {1, 2, 9, 10};
  std::size_t pos = 0;
  pos = insert_key<int, O>(11, a);
  EXPECT_EQ(4, pos);

  int b[4] = {1, 2, 9, 10};
  pos = insert_key<int, O>(-100, b);
  EXPECT_EQ(0, pos);

  int c[4] = {1, 2, 9, 10};
  pos = insert_key<int, O>(7, c);
  EXPECT_EQ(2, pos);
}
*/
TEST(btree_test, insert_to_leaf){
  auto root_node = create_btree<int, 3>();
  for(int idx = 0; idx < 3; ++idx){
    insert(root_node, idx+1);
  }
  print_bfs(root_node, 0);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}