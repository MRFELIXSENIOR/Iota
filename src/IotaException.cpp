#include "IotaException.hpp"
#include "IotaApplication.hpp"

#include <iostream>

using namespace iota;
using namespace Application;

void Application::Throw(ErrorType type, const std::string& msg) {
	switch (type) {
	case ErrorType::ERROR:
		std::cerr << "[IOTA] [Error!]\n[Message]: " << msg << '\n';
		throw Error(msg);
		break;

	case ErrorType::RUNTIME_ERROR:
		std::cerr << "[IOTA] [Panic!]\n[Message]: " << msg << '\n';
		throw RuntimeError(msg);
		Exit();
		break;
	}
}

void Application::Throw(ErrorType type, const std::string& title, const std::string& msg) {
	switch (type) {
	case ErrorType::ERROR:
		std::cerr << "[IOTA] [Error!]\n[Title]: " << title << '\n' << "[Message]: " << msg << '\n';
		throw Error(msg);
		break;

	case ErrorType::RUNTIME_ERROR:
		std::cerr << "[IOTA] [Panic!]\n[Title]: " << title << '\n' << "[Message]: " << msg << '\n';
		Exit();
		throw RuntimeError(msg);
		break;
	}
}