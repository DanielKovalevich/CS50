// This is a program that will take a string and an code word to scramble the string

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cs50.h>
#include <ctype.h>

int scramble(string message, string key);

int main(int argc, string argv[]){
    if (argv[1] < 0 || argc > 2 || argc <= 1){
        printf("Invalid command-line arguement.\n");
        return 1;``
    }
    string k = argv[1];
    
    //Makes sure all letters are letters
    for (int i = 0; i < strlen(k); i++){
        if (isalpha(k[i]) == false){
            printf("No numbers in the key");
            return 1;
        }
    }
    
    string input = GetString();
    scramble(input, k);
    
    return 0;
}

int scramble(string message, string key){
    int j = 0;
    for (int i = 0; i < strlen(message); i++){
        int c = 0, b = 0;
        
        //This is to change the letter of the key to represent numbers
        if (key[j] > 25){
            if (key[j] >= 65 && key[j] <= 90)
                b = key[j] - 65;
            if (key[j] >= 97 && key[j] <= 122)
                b = key[j] - 97;
        }
        //This changes the letter when it is a capital
        if (message[i] >= 65 && message[i] <= 90){
            c = message[i] - 65;
            message[i] = (c + b) % 26 + 65;
            j++;
        }
        //This changes the letter when it is lower case
        else if (message[i] >= 97 && message[i] <= 122){
            c = message[i] - 97;
            message[i] = (c + b) % 26 + 97;
            j++;
        }
        
        //So that you can continually iterate over the key
        if (j == strlen(key))
            j = 0;
    }
    
    printf("%s\n", message);
    return 0;
}
