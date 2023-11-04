#include "debugger.hpp"

#include <vector>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sstream>
#include <iostream>

#include "linenoise.h"

std::vector<std::string>	split(const std::string &s, char delimiter){
	std::vector<std::string> out{};
	std::stringstream ss {s};
	std::string item;

	while (std::getline(ss, item, delimiter)){
		out.push_back(item);
	}
	return out;
}

bool	is_prefix(const std::string &s, const std::string &of){
	if (s.size() > of.size())
		return false;
	return std::equal(s.begin(), s.end(), of.begin());
}

void	igdb::Debugger::handle_command(const std::string &line){
	std::vector<std::string> args = split(line, ' ');
	auto command = args[0];

	if (is_prefix(command, "cont"))
		continue_execution();
	else
		std::cerr << "Unknown command\n";
}

void	igdb::Debugger::continue_execution(){
	ptrace(PTRACE_CONT, pid, nullptr, nullptr);

	int wait_status;
	auto options = 0;
	waitpid(pid, wait_status, options);
}

void	igdb::Debugger::run(){
	int wait_status;
	auto options = 0;
	//When the traced process is launched, it will be sent a SIGTRAP
	//Which is a Breakpoint trap, we can wait for this signal with waitpid
	waitpid(pid, &wait_status, options);

	char* line = nullptr;
	while ((line = linenoise("igdb >") != nullptr){
		handle_command(line);
		linenoiseHistoryAdd(line);
		linenoiseFree(line);
	}
}
