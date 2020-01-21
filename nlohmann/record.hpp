#ifndef __NLOHMANN_RECORD_HPP_INCLUDED__
#define __NLOHMANN_RECORD_HPP_INCLUDED__

#include <vector>
#include <string>
#include <sstream>

#include <nlohmann/json.hpp>

namespace nlohmann_test 
{
    std::string serialize(Record& rec);
    Record deserialize(std::string& data, unsigned int binary=0);

    typedef std::vector<int64_t>     Integers;
    typedef std::vector<std::string> Strings;

    class Record
    {
        public:
            Integers ids;
            Strings  strings;

            Record(unsigned int binary=0)
                : binary_status(binary)
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
            std::vector<uint8_t> bsondata = json::to_bson(jsondata);
            dumped = std::string(bsondata.begin(), bsondata.end());
        }
        return dumped;
    }

    Record deserialize(std::string& data, unsigned int binary)
    {
        nlohmann::json jsondata;
        if (binary == 0)
        {
            jsondata = json::parse(data);
        }
        else
        {
            jsondata = json::from_bson(std::vector<uint8_t>(data.begin(), data.end()));
        }
        Record rec(binary);
        rec.ids = jsondata["ids"];
        rec.strings = jsondata["strings"];
        return rec;
    }

}

#endif
