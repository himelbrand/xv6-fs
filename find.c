#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

//arguments info
static char fname[512];
static int size;
static char size_modifier;
static char type;
static int follow;
static char key[10];
static char val[30];

char *
fmtname(char *path) //taken from ls.c
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}
int filter(int fd, struct stat st, char *name)
{
    //char buf[30];

    if ((fname[0] != 0) && (strcmp(fname, name) != 0))
        return 0;
    if (size != -1)
    {
        switch (size_modifier)
        {
        case 0:
            if (st.size != size)
                return 0;
            break;
        case '+':
            if (st.size <= size)
                return 0;
            break;
        case '-':
            if (st.size > size)
                return 0;
            break;
        default:
            break;
        }
    }
    switch (type)
    {
    case 'd':
        if (st.type != T_DIR)
            return 0;
        break;
    case 'f':
        if (st.type != T_FILE)
            return 0;
        break;
    case 's':
        if (st.type != T_SYMLINK)
            return 0;
        break;
    default:
        break;
    }
    if ((key[0] != 0) && (val[0] != 0))
    { //TODO: add this part after gettag is implemented
        // if (gettag(fd, key, buf) > 0)
        // {
        //     if (strcmp(val, "?") && strcmp(val, buf)) //not wild card and not equal
        //         return 0;
        // }
        // else //not found
        // {
        //     return 0;
        // }
    }
    return 1; //keep this file
}

int find(char *path, char *name)
{
    char buf[512], *p;
    char sympath[512];
    int fd;
    struct dirent de;
    struct stat st;
    if ((!follow) && (readlink(path, sympath, (uint)64) != -1))
    { //handle symlink - when no follow , if follow it will become T_FILE or T_DIR
        if ((fname[0] != 0) && (strcmp(fname, name) != 0))
            return 0;
        if (size != -1)
        {
            switch (size_modifier)
            {
            case 0:
                if (0 != size)
                    return 0;
                break;
            case '+':
                if (0 <= size)
                    return 0;
                break;
            case '-':
                if (0 > size)
                    return 0;
                break;
            default:
                break;
            }
        }
        switch (type)
        {
        case 'd':
            return 0;
        case 'f':
            return 0;
        default:
            break;
        }

        printf(1, "%s\n", path);
        return 0;
    }
    //this is for symlinks with -follow set or dirs or files
    if ((fd = open(path, 0)) < 0)
    {
        printf(2, "find: cannot open %s\n", path);
        return -1;
    }

    if (fstat(fd, &st) < 0)
    {
        printf(2, "find: cannot stat %s\n", path);
        close(fd);
        return -1;
    }

    switch (st.type)
    {
    case T_FILE:
        if (filter(fd, st, name))
        {
            printf(1, "%s\n", path);
        }
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf(1, "find: path too long\n");
            break;
        }
        if (readlink(path, sympath, sizeof(sympath)) != -1)
        { /* it is a symlink */
            strcpy(buf, sympath);
        }
        else
        {
            strcpy(buf, path);
        }

        if (filter(fd, st, fmtname(path)))
        {
            printf(1, "%s\n", buf);
        }
        p = buf + strlen(buf);
        if (*(p - 1) != '/') //if path was home, meaning / then we don't want to add the extra / to the  path print
            *p++ = '/';

        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;

            if (de.name[0] == '.') /* don't loop yourself to death
                                      with '.' and '..' */
                continue;
            if (follow && readlink(buf, sympath, 50) != -1)
            { /* it is a symlink */
                find(sympath, fmtname(sympath));
            }
            else
            {
                find(buf, de.name);
            }
        }
        break;
    }
    close(fd);
    return 0;
}

int main(int argc, char *argv[])
{
    int i;
    char *dlimiter;
    fname[0] = 0;
    size = -1;
    size_modifier = 0;
    type = 0;
    follow = 0;
    key[0] = 0;
    val[0] = 0;

    if (argc < 2)
    {
        printf(2, "Usage: find <path> <options> <tests>\n");
        exit();
    }

    if (!(strcmp(argv[2], "-follow")))
        follow = 1; /* Dereference symlinks */

    for (i = 2 + follow; i < argc; i += 2)
    {
        if (!(strcmp(argv[i], "-name")))
            strcpy(fname, argv[i + 1]);
        else if (!(strcmp(argv[i], "-size")))
        {
            switch (argv[i + 1][0])
            {
            case '+':
                size_modifier = '+';
                size = atoi(&argv[i + 1][1]);
                break;
            case '-':
                size_modifier = '-';
                size = atoi(&argv[i + 1][1]);
                break;
            default:
                size_modifier = 0;
                size = atoi(&argv[i + 1][0]);
                break;
            }
        }
        else if (!(strcmp(argv[i], "-type")))
        {
            switch (argv[i + 1][0])
            {
            case 'd':
                type = 'd';
                break;
            case 'f':
                type = 'f';
                break;
            case 's':
                type = 's';
                break;
            default:
                printf(2, "illegal file type: %s\n", argv[i + 1]);
                return -1;
            }
        }
        else if (!(strcmp(argv[i], "-tag")))
        {
            dlimiter = strchr(argv[i + 1], '=');
            *dlimiter = 0;
            strcpy(key, argv[i + 1]);
            strcpy(val, dlimiter + 1);
            *dlimiter = '=';
        }
    }
    find(argv[1], fmtname(argv[1]));
    exit();
}