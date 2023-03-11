//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

//Finder boost, asio, ip og tcp frem fra libary for at kunne til gå det
using boost::asio::ip::tcp;


int main(int argc, char* argv[])
{
  //Try block, dette har samme funktion som i daytime_server
  try
  {
    //Hvis argc (socket nummer?) ikke er lig 2, bliver programmet stoppet.
    //Da det ikke ikke stemmer overens med den host som klienten prøver at forbinde til.
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }
  
    boost::asio::io_context io_context;

    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints =
      resolver.resolve(argv[1], "daytime");

    //Laver en socket, som i server
    tcp::socket socket(io_context);
    //Bruger socket og endpoint til at forbinde til server
    boost::asio::connect(socket, endpoints);

    //Endless loop
    while(true)
    {
      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      //If funktion der tjekker for errors
      if (error == boost::asio::error::eof)
        //Hvis filen er læst til ende bliver programmet stoppet
        break; // Connection closed cleanly by peer.
      else if (error)
        //Ellers
        throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}