#include "console.h"


extern fifo_s_t uart1_rx_fifo;

int int_arg =0;
char str_arg[128]={0};

typedef void (*cmd_cb)(char **, char);

typedef struct
{
    char *cmd;
    cmd_cb cb;
} ST_CMD;

void list_proc(char **str, char);

void led_proc(char **str, char num)
{
    char buf[128] = {0};
    for (char i = 0; i < num; i++)
    {
        strcat(buf, *str);
        if (i >= num - 1)
        {
        }
        else
        {
            strcat(buf, " ");
        }
        str++;
    }
    if (num > 2)
    {
        printf("arg is to many\n");
        return;
    }
    if (strstr(buf, "-h"))
    {
        printf("led --help\nUsage: led [on/off]\n");
    }
    else if (strstr(buf, "on"))
    {
        led_write(1, 1);
        printf("led is on\n");
    }
    else if (strstr(buf, "off"))
    {
        led_write(1, 0);
        printf("led is off\n");
    }
    else
    {
        printf("cannot access \"%s\"\n", buf);
    }
}
void pwm_proc(char **str, char num)
{
    if(num>3)
    {
        printf("arg is to many\n");
        return;
    }
    if(num<2)
    {
        printf("arg is to few\n");
        return;
    }
    if(strcmp(*(str+1),"freq")==0)
    {
        pwm_freq(atoi(*(str+2)));
        pwm_help();
    }
    else if(strcmp(*(str+1),"duty")==0)
    {
        pwm_duty(atoi(*(str+2)));
        pwm_help();
    }
    else if(strcmp(*(str+1),"-h")==0)
    {
        pwm_help();
    }
    else
    {
        printf("cannot access \"config %s\"\n",*(str+1));
    }

}
void config_proc(char **str, char num)
{
    if(num>3)
    {
        printf("arg is to many\n");
        return;
    }
    if(num<3)
    {
        printf("arg is to few\n");
        return;
    }
    if(strcmp(*(str+1),"int_arg")==0)
    {
        int_arg=atoi(*(str+2));
        printf("\nconfig int_arg success\nint_arg is :%d",int_arg);
    }
    else if(strcmp(*(str+1),"str_arg")==0)
    {
        strcpy(str_arg,*(str+2));
        printf("\nconfig str_arg success\nint_arg is :%s",str_arg);
    }
    else
    {
        printf("cannot access \"config %s\"\n",*(str+1));
    }

}

ST_CMD cmd_list[] =
    {
        {"ls", list_proc},
        {"led", led_proc},
        {"config",config_proc},
        {"pwm",pwm_proc},
};

void list_proc(char **str, char num)
{
    int i, size = sizeof(cmd_list) / sizeof(ST_CMD);
    for (i = 0; i < size; i++)
    {
        printf("%s ", cmd_list[i].cmd);
    }
    printf("\n");
    // 以下代码调试使用，可以输出分割后的全部参数
    // for(i=0;i<num;i++)
    // {
    //     printf("%s\n",*str);
    //     str++;
    // }
}

void shell_proc()
{
    char buf[128] = {0};
    char *sub_buf[20];
    char num = 0;
    if (fifo_s_isempty(&uart1_rx_fifo) == 0)
    {
        fifo_s_gets(&uart1_rx_fifo, buf, fifo_s_used(&uart1_rx_fifo));
        num = split_string(buf, " ", sub_buf, 20);
        int i, size = sizeof(cmd_list) / sizeof(ST_CMD);
        for (i = 0; i < size; i++)
        {
            if (strcmp(sub_buf[0], cmd_list[i].cmd) == 0)
            {
                cmd_list[i].cb((char **)sub_buf, num);
                break;
            }
            else if (i >= size - 1)
            {
                printf("%s is not an executable command\n", sub_buf[0]);
            }
        }
    }
}

char split_string(char *str, const char *delim, char *sub_ptr[], char size)
{
    char *token = NULL;
    unsigned char idx = 0;
    token = strtok(str, delim);
    while (token && idx < size)
    {
        sub_ptr[idx++] = token;
        token = strtok(NULL, delim);
    }
    return idx;
}
