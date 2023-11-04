
int	main(int argc, char **argv){
	if (argc < 2){
		std::cerr << "Specify program name\nExample: igdb <executable name>";
		return -1;
	}

	auto prog = argv[1];
	
	pid_t pid = fork();

	if (pid == 0){
		//We re in the child process
		//execute debugee
		ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
		execl(prog, prog, nullptr);
	}else if (pid >= 1){
		//We re in the parent process
		//execute debugger
		std::cout << "Debugging process:" << pid << '\n';
		
		Debugger gdb{prog, pid};
		
		gdb.run()
	}
}
