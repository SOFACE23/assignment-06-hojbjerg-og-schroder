//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <vector>
#include <fstream>

using boost::asio::ip::tcp;

std::vector<uint8_t> get_image(const char* file)
{
  //Laver en variabel der skal indeholde længden af filen
  std::streampos size;
  //Tager filen og specifier den som en binary file 
  std::ifstream binaryFile(file, std::ifstream::binary);

  //angiveeer længden an filen
  binaryFile.seekg(0, binaryFile.end);
  size = binaryFile.tellg();
  binaryFile.seekg(0, binaryFile.beg);

  //laver data som indeholder størrelsen som en vektor
  std::vector<uint8_t> data(size);

  //læser filen
  binaryFile.read((char*) &data[0], size);

  //Returner binary dataen
  return data;
  
}

//Mange af kommentarene fra daytime_server beskriver også hvad der sker her
int main()
{
  try
  {
    boost::asio::io_context io_context;

    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));

    while (true)
    {
      tcp::socket socket(io_context);
      acceptor.accept(socket);

      auto message = get_image("cat.jpg");

      boost::system::error_code ignored_error;
      boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
    }
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}