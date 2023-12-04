#include <string.h>
#include <jansson.h>
int main(int argc, const char **argv)
{
    size_t i;
    char *text;

    json_t *root;
    json_error_t error;

    FILE *file = fopen(argv[1], "rb");
    if (!file)
        return 1;

    char data[20000] = {0};
    fread(data, 20000 , 1, file);

    for(int i = 0; i<10; i++)
        printf("%u ", data[i]);
    printf("%s\n", data);

    root = json_loads(data, 0, &error);
    
    if(!root)
    {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        return 1;
    }
    return 0;

}
