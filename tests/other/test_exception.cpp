//
// Created by tabis on 10/10/2019.
//

#include "Exception.hpp"
#include <gtest/gtest.h>

TEST(ExceptionTests, normalThrow)
{
    testing::internal::CaptureStderr();
    std::string output;

    try {
        throw Log::Exception("An error occurred", "Somewhere");
    } catch (const Log::Exception &exception) {
        exception.debugErrorMessage();
    }
    output = testing::internal::GetCapturedStderr();
    ASSERT_STREQ("\x1B[0;31m[Exception] \x1B[0;35mAn error occurred\x1B[m in method \x1B[0;34mSomewhere\n", output.c_str());
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}