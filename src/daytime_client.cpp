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
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    //Laver en io_context variabel 
    boost::asio::io_context io_context;

    //Laver en resovler som har io_context som input
    tcp::resolver resolver(io_context);
    //Definer endpoint, der siger hvilken server der skal forbindes til
    tcp::resolver::results_type endpoints =
      resolver.resolve(argv[1], "daytime");

    //Laver en socket med io_context som input
    tcp::socket socket(io_context);
    
    //Forbinder til endpoint med den socket der er blevet difineret
    boost::asio::connect(socket, endpoints);

    //Endless loop
    while(true)
    {
      //Laver et array variabel
      boost::array<char, 128> buf;
      //Laver en error msg variabel
      boost::system::error_code error;

      //Definer længden af day_time string
      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      //If funktion der tjekker for errors
      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        //Ellers bliver error'en smidt på en stack som catch kan bruge til at skrive fejlen
        throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
    }
  }
  catch (std::exception& e)
  {
    //Skriver error fra throw i try-blocken
    std::cerr << e.what() << std::endl;
  }

  return 0;
}