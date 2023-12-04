#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include <stdint.h>

#define FLAG_MAKE_STR 0x1
#define FLAG_ADD_TO_OBJ 0x2
#define FLAG_ADD_TO_ARR 0x4

#define POP(_name, _data, _size, _type) \
    if (_size < sizeof(_type)) \
        goto cleanup; \
    _size -= sizeof(_type); \
    _type _name = (_type)*_data; \
    _data += sizeof(_type); 

#define POP_PTR(_name, _data, _size, _type, _cus_size) \
    if (_size < _cus_size) \
        goto cleanup; \
    _size -= _cus_size; \
    _type *_name = ((_type*)_data); \
    _data += _cus_size;


int LLVMFuzzerTestOneInput(char *data, size_t size)
{
    json_t *root = NULL;
    json_error_t error = {0};

    json_t *random_str = NULL;
    char *new_data = malloc(size + 1);
    char *data_cur = new_data;
    if (!data_cur)
        return 1;
    memcpy(data_cur, data, size);
    data_cur[size] = '\0';

    POP(json_decode_flags, data_cur, size, uint8_t);
    POP(json_encode_flags, data_cur, size, uint32_t);
    POP(custom_flags, data_cur, size, uint8_t);

    size_t i;
    char *text;

    root = json_loadb(data_cur, size, json_decode_flags, &error);
    if (!root)
        goto cleanup;

    if (custom_flags & FLAG_MAKE_STR) {
        POP_PTR(json_new_string, data_cur, size, char, 255);
        json_new_string[254] = '\0';
        random_str = json_string(json_new_string);
    }

    if (custom_flags & FLAG_ADD_TO_OBJ) {
        if (random_str)
            json_object_set(root, "lmao", random_str);
    }

    if (custom_flags & FLAG_ADD_TO_ARR) {
        if (random_str)
            json_array_append(root, random_str);
    }

    {
        char *dump = json_dumps(root, json_encode_flags);
        if (dump)
            free(dump);
    }

    {
        size_t json_size = json_dumpb(root, NULL, 0, 0);
        if (json_size == 0)
            goto cleanup;
        char *buf = malloc(json_size);
        if (!buf)
            goto cleanup;
        json_size = json_dumpb(root, (char*)buf, json_size, 0);
        free(buf);
    }

    cleanup:
    if (random_str)
        json_decref(random_str);

    if (root)
        json_decref(root);
        
    free(new_data);
    return 0;

}
