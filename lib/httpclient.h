/*
 * =====================================================================================
 *
 *       Filename:  httpclient.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05.07.2012 12:45:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  vasy (vasy.chan@gmail.com),
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "configure.h"

using boost::asio::ip::tcp;

// namespace http
// {

    class HttpClient
    {
    public:
      HttpClient(boost::asio::io_service& io_service,
          const std::string& server, const std::string& path)
        : resolver_(io_service),
          socket_(io_service)
      {
        // Form the request. We specify the "Connection: close" header so that the
        // server will close the socket after transmitting the response. This will
        // allow us to treat all data up until the EOF as the content.
        std::ostream request_stream(&request_);
        //std::string decode_path = urlDecode(path);
        std::string decode_path = path;
        boost::replace_all(decode_path, " ", "%20");
        request_stream << "GET " << decode_path << " HTTP/1.0\r\n";
        request_stream << "Host: " << server << "\r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Connection: close\r\n\r\n";

        // Start an asynchronous resolve to translate the server and service names
        // into a list of endpoints.
        tcp::resolver::query query(server, "http");
        resolver_.async_resolve(query,
            boost::bind(&HttpClient::handle_resolve, this,
              boost::asio::placeholders::error,
              boost::asio::placeholders::iterator));
      }
    
    std::string getResponse()
    {
        return ss.str();
    }

    void clearResponse()
    {
        ss.str("");
    }

    private:
      void handle_resolve(const boost::system::error_code& err,
          tcp::resolver::iterator endpoint_iterator)
      {
        if (!err)
        {
          // Attempt a connection to the first endpoint in the list. Each endpoint
          // will be tried until we successfully establish a connection.
          tcp::endpoint endpoint = *endpoint_iterator;
          socket_.async_connect(endpoint,
              boost::bind(&HttpClient::handle_connect, this,
                boost::asio::placeholders::error, ++endpoint_iterator));
        }
        else
        {
          //std::cout << "Error: " << err.message() << "\n";
        }
      }

      void handle_connect(const boost::system::error_code& err,
          tcp::resolver::iterator endpoint_iterator)
      {
        if (!err)
        {
          // The connection was successful. Send the request.
          boost::asio::async_write(socket_, request_,
              boost::bind(&HttpClient::handle_write_request, this,
                boost::asio::placeholders::error));
        }
        else if (endpoint_iterator != tcp::resolver::iterator())
        {
          // The connection failed. Try the next endpoint in the list.
          socket_.close();
          tcp::endpoint endpoint = *endpoint_iterator;
          socket_.async_connect(endpoint,
              boost::bind(&HttpClient::handle_connect, this,
                boost::asio::placeholders::error, ++endpoint_iterator));
        }
        else
        {
          std::cout << "Error: " << err.message() << "\n";
        }
      }

      void handle_write_request(const boost::system::error_code& err)
      {
        if (!err)
        {
          // Read the response status line. The response_ streambuf will
          // automatically grow to accommodate the entire line. The growth may be
          // limited by passing a maximum size to the streambuf constructor.
          boost::asio::async_read_until(socket_, response_, "\r\n",
              boost::bind(&HttpClient::handle_read_status_line, this,
                boost::asio::placeholders::error));
        }
        else
        {
          std::cout << "Error: " << err.message() << "\n";
        }
      }

      void handle_read_status_line(const boost::system::error_code& err)
      {
        if (!err)
        {
          // Check that response is OK.
          std::istream response_stream(&response_);
          std::string http_version;
          response_stream >> http_version;
          unsigned int status_code;
          response_stream >> status_code;
          std::string status_message;
          std::getline(response_stream, status_message);
          if (!response_stream || http_version.substr(0, 5) != "HTTP/")
          {
            std::cout << "Invalid response\n";
            return;
          }
          if (status_code != 200)
          {
            std::cout << "Response returned with status code ";
            std::cout << status_code << "\n";
            return;
          }

          // Read the response headers, which are terminated by a blank line.
          boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
              boost::bind(&HttpClient::handle_read_headers, this,
                boost::asio::placeholders::error));
        }
        else
        {
          std::cout << "Error: " << err << "\n";
        }
      }

      void handle_read_headers(const boost::system::error_code& err)
      {
        if (!err)
        {
          // Process the response headers.
          std::istream response_stream(&response_);
          std::string header;
          while (std::getline(response_stream, header) && header != "\r")
            //std::cout << "HEADER VASY" << "\n";
            std::cout << header << "\n";
          std::cout << "\n";

          // Write whatever content we already have to output.
          //if (response_.size() > 0)
           // std::cout << &response_;

          // Start reading remaining data until EOF.
          boost::asio::async_read(socket_, response_,
              boost::asio::transfer_at_least(1),
              boost::bind(&HttpClient::handle_read_content, this,
                boost::asio::placeholders::error));
        }
        else
        {
          //std::cout << "Error: " << err << "\n";
        }
      }

      void handle_read_content(const boost::system::error_code& err)
      {
        if (!err)
        {
          // Write all of the data that has been read so far.
          ss << &response_;
          //response_out << &response_;

          // Continue reading remaining data until EOF.
          boost::asio::async_read(socket_, response_,
              boost::asio::transfer_at_least(1),
              boost::bind(&HttpClient::handle_read_content, this,
                boost::asio::placeholders::error));
        }
        else if (err != boost::asio::error::eof)
        {
          //std::cout << "Error: " << err << "\n";
        }
      }
        // std::string urlDecode(const std::string &SRC) 
        // {
        //     std::cout  << "VASY  urlDecodea" << "\n";
        //     std::string ret;
        //     char ch;
        //     int i, ii;
        //     for (i=0; i<SRC.length(); i++) {
        //         std::cout << " SRC[i] = " <<  SRC[i] << "\n" ;
        //         if (int(SRC[i])==37) {
        //             sscanf(SRC.substr(i+1,2).c_str(), "%x", &ii);
        //             ch=static_cast<char>(ii);
        //             ret+=ch;
        //             i=i+2;
        //         }
        //         else if (SRC[i] == " ")
        //         {
        //             ret+="%20";
        //         }
        //         else 
        //         {
        //             ret+=SRC[i];
        //         }
        //     }
        //     return (ret);
        // }


      tcp::resolver resolver_;
      tcp::socket socket_;
      boost::asio::streambuf request_;
      boost::asio::streambuf response_;
      std::ostringstream ss;
    };



//}

#endif
