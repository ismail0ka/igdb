#include <libigdb/libigdb.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <string_view>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

namespace {
	std::vector<std::string>	split(std::string_view str, char delimiter){
		std::vector<std::string>	result{};
		std::stringstream			ss{std::string{str}};
		std::string					token;

		while ((std::getline(ss, token, delimiter)))
			result.push_back(token);
		return (result);
	}

	bool is_prefix(std::string_view str, std::string_view of){
		if (str.size() > of.size())
			return (false);
		return (std::equal(str.begin(), str.end(), of.begin()));
	}

	void	resume(pid_t pid){
		if (ptrace(PTRACE_CONT, pid, nullptr, nullptr) == -1)
		{
			std::cerr << "Could not resume process" << std::endl;
			std::exit(-1);
		}
	}

	void	wait_on_signal(pid_t pid){
		int	status;
		int	options;

		if (waitpid(pid, &status, options) == -1)
		{
			std::perror("Waiting on child process failed");
			std::exit(-1);
		}
	}
	void	handle_command(pid_t pid, std::string_view line)
	{
		auto args = split(line, ' ');
		auto cmd = args[1];

		if (is_prefix(cmd, "continue"))
		{
			resume(pid);
			wait_on_signal(pid);
		}
		else
		{
			std::cerr << "Unknown Command " << cmd << std::endl;
		}
	}
	pid_t	attach(int argc, const char **argv)
	{
		pid_t	pid;

		pid = 0;
		if (argc == 3 && std::string_view(argv[1]) == "-p")
		{
			pid = std::atoi(argv[2]);
			if (pid <= 0)
			{
				std::cerr << "Invalid PID" << std::endl;
				return (pid);
			}
			if (ptrace(PTRACE_ATTACH, pid, nullptr, nullptr) == -1)
			{
				std::perror("Failed to attach to process");
				return (-1);
			}
		}
		else if (argc == 2)
		{
			const char	*program_path = argv[1];

			pid = fork();
			if (pid == -1)
			{
				std::perror("Failed to fork");
				return (-1);
			}
			if (pid == 0)
			{
				//We are in child process
				if (ptrace(PTRACE_TRACEME, 0, nullptr, nullptr) == -1)
				{
					std::perror("Failed to trace child process");
					return (-1);
				}
				if (execlp(program_path, program_path, nullptr) == -1)
				{
					std::perror("Failed to execute program");
					return (-1);
				}
			}
		}
		return (pid);
	}
}

int main(int argc, const char **argv)
{
	pid_t	pid;
	int		wait_status;
	int		options;
	char	*line;

	if (argc == 1)
	{
		std::cerr << "No Arguments Passed" << std::endl;
		std::cerr << "Usage: " << argv[0] << " <pid>" << std::endl;
		std::cerr << "Or Attach to already running process\n" << argv[0] << " -p <process_pid>" << std::endl;
		std::exit(0);
	}
	pid = attach(argc, argv);
	options = 0;
	if (waitpid(pid, &wait_status, options) == -1)
	{
		std::perror("Failed to wait for child process");
		return (-1);
	}

	line = nullptr;
	while ((line = readline("igdb>")) != nullptr)
	{
		std::string_view	line_view;

		if (line == std::string_view(""))
		{
			if (history_length > 0)
			{
				free(line);
				line_view = history_list()[history_length - 1]->line;
			}
		}
		else
		{
			line_view = line;
			add_history(line);
			free(line);
		}
		if (!line_view.empty())
		{
			handle_command(pid, line_view);
		}
	}	
	return (0);
}