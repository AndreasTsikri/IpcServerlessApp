#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <iostream>
#include <thread>
#include <atomic>
#include <cstring>
#include <csignal>
#include "IpcServerlessApp/Message.hpp"

using namespace boost::interprocess;

std::atomic<bool> g_terminate{false};
std::string username;

void producer(SharedBuffer* buf) {
    std::cout << "PRODUCER THREAD STARTED: " << username << std::endl;
    std::string user_input;

    while (!g_terminate) {
        
	std::getline(std::cin, user_input);
        if (user_input.empty())
            continue;

        {
            scoped_lock<interprocess_mutex> lock(buf->mutex);

            if (user_input == "exit" || user_input == "quit") {
                g_terminate = true;
                buf->cond.notify_all(); 
                break;
            }

            uint64_t index = (buf->next_seq - 1) % BUF_SIZE;

            std::strncpy(buf->msgs[index].text, user_input.c_str(), MSG_SIZE - 1);
            buf->msgs[index].text[MSG_SIZE ] = '\0';
            std::strncpy(buf->msgs[index].name, username.c_str(), NAME_SIZE - 1);
	    buf->msgs[index].text[NAME_SIZE ] = '\0';

            ++buf->next_seq;

            buf->cond.notify_all();
        }
    }
}

void consumer(SharedBuffer* buf){

	std::cout << "consumer THREAD STARTED : " << username << ":"<< std::endl;
	uint64_t last_read_count = 0;
	while(!g_terminate){
		scoped_lock<interprocess_mutex> lock(buf->mutex);
		buf->cond.wait(lock,[&]{
				return g_terminate || last_read_count < buf->next_seq - 1;
				});
		std::this_thread::sleep_for(std::chrono::microseconds(5));
		if(g_terminate)
			break;
		while(last_read_count < buf->next_seq - 1){
			uint64_t s = last_read_count;//buf->next_seq - 1;
			uint64_t index = s % BUF_SIZE;
			std::string strmes = std::string(buf->msgs[index].text);
			std::string strname = std::string(buf->msgs[index].name);
			//std::cout << "CONUMER CONSUMED" << std::endl;
			std::cout << "(" << strname << ") :" << strmes << std::endl;
			++last_read_count;
		}

	}
}

//void signal_handler(int sig){
//g_terminate = true;
//}

int main(){
	//shared_memory_object::remove(SHM_NAME);
    std::cout << "==========CHAT CLIENT=========="<<std::endl;
    std::cout << "Enter your username:" << std::endl;
    std::cin >> username;
    std::cout << "Welcome " << username << std::endl;
    
    //initialize Shared Memory Segment
    managed_shared_memory segment(open_or_create, SHM_NAME, SHM_SIZE);

    //if(!segment.find<SharedBuffer>(SHM_NAME).first){shared_memory_object::remove(SHM_NAME);}
    
    SharedBuffer * buf = segment.find_or_construct<SharedBuffer>(SHARED_OBJ_NAME)();
    {
	    scoped_lock<interprocess_mutex> lock(buf->mutex);
	    ++buf->client_counter;
	    std::cout << "CLIENTS : " << buf->client_counter <<std::endl; 
    }
    std::jthread t_writer(consumer, buf), t_reader(producer, buf);

    t_reader.join();
    t_writer.join();
    {
	    scoped_lock<interprocess_mutex> lock(buf->mutex);
	    if(buf->client_counter <= 1){
		    shared_memory_object::remove(SHM_NAME);
		    std::cout << "MEMORY SEGMENT COMPLETELY REMOVED!!" << username << std::endl;
	    }else{
	    	--buf->client_counter;
	    }
	}
}
