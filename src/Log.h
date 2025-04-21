#pragma once
#include <iostream>

#ifndef NDEBUG

#define LOG(message) std::cout << "[DEBUG] " << message << std::endl
#define LOG_ERROR(message) std::cerr << "[ERROR] " << message << std::endl
#define LOG_WARNING(message) std::cout << "[WARNING] " << message << std::endl

#else

#define LOG(message)
#define LOG_ERROR(message)
#define LOG_WARNING(message)

#endif
