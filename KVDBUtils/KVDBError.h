#pragma once

#include <exception>

#define KVDB_UNDEFINED_ERROR -1
#define KVDB_OK 0
#define KVDB_TABLE_DOESNT_EXISTS 1
#define KVDB_TABLE_ALREADY_EXISTS 2
#define KVDB_INVALID_KEY_SIZE 3
#define KVDB_NO_SUCH_KEY 4


namespace kvdb_utils {

    struct UndefinedErrorException : public std::exception
    {
        const char* what() const throw ()
        {
            return "Undefined Error";
        }
    };

    struct TableDoesNotExistsException : public std::exception
    {
        const char* what() const throw ()
        {
            return "Table does not exists";
        }
    };

    struct TableAlreadyExistsException : public std::exception
    {
        const char* what() const throw ()
        {
            return "Table already exists";
        }
    };

    struct InvalidKeySizeException : public std::exception
    {
        const char* what() const throw ()
        {
            return "Invalid key size";
        }
    };

    struct NoSuchKeyException : public std::exception
    {
        const char* what() const throw ()
        {
            return "No such key";
        }
    };

}