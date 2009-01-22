
/* config.h needs to included first for gnulib to work its magic */
#include <config.h>

/*
 * standard includes
 */
#include <errno.h>
#include <getopt.h>
#include <libintl.h>
#include <locale.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/wait.h>

/*
 * gnulib utilities
 */
#include <closeout.h>


#define PROGRAM_NAME "hangon"
#define AUTHORS "Michael Schuerig"
#define COPYRIGHT_YEAR 2008

#define _(msgid) gettext(msgid)
#define N_(msgid) msgid

#ifdef DEBUG
#define debug(format, ...)                              \
    do {                                                \
        if (debugging) {                                \
            fprintf (stderr, format, ## __VA_ARGS__);   \
        }                                               \
    } while (0)
#else
#define debug(format, ...)
#endif

enum { COMMAND_TIMEOUT = -2, COMMAND_ERROR = -3, COMMAND_DONE = 1 };

static const struct option long_options[] = {
    { "timeout", required_argument, NULL, 't' },
    { "retries", required_argument, NULL, 'r' },
    { "quiet", no_argument, NULL, 'q' },
    { "debug", no_argument, NULL, 'd' },
    { "help", no_argument, NULL, 'h' },
    { "version", no_argument, NULL, 'v' },
    { NULL, 0, NULL, 0 }
};

static char *program_name;
static char **command_args;
static int timeout_secs = 60;
static int max_retries = 0;
static bool quiet = false;
#ifdef DEBUG
static bool debugging = false;
#endif

static void
usage(int status)
{
    if (status != EXIT_SUCCESS) {
        fprintf(stderr, _("Try %s --help for more information.\n"), program_name);
    } else {
        fprintf(stdout, _("\
Usage: %s [OPTION]... -- COMMAND [COMMAND OPTION]...\n\
"),
                program_name);
        fputs(_("\
Restart an erratic COMMAND until it finishes cleanly.\n\
  -q, --quiet              suppress diagnostic messages\n\
  -r, --retries=TIMES      maximum number of times COMMAND is restarted\n\
  -t, --timeout=SECONDS    timeout in seconds\n\
"), stdout);
#ifdef DEBUG
        fputs(_("\
  -d, --debug              show debugging information\n\
"), stdout);
#endif
        fputs(_("\
      --help               display this help and exit\n\
"), stdout);
        fputs(_("\
      --version            output version information and exit\n\
"), stdout);
    }
    exit(status);
}

void
version_etc()
{
    fprintf(stdout, "%s %s\n", PROGRAM_NAME, VERSION);
    fprintf(stdout, "Copyright %s %d %s\n", _("(C)"), COPYRIGHT_YEAR, AUTHORS);
    fputs (_("\
License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>\n\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.\n\
"),
           stdout);
}

static int
watch_command_stdout(pid_t pid, int fd)
{
    fd_set rfds;
    struct timespec ts = { .tv_sec = timeout_secs, .tv_nsec = 0 };
    struct timespec *timeout = timeout_secs ? &ts : NULL;
    char buffer[BUFSIZ];

    FD_ZERO(&rfds);

    int status = 0;
    while (status == 0) {
        FD_SET(fd, &rfds);
        debug("PSELECT: %d\n", fd);
        status = pselect(fd + 1, &rfds, NULL, NULL, timeout, NULL);
        switch (status) {
        case -1:
            if (errno == EINTR) {
                status = 0;
                continue;
            }
            break;
        case 0: /* timeout */
            debug("TIMEOUT\n");
            status = COMMAND_TIMEOUT;
            break;
        default:
            /* @beginexcerpt read_stdout */
            status = 0;
            if (FD_ISSET(fd, &rfds)) {
                /* @beginexcerpt read */
                ssize_t bytes_read = 0;
                debug("READ: ");
                bytes_read = read(fd, buffer, sizeof(buffer));
                debug("%.*s\n", bytes_read, buffer, sizeof(buffer[0]));
                if (bytes_read < 0) {
                    if (errno == EINTR) { /* @callout EINTR read */
                        status = 0;
                        continue;
                    }
                    status = -1;
                    /* @endexcerpt read */
                } else if (bytes_read == 0) {
                    /* @beginexcerpt wait_command */
                    int command_status;
                    if (waitpid(pid, &command_status, 0) != -1) { /* @callout waitpid */
                        debug("COMMAND STATUS: %d\n", WEXITSTATUS(command_status));
                        if (WIFEXITED(command_status) && WEXITSTATUS(command_status) != 0) {
                            status = COMMAND_ERROR;
                        }
                    }
                    /* @endexcerpt wait_command */
                    if (status == 0) {
                        status = COMMAND_DONE;
                    }
                } else {
                    debug("WRITE\n");
                    if (write(STDOUT_FILENO, buffer, bytes_read) < 0) {
                        status = -1;
                    }
                }
            }
            /* @endexcerpt read_stdout */
            break;
        }
    }
    return(status);
}


static int
hangon()
{
    int status;
    int pfd[2];

    if (pipe(pfd) < 0) {
        return(-1);
    }

    pid_t pid = fork();
    if (pid < 0) {
        return(-1);
    } else if (pid == 0) { /* child */
        close(pfd[0]);
        if (pfd[1] != STDOUT_FILENO) {
            dup2(pfd[1], STDOUT_FILENO);
            close(pfd[1]);
        }
        execvp(command_args[0], command_args);
        perror(command_args[0]);
        exit(EXIT_FAILURE);
    }

    /* parent */
    close(pfd[1]);
    status = watch_command_stdout(pid, pfd[0]);
    close(pfd[0]);

    return(status);
}


int
main(int argc, char *argv[])
{
    program_name = argv[0];

    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain(PACKAGE);

    atexit(close_stdout);

    int optc;
    while ((optc = getopt_long(argc, argv, "t:r:qdhv", long_options, NULL)) != -1) {
        switch (optc) {
        case 't':
            timeout_secs = atoi(optarg);
            if (timeout_secs < 0) {
                fprintf(stderr, _("timeout must not be negative\n"));
                exit(EXIT_FAILURE);
            }
            break;
        case 'r':
            max_retries = atoi(optarg);
            if (max_retries < 0) {
                fprintf(stderr, _("retries must not be negative\n"));
                exit(EXIT_FAILURE);
            }
            break;
        case 'q':
            quiet = true;
            break;
#ifdef DEBUG
        case 'd':
            debugging = true;
            break;
#endif
        case 'h':
            usage(EXIT_SUCCESS);
            break;
        case 'v':
            version_etc();
            exit(EXIT_SUCCESS);
            break;
        default:
            usage(EXIT_FAILURE);
            break;
        }
    }

    debug("OPTIONS: timeout %ds, %d retries\n", timeout_secs, max_retries);

    int hangon_status = 0;
    int retries = 0;

    if (optind < argc) {
        (void)setsid();
        command_args = &argv[optind];

        do {
            hangon_status = hangon();
            debug("HANGON STATUS: %d\n", hangon_status);
            if (hangon_status == COMMAND_TIMEOUT) {
                if (max_retries == 0 || retries < max_retries) {
                    hangon_status = 0;
                    if (!quiet) {
                        fprintf(stderr, "Command timed out, retrying...\n");
                    }
                }
                retries++;
            }
        } while (hangon_status == 0);

    } else {
        usage(EXIT_FAILURE);
    }

    if (!quiet) {
        switch (hangon_status) {
        case -1:
            perror("Error executing command");
            break;
        case COMMAND_TIMEOUT:
            if (max_retries != 0) {
                fprintf(stderr, "Command timed out too often.\n");
            } else {
                fprintf(stderr, "Command timed out.\n");
            }
            break;
        case COMMAND_ERROR:
            fprintf(stderr, "Command terminated with status: %d\n", 0); /* FIXME status */
            break;
        }
    }
    exit(hangon_status == COMMAND_DONE ? EXIT_SUCCESS : EXIT_FAILURE);
}
