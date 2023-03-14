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

//Finder boost, asio, ip og tcp frem fra libary, da det skal bruges i programmet
using boost::asio::ip::tcp;

//Laver en string funktion, som returner nuværnende kl. og dato
std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  //Laver en typedef som hedder "now" som indeholder time.
  //time(0) er den funktion som henter den nuværende tid
  time_t now = time(0);

  //Retuner ctime med adressen til now som input
  return ctime(&now);
}

int main()
{
  //Prommet for afvide den skal prøve disse linjer af koder i instruktionen 
  try
  {
    //Laver en io_context som skal bruges til at forbinde til serveren
    boost::asio::io_context io_context;
    //Laver en acceptor som bruger v4 protokol og port 13 (typisk for processing time)
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));

    //Et loop der kører for evigt indtil programmet slukkes
    while (true)
    {
      //Laver en socket med io_context
      tcp::socket socket(io_context);
      //Skal accepter socket som håndter information for serveren
      acceptor.accept(socket);

      //Gemmer content af make_daytime_string funktionen i message 
      std::string message = make_daytime_string();

      //Laver en error msg var
      boost::system::error_code ignored_error;
      //Skriver message til server, samt socket og error msg
      boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
    }
  }
  //Viser hvilken fejl der skete i try-blocken (hvis der skete en fejl)
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}