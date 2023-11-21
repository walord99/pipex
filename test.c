int	get_arg_len(char *str);
char	*find_arg_end(char *str);
#include "pipex.h"
t_list	*split_args(char *args);

int main()
{
    t_list *args = split_args("echo 'wer ' \\\"fuck");
}