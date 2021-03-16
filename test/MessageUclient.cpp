#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <cpr/cpr.h>
#include <WinSock2.h>
#include <Windows.h>
#include <conio.h>
#include <string>
#include "json.hpp"

using json = nlohmann::json;

// MessageUclient.cpp : This file contains the 'main' function. Program execution begins and ends there.
int main(int argc, char** argv) {    
    {
        int x = 99;
        while (x != 0)
        {
            //choices
            std::cout << "1)Register \n";
            std::cout << "2)request for client list\n";
            std::cout << "3)Request for public key\n";
            std::cout << "4)Request for waiting messages\n";
            std::cout << "5)Send a text message\n";
            std::cout << "51)Send a request for symmetric key\n";
            std::cout << "52)Send your symmetric key\n";
            std::cout << "0)Exit client\n";

            std::cout << "press your choise: "; // Type a number and press enter
            std::cin >> x; // Get user input from the keyboard

            //Register to http://127.0.0.1:8000
            if (x == 1) {
                //get detail from client 
                std::string firstName;
                std::cout << "Type your first name: ";
                std::cin >> firstName; // get user input from the keyboard
                std::cout << "Your name is: " << firstName + " is registered to the system\n";
                
                //Post http://127.0.0.1:8000/client with firstName and generated public key

                auto publicKey = "key";

                json request = json::parse("{ \"Name\": \"" + firstName + "\", \"PublicKey\":\"" + publicKey + "\"}");

                auto newuser = cpr::Post(
                    cpr::Url{ "http://127.0.0.1:8000/client/" },
                    cpr::Authentication{ "admin", "admin" },
                    cpr::Body{ request.dump() },
                    cpr::Header{ { "Content-Type", "application/json" } });
            }
            //http get clients from http://127.0.0.1:8000/client/
            if (x == 2) {
                std::cout << "Client list of MessageU is :\n";
                auto clients = cpr::Get(
                    cpr::Url{ "http://127.0.0.1:8000/client/" },
                    cpr::Authentication{ "admin", "admin" });

                json j = json::parse(clients.text);
                for (json::iterator it = j.begin(); it != j.end(); ++it) {
                    std::cout << (*it)["Name"] << '\n';
                }
            }
            //http get public keys from http://127.0.0.1:8000/client/
            if (x == 3) {
                std::cout << "Publickeys list of MessageU is :\n";
                auto publickeys = cpr::Get(
                    cpr::Url{ "http://127.0.0.1:8000/client/" },
                    cpr::Authentication{ "admin", "admin" });

                json j = json::parse(publickeys.text);
                for (json::iterator it = j.begin(); it != j.end(); ++it) {
                    std::cout << (*it)["id"] << " " << (*it)["Name"] << '\n';
                }

                int id;
                std::cout << "\nType client id:\n";
                std::cin >> id;

                for (json::iterator it = j.begin(); it != j.end(); ++it) {
                    if ((*it)["id"] == id) {
                        std::cout << (*it)["PublicKey"] << '\n';
                    }
                }
            }
            
            if (x == 4) {
                std::cout << "All messages from MessageU is :\n";
                auto messages = cpr::Get(
                    cpr::Url{ "http://127.0.0.1:8000/message/" },
                    cpr::Authentication{ "admin", "admin" });

                json j = json::parse(messages.text);
                for (json::iterator it = j.begin(); it != j.end(); ++it) {
                    std::cout << (*it)["Blob"] << '\n';
                }
            }
            //http post public keys from http://127.0.0.1:8000/message/
            if (x == 5) {
                auto newmessage = cpr::Post(
                    cpr::Url{ "http://127.0.0.1:8000/message/" },
                    cpr::Authentication{ "admin", "admin" },
                    cpr::Body{ R"({"ToClient": "2" ,"FromClient": "2","Type": "2" ,"Blob": "Hi friend"})" },
                    cpr::Header{ { "Content-Type", "application/json" } });

                std::cout << newmessage.text << '\n';

            }
            if (x == 51) {

            }
            if (x == 52) {

            }
        }

        return 0;

    }


}
