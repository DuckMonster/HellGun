#pragma once
#include <boost\asio.hpp>
#include <thread>
#include <vector>
#define MAX_SIZE 1024
#undef near
#undef far

using namespace boost::asio;
using boost::asio::ip::tcp;

struct ScoreEntry {
public:
	int position;
	int score;
	std::string name;

	ScoreEntry() {
		position = -1;
		score = -1;
		name = "[UNKNOWN]";
	}

	ScoreEntry(std::string name, int position, int score) {
		this->name = name;
		this->position = position;
		this->score = score;
	}
};

class Connection {
public:
	Connection(std::string ip, std::string port);
	~Connection();
	
	int addEntry(std::string name, int score);
	std::vector<ScoreEntry> requestEntry(int index, int count);
	void disconnect();

private:
	static io_service service;

	tcp::resolver resolver;
	tcp::socket socket;

	//----------

	void sendData(const std::vector<char>& data);
	std::vector<char> receive();
};