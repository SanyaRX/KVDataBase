#include "pch.h"
#include <KVDBApi.h>
#include "KVDBError.h"

static const std::string ipAddress = "127.0.0.1";
static const int port = 8000;

static kvdb::KVDBApi database(ipAddress, port);


TEST(CreateTable, CreateTable) {

    try {
        database.createTable("table1", { "key1", "key2", "key3" });
        EXPECT_TRUE(true);
    }
    catch (...) {
        EXPECT_TRUE(false);
    }
}


TEST(TableAlreadyExists, TableAlreadyExists) {
    
    try {
        database.createTable("table2", { "key1" });
        database.createTable("table2", { "key1", "key2" });
        EXPECT_TRUE(false);
    } 
    catch (const kvdb_utils::TableAlreadyExistsException & taee) {
        EXPECT_TRUE(true);
    }
    catch (...) {
        EXPECT_TRUE(false);
    }
}


TEST(AddValue, AddValue) {

    try {
        database.createTable("car", { "key1", "key2", "key3" });
        database.addValue("car", { "1", "1", "1" }, "value1");
        database.addValue("car", { "2", "2", "2" }, "value1");
        EXPECT_TRUE(true);
    }
    catch (...) {
        EXPECT_TRUE(false);
    }
}


TEST(GetValueByKey, GetValueByKey) {

    try {
        database.createTable("getValues", { "key1", "key2", "key3" });
        database.addValue("getValues", { "1", "1", "1" }, "value1");
        database.addValue("getValues", { "1", "1", "1" }, "value2");

        auto values = database.getByKey("getValues", { "1", "1", "1" });
        std::vector<std::string> expected = { "value1", "value2" };

        ASSERT_EQ(values.size(), expected.size());

        for (int i = 0; i < values.size(); i++) {
            EXPECT_STREQ(expected[i].c_str(), values[i].c_str());
        }
    }
    catch (...) {
        EXPECT_TRUE(false);
    }
}

TEST(NoSuchKey, NoSuchKey) {

    try {
        database.createTable("noSuchKey", { "key1", "key2" });
        database.addValue("noSuchKey", { "0", "0" }, "value");
        auto output = database.getByKey("noSuchKey", { "o", "0" });
        EXPECT_TRUE(false);
    }
    catch (const kvdb_utils::NoSuchKeyException& nske) {
        EXPECT_TRUE(true);
    }
    catch (...) {
        EXPECT_TRUE(false);
    }
}


TEST(NoSuchTable, NoSuchTable) {
   
    try {
        database.addValue("noTable", { "key1", "key2", "key3" }, "value");
        EXPECT_TRUE(false);
    }
    catch (const kvdb_utils::TableDoesNotExistsException & tdne) {
        EXPECT_TRUE(true);
    }
    catch (...) {
        EXPECT_TRUE(false);
    }
}


TEST(InvalidKeySize, InvalidKeySize) {

    try {
        database.createTable("invalidKeySize", { "key1", "key2", "key3" });
        database.addValue("invalidKeySize", { "1", "2" }, "value");
        EXPECT_TRUE(false);
    }
    catch (const kvdb_utils::InvalidKeySizeException & ikse) {
        EXPECT_TRUE(true);
    }
    catch (...) {
        EXPECT_TRUE(false);
    }
}


TEST(GetFirstValue, GetFirstValue) {

    try {
        database.createTable("firstValue", { "key1", "key2", "key3" });
        database.addValue("firstValue", { "0", "1", "0" }, "value1");
        database.addValue("firstValue", { "1", "0", "1" }, "value2");
        database.addValue("firstValue", { "6", "3", "1" }, "value3");
        database.addValue("firstValue", { "4", "2", "2" }, "value4");

        auto output = database.getFirstValue("firstValue", "key2");
        EXPECT_STREQ("value2", output.c_str());
    }
    catch (...) {
        EXPECT_TRUE(false);
    }
}


TEST(GetLastValue, GetLastValue) {

    try {
        database.createTable("lastValue", { "key1", "key2", "key3" });
        database.addValue("lastValue", { "0", "1", "0" }, "value1");
        database.addValue("lastValue", { "1", "0", "1" }, "value2");
        database.addValue("lastValue", { "6", "3", "1" }, "value3");
        database.addValue("lastValue", { "4", "2", "2" }, "value4");

        auto output = database.getLastValue("lastValue", "key2");
        EXPECT_STREQ("value3", output.c_str());
    }
    catch (...) {
        EXPECT_TRUE(false);
    }
}


TEST(UpdateByKey, UpdateByKey) {

    try {
        database.createTable("updateByKey", { "key1", "key2" });
        database.addValue("updateByKey", { "0", "0" }, "value");
        database.updateByKey("updateByKey", { "0", "0" }, "updated");

        auto output = database.getByKey("updateByKey", { "0", "0" });

        ASSERT_EQ(1, output.size());
        EXPECT_STREQ("updated", output[0].c_str());
    }
    catch (...) {
        EXPECT_TRUE(false);
    }
}


TEST(GetFirstKey, GetFirstKey) {

    try {
        database.createTable("firstKey", { "key1", "key2", "key3" });
        database.addValue("firstKey", { "0", "1", "0" }, "value1");
        database.addValue("firstKey", { "1", "0", "1" }, "value2");
        database.addValue("firstKey", { "6", "3", "1" }, "value3");
        database.addValue("firstKey", { "4", "2", "2" }, "value4");

        auto values = database.getFirstKey("firstKey");
        std::vector<std::string> expected = { "0", "1", "0" };

        ASSERT_EQ(values.size(), expected.size());

        for (int i = 0; i < values.size(); i++) {
            EXPECT_STREQ(expected[i].c_str(), values[i].c_str());
        }
    }
    catch (...) {
        EXPECT_TRUE(false);
    }
}


TEST(GetLastKey, GetLastKey) {

    try {
        database.createTable("lastKey", { "key1", "key2", "key3" });
        database.addValue("lastKey", { "0", "1", "0" }, "value1");
        database.addValue("lastKey", { "1", "0", "1" }, "value2");
        database.addValue("lastKey", { "4", "2", "2" }, "value3");
        database.addValue("lastKey", { "6", "3", "1" }, "value4");

        auto values = database.getLastKey("lastKey");
        std::vector<std::string> expected = { "6", "3", "1" };

        ASSERT_EQ(values.size(), expected.size());

        for (int i = 0; i < values.size(); i++) {
            EXPECT_STREQ(expected[i].c_str(), values[i].c_str());
        }
    }
    catch (...) {
        EXPECT_TRUE(false);
    }
}


TEST(GetNextKey, GetNextKey) {

    try {
        database.createTable("nextKey", { "key1", "key2", "key3" });
        database.addValue("nextKey", { "0", "1", "0" }, "value1");
        database.addValue("nextKey", { "1", "0", "1" }, "value2");
        database.addValue("nextKey", { "4", "2", "2" }, "value3");
        database.addValue("nextKey", { "6", "3", "1" }, "value4");

        auto values = database.getNextKey("nextKey", { "1", "0", "1" });
        std::vector<std::string> expected = { "4", "2", "2" };

        ASSERT_EQ(values.size(), expected.size());

        for (int i = 0; i < values.size(); i++) {
            EXPECT_STREQ(expected[i].c_str(), values[i].c_str());
        }
    }
    catch (...) {
        EXPECT_TRUE(false);
    }
}


TEST(GetPrevKey, GetPrevKey) {

    try {
        database.createTable("prevKey", { "key1", "key2", "key3" });
        database.addValue("prevKey", { "0", "1", "0" }, "value1");
        database.addValue("prevKey", { "1", "0", "1" }, "value2");
        database.addValue("prevKey", { "4", "2", "2" }, "value3");
        database.addValue("prevKey", { "6", "3", "1" }, "value4");

        auto values = database.getPrevKey("prevKey", { "1", "0", "1" });
        std::vector<std::string> expected = { "0", "1", "0" };

        ASSERT_EQ(values.size(), expected.size());

        for (int i = 0; i < values.size(); i++) {
            EXPECT_STREQ(expected[i].c_str(), values[i].c_str());
        }
    }
    catch (...) {
        EXPECT_TRUE(false);
    }
}

/**

tests:
1. table is already exists +
2. add value +
3. no such table +
4. invalid key size +
5. get by key value +
6. get first value +
7. get last value +
8. no such key +
9. update by key +
10. get first key +
11. get last key +
12. get next key +
13. get prev key +
14. delete table +
*/







/*TEST(CreateTable, CreateTable) {

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
*/
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