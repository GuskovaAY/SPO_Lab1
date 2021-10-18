#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#define WINVER 0x0A00

using namespace std;

int main()
{
	// пункт 1
	SECURITY_ATTRIBUTES attributes;
	attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	attributes.bInheritHandle = TRUE;
	attributes.lpSecurityDescriptor = NULL;

	HANDLE input_pipe_read_end;
	HANDLE input_pipe_write_end;
	CreatePipe(&input_pipe_read_end, &input_pipe_write_end, &attributes, 0);

	HANDLE output_pipe_read_end;
	HANDLE output_pipe_write_end;
	CreatePipe(&output_pipe_read_end, &output_pipe_write_end, &attributes, 0);

	SetHandleInformation(input_pipe_write_end, HANDLE_FLAG_INHERIT, 0);
	SetHandleInformation(output_pipe_read_end, HANDLE_FLAG_INHERIT, 0);

	// пункт 2
	STARTUPINFO startup_info;
	ZeroMemory(&startup_info, sizeof(startup_info));
	startup_info.cb = sizeof(startup_info);
	startup_info.hStdInput = input_pipe_read_end;
	startup_info.hStdOutput = output_pipe_write_end;
	startup_info.hStdError = output_pipe_write_end;
	startup_info.dwFlags |= STARTF_USESTDHANDLES;

	//char cmd[64] = "cmd.exe";
	PROCESS_INFORMATION pi;
	CreateProcess(
		NULL,
		"cmd.exe",
		NULL,
		NULL,
		TRUE,
		0,
		NULL,
		NULL,
		&startup_info,
		&pi);

	// пункт 3
	bool flag = true;
	do {
		char buffer[256];
		DWORD buf_bytes;
		do {
			ReadFile(output_pipe_read_end, buffer, sizeof(buffer), &buf_bytes, NULL);
			fwrite(buffer, buf_bytes, 1, stdout);
		} while (buffer[buf_bytes -1] != '>');

		// пункт 4
		const char PLEASE[] = "please";
		char* input = NULL;
		while (!input) {
			fgets(buffer, sizeof(buffer), stdin);
			if (!strncmp(buffer, "thanks", 6)) {
                CloseHandle(output_pipe_read_end);
                CloseHandle(output_pipe_write_end);
                CloseHandle(input_pipe_read_end);
                CloseHandle(input_pipe_write_end);
                TerminateProcess(pi.hProcess, 0);
                flag = false;
				break;
			}
			else if (strncmp(buffer, "please", 6)) {
				fprintf(stderr, "Please ask politely!\n> ");
			}
			else {
				input = buffer + sizeof(PLEASE);
				WriteFile(input_pipe_write_end, input, sizeof(input), NULL, NULL);
				break;
			}
		}
	} while (flag);
	return 0;
}
