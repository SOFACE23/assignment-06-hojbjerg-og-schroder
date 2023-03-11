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
    //Laver en var som indeholder det input/output som serveren skal sende/InDdhØsTe
    boost::asio::io_context io_context;
    //Laver en acceptor var som io_context input og skal bruge et endpoint(socket) som input
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));

    //Et loop der kører for evigt indtil programmet slukkes
    while (true)
    {
      //Laver en tcp::socket variabel som hedder socket, som tager io_context som input
      tcp::socket socket(io_context);
      //Skal accepter en forbinelse med klienten og få endpoint
      acceptor.accept(socket);

      //Gemmer content af make_daytime_string funktionen i message 
      std::string message = make_daytime_string();

      //Laver en error code som programmet bare skal ignorer, da write funktionen skal bruge en error msg som input
      boost::system::error_code ignored_error;
      //Skriver message og ignored_error til socket
      boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
    }
  }
  //Vis intet i try-blocken virker, skal programmet printe hvor i blocken det gik galt
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}