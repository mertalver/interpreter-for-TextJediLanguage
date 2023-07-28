#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef enum { //Tokenize etmek için token tiplerini tanımlar
    Identifier,
    IntegerConstant,
    Operator,
    StringConstant,
    Keyword,
    EndOfLine,
} TokenType;

typedef struct {
    TokenType type; //Tanımlama yaptıktan sonra tokenleri saklar.
    union {
        int intValue;
        char stringValue[500];
        char keyword[500];
        char identifier[500];
        char operator[500];
        char eof[500];
    } value;
} Token;

typedef enum { //Değişken içerisinde ne tür bir veri saklandığını tanımlar (assign işlemi sonucunda)
    Integer,
    String
} ValueType;

typedef struct { //Değişkenin içerisindeki değerleri saklar.
    char name[500];
    ValueType valueType;
    union {
        char strValue[500];
        int intValue;
    } value;
} Variable;

bool delimiterCheck(char ch) //Okunan karakterin sınırlayıcı (delimiter) olup olmadığını kontrol edip true veya false döndürür.
{
    if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == ',' || ch == ':' || ch == ';' ||
        ch == '=' || ch == '(' || ch == ')' ||
        ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '"'||
        ch == '\t' || ch == '\n')
        return (true);
    return (false);
}

bool identifierCheck(const char* input) //Okunan karakterin identifier olup olmadığını kontrol edip true veya false döndürür.
{                                       //Aynı zamanda identifier'ın rakamla başlamasını engeller.
    //Eğer identifier rakamla başladıysa algılamaz. (Hoca engellemese de olur sıkıntı olmaz diye de belirtmişti.)
    if  (input[0] == '0' || input[0] == '1' || input[0] == '2' ||
         input[0] == '3' || input[0] == '4' || input[0] == '5' ||
         input[0] == '6' || input[0] == '7' || input[0] == '8' ||
         input[0] == '9' || delimiterCheck(input[0]) == true)
        return false;
    return true;
}


bool integerCheck(char* input) //Okunan karakterin tam sayı olup olmadığını kontrol edip true veya false döndürür.
{
    int i;
    int len = strlen(input);

    if (len == 0)
        return false;
    for (i = 0; i < len; i++) {
        if (input[i] != '0' && input[i] != '1' && input[i] != '2'
            && input[i] != '3' && input[i] != '4' && input[i] != '5'
            && input[i] != '6' && input[i] != '7' && input[i] != '8'
            && input[i] != '9')
            return false;
    }
    return true;
}

bool operatorCheck(char input) //Okunan karakterin operatör olup olmadığını kontrol edip true veya false döndürür.
{
    if (input == '+' || input == '-' || input == ':' || input == '=' || input == '*' || input == '/')
        return true;
    return false;
}

bool keywordCheck(char* input) //Okunan karakterin keywordlerden biriyle eşleşip eşleşmediğini kontrol eder.
{
    if (!strcmp(input, "new") || !strcmp(input, "int") ||
        !strcmp(input, "text") || !strcmp(input, "size") ||
        !strcmp(input, "subs") ||
        !strcmp(input, "locate") || !strcmp(input, "insert")
        || !strcmp(input, "override") || !strcmp(input, "read")
        || !strcmp(input, "write") || !strcmp(input, "from")
        || !strcmp(input, "to") || !strcmp(input, "input")
        || !strcmp(input, "output") || !strcmp(input, "asText")
        || !strcmp(input, "asString"))
        return true;
    return false;
}

char* subString(const char* str, int left, int right) //Eğer art arda gelen karakterler birer harfse bunları harf harici bir karakter gelene kadar alt dizeye (substring) ayırır.
{
    int i;
    char* subStr = (char*)malloc(sizeof(char) * (right - left + 2));

    for (i = left; i <= right; i++)
        subStr[i - left] = str[i];
    subStr[right - left + 1] = '\0';
    return subStr;
}

void appendToken(Token token, Token* tokenList, int* tokenCount) { //Tokenleri tokenList adlı yapıya ekler.
    tokenList[*tokenCount] = token;
    (*tokenCount)++;
}

void removeSubstring(char* str, const char* sub) { //İki string arasında çıkarma işlemini yapar.
    char* match = strstr(str, sub);

    if ((strlen(sub) > strlen(str)) || match == NULL) {
        printf("ERROR! SUBSTRING IS LARGER THAN THE STRING.\n"); //Eğer çıkan değeri, eksilenden büyük veya eksilen kelimesinin içerisinde bulunamadıysa hata verir.
    }

    if (match != NULL) {
        memmove(match, match + strlen(sub), strlen(match + strlen(sub)) + 1);
    }
}

int locate(const char *firstText, const char *secondText, int start) { //Verilen stringi bir diğer string içerisinde arar, bulunulan indeks sonucu integer olarak döndürür.
    int i, j;

    for (i = start; firstText[i] != '\0'; i++) {
        if (firstText[i] == secondText[0]) {
            for (j = 0; secondText[j] != '\0'; j++) {
                if (firstText[i + j] != secondText[j])
                    break;
            }
            if (secondText[j] == '\0')
                return i;
        }
    }
    return 0;
}

char* insert(const char* firstText, int location, const char* insertText) { //Bir stringi diğer string üzerine verilen indeks değerinin olduğu yere ekler.
    int myTextLen = strlen(firstText);
    int insertTextLen = strlen(insertText);

    if (location < 0 || location > myTextLen)
        return NULL;

    int resultLen = myTextLen + insertTextLen + 1;
    char* result = (char*)malloc(resultLen * sizeof(char));

    if (result == NULL)
        return NULL;

    strncpy(result, firstText, location);
    strncpy(result + location, insertText, insertTextLen);
    strcpy(result + location + insertTextLen, firstText + location);

    return result;
}

void override(char* firstText, int location, const char* secondText) { //Bir stringi diğer string üzerine verilen indeks değerinin olduğu yere override olacak şekilde ekler.
    int firstTextLength = strlen(firstText);
    int secondTextLength = strlen(secondText);
    int i;

    for (i = 0; i < secondTextLength; i++) {
        if (location + i >= firstTextLength) {
            break;
        }
        firstText[location + i] = secondText[i];
    }
}

char* read_fromTXT(const char* filename) { //Adı kullanıcıdan alınan text dosyasından okuma yapar.
    char* full_filename = malloc(strlen(filename) + 5);
    strcpy(full_filename, filename);
    strcat(full_filename, ".txt");

    FILE* file = fopen(full_filename, "r");
    if (file == NULL) {
        printf("ERROR: FILE DOES NOT EXIST, NO SUCH FILE OR DIRECTORY, INTERPRETER CANNOT FIND THE FILE SPECIFIED\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = malloc(file_size + 1);
    if (content == NULL) {
        printf("MEMORY ERROR!\n");
        fclose(file);
        return NULL;
    }

    fread(content, 1, file_size, file);
    content[file_size] = '\0';

    fclose(file);
    return content;
}

void write_toTXT(const char* filename, const char* content) { //Adı verilen text dosyasına yazdırma yapar, böyle bir dosya yoksa oluşturur.
    char* full_filename = malloc(strlen(filename) + 5);
    strcpy(full_filename, filename);
    strcat(full_filename, ".txt");

    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        printf("ERROR: FILE DOES NOT EXIST, NO SUCH FILE OR DIRECTORY, INTERPRETER CANNOT FIND THE FILE SPECIFIED.\n");
        return;
    }

    fputs(content, file);

    if (ftell(file) > 0) {
        fputs(" ", file);
    }

    fclose(file);
}

void inputText(char* myText, const char* promptText) { //Terminal üzerinden input olarak 100 kelimelik string alır bunu myText'e yazdırır.
    printf("%s", promptText);
    fgets(myText, 100, stdin);
}

char* asString(int myInt) { //Girilen integer'ı, stringe çevirir (type conversion)
    char* str = (char*)malloc(100 * sizeof(char));
    snprintf(str, 12, "%d", myInt);
    return str;
}

int asText(const char *myString) { //Girilen integer'ı, stringe çevirir (type conversion)
    return atoi(myString);
}

void parse(char* input, Token* tokenList, int* tokenCount) //Parse işlemini yapar, işlemi yaparken tokenleri sınıflandırır.
{
    int left = 0;
    int right = 0;
    int len = strlen(input);
    Token token;

    while (right < len) {
        if (delimiterCheck(input[right]) == false)
            right++;

        if (delimiterCheck(input[right]) == true && left == right) {
            if (input[right] == '"') {
                right++;
                char *str_constant = (char*) malloc(1000);
                int i = 0;
                while (input[right] != EOF) {
                    str_constant[i++] = input[right];
                    right++;
                    if (input[right] == '"') {
                        str_constant[i] = '\0';
                        strncpy(token.value.stringValue, str_constant, sizeof(token.value.stringValue) - 1);
                        token.type = StringConstant;
                        appendToken(token, tokenList, tokenCount);
                        right++;
                        break;
                    }
                }
                if (input[right] == EOF){
                    printf("ERROR! STRING CONSTANT IS NOT CLOSED.\n"); //String constantın tırnak işareti kapanmadıysa hata yazdırır.
                    break;
                }
            }

            if (input[right] == '/' && input[right + 1] == '*') {
                right = right + 2;
                while (input[right] != EOF) {
                    right++;
                    if (input[right] == '*' && input[right + 1] == '/') {
                        right = right + 2;
                        break;
                    }
                }
                if (input[right] == EOF) {
                    printf("ERROR! COMMENT LINE IS NOT CLOSED.\n"); //Yorum satırı kapanmadıysa hata yazdırır
                    break;
                }
            }

            if (operatorCheck(input[right]) == true) {
                if (input[right] == '+') {
                    strncpy(token.value.operator, "+", sizeof(token.value.operator) - 1);
                    token.type = Operator;
                    appendToken(token, tokenList, tokenCount);
                }

                if (input[right] == ':') {
                    if (input[right + 1] == '=') {
                        strncpy(token.value.operator, ":=", sizeof(token.value.operator) - 1);
                        token.type = Operator;
                        appendToken(token, tokenList, tokenCount);
                        right++;
                    }
                }

                else if (input[right] == '-') {
                    strncpy(token.value.operator, "-", sizeof(token.value.operator) - 1);
                    token.type = Operator;
                    appendToken(token, tokenList, tokenCount);
                }
            }
            if (input[right] == ';') {
                strncpy(token.value.eof, ";", sizeof(token.value.eof) - 1);
                token.type = EndOfLine;
                appendToken(token, tokenList, tokenCount);
            }

            right++;
            left = right;
        }
        else if (delimiterCheck(input[right]) == true && left != right || (right == len && left != right)) {
            char* subStr = subString(input, left, right - 1);

            if (keywordCheck(subStr) == true) {
                strncpy(token.value.keyword, subStr, sizeof(token.value.keyword) - 1);
                token.type = Keyword;
                appendToken(token, tokenList, tokenCount);
            }

            else if (identifierCheck(subStr) == true && delimiterCheck(input[right - 1]) == false)
                if(strlen(subStr) > 30) {
                    printf("ERROR! IDENTIFIER NAMES CANNOT BE MORE THAN 30 CHARACTERS.\n"); //Identifier 30 karakterden uzun olursa hata yazdırır.
                    break;
                }
                else {
                    strncpy(token.value.identifier, subStr, sizeof(token.value.identifier) - 1);
                    token.type = Identifier;
                    appendToken(token, tokenList, tokenCount);
                }

            else if (integerCheck(subStr) == true) {
                token.value.intValue = atoi(subStr);
                token.type = IntegerConstant;
                appendToken(token, tokenList, tokenCount);
            }
            left = right;
        }
    }
}

void interpret(Token* tokenList, int tokenCount) { //Interpreter işlemlerini yapar, sıradaki token'a göre ne işlem yapılacağına karar verir.
    Variable declaredVariables[1000];
    char strtemp1[200];
    char strtemp2[200];
    char myText[100];
    int temp;
    int count = 0;
    int i = 0;
    char* myString;
    const char* myText1;
    char* myText2;

    while (i < tokenCount) {
        TokenType type = tokenList[i].type;
        Token token = tokenList[i];

        switch (type) {
            case Identifier:
                break;
            case IntegerConstant:
                break;
            case StringConstant:
                break;
            case Keyword:
                if (!strcmp(token.value.keyword, "new")) {
                    i++;
                    token = tokenList[i];
                    if (!strcmp(token.value.keyword, "text")) { //"new text a" şeklinde kullanım gördüğünde atama işlemi yapar ve string olarak kabul eder.
                        i++;
                        token = tokenList[i];
                        if (token.type == Identifier) {
                            int isDeclared = 0;
                            for (int j = 0; j < count; j++) {
                                if (strcmp(token.value.identifier, declaredVariables[j].name) == 0) {
                                    isDeclared = 1;
                                    break;
                                }
                            }
                            if (isDeclared) {
                                printf("ERROR! THIS VARIABLE IS ALREADY DECLARED: %s\n", token.value.identifier); //Eğer değişken adı daha önce atandıysa hata verir.
                                break;
                            } else {
                                declaredVariables[count].valueType = String;
                                strcpy(declaredVariables[count].name, token.value.identifier);
                                strcpy(declaredVariables[count].value.strValue, token.value.stringValue);
                                count++;
                            }
                        }
                    }
                    if (!strcmp(token.value.keyword, "int")) { //"new int a" şeklinde kullanım gördüğünde atama işlemi yapar ve int olarak kabul eder.
                        i++;
                        token = tokenList[i];
                        if (token.type == Identifier) {
                            int isDeclared = 0;
                            for (int j = 0; j < count; j++) {
                                if (!strcmp(token.value.identifier, declaredVariables[j].name)) {
                                    isDeclared = 1;
                                    break;
                                }
                            }
                            if (isDeclared) {
                                printf("ERROR! THIS VARIABLE IS ALREADY DECLARED: %s\n", token.value.identifier); //Eğer değişken adı daha önce atandıysa hata verir.
                                break;
                            } else {
                                declaredVariables[count].valueType = Integer;
                                strcpy(declaredVariables[count].name, token.value.identifier);
                                declaredVariables[count].value.intValue = token.value.intValue;
                                count++;
                            }
                        }
                    }
                }
                if (!strcmp(token.value.keyword, "size")) { //Gelen token "size" ise TextJedi dilindeki size(myText) fonksiyonunu çağırır. Sıradaki tokena bakarak myText'i belirler.
                    i++;
                    if (tokenList[i].type == StringConstant) {
                        strcpy(strtemp1, tokenList[i].value.stringValue);
                        size_t length = strlen(strtemp1);
                        printf("String length: %zu\n", length);
                    }
                    if (tokenList[i].type == Identifier) {
                        for (int j = 0; j < count; j++) {
                            if (!strcmp(tokenList[i].value.identifier, declaredVariables[j].name)) {
                                strcpy(strtemp1, declaredVariables[j].value.strValue);
                            }
                        }
                        size_t length = strlen(strtemp1);
                        printf("String length: %zu\n", length);
                    }
                }

                if (!strcmp(token.value.keyword, "subs")) { //Gelen token "subs" ise TextJedi dilindeki subs(myText, begin, end) fonksiyonunu çağırır. Sıradaki tokenlara bakarak begin ve end'i belirler.
                    i++;
                    int subLength = tokenList[i+2].value.intValue - tokenList[i+1].value.intValue;
                    if (tokenList[i].type == StringConstant) {
                        char* substring = (char*)malloc((subLength + 1) * sizeof(char));
                        strncpy(substring, tokenList[i].value.stringValue + tokenList[i+1].value.intValue, subLength);
                        substring[subLength] = '\0';
                        printf("Substring: %s\n", substring);
                        i+=2;
                    }
                    if (tokenList[i].type == Identifier) {
                        for (int j = 0; j < count; j++) {
                            if (!strcmp(tokenList[i].value.identifier, declaredVariables[j].name)) {
                                strcpy(strtemp1, declaredVariables[j].value.strValue);
                            }
                        }
                        char* substring = (char*)malloc((subLength + 1) * sizeof(char));
                        strncpy(substring, strtemp1 + tokenList[i+1].value.intValue, subLength);
                        printf("Substring: %s\n", substring);
                        i+=2;
                    }
                }

                if (!strcmp(token.value.keyword, "locate")) { //Gelen token "locate" ise TextJedi dilindeki locate(bigText, smallText, start) fonksiyonunu çağırır. Sıradaki tokenlara bakarak bigText, smallText ve start'ı belirler.
                    i++;
                    int val = tokenList[i+2].value.intValue;
                    if (tokenList[i].type == StringConstant & tokenList[i+1].type == StringConstant) {
                        int position = locate(tokenList[i].value.stringValue, tokenList[i+1].value.stringValue, val);
                        printf("Text found at position: %d\n", position);
                    }
                    if (tokenList[i].type == StringConstant & tokenList[i+1].type == Identifier) {
                        for (int j = 0; j < count; j++) {
                            if (!strcmp(tokenList[i+1].value.identifier, declaredVariables[j].name)) {
                                strcpy(strtemp1, declaredVariables[j].value.strValue);
                            }
                        }
                        int position = locate(tokenList[i].value.stringValue, strtemp1, val);
                        printf("Text found at position: %d\n", position);
                    }
                    if (tokenList[i].type == Identifier & tokenList[i+1].type == StringConstant) {
                        for (int j = 0; j < count; j++) {
                            if (!strcmp(tokenList[i].value.identifier, declaredVariables[j].name)) {
                                strcpy(strtemp1, declaredVariables[j].value.strValue);
                            }
                        }
                        int position = locate(strtemp1, tokenList[i+1].value.stringValue, val);
                        printf("Text found at position: %d\n", position);

                    }
                    if (tokenList[i].type == Identifier & tokenList[i+1].type == Identifier) {
                        for (int j = 0; j < count; j++) {
                            if (!strcmp(tokenList[i].value.identifier, declaredVariables[j].name)) {
                                strcpy(strtemp1, declaredVariables[j].value.strValue);
                            }
                            if (!strcmp(tokenList[i+1].value.identifier, declaredVariables[j].name)) {
                                strcpy(strtemp2, declaredVariables[j].value.strValue);
                            }
                        }
                        int position = locate(strtemp1, strtemp2, val);
                        printf("Text found at position: %d\n", position);
                    }
                    i+=2;
                }

                if (!strcmp(token.value.keyword, "insert")) { //Gelen token "insert" ise TextJedi dilindeki insert(myText, location, insertText) fonksiyonunu çağırır. Sıradaki tokenlara bakarak myText, location ve insertText'i belirler.
                    i++;
                    int val = tokenList[i+1].value.intValue;
                    if (tokenList[i].type == StringConstant & tokenList[i+2].type == StringConstant) {
                        char* result = insert(tokenList[i].value.stringValue,val,tokenList[i+2].value.stringValue);
                        printf("Result of insertion: %s\n", result);
                    }
                    if (tokenList[i].type == StringConstant & tokenList[i+2].type == Identifier) {
                        for (int j = 0; j < count; j++) {
                            if (!strcmp(tokenList[i+2].value.identifier, declaredVariables[j].name)) {
                                strcpy(strtemp1, declaredVariables[j].value.strValue);
                            }
                        }
                        char* result = insert(tokenList[i].value.stringValue,val,strtemp1);
                        printf("Result of insertion: %s\n", result);
                    }
                    if (tokenList[i].type == Identifier & tokenList[i+2].type == StringConstant) {
                        for (int j = 0; j < count; j++) {
                            if (!strcmp(tokenList[i].value.identifier, declaredVariables[j].name)) {
                                strcpy(strtemp1, declaredVariables[j].value.strValue);
                            }
                        }
                        char* result = insert(strtemp1,  val,tokenList[i+2].value.stringValue);
                        printf("Result of insertion: %s\n", result);

                    }
                    if (tokenList[i].type == Identifier & tokenList[i+2].type == Identifier) {
                        for (int j = 0; j < count; j++) {
                            if (!strcmp(tokenList[i].value.identifier, declaredVariables[j].name)) {
                                strcpy(strtemp1, declaredVariables[j].value.strValue);
                            }
                            if (!strcmp(tokenList[i+2].value.identifier, declaredVariables[j].name)) {
                                strcpy(strtemp2, declaredVariables[j].value.strValue);
                            }
                        }
                        char* result = insert(strtemp1,  val,strtemp2);
                        printf("Result of insertion: %s\n", result);
                    }
                    i+=2;
                }

                if (!strcmp(token.value.keyword, "override")) { //Gelen token "override" ise TextJedi dilindeki override(myText,location, ovrText) fonksiyonunu çağırır. Sıradaki tokenlara bakarak myText, location ve ovrText'i belirler.
                    i++;
                    int val = tokenList[i + 1].value.intValue;
                    if (tokenList[i].type == StringConstant && tokenList[i + 2].type == StringConstant) {
                        override(tokenList[i].value.stringValue, val, tokenList[i + 2].value.stringValue);
                        printf("After override: %s\n", tokenList[i].value.stringValue);
                    }
                    if (tokenList[i].type == StringConstant && tokenList[i + 2].type == Identifier) {
                        for (int j = 0; j < count; j++) {
                            if (!strcmp(tokenList[i + 2].value.identifier, declaredVariables[j].name)) {
                                strcpy(strtemp1, declaredVariables[j].value.strValue);
                            }
                        }
                        override(tokenList[i].value.stringValue, val, strtemp1);
                        printf("After override: %s\n", tokenList[i].value.stringValue);
                    }

                    if (tokenList[i].type == Identifier && tokenList[i + 2].type == StringConstant) {
                        for (int j = 0; j < count; j++) {
                            if (!strcmp(tokenList[i].value.identifier, declaredVariables[j].name)) {
                                strcpy(strtemp1, declaredVariables[j].value.strValue);
                            }
                        }
                        override(strtemp1, val, tokenList[i + 2].value.stringValue);
                        printf("After override: %s\n", strtemp1);
                    }
                    if (tokenList[i].type == Identifier && tokenList[i + 2].type == Identifier) {
                        for (int j = 0; j < count; j++) {
                            if (!strcmp(tokenList[i].value.identifier, declaredVariables[j].name)) {
                                strcpy(strtemp1, declaredVariables[j].value.strValue);
                            }
                            if (!strcmp(tokenList[i + 2].value.identifier, declaredVariables[j].name)) {
                                strcpy(strtemp2, declaredVariables[j].value.strValue);
                            }
                        }
                        override(strtemp1, val, strtemp2);
                        printf("After override: %s\n", strtemp1);
                    }
                    i += 2;
                }

                if (!strcmp(token.value.keyword, "asString")) { //Gelen token "asString" ise TextJedi dilindeki asString(myInt) fonksiyonunu çağırır. Sıradaki tokena bakarak myInt değerini belirler.
                    i++;
                    if (tokenList[i].type == IntegerConstant) {
                        asString(tokenList[i].value.intValue);
                    }
                    if (tokenList[i].type == Identifier) {
                        for (int j = 0; j < count; j++) {
                            if (!strcmp(tokenList[j].value.identifier, declaredVariables[j].name)) {
                                temp = declaredVariables[j].value.intValue;
                            }
                        }
                    }
                    asString(temp);
                }

                if (!strcmp(token.value.keyword, "asText")) { //Gelen token "asText" ise TextJedi dilindeki asText(myString) fonksiyonunu çağırır. Sıradaki tokena bakarak myString değerini belirler.
                    i++;
                    if (tokenList[i].type == StringConstant) {
                        asText(tokenList[i].value.stringValue);
                    }
                    if (tokenList[i].type == Identifier) {
                        for (int j = 0; j < count; j++) {
                            if (!strcmp(tokenList[j].value.identifier, declaredVariables[j].name)) {
                                strcpy(strtemp1, declaredVariables[j].value.strValue);
                            }
                        }
                    }
                    asText(strtemp1);
                }

                if (!strcmp(token.value.keyword, "read")) { //Gelen token "read" ise TextJedi dilindeki "read myString from myTextFile" komutunu çağırır. Sıradaki tokenlara bakarak myString'i ve myTextFile'ı belirler.
                    i++;
                    myString = read_fromTXT(tokenList[i+2].value.stringValue);
                    if (myString != NULL) {
                        printf("Reading string from given text file: %s\n", myString);
                        free(myString);
                    }
                    i+=2;
                }

                if (!strcmp(token.value.keyword, "write")) { //Gelen token "write" ise TextJedi dilindeki "write myText to yourTextFile" komutunu çağırır. Sıradaki tokenlara bakarak myText'i ve yourTextFile'ı belirler.
                    i++;
                    if (tokenList[i].type == StringConstant) {
                        myText1 = tokenList[i].value.stringValue;
                        write_toTXT(tokenList[i+2].value.stringValue, myText1);
                    }
                    if (tokenList[i].type == Identifier) {
                        myText2 = NULL;
                        for (int j = 0; j < count; j++) {
                            if (!strcmp(tokenList[i].value.identifier, declaredVariables[j].name)) {
                                if (myText2 == NULL) {
                                    myText2 = malloc(strlen(declaredVariables[j].value.strValue) + 1);
                                    strcpy(myText2, declaredVariables[j].value.strValue);
                                } else {
                                    size_t totalSize = strlen(myText2) + strlen(declaredVariables[j].value.strValue) + 1;
                                    myText2 = realloc(myText2, totalSize);
                                    strcat(myText2, declaredVariables[j].value.strValue);
                                }
                            }
                        }
                        write_toTXT(tokenList[i + 2].value.stringValue, myText2);
                    }
                    i += 2;
                }

                if (!strcmp(token.value.keyword, "input")) { //Gelen token "input" ise TextJedi dilindeki "input myText prompt promptText" komutunu çağırır. Sıradaki tokenlara bakarak myText'i ve promptText'i belirler.
                    i++;
                    if(tokenList[i].type == StringConstant) {
                        inputText(myText, tokenList[i + 2].value.stringValue);
                    }
                    if(tokenList[i].type == Identifier) {
                        for (int j = 0; j < count; j++) {
                            if (!strcmp(tokenList[i].value.identifier, declaredVariables[j].name)) {
                                inputText(declaredVariables[j].value.strValue, tokenList[i + 2].value.stringValue);
                                break;
                            }
                        }
                    }
                    i += 2;
                }

                if (!strcmp(token.value.keyword, "output")) { //Gelen token "output" ise TextJedi dilindeki "output myText" komutunu çağırır. Sıradaki tokena bakarak myText'i belirler.
                    i++;
                    for (int j = 0; j < count; j++) {
                        if (!strcmp(tokenList[i].value.identifier, declaredVariables[j].name)) {
                            printf("%s", declaredVariables[j].value.strValue);
                            break;
                        }
                        else {
                            printf("%s", myText);
                        }
                    }
                }

                break;
            case Operator:
                if (!strcmp(token.value.operator, ":=")) { //Assign işlemini gerçekleştirir.
                    if (tokenList[i - 1].type == Identifier) {
                        int isDeclared = 0;
                        for (int j = 0; j < count; j++) {
                            if (!strcmp(tokenList[i - 1].value.identifier, declaredVariables[j].name)) {
                                isDeclared = 1;
                                break;
                            }
                        }
                        if (!isDeclared) {
                            printf("ERROR! THIS VARIABLE IS NOT DECLARED: %s\n", tokenList[i - 1].value.identifier); //Eğer değişken daha önce tanımlanmadıysa hata mesajı yazdırır.
                        } else {
                            i++;
                            token = tokenList[i];
                            for (int j = 0; j < count; j++) {
                                if (!strcmp(tokenList[i - 2].value.identifier, declaredVariables[j].name)) {
                                    if (token.type == IntegerConstant) {
                                        declaredVariables[j].value.intValue = token.value.intValue;
                                    } else if (token.type == StringConstant) {
                                        strcpy(declaredVariables[j].value.strValue, token.value.stringValue);
                                    } else if (token.type == Identifier) {
                                        for (int k = 0; k < count; k++) {
                                            if (!strcmp(token.value.identifier, declaredVariables[k].name)) {
                                                if(declaredVariables[k].valueType == String) {
                                                    strcpy(declaredVariables[j].value.strValue,declaredVariables[k].value.strValue);
                                                }
                                                if(declaredVariables[k].valueType == Integer) {
                                                    declaredVariables[j].value.intValue = declaredVariables[k].value.intValue;
                                                }
                                            }
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }

                if (!strcmp(tokenList[i + 1].value.operator, "+")) { //Sayı ve string toplama işlemlerini gerçekleştirir.
                    if (tokenList[i].type == IntegerConstant) {
                        if (tokenList[i + 2].type == IntegerConstant) {
                            temp = tokenList[i].value.intValue + tokenList[i + 2].value.intValue;
                            for (int j = 0; j < count; j++) {
                                if (!strcmp(tokenList[i - 2].value.stringValue, declaredVariables[j].name)) {
                                    declaredVariables[j].value.intValue = temp;
                                    printf("%d",temp);
                                }
                            }
                        }
                        if (tokenList[i + 2].type == Identifier) {
                            for (int j = 0; j < count; j++) {
                                if (!strcmp(tokenList[i + 2].value.stringValue, declaredVariables[j].name)) {
                                    temp = tokenList[i].value.intValue + declaredVariables[j].value.intValue;
                                }
                            }
                            for (int k = 0; k < count; k++) {
                                if (!strcmp(tokenList[i - 2].value.stringValue, declaredVariables[k].name)) {
                                    declaredVariables[k].value.intValue = temp;
                                }
                            }
                        }
                    }
                    if (tokenList[i].type == Identifier) {
                        if (tokenList[i + 2].type == IntegerConstant) {
                            for (int j = 0; j < count; j++) {
                                if (!strcmp(tokenList[i].value.stringValue, declaredVariables[j].name)) {
                                    temp = tokenList[i + 2].value.intValue + declaredVariables[j].value.intValue;
                                }
                                for (int k = 0; k < count; k++) {
                                    if (!strcmp(tokenList[i - 2].value.stringValue, declaredVariables[k].name)) {
                                        declaredVariables[k].value.intValue = temp;
                                    }
                                }
                            }
                        }
                        if (tokenList[i + 2].type == Identifier) {
                            int temp1;
                            int temp2;
                            for (int j = 0; j < count; j++) {
                                if (!strcmp(tokenList[i].value.stringValue, declaredVariables[j].name)) {
                                    temp1 = declaredVariables[j].value.intValue;
                                }
                                if (!strcmp(tokenList[i + 2].value.stringValue, declaredVariables[j].name)) {
                                    temp2 = declaredVariables[j].value.intValue;
                                }
                            }
                            temp = temp1 + temp2;
                            for (int k = 0; k < count; k++) {
                                if (!strcmp(tokenList[i - 2].value.stringValue, declaredVariables[k].name)) {
                                    declaredVariables[k].value.intValue = temp;
                                }
                            }
                        }
                    }
                    if (tokenList[i].type == StringConstant) {
                        if (tokenList[i + 2].type == StringConstant) {
                            strcpy(strtemp1, tokenList[i].value.stringValue);
                            strcat(strtemp1, tokenList[i + 2].value.stringValue);
                            for (int j = 0; j < count; j++) {
                                if (!strcmp(tokenList[i - 2].value.stringValue, declaredVariables[j].name)) {
                                    strcpy(declaredVariables[j].value.strValue, strtemp1);
                                }
                            }
                        }

                        if (tokenList[i + 2].type == Identifier) {
                            strcpy(strtemp1, tokenList[i].value.stringValue);
                            for (int j = 0; j < count; j++) {
                                if (!strcmp(tokenList[i + 2].value.stringValue, declaredVariables[j].name)) {
                                    strcat(strtemp1, declaredVariables[j].value.strValue);
                                }
                            }
                            for (int k = 0; k < count; k++) {
                                if (!strcmp(tokenList[i - 2].value.stringValue, declaredVariables[k].name)) {
                                    strcpy(declaredVariables[k].value.strValue, strtemp1);
                                }
                            }
                        }
                    }
                    if (tokenList[i].type == Identifier) {
                        if (tokenList[i + 2].type == StringConstant) {
                            for (int j = 0; j < count; j++) {
                                if (!strcmp(tokenList[i].value.stringValue, declaredVariables[j].name)) {
                                    strcpy(strtemp1, declaredVariables[j].value.strValue);
                                }
                            }
                            strcat(strtemp1, tokenList[i+2].value.stringValue);
                            for (int k = 0; k < count; k++) {
                                if (!strcmp(tokenList[i - 2].value.stringValue, declaredVariables[k].name)) {
                                    strcpy(declaredVariables[k].value.strValue, strtemp1);
                                }
                            }
                        }
                        if (tokenList[i + 2].type == Identifier) {
                            for (int j = 0; j < count; j++) {
                                if (!strcmp(tokenList[i].value.stringValue, declaredVariables[j].name)) {
                                    if (declaredVariables[j].valueType == String) {
                                        strcpy(strtemp1, declaredVariables[j].value.strValue);
                                    }
                                }
                            }
                            for (int k = 0; k < count; k++) {
                                if (!strcmp(tokenList[i + 2].value.stringValue, declaredVariables[k].name)) {
                                    if (declaredVariables[k].valueType == String) {
                                        strcat(strtemp1, declaredVariables[k].value.strValue);
                                    }
                                }
                            }
                            for (int k = 0; k < count; k++) {
                                if (!strcmp(tokenList[i - 2].value.stringValue, declaredVariables[k].name)) {
                                    if (declaredVariables[k].valueType == String){
                                        strcpy(declaredVariables[k].value.strValue, strtemp1);
                                    }
                                }
                            }
                        }
                    }
                }
                if (!strcmp(tokenList[i + 1].value.operator, "-")) { //Sayı ve string çıkarma işlemlerini yaparlar.
                    if (tokenList[i].type == IntegerConstant) {
                        if (tokenList[i + 2].type == IntegerConstant) {
                            temp = tokenList[i].value.intValue - tokenList[i + 2].value.intValue;
                            if(temp >= 0) {
                                for (int j = 0; j < count; j++) {
                                    if (!strcmp(tokenList[i - 2].value.stringValue, declaredVariables[j].name)) {
                                        declaredVariables[j].value.intValue = temp;
                                    }
                                }
                            }
                            else{
                                for (int j = 0; j < count; j++) {
                                    if (!strcmp(tokenList[i - 2].value.stringValue, declaredVariables[j].name)) {
                                        declaredVariables[j].value.intValue = 0;
                                    }
                                }
                                printf("ERROR! CANNOT SUBTRACT A LARGER VALUE FROM SMALLER VALUE.\n"); //Küçük sayıdan büyük sayı çıkarmaya çalışılırsa hata verir ve ekrana sonucu 0 olarak döndürür.
                                break;
                            }
                        }
                        if (tokenList[i + 2].type == Identifier) {
                            for (int j = 0; j < count; j++) {
                                if (!strcmp(tokenList[i + 2].value.stringValue, declaredVariables[j].name)) {
                                    temp = tokenList[i].value.intValue - declaredVariables[j].value.intValue;
                                }
                            }
                            if (temp >= 0) {
                                for (int k = 0; k < count; k++) {
                                    if (!strcmp(tokenList[i - 2].value.stringValue, declaredVariables[k].name)) {
                                        declaredVariables[k].value.intValue = temp;
                                    }
                                }
                            }
                            else {
                                for (int j = 0; j < count; j++) {
                                    if (!strcmp(tokenList[i - 2].value.stringValue, declaredVariables[j].name)) {
                                        declaredVariables[j].value.intValue = 0;
                                    }
                                }
                                printf("ERROR! CANNOT SUBTRACT A LARGER VALUE FROM SMALLER VALUE.\n"); //Küçük sayıdan büyük sayı çıkarmaya çalışılırsa hata verir ve ekrana sonucu 0 olarak döndürür.
                                break;
                            }
                        }
                    }
                    if (tokenList[i].type == Identifier) {
                        if (tokenList[i + 2].type == IntegerConstant) {
                            for (int j = 0; j < count; j++) {
                                if (!strcmp(tokenList[i].value.stringValue, declaredVariables[j].name)) {
                                    temp = declaredVariables[j].value.intValue - tokenList[i + 2].value.intValue;
                                }
                                if (temp >= 0) {
                                    for (int k = 0; k < count; k++) {
                                        if (!strcmp(tokenList[i - 2].value.stringValue, declaredVariables[k].name)) {
                                            declaredVariables[k].value.intValue = temp;
                                        }
                                    }
                                }
                                else {
                                    for (int l = 0; l < count; l++) {
                                        if (!strcmp(tokenList[i - 2].value.stringValue, declaredVariables[l].name)) {
                                            declaredVariables[l].value.intValue = 0;
                                        }
                                    }
                                    printf("ERROR! CANNOT SUBTRACT A LARGER VALUE FROM SMALLER VALUE.\n"); //Küçük sayıdan büyük sayı çıkarmaya çalışılırsa hata verir ve ekrana sonucu 0 olarak döndürür.
                                    break;
                                }
                            }
                        }
                        if (tokenList[i + 2].type == Identifier) {
                            int temp1;
                            int temp2;
                            for (int j = 0; j < count; j++) {
                                if (!strcmp(tokenList[i].value.stringValue, declaredVariables[j].name)) {
                                    temp1 = declaredVariables[j].value.intValue;
                                }
                                if (!strcmp(tokenList[i + 2].value.stringValue, declaredVariables[j].name)) {
                                    temp2 = declaredVariables[j].value.intValue;
                                }
                            }
                            temp = temp1 - temp2;
                            if (temp >= 0) {
                                for (int k = 0; k < count; k++) {
                                    if (!strcmp(tokenList[i - 2].value.stringValue, declaredVariables[k].name)) {
                                        declaredVariables[k].value.intValue = temp;
                                    }
                                }
                            }
                            else {
                                printf("ERROR! CANNOT SUBTRACT A LARGER VALUE FROM SMALLER VALUE.\n"); //Küçük sayıdan büyük sayı çıkarmaya çalışılırsa hata verir ve ekrana sonucu 0 olarak döndürür.
                                for (int j = 0; j < count; j++) {
                                    if (!strcmp(tokenList[i - 2].value.stringValue, declaredVariables[j].name)) {
                                        declaredVariables[j].value.intValue = 0;
                                    }
                                }
                            }
                        }
                    }

                    if (tokenList[i].type == StringConstant) {
                        if (tokenList[i + 2].type == StringConstant) {
                            removeSubstring(tokenList[i].value.stringValue, tokenList[i + 2].value.stringValue);
                            for (int j = 0; j < count; j++) {
                                if (!strcmp(tokenList[i - 2].value.stringValue, declaredVariables[j].name)) {
                                    strcpy(declaredVariables[j].value.strValue, tokenList[i].value.stringValue);
                                }
                            }
                        }
                        if (tokenList[i + 2].type == Identifier) {
                            for (int j = 0; j < count; j++) {
                                if (!strcmp(tokenList[i+2].value.stringValue, declaredVariables[j].name)) {
                                    strcpy(strtemp1, declaredVariables[j].value.strValue);
                                }
                            }
                            removeSubstring(tokenList[i].value.stringValue, strtemp1);
                            for (int j = 0; j < count; j++) {
                                if (!strcmp(tokenList[i - 2].value.stringValue, declaredVariables[j].name)) {
                                    strcpy(declaredVariables[j].value.strValue, tokenList[i].value.stringValue);
                                }
                            }
                        }
                    }

                    if (tokenList[i].type == Identifier) {
                        if (tokenList[i + 2].type == StringConstant) {
                            for (int j = 0; j < count; j++) {
                                if (!strcmp(tokenList[i].value.stringValue, declaredVariables[j].name)) {
                                    strcpy(strtemp1, declaredVariables[j].value.strValue);
                                }
                            }
                            removeSubstring(strtemp1, tokenList[i+2].value.stringValue);
                            for (int k = 0; k < count; k++) {
                                if (!strcmp(tokenList[i - 2].value.stringValue, declaredVariables[k].name)) {
                                    strcpy(declaredVariables[k].value.strValue, strtemp1);
                                }
                            }
                        }
                        if (tokenList[i + 2].type == Identifier) {
                            for (int j = 0; j < count; j++) {
                                if (!strcmp(tokenList[i].value.stringValue, declaredVariables[j].name)) {
                                    if (declaredVariables[j].valueType == String) {
                                        strcpy(strtemp1, declaredVariables[j].value.strValue);
                                    }
                                }
                            }
                            for (int k = 0; k < count; k++) {
                                if (!strcmp(tokenList[i + 2].value.stringValue, declaredVariables[k].name)) {
                                    if (declaredVariables[k].valueType == String) {
                                        removeSubstring(strtemp1, declaredVariables[k].value.strValue);
                                    }
                                }
                            }
                            for (int k = 0; k < count; k++) {
                                if (!strcmp(tokenList[i - 2].value.stringValue, declaredVariables[k].name)) {
                                    if (declaredVariables[k].valueType == String){
                                        strcpy(declaredVariables[k].value.strValue, strtemp1);
                                    }
                                }
                            }
                        }
                    }
                }
                break;
            case EndOfLine:
                break;
        }
        i++;
    }
}

int main(int argc, char *argv[]) {
    char character, *chars;
    int i = 0;
    Token tokenList[1000];
    int tokenCount = 0;

    if (argc != 2) {
        printf("Usage -> TextJedi 'filename'\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file! : %s\n", argv[1]);
        return 1;
    }

    chars = malloc(5000);
    while ((character = fgetc(file)) != EOF)
    {
        if(character == '\n' ) {
            character = ' ';
        }
        chars[i++] = character;
    }

    chars[i] = '\0';

    fclose(file);
    parse(chars, tokenList, &tokenCount);
    interpret(tokenList, tokenCount);

    return 0;
}
