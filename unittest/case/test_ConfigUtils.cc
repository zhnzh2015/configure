#include "ConfigUtils.h"

#include <iostream>
#include <string>
#include <vector>
#include <gtest/gtest.h>

using std::string;
using std::vector;
using namespace jcpp::configure;

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

class TestConfigUtils : public ::testing::Test {
protected:
    TestConfigUtils() {}

    virtual ~TestConfigUtils() {}

    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST_F(TestConfigUtils, str2str) {
    string res;

    EXPECT_EQ(0, ConfigUtils::str2str("abc : 123", res));
    EXPECT_EQ("abc : 123", res);

    EXPECT_EQ(0, ConfigUtils::str2str("\"abc : \\t\"", res));
    EXPECT_EQ("abc : \t", res);

    EXPECT_EQ(0, ConfigUtils::str2str("abc : \\t", res));
    EXPECT_EQ("abc : \\t", res);

    EXPECT_EQ(CONFIG_ERROR_FORMAT_ERROR, ConfigUtils::str2str("\"abc : \\t", res));

    EXPECT_EQ(0, ConfigUtils::str2str("abc : \\x41", res));
    EXPECT_EQ("abc : \\x41", res);

    EXPECT_EQ(0, ConfigUtils::str2str("\"abc : \\x41\"", res));
    EXPECT_EQ("abc : A", res);

    EXPECT_EQ(0, ConfigUtils::str2str("\"abc : \\101\"", res));
    EXPECT_EQ("abc : A", res);
}

TEST_F(TestConfigUtils, str2int64) {
    int64_t num;

    EXPECT_EQ(CONFIG_ERROR_NULL_VALUE, ConfigUtils::str2int64("", num));

    EXPECT_EQ(0, ConfigUtils::str2int64("65537", num));
    EXPECT_EQ(65537, num);

    EXPECT_EQ(0, ConfigUtils::str2int64("9223372036854775807", num));
    EXPECT_EQ(9223372036854775807ll, num);

    EXPECT_EQ(0, ConfigUtils::str2int64("+9223372036854775807", num));
    EXPECT_EQ(9223372036854775807ll, num);

    EXPECT_EQ(0, ConfigUtils::str2int64("-9223372036854775808", num));
    EXPECT_EQ(-9223372036854775808ull, num);

    EXPECT_EQ(CONFIG_ERROR_OUT_OF_RANGE, ConfigUtils::str2int64("-9223372036854775809", num));

    EXPECT_EQ(CONFIG_ERROR_OUT_OF_RANGE, ConfigUtils::str2int64("9223372036854775808", num));

    EXPECT_EQ(CONFIG_ERROR_FORMAT_ERROR, ConfigUtils::str2int64("9223372036854775807abs", num));
}

TEST_F(TestConfigUtils, str2uint64) {
    uint64_t num;

    EXPECT_EQ(CONFIG_ERROR_NULL_VALUE, ConfigUtils::str2uint64("", num));

    EXPECT_EQ(0, ConfigUtils::str2uint64("65537", num));
    EXPECT_EQ(65537, num);

    EXPECT_EQ(CONFIG_ERROR_OUT_OF_RANGE, ConfigUtils::str2uint64("-65537", num));

    EXPECT_EQ(0, ConfigUtils::str2uint64("18446744073709551615", num));
    EXPECT_EQ(18446744073709551615ull, num);

    EXPECT_EQ(CONFIG_ERROR_FORMAT_ERROR, ConfigUtils::str2uint64("1844674407351615asd", num));

    EXPECT_EQ(CONFIG_ERROR_OUT_OF_RANGE, ConfigUtils::str2uint64("18446744073709551616", num));
}

TEST_F(TestConfigUtils, str2double) {
    double num;

    EXPECT_EQ(CONFIG_ERROR_NULL_VALUE, ConfigUtils::str2double("", num));

    EXPECT_EQ(0, ConfigUtils::str2double("6.5537", num));
    EXPECT_EQ(6.5537, num);

    EXPECT_EQ(0, ConfigUtils::str2double("-6.551e02", num));
    EXPECT_EQ(-655.1, num);

    EXPECT_EQ(0, ConfigUtils::str2double("-65.51E-02", num));
    EXPECT_EQ(-0.6551, num);

    EXPECT_EQ(CONFIG_ERROR_FORMAT_ERROR, ConfigUtils::str2double("-65.51e-02.3", num));
}

TEST_F(TestConfigUtils, is_integer) {
    EXPECT_EQ(true, ConfigUtils::is_integer("1234"));
    EXPECT_EQ(false, ConfigUtils::is_integer("1234F"));
    EXPECT_EQ(false, ConfigUtils::is_integer("123.4"));
    EXPECT_EQ(false, ConfigUtils::is_integer("   123"));
}

TEST_F(TestConfigUtils, is_float) {
    EXPECT_EQ(true, ConfigUtils::is_float("1234"));
    EXPECT_EQ(false, ConfigUtils::is_float("1234F"));
    EXPECT_EQ(true, ConfigUtils::is_float("123.4"));
    EXPECT_EQ(false, ConfigUtils::is_float("  1.23"));
    EXPECT_EQ(true, ConfigUtils::is_float("123.4e03"));
    EXPECT_EQ(true, ConfigUtils::is_float("123.4e-03"));
}

TEST_F(TestConfigUtils, error_info) {
    EXPECT_EQ(16, ConfigUtils::set_error_info("I'm %d years old", 25));
    EXPECT_STREQ("I'm 25 years old", ConfigUtils::get_error_info());

    EXPECT_EQ(9, ConfigUtils::set_error_info("I'm a boy"));
    EXPECT_STREQ("I'm a boy", ConfigUtils::get_error_info());
}

TEST_F(TestConfigUtils, line_parser) {
    string key, value;
    
    EXPECT_EQ(0, ConfigUtils::line_parser("age :   25", key, value));
    EXPECT_EQ("age", key);
    EXPECT_EQ("25", value);

    EXPECT_EQ(0, ConfigUtils::line_parser("     age    :25   ", key, value));
    EXPECT_EQ("age", key);
    EXPECT_EQ("25", value);

    EXPECT_EQ(CONFIG_ERROR_FORMAT_ERROR, ConfigUtils::line_parser("   age  25  ", key, value));
    EXPECT_EQ(CONFIG_ERROR_NULL_VALUE, ConfigUtils::line_parser("  : age  25  ", key, value));

    EXPECT_EQ(0, ConfigUtils::line_parser("   age : : 25  ", key, value));
    EXPECT_EQ("age", key);
    EXPECT_EQ(": 25", value);

    EXPECT_EQ(CONFIG_ERROR_FORMAT_ERROR, ConfigUtils::line_parser(" age space : 25", key, value));

    EXPECT_EQ(0, ConfigUtils::line_parser(" age: ", key, value));
    EXPECT_EQ("age", key);
    EXPECT_EQ("", value);
}

TEST_F(TestConfigUtils, section_parser) {
    string section;
    
    EXPECT_EQ(0, ConfigUtils::section_parser(" [server] #comment", section));
    EXPECT_EQ("server", section);

    EXPECT_EQ(CONFIG_ERROR_FORMAT_ERROR, ConfigUtils::section_parser(" [server] cm", section));
    EXPECT_EQ(CONFIG_ERROR_FORMAT_ERROR, ConfigUtils::section_parser(" [server ip] ", section));
    EXPECT_EQ(CONFIG_ERROR_FORMAT_ERROR, ConfigUtils::section_parser(" #[server ip] ", section));
    EXPECT_EQ(CONFIG_ERROR_NULL_VALUE, ConfigUtils::section_parser(" [  ] ", section));

    EXPECT_EQ(0, ConfigUtils::section_parser(" [ server.ip ] ", section));
    EXPECT_EQ("server.ip", section);
}
