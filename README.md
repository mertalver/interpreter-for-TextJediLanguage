# Interpreter for the Text Jedi Language
Interpreter was written in C.

## Objectives
Text Jedi is a programming language that was designed for editing text files mainly by cutting and pasting operations. (It is imaginary)
Interpreter executable should be named as TextJedi.exe and should be able to run a TextJedi program when called with an argument that contains the filename of the code to be run. (from the operating system command prompt) <br>
**Example**: TextJedi myProg.tj

## Data Types 
The only two data types in TextJedi are Strings (called as text) and Integers. <br>
String constants are written “like this” and integer constants are natural (zero or positive whole) numbers. <br>
All variables should be declared before they are used. <br> 
Declarations are done with the “new” keyword. <br>
**Ex**: new text myText;  new int myInt; /* This is a comment*/
                                    /* And two lines of code on one line is OK */
     
## Assignment, Operations and Functions
“:=”  is the assignment operator. On the left hand side there has to be a variable name and on the right hand side there may be either a value, an arithmetic expression or a function call. <br>
A value is either a constant or a variable. <br>
An arithmetic expression is composed of two operands and an operator written in infix form. Valid operators are “+” and “-”. <br>
**Ex**: 
 MyText := “Hello world”; <br>
 myInt:=0; <br>
 myInt := 2 + yourInt; <br>
 MyText:= FirstText+SecondText; <br>
• Subtracting a larger value from a smaller value generates an error message. Other than that Addition and subtraction on integers works just as expected. <br>
• Addition over text values results in concatenation of the two texts. <br>
**Ex**: s := “Hello”+ “ world”; /*Assigns “Hello world” to s*/ <br>
• Subtraction over text values results in the removal of the first occurrence of the Subtrahend from the Minuend (finds the substring and cuts and removes it) <br>
**Ex**: myString := “Hello world”-“wo”;  /* Assigns “Hello rld” to myString*/ <br>
• If the substring cannot be found, it is not an error condition. However, if the second string is bigger than the first one, an error message is generated. <br>

•	A function call is a call made to one of the predefined (built in) functions of the language. The predefined functions are as follows: <br>
**size(myText)** : returns the size of myText as an int <br>
**subs(myText, begin, end)** : returns substring of myText between characters marked by the two integers begin and end <br>
**locate(bigText, smallText, start)**: Finds (the first occurrence of)  smallText in bigtext and returns its position as an integer. It returns 0 if it fails to find it. Starts its search from the location marked by the integer “start”. <br>
**insert(myText, location, insertText)** : inserts insertText into the position marked by the integer “location”  of myText and returns the resulting text. <br>
**override(myText,location, ovrText)**: writes ovrText onto myText by overriding whatever that was previously there, starting from the location (given as an integer) If the writing operation exceeds the size of myText, it terminates at the end of myText without creating any error condition. <br>
**asString(myInt)**: returns myInt as a string (type conversion) <br>
**asText(myString)**: returns myString as an int <br>
    
## Commands:
**read myString from myTextFile**: Reads a string from a text file called myTextFile.txt Please note that there is no size limit. <br>
**write myText to yourTextFile**: Write the string called myText onto a text file named as yourTextFile. <br>
**input myText prompt promptText**: Receive input from keyboard into myText. Use promptText as a prompt. Max 100 characters buffer is enough. <br>
**output myText**: Write myText on the screen. <br>

## Lexical rules for the programming language TextJedi  are as follows:
### 1- Identifiers: 
•	Maximum identifier size is 30 characters. If you use an identifier larger than that, the lexical analyzer issues an error message. <br>
•	TextJedi language is case sensitive, and all keywords are lower case.<br>
•	Identifiers start with an alphabetic character (any letter) and are composed of one or more letters/digits/_ (underscore)
### 2- Integer constants
•	Maximum integer size is the same as a C unsigned int.<br>
•	Negative values are not supported. All integers are zero or positive.
### 3- Operators
•	Valid operators of the language are +,-,:= <br>
### 4- String constants
•	String constants of TextJedi are delimited by double quotes (ASCII code 34)as in “this is a string” <br>
•	String constants have unlimited size <br>
•	String constants cannot contain the double quote character. when you reach one, the string terminates. <br>
•	If a string constant cannot terminate before the file end, there should be a lexical error issued.
### 6- Keywords:
•	Keywords are: new, int, text, size, subs, locate, insert, override, read, write, from, to, input, output, asText, asString
### 7- End of line mark:
• ";" means end of line mark.
### 8- Comments: Anything between /* and */ is a comment.
•	If a comment fails to terminate before the file end, there should be a lexical error issued.
