#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>

int main(int argc, char *argv[])
{
    const char *writefile;
    const char *writestr;
    FILE *file;

    openlog("writer", LOG_PID, LOG_USER);

    if (argc != 3) {
        fprintf(stderr, "Invalid number of arguments.\n 1) File Path. \n 2) String to be entered in this file.\n");
        syslog(LOG_ERR, "Invalid number of arguments: %d", argc - 1);
        closelog();
        return 1;
    }

    writefile = argv[1];
    writestr = argv[2];

    file = fopen(writefile, "w");

    if (file == NULL) {
        fprintf(stderr, "Error: could not open file '%s': %s\n",
                writefile, strerror(errno));
        syslog(LOG_ERR, "Could not open file '%s': %s",
               writefile, strerror(errno));
        closelog();
        return 1;
    }

    if (fprintf(file, "%s\n", writestr) < 0) {
        int saved_errno = errno;

        fprintf(stderr, "Error: could not write to file '%s': %s\n",
                writefile, strerror(saved_errno));
        syslog(LOG_ERR, "Could not write to file '%s': %s",
               writefile, strerror(saved_errno));

        fclose(file);
        closelog();
        return 1;
    }

    if (fclose(file) != 0) {
        int saved_errno = errno;

        fprintf(stderr, "Error: could not close file '%s': %s\n",
                writefile, strerror(saved_errno));
        syslog(LOG_ERR, "Could not close file '%s': %s",
               writefile, strerror(saved_errno));

        closelog();
        return 1;
    }

    closelog();
    return 0;
}
