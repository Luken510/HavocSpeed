#pragma once

#pragma once

/**
* @Author	DJ Coombes
* @date		17th January 2017
* @brief	Logging class, used for logging messages, errors and warnings.
*/

#define Location "logfile.txt"

#include <iostream>
#include <fstream>
#include <string>
namespace ENGINE {
	class LOG {
	public:
		/*!
		\brief Types of message.
		*/
		enum Type {
			DEBUG,
			INFO,
			WARNING,
			ERRORR
		};

		/*!
		\brief Constructor.
		*/
		LOG() {}

		/*!
		\brief Constructor.
		\param type Type of the message to log.
		*/
		LOG(Type type) {
			currentType = type;
			if (outputToFile) {
				logFile.open(Location, std::ios_base::app);
			}
			if (headers) {
				operator<< ("[" + GetLabel(type) + "] ");
			}
		}

		/*!
		\brief Destructor.
		*/
		~LOG() {
			if (opened) {
				if (outputToConsole)
					std::cout << std::endl;
				if (outputToFile) {
					logFile << "\n";
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
		\brief Overloaded stream operator, allows the passing of messages easily.
		\param msg Message to display.
		\return Reference to the logger.
		*/
		template<typename T>
		LOG& operator<<(const T& msg) {
			if (currentType >= minType) {
				if (outputToConsole)
					std::cout << msg;
				opened = true;
				if (outputToFile) {
					logFile << msg;
				}
			}
			return *this;
		}
	private:
		/*!
		\brief Convert the type enum into a string.
		\param type Enum type of the message.
		\return String of the enum type.
		*/
		std::string GetLabel(Type type) {
			std::string label;
			switch (type) {
			case DEBUG:		label = "DEBUG";	break;
			case INFO:		label = "INFO";		break;
			case WARNING:	label = "WARNING";	break;
			case ERRORR:		label = "ERROR";	break;
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
		Type	minType = WARNING; //!< Minimum type of message to display.
#endif
		Type	currentType; //!< Current type of message.
		std::fstream logFile; //!< File to output messages to.
	};
}