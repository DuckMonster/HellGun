#include "ScoreboardLoader.hpp"
#include <SimpleBinStream.h>

io_service Connection::service;

Connection::Connection(std::string ip, std::string port) : resolver(service), socket(service) {
	connect(socket, resolver.resolve({ ip, port }));
}

Connection::~Connection() {
	disconnect();
}

void Connection::disconnect() {
	if (!socket.is_open())
		return;

	socket.shutdown(socket_base::shutdown_both);
	socket.close();
}

int Connection::addEntry(std::string name, int score) {
	//Data stream
	simple::mem_ostream stream;

	//0 = Add entry
	//Then name
	//Then the score
	stream << (char)0 << name << score;

	//Send it to the server and receive answer
	sendData(stream.get_internal_vec());
	simple::mem_istream answer = receive();

	//Answer is my position on the scoreboard
	int myPos;
	answer >> myPos;

	return myPos;
}

std::vector<ScoreEntry> Connection::requestEntry(int index, int count) {
	//Data stream
	simple::mem_ostream stream;

	//1 = Get entries
	//Then what start index
	//Then the count (byte)
	stream << (char)1 << index << (char)count;

	//Send and receive answer
	sendData(stream.get_internal_vec());
	simple::mem_istream answer = receive();

	//First 4 bytes (int) is number of entries received
	int nmbrOfEntries;
	answer >> nmbrOfEntries;

	std::vector<ScoreEntry> resultList;

	//Loop through them
	for (int i = 0; i < nmbrOfEntries; i++) {
		std::string name;
		int position;
		int score;

		//Name - Score
		answer >> name >> score >> position;
		resultList.push_back(ScoreEntry(name, position, score));
	}

	return resultList;
}

//Send a char vector to the server
void Connection::sendData(const std::vector<char>& data) {
	socket.send(buffer(&data[0], data.size()));
}

//Receive char vector from the server
std::vector<char> Connection::receive() {
	char data[MAX_SIZE];
	int received = socket.read_some(buffer(data, MAX_SIZE));

	return std::vector<char>(data, data + received);
}