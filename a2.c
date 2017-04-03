/* a2.c 
   Yashar Nesvaderani
   yashar93@gmail.com
   March 3, 2017
   
   Translates one set of given characters to a second set of given characters */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXSIZE 512

char convert_slash(char);
void translate(char[], char[]);
int fix_escapes(char[]);
void truncate(char[], size_t);
void extend(char[], size_t, size_t);
int range(char set[]);

/* main function that processes the input sets and calls
   the translate function */
int main(int argc, char * argv[]) {
    char set1[MAXSIZE];
    char set2[MAXSIZE];
    
    /* checks for correct number of command line inputs and 
       prints error message on correct usage if invalid */
    if (argc != 4 && argc != 3)  {
        fprintf(stderr, "usage: %s [set1] [set2]", argv[0]);
        return 1;
    }
    
    /* checks for presence of -e switch and calls appropriate 
       function in each case */
    if(argc == 3 && strcmp(argv[1], "-e") != 0) {
        strcpy(set1, argv[1]);
        strcpy(set2, argv[2]);
        
        /* fixes the escape characters, if valid. Otherwise
           return and print error message */
        if((fix_escapes(set1))) {
            fprintf(stderr, "invalid escape characters\n");
            return 3;
        }
        
        if((fix_escapes(set2))) {
            fprintf(stderr, "invalid escape characters\n");
            return 3;
        }
        
         /* fixes the range of characters, if valid. Otherwise
            return and print error message */
        if((range(set1)) == 1) {
            fprintf(stderr, "invalid range\n");
            return 3;
        }
        
        if((range(set2)) == 1) {
            fprintf(stderr, "invalid range\n");
            return 3;
        }
        
        /* truncates the appropriate set if one is longer
           than the other */
        if (strlen(set1) > strlen(set2)){
            truncate(set1, strlen(set2));
        } else if (strlen(set2) > strlen(set1)) {
            truncate(set2, strlen(set1));
        }    
    } else if (argc == 4 && strcmp(argv[1], "-e") == 0) {
        strcpy(set1, argv[2]);
        strcpy(set2, argv[3]);
        
        /* fixes the escape characters, if valid. Otherwise
           return and print error message */
        if((fix_escapes(set1))) {
            fprintf(stderr, "invalid escape characters\n");
            return 3;
        }
        
        if((fix_escapes(set2))) {
            fprintf(stderr, "invalid escape characters\n");
            return 3;
        }
        
        /* fixes the range of characters, if valid. Otherwise
            return and print error message */
        if((range(set1))) {
            fprintf(stderr, "invalid range\n");
            return 3;
        }
        
        if((range(set2))) {
            fprintf(stderr, "invalid range\n");
            return 3;
        }
        
        /* extends the second set of the string, if necessary */
        if (strlen(set1) > strlen(set2)) {
            extend(set2, strlen(set1), strlen(set2));
        }
    }
    
    /* calls the translate function based on the two sets */
    translate(set1, set2);
    
    
    return 0;
}

/* translates the input from stdin and prints it
   to stdout */
void translate(char set1[], char set2[]) {
    int c;
    int translated = 0;
    size_t i, j, position;
    while ((c = getchar()) != EOF) {
        for (i = 0; set1[i] != '\0'; i++) {
            /* find the last occurence of the character
               to translate */
            for (j = 0; set1[j] != '\0'; j++) {
               if (set1[i] == set1[j]) {
                   position = j;
               }
            }
            /* translate the specified character */
            if (c == set1[position]) {
                putchar(set2[position]);
                translated = 1;
                break;
            }
        }
        /* prints the untranslated input character if no
           match in the sets */
        if (!translated){
            putchar(c);
        }
        translated = 0;
    }
}

/* fixes the escape characters in the given set */
int fix_escapes(char set[]) {
	size_t i, j = 0;
	for(i = 0; set[i] != '\0'; i++) {
		if(set[i] == '\\') {
			if((set[j] = convert_slash(set[i + 1])) == 0) {
                return 1;   
            }
            j++;
            i++;
		} else {
            set[j] = set[i];
            j++;
        }
	}
    set[j] = '\0';
    
    return 0;
}

/* fixes the specified escape character in the set */
char convert_slash(char a) {
	switch (a) {
		case 'a'  :
			return  '\a'; 
		case 'b'  :
			return  '\b';
		case 'f'  :
			return  '\f';
		case 'n'  :
			return  '\n';
		case 'r'  :
			return  '\r';
		case 't'  :
			return  '\t';
        case 'v'  :
            return  '\v';
        case '\'' :
            return  '\'';
        case '\"' : 
            return  '\"';
        case '\\' :
            return  '\\';
        default   :
            return 0;
	}
}

/* truncates the set to match the set size of the other */
void truncate(char set[], size_t n) {
    set[n] = '\0';
}

/* extends the set by repeating the last character
   until the length matches the longer set */
void extend(char set[], size_t n1, size_t n2) {
    size_t i;
    int character = set[n2 - 1];
    for (i = n2; i < n1; i++) {
        set[i] = character;
    }
    set[i] = '\0';
}

/* expands character set if valid ranges are given */
int range(char set[]) {
    size_t i, j = 0, isrange = 1;
    int k;
    char temp[MAXSIZE];
    
    temp[j] = set[0];
    j++;
    
    for (i = 1; set[i] != '\0'; i++) {
        if (set[i] == '-' && isrange) {
            if (set[i - 1] <= set[i + 1]) {
                isrange = 0;
                for(k = set[i - 1] + 1; k <= set[i + 1]; k++) {
                    temp[j] = k;
                    j++;
                }    
            } else {
                return 1;
            } 
            i++;    
        } else {
            temp[j] = set[i];
            j++;
            isrange = 1;
        }
    }
    temp[j] = '\0';
    strcpy(set, temp);
    
    return 0;
}