/*********************************************************************
 PROGRAM:    CSCI 330 Assignment 9
 PROGRAMMER: Michael Rizza
 ￼LOGON ID:Z1035469
 DUE DATE:4/15/2015
 FUNCTION:UDP client program
 *********************************************************************/

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <iomanip>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <string>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>

using namespace std;

int main() {
    
    //variables
    char message[64];
    int cardNumlen, sock, echolen, received = 0, blankCount = 0, digits;
    char buffer[256];
    string cardAmt, cardAmtLen, cardName, expDate, cardNum;
    unsigned int addrlen;
    
    struct sockaddr_in server;  // structure for address of server
    
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Failed to create socket"); exit(EXIT_FAILURE); }
    // Construct the server sockaddr_in structure
    memset(&server, 0, sizeof(server));       // Clear struct
    server.sin_family = AF_INET;                  // Internet IP
    server.sin_addr.s_addr = inet_addr("127.0.0.1");  // IP address
    server.sin_port = htons(4445);       // server port
   
   
    //Welcome the user
    cout << "Welcome to the Credit Card Transaction verification utility!\n";
    
    //ask user for card holder name or quit
    cout << "Enter card holder name (or quit): ";
    getline(cin,cardName);
    
    //check name is less than 64 characters
    while (strlen(cardName.c_str()) > 64)
    {
        //ask user for card holder name or quit
        cout<<"Name must be less than 64 characters."<<endl;
        cout << "Enter card holder name (or quit): ";
        getline(cin,cardName);
    }
    
    //Make loop that will quit
    while(strcmp(cardName.c_str(), "quit") != 0)
    {
        //Ask for numbers
        cout << "Enter CC number: ";
        getline(cin, cardNum);
        
        //count number of digits
        cardNumlen = strlen(cardNum.c_str());
       
        //check for spaces
        for (int i=0; i<cardNumlen;i++)
        {
            if (cardNum[i] == ' ')
            blankCount++;
        }
        
        //subtract spaces
        digits = cardNumlen - blankCount;
        
        
        //make sure there are 15-16 digits
        while (digits < 15 or digits > 16)
        {
            cout << "You have entered an invalid CC number. Please try again: ";
            getline(cin, cardNum);

            
            cardNumlen = strlen(cardNum.c_str());
            
            blankCount = 0;  //reset blankcountA
            
            for (int i=0; i<cardNumlen;i++)
            {
                if (cardNum[i] == ' ')
                    blankCount++;
            }
            digits = cardNumlen - blankCount;
            
        }
        
        
        
       //ask for experation date
        
        cout << "Enter experation (form mm/yyyy): ";
         getline(cin,expDate);
        
        
        //check for correct expdate
        while (strlen(expDate.c_str()) != 7)
        {
            cout<<"...experiration date invalid. Try again (form mm/yyyy): ";
            getline(cin,expDate);
        }
        
        string month = expDate.substr(0,2);
    
       
        while (atoi(month.c_str())>=13)
        {
            cout<<"...month invalid. Try again (form mm/yyyy): ";
            getline(cin,expDate);
            
            month = expDate.substr(0,2);
            
        }

        //ask for card amount
        cout << "Enter amount (form $$.$$): ";
        
        getline(cin,cardAmt);
        cout<<endl;
        
        //check for two decimal places
        cardAmtLen = cardAmt.substr(cardAmt.find(".")+1);
        while (strlen(cardAmtLen.c_str()) != 2)
        {
            cout<<" You have entered an invalid amount."<<endl;
           // ask for card amount
            cout << "Enter amount (form $$.$$): ";
        
        getline(cin,cardAmt);
        cardAmtLen = cardAmt.substr(cardAmt.find(".")+1);

        }
        
    //make message
        strcpy(message, cardNum.c_str());
        strcat(message, ":");
        strcat(message, expDate.c_str());
        strcat(message, ":");
        strcat(message, cardAmt.c_str());
        strcat(message, ":");
        strcat(message, cardName.c_str());
    
        // Send the message to the server
        echolen = strlen(message);
        if (sendto(sock, message, strlen(message), 0,
                   (struct sockaddr *) &server, sizeof(server)) != echolen) {
            perror("Mismatch in number of sent bytes"); exit(EXIT_FAILURE);}
        
        // Receive the message back from the server
        addrlen = sizeof(server);
        received = recvfrom(sock, buffer, 256, 0, (struct sockaddr *) &server, &addrlen);
        
        buffer[received] = '\0';
        cout << "Message received: " << buffer << endl;
    
        
        //ask user for card holder name or quit
        cout << "Enter card holder name (or quit): ";
        getline(cin,cardName);
        
        //check name is less than 64 characters
        while (strlen(cardName.c_str()) > 64)
        {
            //ask user for card holder name or quit
            cout<<"Name must be less than 64 characters."<<endl;
            cout << "Enter card holder name (or quit): ";
            getline(cin,cardName);
        }
        
        
    }
    return 0;
    
}





