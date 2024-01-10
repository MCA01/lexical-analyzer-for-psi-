#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 10
#define MAX_LEN 100
#define MAX_IDENTIFIER 30


int isDelimiter(char ch) {
    if (ch == '+' || ch == '-' || ch == ':' || ch == '{' ||
        ch == ',' || ch == ';' || ch == '>' || ch == '}' ||
        ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
        ch == '[' || ch == ']' || ch == '/' || ch == '*' || ch == ' ') {
        return 0; // It's valid delimiter.
    }
    else {
        return 1; // It's not a valid delimiter.
    }
}

int isIdentifier(char* string) {

    if (strlen(string) > MAX_IDENTIFIER) {
        return 1; // "Identifier is larger than max identifier value."
    }

    else if ( string[0] == 0 || string[0] == 1 || string[0] == 2 || string[0] == 3 || string[0] == 4 || string[0] == 5 ||
              string[0] == 6 || string[0] == 7 || string[0] == 8 || string[0] == 9 || isDelimiter(string[0]) == 0
            ) {
        return 2; // "Identifier cannot be started with numbers or delimiters."
    }

    else {
        // Makes the string lowercase.
        int i = 0;
        for (i; i < strlen(string); i++) {
            string[i] = tolower(string[i]);
        }
        return 0; // "Identifier(" + string + ")"
    }
}

int isIntConst(char* string) {

    int len = strlen(string);
    if (len == 0) {
        return 1; // "Null."
    }
    int i = 0;
    for (i; i < len; i++) {
        if ( string[i] != '0' && string[i] != '1' && string[i] != '2' && string[i] != '3' && string[i] != '4' && string[i] != '5' &&
             string[i] != '6' && string[i] != '7' && string[i] != '8' && string[i] != '9'
                ) {
            return 2; // "Not a valid integer constant."
        }
    }
    return 0; // " IntConst(" + string + ")"
}

int isOperator(char* string) {
    if ( strcmp(string, "+") == 0) {
        return 0;
    }
    else if (strcmp(string, "-") == 0) {
        return 0;
    }
    else if (strcmp(string, "*") == 0) {
        return 0;
    }
    else if (strcmp(string, "/") == 0) {
        return 0;
    }
    else if (strcmp(string, "++") == 0) {
        return 0;
    }
    else if (strcmp(string, "--") == 0) {
        return 0;
    }
    else if (strcmp(string, ":=") == 0) {
        return 0;
    }
    else {
        return 1; // "Not a valid operator."
    }
}

int isBracket(char string) {
    if (string == '(') {
        return 0; // "LeftPar"
    }
    else if (string == ')') {
        return 1; // "RightPar"
    }
    else if (string == '[') {
        return 2; // "LeftSquareBracket"
    }
    else if (string == ']') {
        return 3; // "RightSquareBracket"
    }
    else if (string == '{') {
        return 4; // "LeftCurlyBracket"
    }
    else if (string == '}') {
        return 5; // "RightCurlyBracket"
    }
}


int isKeyword(char* string) {

    char* temp = strlwr(string);

    char keywords[18][8] = { "break", "case", "char", "const", "continue", "do", "else", "enum", "float", "for", "goto", "if", "int", "long", "record", "return", "static", "while" };
    int j = 0;
    for (j; j < 18; j++) {
        if (strcmp(keywords[j], temp) == 0) {
            return 0; // "Keyword(" + keywords[j] + ")"
        }
    }
    return 1; // "Not a valid keyword."
}

int isEndOfLine(char ch) {

    if (ch == ';') {
        return 0; // "EndOfLine"
    }
    else {
        return 1; // Not end of line
    }
}

int isComment(char* string) {

    if (string[0] == '/' && string[1] =='*') {

        int i = 2; // Starting from the index 2 to pass comment mark

        while (string[i] != '*' || string[i+1] != '/') {

            if (string[i] == '\0') {
                return 1; // "Comment token isn't terminated before file end."
            }

            i++;

        }
        return 0; // Comment found but it doesn't provide token.
    }
    return 2; // Not a string.
}


char* getSubstring(char* string, int left, int right) {
    int i;
    char* subStr = (char*) malloc(sizeof(char) * (right - left + 2));

    for (i = left; i <= right; i++)
        subStr[i - left] = string[i];

    subStr[right - left + 1] = '\0';
    return subStr;
}

void removeSpaces(char* s) {
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
}


void detectTokens(char* string, FILE* fp) {
    int left = 0, right = 0;
    int length = strlen(string);
    int isString = 0;

    while (right <= length && left <= right) {
        if (isDelimiter(string[right]) == 1) {
            right++;
        }

        if (isDelimiter(string[right]) == 0 && left == right && isString == 0) {

            // Making a string with string[right] and the next char
            char concatenated[3];
            strcpy(concatenated, "");
            strncat(concatenated, &string[right], 1);
            strncat(concatenated, &string[right + 1], 1);

            // Making a string with only string[right] to be able to call isOperator()
            char str1[2] = {string[right], '\0'};

            if (string[right] == '"') { // Start of a string constant
                isString = 1;
                right++;
                continue;
            }
            else if (isOperator(concatenated) == 0) {
                printf("Operator(%s)\n", concatenated);
                fprintf(fp, "Operator(%s)\n", concatenated);
            }
            else if (isDelimiter(string[right - 1]) == 1 && isOperator(str1) == 0) {
                printf("Operator(%s)\n", str1);
                fprintf(fp, "Operator(%s)\n", str1);
            }
            else if (isEndOfLine(string[right]) == 0) {
                printf("End Of Line\n");
                fputs("End Of Line\n",fp);
            }
            else if (isBracket(string[right]) == 0) {
                printf("LeftPar\n", string[right]);
                fputs("LeftPar\n", fp);
            }
            else if (isBracket(string[right]) == 1) {
                printf("RightPar\n", string[right]);
                fputs("RightPar\n", fp);
            }
            else if (isBracket(string[right]) == 2) {
                printf("LeftSquareBracket\n", string[right]);
                fputs("LeftSquareBracket\n", fp);
            }
            else if (isBracket(string[right]) == 3) {
                printf("RightSquareBracket\n", string[right]);
                fputs("RightSquareBracket\n", fp);
            }
            else if (isBracket(string[right]) == 4) {
                printf("LeftCurlyBracket\n", string[right]);
                fputs("LeftCurlyBracket\n", fp);
            }

            else if (isBracket(string[right]) == 5) {
                printf("RightCurlyBracket\n", string[right]);
                fputs("RightCurlyBracket\n", fp);
            }

            right++;
            left = right;
        }

        if (((isDelimiter(string[right]) == 0 && left != right) || (right == length && left != right)) && isString == 0) {

            char *subStr = getSubstring(string, left, right - 1);
            // Deleting whitespaces from subStr
            removeSpaces(subStr);

            if (isKeyword(subStr) == 0) {
                printf("Keyword(%s)\n", subStr);
                fprintf( fp, "Keyword(%s)\n", subStr);
            }
            else if (isIntConst(subStr) == 0) {
                printf("IntConst(%s)\n", subStr);
                fprintf(fp, "IntConst(%s)\n", subStr);
            }
            else if (isIdentifier(subStr) == 0 && isDelimiter(string[right - 1]) == 1) {
                printf("Identifier(%s)\n", subStr);
                fprintf(fp, "Identifier(%s)\n", subStr);
            }
            else if (isIdentifier(subStr) == 1 && isDelimiter(string[right - 1]) == 1) {
                printf("Invalid Identifier (Identifier is larger than max identifier value): '%s'\n", subStr);
            }
            else if (isIdentifier(subStr) == 2 && isDelimiter(string[right - 1]) == 1) {
                printf("Invalid Identifier (Identifier cannot be started with numbers or delimiters): '%s'\n", subStr);
            }

            left = right;
        }
        if (isString == 1 && string[right] != '"') {
            right++;
        }

        if (isString == 1 && string[right] == '"') {
            // Extract the string constant (including spaces)
            char* subStr = getSubstring(string, left + 1, right - 1);
            printf("StringConst(%s)\n", subStr);
            fprintf(fp, "StringConst(%s)\n", subStr);
            isString = 0;
            right++;
            left = right;
        }
    }
    return;
}

int main(int argc, char *argv[]) {

    // Reading the code.psi file
    FILE *cfPtr; // File pointer

    char data[MAX_LINE][MAX_LEN]; // The array that we store the data, we store each line of the file as a string

    int line = 0;

    cfPtr = fopen("code.psi", "r");

    // Exits the file if file cannot be opened
    if (cfPtr == NULL) {
        printf("File could not be opened!\n");
    } else {

        // Reads the line if end-of-file is not true and an error doesn't occur
        while (!feof(cfPtr) && !ferror(cfPtr)) {

            if (fgets(data[line], MAX_LEN, cfPtr) != NULL) {
                line++;
            }
        }
        fclose(cfPtr);
    }
    // End of reading

    // Creating the code.lex file
    FILE *fp;
    fp = fopen("code.lex", "w");


    // Test
    int j = 0;
    for (j; j < line; j++) {
        printf("%s\n", data[j]);
    }

    int i = 0;

    for (i; i < line; i++) {

        // Deleting \n from the end
        size_t ln = strlen(data[i]) - 1;
        if (*data[i] && data[i][ln] == '\n')
            data[i][ln] = '\0';

        detectTokens(data[i], fp);
    }

    return 0;
}
