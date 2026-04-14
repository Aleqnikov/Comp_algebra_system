#ifndef CAS_EXCEPTIONS_H
#define CAS_EXCEPTIONS_H

#include <string>
#include <exception>

// Твой базовый класс
class UniversalStringException : public std::exception {
public:
	UniversalStringException(const std::string& message) : message{message} {}
	const char* what() const noexcept override {
		return message.c_str();
	}
private:
	std::string message;
};

// Ошибка, связанная со структурой данных Aho-Corasick
class AhoCorasickException : public UniversalStringException {
public:
	explicit AhoCorasickException(const std::string& msg)
		: UniversalStringException("Aho-Corasick Error: " + msg) {}
};

// Ошибка токенизации (например, неизвестный символ)
class TokenizerException : public UniversalStringException {
public:
	explicit TokenizerException(const std::string& msg, size_t pos = 0)
		: UniversalStringException("Tokenizer Error at pos " + std::to_string(pos) + ": " + msg) {}
};

#endif // CAS_EXCEPTIONS_H