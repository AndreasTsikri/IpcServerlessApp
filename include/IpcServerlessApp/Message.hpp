#pragma once
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <string>
#include <utility>
#include <ctime>
#include <mutex>
#include <condition_variable>

class Message{
	std::string sender;
	std::string content;
	std::time_t timestamp;

  public:
	bool has_new_message;
	Message() : sender(""), content(""), timestamp(0),has_new_message(false) {};  	
	Message(std::string s, std::string c, std::time_t t) : sender(s), content(c), timestamp(t), has_new_message(false){};

	void Set(std::string s, std::string c, std::time_t t);

	const std::string& GetSender() const {return sender;}
	void SetSender(std::string s){sender = std::move(s);}

	const std::string& GetContent() const {return content;}
	void SetContent(std::string c){content = std::move(c);}

	const std::time_t GetTimestamp() const {return timestamp;}
};

inline constexpr char SHM_NAME[] = "MySharedMemorySegment";
inline constexpr char SHARED_OBJ_NAME[] = "SharedBufferObject";
inline constexpr std::size_t SHM_SIZE = 65536;//65kb
inline constexpr std::size_t BUF_SIZE = 64;
 
inline constexpr std::size_t MSG_SIZE = 100;
inline constexpr std::size_t NAME_SIZE = 10;

struct MessageStruct { 
	uint64_t seq; 
	char text[MSG_SIZE]; 
	char name[NAME_SIZE]; 
	MessageStruct() : seq(0){text[0]='\0';name[0]='\0'; }
};
 
struct SharedBuffer { 
	
	std::size_t head{0}; 
	uint64_t next_seq{1};
   	uint64_t client_counter{0};	

	//array of messages
	MessageStruct msgs[BUF_SIZE]; 
	boost::interprocess::interprocess_mutex mutex; //std::mutex mtx;
	boost::interprocess::interprocess_condition cond; //std::condition_variable cv;
};
