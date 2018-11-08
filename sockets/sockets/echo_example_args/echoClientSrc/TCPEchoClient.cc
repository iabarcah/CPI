/*
 *   C++ sockets on Unix and Windows
 *   Copyright (C) 2002
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "YASL.h"      // For Socket and SocketException
#include "checkArgs.h"
#include <iostream>    // For cerr and cout
#include <cstdlib>     // For atoi()
#include <fstream>
#include <iomanip>
const uint64_t RCVBUFSIZE = 4096;    // Size of receive buffer

int main(int argc, char *argv[]) {

	checkArgs* argumentos = new checkArgs(argc, argv);
	
    std::string servAddress; 
	uint16_t    echoServPort;
    std::string echoString;
    std::string archivo;   
	
	servAddress   = argumentos->getArgs().SERVER;
	echoServPort  = argumentos->getArgs().PORT;
	echoString    = argumentos->getArgs().DATA;
	archivo = argumentos->getArgs().archivo;
	delete argumentos;
	ofstream archivo;  // objeto de la clase ofstream
	uint64_t echoStringLen = echoString.length();   // Determine input leng64
	archivo=fopen("datos.txt",rw);
	try {
		// Establish connection with the echo server
		TCPSocket sock(servAddress, echoServPort);

		// Send the string to the echo server
		sock.send(echoString.c_str(), echoStringLen);

		char echoBuffer[RCVBUFSIZE + 1];    // Buffer for echo string + \0
		uint64_t bytesReceived = 1;              // Bytes read on each recv()
		uint64_t totalBytesReceived = 0;         // Total bytes read

													// Receive the same string back from the server
		std::cerr << "Received: ";               // Setup to print the echoed string
		while (totalBytesReceived < echoStringLen) {
			std::cerr << totalBytesReceived;
			std::cerr << "\n";
			std::cerr << bytesReceived;
			//std::cerr << "entro";
			// Receive up to the buffer size bytes from the sender
			if ((bytesReceived = (sock.recv(echoBuffer, RCVBUFSIZE))) <= 0) {
				std::cerr << "Unable to read";
				exit(EXIT_FAILURE);

			}

			totalBytesReceived += bytesReceived;     // Keep tally of total bytes
			echoBuffer[bytesReceived] += '\0';        // Terminate the string!
			std::cerr << echoBuffer;                      // Print the echo buffer
			archivo << echoBuffer ;
		}
//		fclose(archivo);
		std::cerr << "salio";
		std::cerr << std::endl;

		// Destructor closes the socket

	} catch(SocketException &e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
