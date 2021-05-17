#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>

#define CMDLINE_DBG true

// Global variables to store configuration parameters
#define CFGSTR_SIZE 64
#define FILENAME_SIZE 1024
#define TTYLINE_SIZE 256
#define rowLenght 12
uint32_t g_serial_speed = 0;
uint32_t g_termio_speed = 0;
char g_serial_device[CFGSTR_SIZE] = "";
char g_infile[FILENAME_SIZE] = "stdin";
char g_outfile[FILENAME_SIZE] = "stdout";

// Accessing termios.h Bnum type speed definition from the command line

struct BNUM_speed
{
    uint32_t speed;
    uint32_t bnum;
};

// Constants from /usr/include/asm-generic/termbits.h
// Constants are octal in termbits.h
struct BNUM_speed g_speed[] = {
    {50, B50},
    {75, B75},
    {110, B110},
    {134, B134},
    {150, B150},
    {200, B200},
    {300, B300},
    {600, B600},
    {1200, B1200},
    {1800, B1800},
    {2400, B2400},
    {4800, B4800},
    {9600, B9600},
    {19200, B19200},
    {38400, B38400},
    {57600, B57600},
    {115200, B115200},
    {230400, B230400},
    {460800, B460800},
    {500000, B500000},
    {576000, B576000},
    {921600, B921600},
    {1000000, B1000000},
    {1152000, B1152000},
    {1500000, B1500000},
    {2000000, B2000000},
    {2500000, B2500000},
    {3000000, B3000000},
    {3500000, B3500000},
    {4000000, B4000000},
    {0, 0} // Null termination, keep as the last element...
};

bool set_g_speed(int speed)
{
    int i = 0;
    for (i = 0; g_speed[i].speed != speed; i++)
    {
        if (g_speed[i].speed == 0)
        {
            return false;
        }
    }
    g_termio_speed = g_speed[i].bnum;
    return true;
}

// Print help for -h command line option
void print_help(void)
{
    printf("Help for PEP HF\n");
    printf("Command line parameters:\n");
    printf("-h                           : Print help\n");
    printf("-s d=port_file,s=baud_rate   : Set serial port device file and baud rate, then start the game\n");
    printf("-o file                      : Write LOG to output file\n");
    printf("                             : Press 'q' to exit\n");
}

int main(int argc, char *argv[])
{
    int opt;
    int i;

    // Stuff required by getopt-subopt handling for the serial port
    enum
    {
        DEVICE_OPT = 0,
        SPEED_OPT
    };

    char *const token[] = {
        [DEVICE_OPT] = "d",
        [SPEED_OPT] = "s",
        NULL};

    char *subopts;
    char *value;
    int errfnd = 0;

    if (argc <= 1)
    {
        printf("No command line parameters, does not know what to do...\n");
    }

    // Handling commmad line arguments, and storing them in globals
    while ((opt = getopt(argc, argv, "hs:o:i:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            if (CMDLINE_DBG)
                printf("-h cmd line par received, printing help\n");
            print_help();
            exit(EXIT_SUCCESS);
            break;
        case 's':
            if (CMDLINE_DBG)
                printf("-s cmd line par received, setting serial port\n");
            subopts = optarg;
            while ((*subopts != '\0') && !errfnd)
            {
                switch (getsubopt(&subopts, token, &value))
                {
                case DEVICE_OPT:
                    if (strlen(value) < CFGSTR_SIZE)
                    {
                        strcpy(g_serial_device, value);
                        if (CMDLINE_DBG)
                            printf("Serial port: %s\n", g_serial_device);
                    }
                    else
                    {
                        printf("Serial device file name is too long (max size is %d), exiting...\n", CFGSTR_SIZE);
                        exit(EXIT_FAILURE);
                    }
                    break;
                case SPEED_OPT:
                    g_serial_speed = atoi(value);
                    if (g_serial_speed == 0)
                    {
                        printf("Serial device speed is invalid, exiting...\n");
                        exit(EXIT_FAILURE);
                    }
                    if (set_g_speed(g_serial_speed))
                    {
                        printf("Serial port speed: %d (%#x)\n", g_serial_speed, g_termio_speed);
                        printf("B115200 %x\n", B115200);
                    }
                    else
                    {
                        printf("Specified serial speed is not supported by termios.h\n");
                        exit(EXIT_FAILURE);
                    }
                    break;
                }
            }
            break;
        case 'o':
            if (CMDLINE_DBG)
                printf("-f cmd line par received, setting output file\n");
            if (strlen(optarg) < FILENAME_SIZE)
            {
                strcpy(g_outfile, optarg);
                if (CMDLINE_DBG)
                    printf("Output file: %s\n", g_outfile);
            }
            else
            {
                printf("Output file name is too long (max size is %d), exiting...\n", FILENAME_SIZE);
                exit(EXIT_FAILURE);
            }
            break;
        default:
            if (CMDLINE_DBG)
                printf("Unknown command line parameter is received\n");
            break;
        }
    }

    // Put stuff here
    struct termios gecko_ser, std_set, std_reset;
    int ser_fd;
    int out_fd;
    int out_file;
    memset(&gecko_ser, 0, sizeof(gecko_ser));

    //Gecko serial communication settings
    gecko_ser.c_iflag = 0;
    gecko_ser.c_oflag = 0;
    gecko_ser.c_cflag = CS8 | CREAD | CLOCAL;
    gecko_ser.c_lflag = 0;
    gecko_ser.c_cc[VMIN] = 1;
    gecko_ser.c_cc[VTIME] = 5;

    tcgetattr(out_fd, &std_reset);

    //Terminal settings
    cfmakeraw(&std_set);
    std_set.c_oflag = ONLCR | OPOST;
    std_set.c_iflag |= (IGNBRK | BRKINT);
    std_set.c_lflag |= ISIG | IEXTEN | ECHOCTL;

    ser_fd = open(g_serial_device, O_RDWR);
    if (ser_fd < 0)
    {
        perror("Serial device open failed");
        exit(EXIT_FAILURE);
    }

    //LOG file open if log file exists
    //LOG file create if log file doesn't exist and log file name is not "stdout"
    if (strcmp(g_outfile, "stdout") != 0)
    {
        out_file = open(g_outfile, O_RDWR | O_CREAT, 0644);
        if (out_file < 0)
        {
            perror("Log file open failed");
            exit(EXIT_FAILURE);
        }
        //Write timestamp
        time_t clk = time(NULL);
        char time_str[35];
        memset(&time_str, 0, sizeof(time_str));
        sprintf(time_str, "%s", ctime(&clk));
        write(out_file, time_str, strlen(time_str));
    }

    //Set termios
    cfsetospeed(&gecko_ser, g_termio_speed);
    cfsetispeed(&gecko_ser, g_termio_speed);
    tcsetattr(ser_fd, TCSANOW, &gecko_ser);
    tcsetattr(out_fd, TCSANOW, &std_set);
    char c = 'n';
    char row[rowLenght] = "";

    //Send a character to Gecko to start a new game
    write(ser_fd, "n", 1);
    //Press a number to select a game map
    printf("Press a number: \n");
    c = getchar();
    write(ser_fd, &c, 1);

    printf("Move:       up, down, left, right: w,s,a,d \n");
    printf("Exit:       q\n");
    printf("Shoot:      any other charachter\n");
    printf("New game:   n\n");
    
    // Start a new thread
    pid_t pid, ownpid;
    pid = fork();
    if (pid == -1)
    {
        // Cannot fork....
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        //Child thread
        //Gecko send log data to linux
        while (true)
        {
            // Required for select
            int selrval;
            fd_set rfds;
            fd_set wfds;
            struct timeval tv;

            FD_ZERO(&rfds);
            FD_SET(ser_fd, &rfds);
            FD_ZERO(&wfds);
            FD_SET(ser_fd, &wfds);
            // Wait up to seven seconds.
            tv.tv_sec = 7;
            tv.tv_usec = 0;
            // Required for select end

            // Actual select call
            selrval = select(ser_fd + 1, &rfds, NULL, NULL, &tv);
            // Donât rely on the value of tv now!
            if (selrval < 0)
            {
                perror("Select failed \n");
            }
            else
            {
                if (FD_ISSET(ser_fd, &rfds))
                {
                    //printf("Reading from Gecko: \n");
                    int i;
                    char b = '\0';
                    //Receive log data 
                    for (i = 0; b != '\n';)
                    {
                        if (FD_ISSET(ser_fd, &rfds))
                        {
                            read(ser_fd, &b, 1);
                            row[i] = b;
                            i++;
                        }
                    }
                    row[i] = '\n';
                    write(STDOUT_FILENO, row, i);
                    //Write to the log file
                    if (strcmp(g_outfile, "stdout") != 0)
                    {
                        write(out_file, row, i);
                    }
                    //printf("Writing to STDOUT: %x\n",c);
                }
            }
        }

        exit(EXIT_SUCCESS);
    }
    else
    {
        //Parent thread
        //Read a character from the Linux terminal and send it to the gecko
        //The exit character is 'q' or 'CTRL+C'
        while (c != 'q' && c != 3)
        {
            c = getchar();
            write(ser_fd, &c, 1);
            if(c == 'n'){
              printf("New game\n");
              printf("Press a number: \n");
            }
               
        }
    }
   // Exiting
    kill(pid, SIGTERM);
    close(ser_fd);
    close(out_file);
    tcsetattr(out_fd, TCSANOW, &std_reset);
    exit(EXIT_SUCCESS);
}
