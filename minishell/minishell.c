#include "minishell.h"

/*
    had l function katxof wax x7al m argument ktebt
*/
size_t ft_arrlen(char **str)
{
    size_t (i) = 0;
    if (str)
    {
        while (str[i])
            i++;
    }
    return (i);
}

/*
    had l function katxof wax dak cmd ra9m wla la 
*/
// int check_if_num(char *cmd)
// {
//     int (i) = 0;
//     int (flag) = 0;
//     while (cmd[i])
//     {
//         if (cmd[i] == '-' && flag == 0)
//             flag = 1;
//         else if (ft_isdigit(cmd[i]) == 0)
//             return (0);
//         i++;
//     }
//     return (1);
// }

// int check_if_space(char *cmd)
// {
//     int (i) = 0;
//     while (cmd[i])
//     {
//         if (ft_isspace(cmd[i]) == 0)
//             return (0);
//         i++;
//     }
//     return (1);
// }

/*
    had l function dertha 3la 9bel l exit built in o fiha l parsing ta3ha tahowa mjmou3
*/
// void pars_and_exc_exit(char **cm, int i, int *state)
// {
//     long (exit_num) = 1;
//     if (cm[i + 1] == NULL || ft_strcmp(cm[i + 1], "0") == 0
//         || ft_strcmp(cm[i + 1], "256") == 0)
//         exit(0);
//     else if (ft_strcmp(cm[i + 1], "\0") == 0)
//     {
//         write(2, "exit: ", 6);
//         write(2, cm[i + 1], ft_strlen(cm[i + 1]));
//         write(2, ": numeric argument required\n", 29);
//         exit(0);
//     }
//     else
//     {
//         if (check_if_num(cm[i + 1]) == 0 && ft_strcmp(cm[i + 1], "$?") != 0)
//         {
//             write(2, "exit: ", 6);
//             write(2, cm[i + 1], ft_strlen(cm[i + 1]));
//             write(2, ": numeric argument required\n", 29);
//             exit(2);
//         }
//         if (ft_strcmp(cm[i + 1], "$?") != 0)
//             exit_num = ft_atoi(cm[i + 1]);
//         if (exit_num == 0 || exit_num > 9223372036854775807 || exit_num < -9223372036854775807LL -1)
//         {
//             write(2, "exit: ", 6);
//             write(2, cm[i + 1], ft_strlen(cm[i + 1]));
//             write(2, ": numeric argument required\n", 29);
//             exit(2);
//         }
//         else if (ft_arrlen(cm) > 2)
//         {
//             write(2, "exit: too many arguments\n", 25);
//             exit (2);
//         }
//         else if (ft_strcmp(cm[i + 1], "$?") == 0)
//             exit(*state);
//         exit(exit_num);
//     }
// }

// void exec_build_in(char *cmd, char **cm, int i, int *state)
// {
//     if (ft_strcmp(cmd, "cd") == 0)
//         chdir(cm[i + 1]);
//     else if (ft_strcmp(cmd, "exit") == 0)
//         pars_and_exc_exit(cm, i, state);
// }

// int ft_echo(char **cmd){}

void ft_error(char *cmd, char *message)
{
    ft_putstr_fd(cmd, 2);
    ft_putstr_fd(message, 2);
}

void chd_and_free(char *cmd)
{
    chdir(cmd);
    free(cmd);
}

int ft_cd(char **cmd, int q, char **env)
{
    (void) env;
    char (*cm) = cmd[1];
    char *tmp = NULL;
    struct stat path_stat;
    
    if (ft_arrlen(cmd) > 2)
        ft_error(cmd[0], ": too many arguments\n");
    else if (cm == NULL || cm[0] == '~')
    {
        if (cm != NULL && cm[1] != '\0')
        {
            tmp = cm;
            cm = ft_strjoin(getenv("HOME"), ft_substr(cm, 1, ft_strlen(cm)));
            free(tmp);
        }
        else
            cm = ft_strdup(getenv("HOME"));
        chd_and_free(cm);
        return (0);
    }
    if (q > 0)
        cm = ft_substr(cmd[1], 1, ft_strlen(cmd[1]) - 2);
    if (ft_strcmp(cm, "-") == 0)
    {
        free(cm);
        cm = getenv("OLDPWD");
        chdir(cm);
        return (0);
    }
    if (stat(cm, &path_stat) == 0)
    {
        if (S_ISDIR(path_stat.st_mode) == 0)
            ft_error(cm, ": Not a directory\n");
    }
    else if (access(cm, F_OK) != 0)
        ft_error(cm, ": no such file or directory\n");
    chdir(cm);
    return (0);
}
// int ft_exit(char **cmd){}
// int ft_pwd(char **cmd){}
// int ft_export(char **cmd){}
int ft_unset(char **cmd, int q, char **env)
{
    int (i) = 1;
    int y;
    char (*cm) = NULL;
    if (ft_arrlen(cmd) < 2)
        return (0);        
    while (cmd[i])
    {
        y = 0;
        while (env[y])
        {
            cm = cmd[i];
            if (q > 0)
            {
                if (cm[0] == '"' || cm[0] == '\'')
                    cm = ft_substr(cm, 1, ft_strlen(cm) - 2);
            }
            if (cm[0] == '_' || ft_isalpha(cm[0]) == 1)
            {
                cm = ft_strjoin(cm, "=");
                if (ft_strncmp(cm, env[y], ft_strlen(cm)) == 0)
                    env[y] = "\0";
                free(cm);
            }
            y++;
        }
        i++;
    }
    return (0);
}

int ft_env(char **cmd, int q, char **env)
{
    (void) q;
    (void) cmd;
    int (i) = 0;
    while (env[i])
    {
        printf("%s\n", env[i]);
        i++;
    }
    return (0);
}


void check_build_in(char **cmd, int quotes, char **env)
{
    int (i) = 0;
    t_build built_in[] = 
    {
        // {"echo", ft_echo},
        {"cd", ft_cd},
        // {"exit", ft_exit},
        // {"pwd", ft_pwd},
        // {"export", ft_export},
        {"unset", ft_unset},
        {"env", ft_env},
        {NULL, NULL}
    };
    while (built_in[i].name)
    {
        if (ft_strcmp(cmd[0], "env") == 0)
        {
            if (ft_arrlen(cmd) > 1)
                ft_error(cmd[0], ": too many arguments\n");
            else
            {
                ft_env(env, quotes, env);
                break ;
            }
        }
        else if (ft_strcmp(cmd[0], built_in[i].name) == 0)
        {
            built_in[i].func(cmd, quotes, env);
            break ;
        }
        i++;
    }
    // int (i) = 0;
    // while (cmd[i])
    // {
    //     exec_build_in(cmd[i], cmd, i);
    //     i++;
    // }
}

// int is_built_in(char **cmd, char **built_in)
// {
//     int (i) = 0;
//     int y;

//     while (cmd[i])
//     {
//         y = 0;
//         while (built_in[y])
//         {
//             if (ft_strcmp(cmd[i], built_in[y]) == 0)
//                 return (0);
//             y++;
//         }
//         i++;
//     }
//     return (1);
// }

// void inti_built_in(char *b[])
// {
//     b[0] = "cd";
//     b[1] = "exit";
//     b[2] = NULL;
// }

// void execute_command(char *line, int *state, char **env)
// {
//     int pid;
//     char *path;
//     char *tmp;
//     char *full_path;
//     char **cmd_path;
//     char **cmd;
//     char *built_in[3];
//     int i = 0;
            
//     inti_built_in(built_in);
//     cmd = ft_split(line);
//     // check_build_in(cmd, state);
//     pid = fork();
//     if (pid == -1)
//         exit(1);
//     if (pid == 0)
//     {
//         if (is_built_in(cmd, built_in) == 1)
//         {
//             path = getenv("PATH");
//             cmd_path = ft_split(path);
//             if (access(cmd[0], X_OK) == 0)
//                 execve(cmd[0], cmd, env);
//             while (cmd_path[i])
//             {
//                 tmp = ft_strjoin(cmd_path[i], "/");
//                 full_path = ft_strjoin(tmp, cmd[0]);
//                 free(tmp);
//                 if (access(full_path, X_OK) == 0)
//                 {
//                     execve(full_path, cmd, env);
//                     exit(1);
//                 }
//                 i++;
//             }
//             if (!cmd_path[i])
//             {
//                 write (2, cmd[0], ft_strlen(cmd[0]));
//                 write (2, ": command not found\n", 20);
//                 exit (127);
//             }
//         }
//     }
//     wait(state);
//     *state = WEXITSTATUS(*state);
// }

int pars_input(char *line)
{
    int (double_quotes) = 0;
    int (single_quotes) = 0;
    int (i) = 0;
    while (line[i])
    {
        if (line[i] == '\'')
            single_quotes++;
        else if (line[i] == '\"')
            double_quotes++;
        i++;
    }
    if (single_quotes != 0)
    {
        if (single_quotes % 2 == 0)
            return (1);
        else
            return (-1);
    }
    else if (double_quotes != 0)
    {
        if (double_quotes % 2 == 0)
            return (2);
        else
            return (-1);
    }
    return (0);
}

int main(int ac, char **av, char **env)
{
    char *line;
    // int (state) = 0;
    char **cmd;
    int quotes;
    (void)ac;
    (void)av;

    line = NULL;
    quotes = 0;
    cmd = NULL;
    while (1)
    {
        line = readline(GREEN "minishell$ " RESET);
        if (line == NULL)
        {
            free(line);
            break ;
        }
        if (*line)
            add_history(line);
        if (pars_input(line) == -1)
            write (2, "Error\n", 6);
        else if (pars_input(line) == 1)
            quotes = 1;
        else if (pars_input(line) == 2)
            quotes = 2;
        cmd = ft_split(line);
        check_build_in(cmd, quotes, env);
        // execute_command(line, &state, env);
        free(line);
    }
    rl_clear_history();
}