#include "pch.h"
#include <KVDBApi.h>


TEST(CreateTable, CreateTable) {

    std::string ipAddress = "127.0.0.1";			
    int port = 8000;

    int result = kvdb::openDataBase(ipAddress, port);
    result = kvdb::createTable("car", { "key1", "key2", "key3"});
    //result = kvdb::createTable("car2", { "key" });
    EXPECT_EQ(0, result);
    EXPECT_TRUE(true);

}


TEST(AddValue, AddValue) {

    std::string ipAddress = "127.0.0.1";
    int port = 8000;

    int result = kvdb::openDataBase(ipAddress, port);
    result = kvdb::addValue("car", { "key1", "key2", "key3"}, "value");
    result = kvdb::addValue("car", { "key1", "key2", "key3" }, "value2");
    //result = kvdb::createTable("car2", { "key" });
    EXPECT_EQ(0, result);
    EXPECT_TRUE(true);

}



TEST(GetValues, GetValues) {

    std::string ipAddress = "127.0.0.1";
    int port = 8000;


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

    std::string ipAddress = "127.0.0.1";
    int port = 8000;

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




TEST(DeleteAllValues, DeleteAllValues) {

    std::string ipAddress = "127.0.0.1";
    int port = 8000;

    std::vector<std::string> output;
    int result = kvdb::openDataBase(ipAddress, port);

    result = kvdb::deleteAllValuesByKey("car", { "key1", "key2", "key3" });
    kvdb::getByKey("car", { "key1", "key2", "key3" }, output);

    EXPECT_EQ(0, output.size());   
}



/*TEST(DeleteTable, DeleteTable) {

    std::string ipAddress = "127.0.0.1";
    int port = 8000;

    int result = kvdb::openDataBase(ipAddress, port);
    result = kvdb::deleteTable("car");
    EXPECT_EQ(0, result);
    EXPECT_TRUE(true);

}*/