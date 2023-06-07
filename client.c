#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define FIB_DEV "/dev/fibonacci"
#define KERNEL_TIME 0
#define COPY_TO_USER_TIME 1
#define PERFORMANCE_FILE "performance.txt"

int main()
{
    char buf[256] = {0};
    char write_buf[] = "testing";
    int offset = 100; /* TODO: try test something bigger than the limit */
    int fd = open(FIB_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }
    FILE *file = fopen(PERFORMANCE_FILE, "w");

    for (int i = 0; i <= offset; i++) {
        struct timespec start_time, end_time;
        long long user_time;
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        lseek(fd, i, SEEK_SET);
        long long sz = read(fd, &buf, sizeof(buf));
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        user_time = (end_time.tv_sec - start_time.tv_sec) * 1000000000LL +
                    (end_time.tv_nsec - start_time.tv_nsec);
        buf[sz] = '\0';
        printf("Reading from " FIB_DEV
               " at offset %d, returned the sequence "
               "%s.\n",
               i, buf);
        lseek(fd, KERNEL_TIME, SEEK_SET);
        long long kernel_time = write(fd, write_buf, strlen(write_buf));
        lseek(fd, COPY_TO_USER_TIME, SEEK_SET);
        long long copy_to_user_time = write(fd, write_buf, strlen(write_buf));
        fprintf(file, "%d %lld %lld %lld\n", i, user_time, kernel_time,
                copy_to_user_time);

    }

    fclose(file);
    close(fd);
    return 0;
}
