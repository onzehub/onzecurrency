#include "poco.hpp"

using namespace Crowd;

Poco::Poco()
{
    leveldb::Options options;
    options.create_if_missing = true;
    s = leveldb::DB::Open(options, usersdb_folder_path, &db);
    std::cout << "s == ok: " << s.ok() << " " << usersdb_folder_path << std::endl;
}
std::string Poco::Get(uint32_t key)
{
    std::stringstream ss;
    ss << key;
    s = db->Get(leveldb::ReadOptions(), ss.str(), &value);
    if (s.ok())
    {
        if (value == "test") return ""; // A hack, somehow leveldb's value is 'test' when there is no entry
        return value;
    }
    else
    {
        return "";
    }
}
bool Poco::Put(uint32_t key, std::string value) // TODO: value must be json!!
{
    std::stringstream ss;
    ss << key;
    s = db->Put(leveldb::WriteOptions(), ss.str(), value);
    if (s.ok()) return true;
    else return false;    
}
bool Poco::Delete(uint32_t key)
{
    std::stringstream ss;
    ss << key;
    leveldb::Status s = db->Delete(leveldb::WriteOptions(), ss.str());
    if (s.ok()) return true;
    else return false;
}
uint32_t Poco::FindChosenOne(uint32_t key)
{
    std::stringstream ss;
    ss << key;

    std::string string_key_real_chosen_one;

    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next())
    {
        if (it->key().ToString() == ss.str())
        {
            string_key_real_chosen_one = it->key().ToString();
            break;
        }
        else if (it->key().ToString() > ss.str())
        {
            for (it->SeekToFirst(); it->Valid(); it->Next())
            {
                string_key_real_chosen_one = it->key().ToString();
                break;
            }
            break;
        }
    }
    delete it;

    std::istringstream iss (string_key_real_chosen_one);
    uint32_t key_real_chosen_one;
    iss >> key_real_chosen_one;
    if (iss.fail())
    {
        std::cerr << "ERROR in creating the uint for the key_real_chosen_one!\n";
        return 1;
    }

    return key_real_chosen_one;
}
uint32_t Poco::FindNextPeer(uint32_t key)
{
    std::stringstream ss;
    ss << (key + 1);

    std::string string_key_next_peer;

    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next())
    {
        if (it->key().ToString() >= ss.str())
        {
            string_key_next_peer = it->key().ToString();
            break;
        }
    }
    delete it;

    std::istringstream iss (string_key_next_peer);
    uint32_t key_next_peer;
    iss >> key_next_peer;
    if (iss.fail())
    {
        std::cerr << "ERROR in creating the uint for the key_next_peer!\n";
        return 1;
    }

    return key_next_peer;
}
uint32_t Poco::FindUpnpPeer(uint32_t key)
{
    std::stringstream ss;
    ss << (key + 1);

    std::string string_key_upnp_peer;

    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next())
    {
        if (it->key().ToString() >= string_key_upnp_peer)
        {
            // TODO: search in it->value for upnp enabledness, see FindChosenOne or FindNextPeer functions
        }
    }
    delete it;

    std::istringstream iss (string_key_upnp_peer);
    uint32_t key_upnp_peer;
    iss >> key_upnp_peer;
    if (iss.fail())
    {
        std::cerr << "ERROR in creating the uint for the key_upnp_peer!\n";
        return 1;
    }

    return key_upnp_peer;
}