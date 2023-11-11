#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

extern char **environ;

char *getenvvar(char *var)
{
    char ** env;
    size_t i;
    env = environ;

    while (*env)
    {
       for (i = 0; var[i] && (*env)[i] && (*env)[i] == var[i]; i++)
           ;
       if ((*env)[i] == '=' && var[i] == '\0')
           return (*env);

       env++;
    }

    return (NULL);
}
int is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

char **parse_string(char *str, char separator) {
    size_t count, current_word, word_count; // count of separators to account for how many words we need to allocate
    char *head, *tail;
    char **list;
    head = str;

    if (!strlen(str))
        return (NULL);

    for (count = 0; *head;)
        if (*head++ == separator)
            count++;

    word_count = count + 1;
    list = malloc(sizeof(char *) * (count + 2)); // 1 for the NULL, and one because count is off-one
    if (!list)
        return (NULL);
    list[count + 1] = NULL; // seal the list with a NULL

    current_word = 0;
    for (head = str; *head && current_word < word_count; head++)
    {
        while (*head == separator)
            head++;
        count = 0; // the size of the current word
        for (tail = head; *tail != separator && *tail; tail++)
            count++;

        list[current_word] = malloc(sizeof(char) * (count + 1));

        for (count = 0; head != tail; count++)
            list[current_word][count] = *head++;

        list[current_word][count] = 0; // seal the string with a '\0'
        current_word++;
    }

    return (list);
}

char *cat(char *one, char *two)
{
    char *result;
    int i;
    char *current_word;

    i = 0;

    for (current_word = one; *current_word; i++)
        current_word++;

    for (current_word = two; *current_word; i++)
        current_word++;

    result = malloc(sizeof(char) * (i + 1));
    current_word = result;

    while (*one)
        *result++ = *one++;

    while (*two)
        *result++ = *two++;

    *result = '\0';

    return (current_word);
}

char *get_file_path(char **path, char *filename)
{
    char *current_path;
    struct stat filestats;
    while(is_space(*filename) && *filename)
        filename++;

    if (!*filename)
        return (NULL);
    while (*path)
    {
        current_path = cat(cat(*path, "/"), filename);
        if (!stat(current_path, &filestats))
            return (current_path);

        path++;
    }

    return (NULL);
}


char *strip(char *word)
{
    while(is_space(*word))
        word++;

    return (word);
}

int main(int argc, char **argv, char **env) {
//    execlp("google-chrome", "google-chrome", "path=\\");
//    int a;
//    printf("> ");
//    scanf("%d", &a);
//    return (0);

    char *line;
    char *filepath;
    size_t letter_count = 0;
    char **args;
    struct stat file_stats;

    filepath = NULL;
    do
    {
        printf("$ ");
        getline(&line, &letter_count, stdin);

        if (!line[0])
            printf("exit!\n");

        line[strlen(line) - 1] = 0;
        line = strip(line);
        args = parse_string(line, ' ');
        if (!args)
            continue;

        filepath = get_file_path(parse_string(getenvvar("PATH") + strlen("PATH") + 1, ':'), args[0]);
        if (!filepath)
        {
            char *error_msg = cat(args[0], " no program with this name!");
            perror(error_msg);
            free(error_msg);
        }
    }
    while (!filepath);

    execve(filepath, args, env);

    return (0);
}