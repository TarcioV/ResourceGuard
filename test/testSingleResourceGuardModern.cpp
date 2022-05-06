#include "ResourceGuard.h"

#include <iostream>
#include <assert.h>
#include <stdexcept>
#include <string>

#define OK 0
#define ERROR 1

bool gDevicePass = false;
bool gWindowPass = false;

int init_device() {
	printf("trying init device... ");
	if(!gDevicePass) {
		printf("device bussy\n");
		return ERROR;
	}
	printf("device initialized\n");
	return OK;
}

void close_device() {
	printf("close_device()\n");
}

int open_window_with_opengl_context() {
	printf("try opening window... ");
	if(!gWindowPass) {
		printf("can't open window\n");
		return ERROR;
	}
	printf("window opened\n");
	return OK;
}

void close_window_with_opengl_context() {
	printf("close_window()\n");
}

class App
{
public:
	App() {	
		if(init_device() != OK) {
			throw std::runtime_error("init_device() failed\n");
		}

		ResourceGuard device_guard([](){
			close_device();
		});

		if(open_window_with_opengl_context() != OK) {
			throw std::runtime_error("open_window_with_opengl_context() failed\n");
		}

		device_guard.free();
	}
};

int main(int argc, char *argv[]) {
	assert(argc == 3);

	for(int i = 1; i < 3; ++i) {
		std::string s = argv[i];
		std::string delim = "=";
		size_t pos = s.find(delim);
		std::string token = s.substr(0, pos);
		std::string value = s.substr(pos+1);

		if(token == "--device-pass") {
			gDevicePass = value == "true" ? true : false;
		} else if(token == "--window-pass") {
			gWindowPass = value == "true" ? true : false;
		}
	}

	try {
		App app;
		printf("app successful created\n");
	} catch (std::exception& e) {
		printf("exception occurs: %s\n", e.what());
		return -1;
	}

	return 0;
}