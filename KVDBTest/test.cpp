#include "pch.h"
#include <KVDBApi.h>
#include "KVDBError.h"

static const std::string ipAddress = "127.0.0.1";
static const int port = 8000;


TEST(CreateTable, CreateTable) {

    int result = kvdb::openDataBase(ipAddress, port);
    result = kvdb::createTable("car", { "key1", "key2", "key3"});
    
    EXPECT_EQ(0, result);
}

TEST(NoSuchTable, NoSuchTable) {

    try {
        kvdb::addValue("sda", { "123" }, "das");
    }
    catch (const kvdb_utils::TableDoesNotExistsException &ex) {
        ASSERT_TRUE(true);
    }
    catch (std::exception &ex) {
        std::cout << ex.what() << std::endl;
        ASSERT_TRUE(false);
    }
}

TEST(AddValue, AddValue) {

    int result = kvdb::openDataBase(ipAddress, port);
    result = kvdb::addValue("car", { "key1", "key2", "key3"}, "value");
    
    EXPECT_EQ(0, result);
    EXPECT_TRUE(true);

}

/*

TEST(GetValues, GetValues) {

    int result = kvdb::openDataBase(ipAddress, port);
    std::vector<std::string> output;
    result = kvdb::getByKey(
        "car",
        { "key1", "key2", "key3" },
        output
    );
    
    std::vector<std::string> expected = {"value", "value2"};

    ASSERT_EQ(output.size(), expected.size());
    
    for (int i = 0; i < output.size(); i++) {
        ASSERT_STREQ(expected[i].c_str(), output[i].c_str());
    }

}


TEST(UpdateValue, UpdateValue) {

    kvdb::openDataBase(ipAddress, port);
    kvdb::createTable("update", {"key1", "key2"});
    kvdb::addValue("update", { "key1", "key2" }, "test");
    kvdb::updateByKey("update", { "key1", "key2" }, "expected");

    std::vector<std::string> output;
    kvdb::getByKey(
        "update",
        { "key1", "key2"},
        output
    );

    ASSERT_STREQ("expected", output[0].c_str());
}


TEST(GetFirstKey, GetFirstKey) {
    
    kvdb::openDataBase(ipAddress, port);
    
    kvdb::createTable("update1", { "key1", "key2" });
    
    kvdb::addValue("update1", { "0", "0" }, "val1");
    kvdb::addValue("update1", { "1", "2" }, "val2");
    kvdb::addValue("update1", { "2", "1" }, "val3");
    kvdb::addValue("update1", { "3", "3" }, "val4");

    std::vector<std::string> output;
    std::vector<std::string> expected = { "0", "0" };

    kvdb::getFirstKey("update1", output);

    ASSERT_EQ(output.size(), expected.size());

    for (int i = 0; i < output.size(); i++) {
        ASSERT_STREQ(expected[i].c_str(), output[i].c_str());
    }

}

TEST(GetLastKey, GetLastKey) {

    std::vector<std::string> output;
    std::vector<std::string> expected = { "3", "3" };

    kvdb::getLastKey("update1", output);

    ASSERT_EQ(output.size(), expected.size());

    for (int i = 0; i < output.size(); i++) {
        ASSERT_STREQ(expected[i].c_str(), output[i].c_str());
    }
}


TEST(GetNextKey, GetNextKey) {
    std::vector<std::string> output;
    std::vector<std::string> currentKey = {"1", "2"};
    std::vector<std::string> expected = { "2", "1" };

    kvdb::getNextKey("update1", currentKey, output);

    ASSERT_EQ(output.size(), expected.size());

    for (int i = 0; i < output.size(); i++) {
        ASSERT_STREQ(expected[i].c_str(), output[i].c_str());
    }
}

TEST(GetPrevKey, GetPrevKey) {
    std::vector<std::string> output;
    std::vector<std::string> currentKey = { "2", "1" };
    std::vector<std::string> expected = { "1", "2" };

    kvdb::getPrevKey("update1", currentKey, output);

    ASSERT_EQ(output.size(), expected.size());

    for (int i = 0; i < output.size(); i++) {
        ASSERT_STREQ(expected[i].c_str(), output[i].c_str());
    }
}


TEST(GetFirstValue, GetFirstValue) {
   
    std::string output;
    int result = kvdb::openDataBase(ipAddress, port);

    kvdb::createTable("table1", { "key1", "key2" });
    kvdb::addValue("table1", { "0", "2" }, "value1");
    kvdb::addValue("table1", { "4", "3" }, "value2");
    kvdb::addValue("table1", { "2", "1" }, "value3");
    kvdb::addValue("table1", { "1", "5" }, "value4");

    kvdb::getFirstValue("table1", "key2", output);

    EXPECT_STREQ("value3", output.c_str());
}


TEST(DeleteAllValues, DeleteAllValues) {

    std::string ipAddress = "127.0.0.1";
    int port = 8000;

    std::vector<std::string> output;
    int result = kvdb::openDataBase(ipAddress, port);

    result = kvdb::deleteAllValuesByKey("car", { "key1", "key2", "key3" });
   // kvdb::getByKey("car", { "key1", "key2", "key3" }, output);
  
    EXPECT_EQ(0, output.size());
}



TEST(DeleteTable, DeleteTable) {

    std::string ipAddress = "127.0.0.1";
    int port = 8000;

    int result = kvdb::openDataBase(ipAddress, port);
    result = kvdb::deleteTable("car");
    kvdb::addValue("car", { "1", "2", "3" }, "lel");
    EXPECT_EQ(0, result);

}*/