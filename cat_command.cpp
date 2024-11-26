#include <iostream>
#include <fcntl.h>  // For open
#include <unistd.h> // For read, write, close
#include <cstring>  // For strerror
#include <cstdlib>  // For EXIT_FAILURE

/**
 * A simple implementation of the cat command.
 * This program reads the contents of files passed as command line arguments
 * and outputs them to standard output. It also handles reading from standard input
 * if a dash ("-") is encountered as an argument.
 */
int main(int argc, char *argv[]) {
    const int bufferSize = 4096; // Size of the buffer for reading files
    char buffer[bufferSize]; // Buffer to hold data read from files
    ssize_t bytesRead; // Number of bytes read from the file

    // Loop over each command line argument (starting from argv[1])
    for (int i = 1; i < argc; ++i) {
        const char *filename = argv[i];

        if (strcmp(filename, "-") == 0) {
            // If the filename is "-", read from standard input
            while ((bytesRead = read(STDIN_FILENO, buffer, bufferSize)) > 0) {
                write(STDOUT_FILENO, buffer, bytesRead);
            }
            if (bytesRead < 0) {
                std::cerr << "Error reading from standard input: " << strerror(errno) << std::endl;
                return EXIT_FAILURE;
            }
        } else {
            // Otherwise, try to open the file
            int fd = open(filename, O_RDONLY);
            if (fd < 0) {
                std::cerr << "Error opening file " << filename << ": " << strerror(errno) << std::endl;
                continue; // Move on to the next file
            }

            // Read from the file and write to standard output
            while ((bytesRead = read(fd, buffer, bufferSize)) > 0) {
                write(STDOUT_FILENO, buffer, bytesRead);
            }

            // Check for read error
            if (bytesRead < 0) {
                std::cerr << "Error reading from file " << filename << ": " << strerror(errno) << std::endl;
            }

            // Close the file descriptor
            close(fd);
        }
    }

    return 0; // Return success
}

