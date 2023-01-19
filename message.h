struct message_struct
{
    char * to, * from, * content;
} __attribute__((packed));

typedef struct message_struct message;