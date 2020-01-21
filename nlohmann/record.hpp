#ifndef __NLOHMANN_RECORD_HPP_INCLUDED__
#define __NLOHMANN_RECORD_HPP_INCLUDED__

#include <vector>
#include <string>
#include <sstream>
#include <cstdio>

#include <nlohmann/json.hpp>

namespace nlohmann_test 
{
    class Record;
    std::string serialize(Record& rec);
    Record deserialize(std::string& data, unsigned int binary=0);

    typedef std::vector<int64_t>     Integers;
    typedef std::vector<std::string> Strings;

    class Record
    {
        public:
            Integers ids;
            Strings  strings;

            Record(unsigned int mode=0)
                : binary_status(mode)
            {}

            bool operator==(const Record& other)
            {
                return (ids == other.ids && strings == other.strings);
            }

            bool operator!=(const Record& other)
            {
                return !(*this == other);
            }

        private:
            unsigned int binary_status;

            friend std::string serialize(Record& rec);
            friend Record deserialize(std::string& data);

    };

    std::string serialize(Record& rec)
    {
        nlohmann::json jsondata;
        jsondata["ids"] = rec.ids;
        jsondata["strings"] = rec.strings;
        std::string dumped;
        if (rec.binary_status == 0)
        {
            dumped = jsondata.dump();
        }
        else
        {
            std::vector<uint8_t> bsondata;
            if (rec.binary_status == 1)
            {
                try
                {
                    bsondata = nlohmann::json::to_bson(jsondata);
                }
                catch(nlohmann::json::exception& e)
                {
                    fprintf(stderr, "Error occured while converting JSON object to BSON:\n");
                    fprintf(stderr, "    Message: %s\n", e.what());
                    fprintf(stderr, "    Exception ID: %d\n", e.id);
                    throw;
                }
            }
            else if (rec.binary_status == 2)
            {
                try
                {
                    bsondata = nlohmann::json::to_cbor(jsondata);
                }
                catch(nlohmann::json::exception& e)
                {
                    fprintf(stderr, "Error occured while converting JSON object to BSON:\n");
                    fprintf(stderr, "    Message: %s\n", e.what());
                    fprintf(stderr, "    Exception ID: %d\n", e.id);
                    throw;
                }
            }
            else if (rec.binary_status == 3)
            {
                try
                {
                    bsondata = nlohmann::json::to_msgpack(jsondata);
                }
                catch(nlohmann::json::exception& e)
                {
                    fprintf(stderr, "Error occured while converting JSON object to BSON:\n");
                    fprintf(stderr, "    Message: %s\n", e.what());
                    fprintf(stderr, "    Exception ID: %d\n", e.id);
                    throw;
                }
            }
            else
            {
                try
                {
                    bsondata = nlohmann::json::to_ubjson(jsondata);
                }
                catch(nlohmann::json::exception& e)
                {
                    fprintf(stderr, "Error occured while converting JSON object to BSON:\n");
                    fprintf(stderr, "    Message: %s\n", e.what());
                    fprintf(stderr, "    Exception ID: %d\n", e.id);
                    throw;
                }
            }
            dumped = std::string(bsondata.begin(), bsondata.end());
        }
        return dumped;
    }

    Record deserialize(std::string& data, unsigned int binary)
    {
        nlohmann::json jsondata;
        if (binary == 0)
        {
            jsondata = nlohmann::json::parse(data);
        }
        else
        {
            if (binary == 1)
            {
                try
                {
                    jsondata = nlohmann::json::from_bson(std::vector<uint8_t>(data.begin(), data.end()));
                }
                catch(nlohmann::json::exception& e)
                {
                    fprintf(stderr, "Error occured while converting BSON back into JSON object:\n");
                    fprintf(stderr, "    Message: %s\n", e.what());
                    fprintf(stderr, "    Exception ID: %d\n", e.id);
                    throw;
                }
            }
            else if (binary == 2)
            {
                try
                {
                    jsondata = nlohmann::json::from_cbor(std::vector<uint8_t>(data.begin(), data.end()));
                }
                catch(nlohmann::json::exception& e)
                {
                    fprintf(stderr, "Error occured while converting BSON back into JSON object:\n");
                    fprintf(stderr, "    Message: %s\n", e.what());
                    fprintf(stderr, "    Exception ID: %d\n", e.id);
                    throw;
                }
            }
            else if (binary == 3)
            {
                try
                {
                    jsondata = nlohmann::json::from_msgpack(std::vector<uint8_t>(data.begin(), data.end()));
                }
                catch(nlohmann::json::exception& e)
                {
                    fprintf(stderr, "Error occured while converting BSON back into JSON object:\n");
                    fprintf(stderr, "    Message: %s\n", e.what());
                    fprintf(stderr, "    Exception ID: %d\n", e.id);
                    throw;
                }
            }
            else
            {
                try
                {
                    jsondata = nlohmann::json::from_ubjson(std::vector<uint8_t>(data.begin(), data.end()));
                }
                catch(nlohmann::json::exception& e)
                {
                    fprintf(stderr, "Error occured while converting BSON back into JSON object:\n");
                    fprintf(stderr, "    Message: %s\n", e.what());
                    fprintf(stderr, "    Exception ID: %d\n", e.id);
                    throw;
                }
            }
        }
        Record rec(binary);
        try
        {
            rec.ids = jsondata["ids"].get<Integers>();
        }
        catch(nlohmann::json::exception& e)
        {
            fprintf(stderr, "Error occured while getting ids:\n");
            fprintf(stderr, "    Message: %s\n", e.what());
            fprintf(stderr, "    Exception ID: %d\n", e.id);
            throw;
        }
        try
        {
            rec.strings = jsondata["strings"].get<Strings>();
        }
        catch(nlohmann::json::exception& e)
        {
            fprintf(stderr, "Error occured while getting strings:\n");
            fprintf(stderr, "    Message: %s\n", e.what());
            fprintf(stderr, "    Exception ID: %d\n", e.id);
            throw;
        }
        return rec;
    }

}

#endif
