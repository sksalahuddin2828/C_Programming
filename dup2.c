#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define BADEXIT -1

int
dup2(int fd1, int fd2)
{
    if (fd1 != fd2) {
#ifdef F_DUPFD
        if (fcntl(fd1, F_GETFL) < 0)
            return BADEXIT;
        if (fcntl(fd2, F_GETFL) >= 0)
            close(fd2);
        if (fcntl(fd1, F_DUPFD, fd2) < 0)
            return BADEXIT;
#else
        errno = ENOTSUP;
        return BADEXIT;
#endif
    }
    return fd2;
}
