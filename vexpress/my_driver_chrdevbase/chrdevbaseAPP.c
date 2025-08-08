#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("Usage: %s <device_file> <operation>\n", argv[0]);
        printf("Available operations: write | read\n");
        return -1;
    }

    const char *file_name = argv[1];
    const int buff_size = 50;
    int fd = 0;
    ssize_t ret = 0;
    char read_buff[buff_size];
    char write_buff[buff_size];

    // 填充写入缓冲区并添加空终止符
    const char *data_to_write = "abcdefghijklmnopqrstuvwxyz";
    strncpy(write_buff, data_to_write, sizeof(write_buff) - 1);
    write_buff[sizeof(write_buff) - 1] = '\0';
    
    // 打开文件
    fd = open(file_name, O_RDWR);
    if (fd < 0) {
        perror("file open error");
        return -1;
    }
    printf("file open successful, fd = %d\n", fd);

    if (!(strcmp(argv[2], "write"))) {
        ret = write(fd, write_buff, strlen(write_buff));
        if (ret < 0) {
            perror("file write error");
            close(fd);
            return -1;
        }
        printf("file write end, size: %zd\n", ret);
    }
    else if (!(strcmp(argv[2], "read"))) {
        // 读取前清空缓冲区，避免脏数据
        memset(read_buff, 0, sizeof(read_buff));
        
        lseek(fd, 0, SEEK_SET);
        ret = read(fd, read_buff, sizeof(read_buff) - 1);
        if (ret < 0) {
            perror("file read error");
            close(fd);
            return -1;
        }
        
        // read 返回的字节数可能少于请求的字节数
        read_buff[ret] = '\0';
        printf("read size: %zd, content: %s\n", ret, read_buff);
    }
    else {
        printf("Invalid operation: %s\n", argv[2]);
        close(fd);
        return -1;
    }

    if (close(fd) == 0) {
        printf("file %s close successful\n", file_name);
    } else {
        perror("file close error");
    }

    return 0;
}