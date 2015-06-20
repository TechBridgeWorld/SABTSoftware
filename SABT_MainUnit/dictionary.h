/*
 * dictionary.h
 *
 * Created: 6/18/2015 8:45:21 AM
 *  Author: Susan Zuo(tzuo)
 */ 


#ifndef DICTIONARY_H_
#define DICTIONARY_H_

typedef struct dictionary dictionary_t;
/**
 *  a struct to store words loaded from SD card
 */
struct dictionary {
    char **words;
    int num_words;
    int index;
    int *index_array;
};

#endif /* DICTIONARY_H_ */