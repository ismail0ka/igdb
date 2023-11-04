#ifndef IGDB_DEBUGGER_HPP
#define IGDB_DEBUGGER_HPP

#include <utility>
#include <string>
#include <linux/types.h>

namespace igdb{
	class Debugger {
		private:
			std::string prog_name;
			pid_t pid;
			
			void handle_command(const std::string& line);
			void continue_execution();
		public:
			Debugger(std::string prog_n, pid_t pid): prog_name{std::move(prog_n)}, pid{pid}
			{}

			void run();
	};
}

#endif
