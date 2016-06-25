#include <iostream>
#include <string>
#include <vector>
#include <gtest/gtest.h>
#include <glog/logging.h>

#include "ConfigUtils.h"
#include "ConfigException.h"
#include "constants.h"
#include "Configure.h"

using namespace std;

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

class TestConfigure : public ::testing::Test {
protected:
    TestConfigure() {}

    virtual ~TestConfigure() {}

    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST_F(TestConfigure, test_ConfigUtils) {
    google::InitGoogleLogging("zero91");

    google::LogToStderr();

    jcpp::configure::Configure cfg;
    cfg.load("./conf", "test.conf", jcpp::configure::GROUP_DUP_LEVEL_3);

    EXPECT_EQ("zero91", cfg["name"].to_string());

    EXPECT_EQ(13, cfg["age"].to_uint32());
    EXPECT_EQ(2, cfg["info"]["family"].size());

    EXPECT_EQ("中国人", cfg["info"]["family"][0].to_string());
    EXPECT_EQ("碧水蓝天", cfg["info"]["family"][1].to_string());

    EXPECT_EQ(3, cfg["info"]["money"].size());
    EXPECT_EQ(1000, cfg["info"]["money"]["young"].to_double());
    EXPECT_EQ(9495.3, cfg["info"]["money"]["old"].to_double());

    EXPECT_EQ(jcpp::configure::CONFIG_TYPE_GROUP, cfg["info"]["money"]["china"].self_type());

    EXPECT_STREQ("include.conf.file_name", cfg["file_name"].to_cstr());

    EXPECT_EQ(5, cfg["china"]["province"].size());

    EXPECT_STREQ("anhui", cfg["china"]["province"][0].to_cstr());
    EXPECT_STREQ("beijing", cfg["china"]["province"][1].to_cstr());
    EXPECT_STREQ("tianjing", cfg["china"]["province"][2].to_cstr());
    EXPECT_STREQ("xiAmen", cfg["china"]["province"][3].to_cstr());
    EXPECT_STREQ("\"jiangsu2\"", cfg["china"]["province"][4].to_cstr());

    EXPECT_STREQ("大败局I", cfg["book"]["business"].to_cstr());

    EXPECT_EQ(jcpp::configure::CONFIG_TYPE_ERROR, cfg["info"]["money"]["not_exist"].self_type());

    EXPECT_STREQ("testing", cfg["info"]["money"]["china"]["haha"].to_cstr());
    EXPECT_STREQ("hello world", cfg["info"]["remain"].to_cstr());

    google::ShutdownGoogleLogging();
}
