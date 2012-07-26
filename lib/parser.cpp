#include "parser.h"

Parser::Parser()
{
}
PlayStructList Parser::ReadPlaylist( std::istream & is )
{
      //using namespace std;
      // populate tree structure pt
      read_xml(is, pt);
      // traverse pt
      try
      {
          BOOST_FOREACH(const ptree::value_type &v, pt.get_child("playlist.tracklist"))
          {
              if(v.first == "track")
              {
                  PlayStruct f;
                  f.song_name = v.second.get<std::string>("title");
                  f.song_url = v.second.get<std::string>("location");
                  ans_playlist.push_back(f);
              }
          }
      }
      catch(std::exception const& e)
      {

      }

      return ans_playlist;
}


SearchListList Parser::ReadSearchlist( std::istream & is )
{
      //using namespace boost;
      //using namespace std;
      // populate tree structure pt
      //using boost::property_tree::ptree;
      //ptree pt;
      read_xml(is, pt);

      try
      {
          // traverse pt
          BOOST_FOREACH(const ptree::value_type &v, pt.get_child("response"))
          {
              if(v.first == "audio")
              {
                  SearchList f;
                  f.aid = v.second.get<int>("aid");
                  f.owner_id = v.second.get<int>("owner_id");
                  f.artist = v.second.get<std::string>("artist");
                  f.title = v.second.get<std::string>("title");
                  f.duration = v.second.get<std::string>("duration");
                  f.url = v.second.get<std::string>("url");
                  //f.lyrics_id = v.second.get<int>("lyrics_id");
                  ans_searchlist.push_back(f);
              }
          }
      }
      catch(std::exception const& e)
      {

      }

      return ans_searchlist;
}
