#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <list>











///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class critical_section
{
public:
	critical_section();
	~critical_section();
	static void init();
	static void destroy();
private:
	critical_section(const critical_section& other);
	critical_section& operator = (const critical_section& other);
	static pthread_mutex_t _mutex;
};

pthread_mutex_t critical_section::_mutex;

critical_section::critical_section() {
	int result = pthread_mutex_lock(&_mutex);
	if(0 != result) {
		perror(NULL);
	}
}

critical_section::~critical_section() {
	int result = pthread_mutex_unlock(&_mutex);
	if(0 != result) {
		perror(NULL);
	}
}

void critical_section::init() {
	pthread_mutexattr_t a;
	pthread_mutexattr_init(&a);
	pthread_mutexattr_settype(&a, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&_mutex, &a);
}

void critical_section::destroy() {
	pthread_mutex_destroy(&_mutex);
}



















///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class task {
public:
	static void* _thread_proc(void* param);
private:
	task();
	~task();
};

void* task::_thread_proc(void* param) {
	critical_section locked;
	int handle = pthread_self();
	printf("[%08x] $$$ start $$$\n", handle);
	sleep(2);
	printf("[%08x] --- exit ---\n", handle);
}


















///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class task_manager {
public:
	static void begin();
	static void wait();
private:
	task_manager();
	~task_manager();
	static pthread_t _begin_thread();
	static std::list<pthread_t> _task;
};

std::list<pthread_t> task_manager::_task;

void task_manager::begin() {
	for(int i = 0; i < 4; i++) {
		pthread_t th;
		int result = pthread_create(&th, NULL, &task::_thread_proc, NULL);
		if(result != 0) {
			continue;
		}
		_task.push_back(th);
	}
}

void task_manager::wait() {
	for(std::list<pthread_t>::iterator i = _task.begin(); i != _task.end(); i++) {
		pthread_t t = *i;
		pthread_join(t, NULL);
	}
}

















///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
	critical_section::init();
	task_manager::begin();
	task_manager::wait();
	critical_section::destroy();
	printf("--- end ---\n");
	return 0;
}
