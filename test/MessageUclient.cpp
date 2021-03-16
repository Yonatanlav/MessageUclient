#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <cpr/cpr.h>
#include <WinSock2.h>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <iostream>
#include <ctime>
//#include <unistd.h>
#include "json.hpp"

using json = nlohmann::json;
static int myId;


std::string gen_random(const int len) {

    std::string tmp_s;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    srand((unsigned)time(NULL) * _getpid());

    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];


    return tmp_s;

}


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

                auto publicKey = gen_random(20);


                json request = json::parse("{ \"Name\": \"" + firstName + "\", \"PublicKey\":\"" + publicKey + "\"}");

                auto newuser = cpr::Post(
                    cpr::Url{ "http://127.0.0.1:8000/client/" },
                    cpr::Authentication{ "admin", "admin" },
                    cpr::Body{ request.dump() },
                    cpr::Header{ { "Content-Type", "application/json" } });


                
                auto clients = cpr::Get(
                    cpr::Url{ "http://127.0.0.1:8000/client/" },
                    cpr::Authentication{ "admin", "admin" });
                json j = json::parse(clients.text);
                json id;
                for (json::iterator it = j.begin(); it != j.end(); ++it) {
                    if ((*it)["Name"] == firstName) {
                        std::cout << (*it)["id"] << '\n';
                        //std::cout << "test is:\n";
                        myId = (*it)["id"].get<std::int32_t>();
                    }
                }
                

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
                int toclient;
                std::cout << "All messages of specific user from MessageU is :\n";
                //std::cout << "\nType your client id:\n";
                //std::cin >> toclient;
                std::string toclient2 = std::to_string(myId);
                auto messages = cpr::Get(
                    cpr::Url{ "http://127.0.0.1:8000/message/" },
                    cpr::Authentication{ "admin", "admin" });

                json j = json::parse(messages.text);
                for (json::iterator it = j.begin(); it != j.end(); ++it) {
                    if ((*it)["ToClient"] == toclient2) {
                        std::cout << (*it)["Blob"] << '\n';
                    }
                    
                }
            }
            //http post public keys from http://127.0.0.1:8000/message/
            if (x == 5) {
                
                //get detail from client 
                std::string type = "1";
                std::string message;
                std::cout << "Type your message:\n ";
                std::cin >> message; // get user input from the keyboard
                std::cout <<  "write the id of the user you want to send to \n";

                auto clients = cpr::Get(
                    cpr::Url{ "http://127.0.0.1:8000/client/" },
                    cpr::Authentication{ "admin", "admin" });

                json j = json::parse(clients.text);
                for (json::iterator it = j.begin(); it != j.end(); ++it) {
                    std::cout << (*it)["id"] << " " << (*it)["Name"] << '\n';
                }
                
                int Fromclient;
                int id;
                std::cout << "\nType client id:\n";
                std::cin >> id;
                //std::cout << "\nType your client id:\n";
                //std::cin >> Fromclient;
                
                std::string toClient = std::to_string(id);
                std::string Fromclient2 = std::to_string(myId);
                
                

                json messegarequest = json::parse("{ \"ToClient\": \"" + toClient + "\", \"FromClient\":\"" + Fromclient2 + "\", \"Type\":\"" + type + "\", \"Blob\":\"" + message + "\"}");
                
                    cpr::Post(cpr::Url{ "http://127.0.0.1:8000/message/" },
                    cpr::Authentication{ "admin", "admin" },
                    cpr::Body{ messegarequest.dump() },
                    cpr::Header{ { "Content-Type", "application/json" } });         
            }
            if (x == 51) {

            }
            if (x == 52) {

            }
        }

        return 0;

    }


}
