#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/inotify.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16))

int main(int arg, char **argv)
{
    int fd = inotify_init();
    if (fd < 0)
    {
        perror("inotify_init");
        return EXIT_FAILURE;
    }

    int wd = inotify_add_watch(fd, ".", IN_CREATE | IN_DELETE | IN_MODIFY);
    if (wd < 0)
    {
        perror("inotify_add_watch");
        close(fd);
        return EXIT_FAILURE;
    }

    char buffer[1024];

    while (1)
    {
        int length = read(fd, buffer, BUF_LEN);
        if (length < 0)
        {
            perror("read");
            break;
        }

        int i = 0;
        while (i < length)
        {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];
            if (event->len)
            {
                if (event->mask & IN_CREATE)
                    printf("The file %s was created.\n", event->name);
                else if (event->mask & IN_DELETE)
                    printf("The file %s was deleted.\n", event->name);
                else if (event->mask & IN_MODIFY)
                    printf("The file %s was modified.\n", event->name);
            }
            i += EVENT_SIZE + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
    return EXIT_SUCCESS;
}