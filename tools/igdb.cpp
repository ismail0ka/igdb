#include <libigdb/libigdb.hpp>
#include <iostream>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <string_view>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

namespace {
	void	handle_command(pid_t pid, std::string_view line);
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

			if ((pid = fork()) == -1)
			{
				std::perror("Failed to fork");
				return (-1);
			}
			if ((pid = fork()) == 0)
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
		std::cerr << "Or Attach to already running process\n" << argv[0] << "-p <process_pid>" << std::endl;
	}
	pid = attach(argc, argv);
	options = 0;
	if (waitpid(pid, &wait_status, options) == -1)
	{
		std::perror("Failed to wait for child process");
		return (-1);
	}

	line = nullptr;
	while ((line = readline("igdb> ")) != nullptr)
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