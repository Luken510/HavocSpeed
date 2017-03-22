#pragma once

#pragma once

/**
* @Author	Luke Newell
* @date		2nd March 2017
* @brief	Logging class for errors and messages
*/

#define Location "logfile.txt"

#include <iostream>
#include <fstream>
#include <string>
namespace UTIL 
{

class LOG {

	public:
		/*!
		\brief Enum of different Messages
		*/
		enum Type { DEBUG, INFO, WARNING, FAULT};

		/*!
		\brief Constructor
		*/
		LOG() {}
		/*!
		\brief Constructor.
		\param type Enum of what message to send
		*/
		LOG(Type type) 
		{
			currentType = type; 

			if (outputToFile) 
			{
				logFile.open(Location, std::ios_base::app); // sets the position of the message
			}

			if (headers) 
			{
				operator<< ("[" + GetLabel(type) + "] ");
			}
		}

		/*!
		\brief Destructor.
		*/
		~LOG() 
		{
			if (opened) 
			{
				if (outputToConsole) //finish by creating a new line
					std::cout << std::endl;

				if (outputToFile) 
				{
					logFile << "\n"; // creates a new line and ends the write
					logFile.close();
				}
			}
			opened = false;
		}

		/*!
		\brief Clears the log file.
		*/
		static void Clear() {
			std::fstream logFile;
			logFile.open(Location, std::ios::out);
			logFile.close();
		}

		/*!
		\brief Overloaded stream operator, for passing the message
		\param msg Message to display
		\return Reference to the logger
		*/
		template<typename T>
		LOG& operator<<(const T& msg) 
		{
			if (currentType >= minType) 
			{
				if (outputToConsole) // if debugging report to console
					std::cout << msg;

				opened = true;

				if (outputToFile) 
					logFile << msg;
				
			}
			return *this;
		}
	private:

		/*!
		\brief Convert the type enum into a string
		\param type Enum type of the message
		\return String of the enum type
		*/
		std::string GetLabel(Type type) {
			std::string label;
			switch (type) {
			case DEBUG:		label = "DEBUG";	break;
			case INFO:		label = "INFO";		break;
			case WARNING:	label = "WARNING";	break;
			case FAULT:		label = "FAULT";	break;
			}
			return label;
		}

		bool 	opened = false; //!< If the logger is open or not.
		bool	outputToFile = true; //!< If the logger should output message to the log file.
		bool	outputToConsole = true; //!< If the logger should output message to the console.
		bool	headers = true; //!< If message type should be displayed before the message.
#ifdef _DEBUG
		Type	minType = DEBUG; //!< Minimum type of message to display.
#else
		Type	minType = INFO; //!< Minimum type of message to display.
#endif
		Type	currentType; //!< Current type of message.
		std::fstream logFile; //!< File to output messages to.
	};
}