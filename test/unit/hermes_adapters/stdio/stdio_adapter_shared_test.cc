/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Distributed under BSD 3-Clause license.                                   *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Illinois Institute of Technology.                        *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of Hermes. The full Hermes copyright notice, including  *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the top directory. If you do not  *
 * have access to the file, you may request a copy from help@hdfgroup.org.   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

TEST_CASE("SharedSTDIORead", "[process=" + std::to_string(TEST_INFO->comm_size_) +
                                 "]"
                                 "[operation=batched_read]"
                                 "[request_size=type-fixed][repetition=" +
                                 std::to_string(TEST_INFO->num_iterations_) +
                                 "]"
                                 "[mode=shared]"
                                 "[pattern=sequential][file=1]") {
  TEST_INFO->Pretest();

  SECTION("read from existing file") {
    test::test_fopen(TEST_INFO->existing_shared_file.c_str(), "r+");
    REQUIRE(test::fh_orig != nullptr);
    std::string data(TEST_INFO->request_size_, '1');
    for (size_t i = 0; i < TEST_INFO->num_iterations_; ++i) {
      test::test_fread(data.data(), TEST_INFO->request_size_);
      REQUIRE(test::size_read_orig == TEST_INFO->request_size_);
    }
    test::test_fclose();
    REQUIRE(test::status_orig == 0);
  }
  posttest();
}