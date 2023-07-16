#include "IotaException.hpp"
#include "IotaApplication.hpp"

#include <iostream>

using namespace iota;
using namespace Application;

void Application::Error(const std::string& msg) {
	std::cerr << "[IOTA] [Error!]\n[Message]: " << msg << '\n';
}

void Application::Error(const std::string& title, const std::string& msg) {
	std::cerr << "[IOTA] [Error!]\n[Title]: " << title << '\n' << "[Message]: " << msg << '\n';
}

void Application::Panic(const std::string& msg) {
	std::cerr << "[IOTA] [Panic!]\n[Message]: " << msg << '\n';
	Exit();
}

void Application::Panic(const std::string& title, const std::string& msg) {
	std::cerr << "[IOTA] [Panic!]\n[Title]: " << title << '\n' << "[Message]: " << msg << '\n';
	Exit();	
}