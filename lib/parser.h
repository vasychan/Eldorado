#ifndef PARSER_H
#define PARSER_H
#include "configure.h"
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <algorithm>

typedef boost::gregorian::date Date;

struct PlayStruct
{
        std::string  song_name;
        std::string  song_url;
};

typedef std::vector<PlayStruct> PlayStructList;


struct SearchList
{
        int  aid;
        int  owner_id;
        std::string  artist;
        std::string  title;
        std::string  duration;
        std::string  url;
        //int  lyrics_id;
};

typedef std::vector<SearchList> SearchListList;

using boost::property_tree::ptree;
using namespace boost;

class Parser
{
public:
    Parser();
    PlayStructList ReadPlaylist( std::istream & is);
    SearchListList ReadSearchlist( std::istream & is);

private:
    ptree pt;
    PlayStructList ans_playlist;
    SearchListList ans_searchlist;
};

#endif // PARSER_H
