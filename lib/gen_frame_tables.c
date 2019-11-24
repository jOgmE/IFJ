#include "gen_frame_tables.h"

// Nastavení výchozích hodnot
size_t GF_SIZE = 0;
size_t LF_SIZE = 0;
size_t TF_SIZE = 0;

fTable_t *gf_table;
fTable_t *lf_table;
fTable_t *tf_table;

char *get_frame_string(frame_t frame)
{
    switch (frame)
    {
    case GLOBAL_FRAME:
        return "GF";
    case LOCAL_FRAME:
        return "LF";
    case TEMP_FRAME:
        return "TF";
    default:
        global_error_code = INTERNAL_ERROR;
        print_internal_error(INTERNAL_ERROR, ERROR, "Frame index mimo rozsah enumu");
        return "";
    }
}

size_t get_table_size(frame_t frame)
{
    switch (frame)
    {
    case GLOBAL_FRAME:
        return GF_SIZE;
    case LOCAL_FRAME:
        return LF_SIZE;
    case TEMP_FRAME:
        return TF_SIZE;
    default:
        // nastavit error, nebo ne?
        return 0;
    }
}

void set_table_size(size_t size, frame_t frame)
{
    switch (frame)
    {
    case GLOBAL_FRAME:
        GF_SIZE = size;
        break;
    case LOCAL_FRAME:
        LF_SIZE = size;
        break;
    case TEMP_FRAME:
        TF_SIZE = size;
        break;
    default:
        // nastavit error, nebo ne?
        break;
    }
}

int hashcode(char *key, frame_t frame)
{
    int retval = 1;
    int keylen = strlen(key);

    for (int i = 0; i < keylen; i++)
    {
        retval += key[i];
    }

    size_t table_size = get_table_size(frame);

    return (retval % table_size);
}

void init_table(size_t size, frame_t frame)
{
    set_table_size(size, frame);

    if (frame == GLOBAL_FRAME)
    {
        gf_table = malloc(sizeof(ftItem_t) * size);

        for (size_t i = 0; i < size; ++i)
        {
            gf_table[i] = NULL;
        }
    }
    else if (frame == LOCAL_FRAME)
    {
        lf_table = malloc(sizeof(ftItem_t) * size);

        for (size_t i = 0; i < size; ++i)
        {
            lf_table[i] = NULL;
        }
    }
    else if (frame == TEMP_FRAME)
    {
        tf_table = malloc(sizeof(ftItem_t) * size);

        for (size_t i = 0; i < size; ++i)
        {
            tf_table[i] = NULL;
        }
    }
}

ftItem_t *search_table(char *key, frame_t frame)
{
    int i = hashcode(key, frame);

    size_t table_size = get_table_size(frame);

    if (i < 0 || i > table_size)
    {
        // global_error_code = INTERNAL_ERROR;
        return NULL;
    }

    ftItem_t *item;

    if (frame == GLOBAL_FRAME)
    {
        item = gf_table[i];
    }
    else if (frame == LOCAL_FRAME)
    {
        item = lf_table[i];
    }
    else if (frame == TEMP_FRAME)
    {
        item = tf_table[i];
    }

    if (item != NULL)
    {
        if (strcmp(item->key, key) == 0)
        {
            return item;
        }

        item = item->next;
    }

    return NULL;
}

void insert_table_item(char *item, e_type type, frame_t frame)
{
    int index = hashcode(item, frame);
    ftItem_t *table_item = search_table(item, frame);

    if (table_item == NULL)
    {
        ftItem_t *new_item = (ftItem_t *)malloc(sizeof(ftItem_t));
        new_item->key = item;
        new_item->type = type;
        new_item->next = NULL;

        if (frame == GLOBAL_FRAME)
        {
            if (gf_table[index] != NULL)
            {
                new_item->next = gf_table[index];
            }
            gf_table[index] = new_item;
        }
        else if (frame == LOCAL_FRAME)
        {
            if (lf_table[index] != NULL)
            {
                new_item->next = lf_table[index];
            }
            lf_table[index] = new_item;
        }
        else if (frame == TEMP_FRAME)
        {
            if (tf_table[index] != NULL)
            {
                new_item->next = tf_table[index];
            }
            tf_table[index] = new_item;
        }
    }
}

e_type get_table_item_type(char *key, frame_t frame)
{
    ftItem_t *item = search_table(key, frame);

    if (item != NULL)
    {
        return item->type;
    }
    else
    {
        return NULL;
    }
}

char *get_table_item(char *key, frame_t frame)
{
    ftItem_t *item = search_table(key, frame);

    if (item != NULL)
    {
        return item->key;
    }
    else
    {
        return NULL;
    }
}

bool item_exists_table(char *key, frame_t frame)
{
    if (get_table_item(key, frame) != NULL)
    {
        return true;
    }

    return false;
}

void delete_table_item(char *key, frame_t frame)
{
    int index = hashcode(key, frame);

    ftItem_t *item;
    ftItem_t *previousItem = NULL;

    if (frame == GLOBAL_FRAME)
    {
        item = gf_table[index];
    }
    else if (frame == LOCAL_FRAME)
    {
        item = lf_table[index];
    }
    else if (frame == TEMP_FRAME)
    {
        item = tf_table[index];
    }

    while (item != NULL)
    {
        if (strcmp(item->key, key) == 0)
        {
            if (previousItem == NULL)
            {

                if (frame == GLOBAL_FRAME)
                {
                    gf_table[index] = item->next;
                }
                else if (frame == LOCAL_FRAME)
                {
                    lf_table[index] = item->next;
                }
                else if (frame == TEMP_FRAME)
                {
                    tf_table[index] = item->next;
                }
            }
            else
            {
                previousItem->next = item->next;
            }

            free(item);
            return;
        }
        previousItem = item;
        item = item->next;
    }
}

void clear_table(frame_t frame)
{
    ftItem_t *item;
    bool all_clear = false;

    size_t table_size = get_table_size(frame);

    do
    {
        all_clear = true;
        for (size_t i = 0; i < table_size; ++i)
        {
            if (frame == GLOBAL_FRAME)
            {
                item = gf_table[i];
            }
            else if (frame == LOCAL_FRAME)
            {
                item = lf_table[i];
            }
            else if (frame == TEMP_FRAME)
            {
                item = tf_table[i];
            }

            if (item != NULL)
            {
                delete_table_item(item->key, frame);
                all_clear = false;
            }
        }
    } while (!all_clear);
}
