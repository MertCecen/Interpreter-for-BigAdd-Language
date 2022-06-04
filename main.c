#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

char *variableList[1000]; int vIndex = 0;
int integerList[1000] = { 0 }; int iIndex = 0;

bool isKeyword(char* str) {

	if (!strcmp(str, "int") ||
		!strcmp(str, "move") || !strcmp(str, "to") || !strcmp(str, "add") ||
		!strcmp(str, "sub") || !strcmp(str, "from") ||
		!strcmp(str, "loop") || !strcmp(str, "times") ||
		!strcmp(str, "out")) {
		return true;
	}

	return false;
}

bool isInteger(char* str) {

	int len = strlen(str);

	if (len > 100) {
		return false;
	}

	for (int i = 0; i < len; i++) {
		if (i > 0) {
			if (str[i] == '-' || len == 1) {
				return false;
			}
		}
		if (!(isdigit(str[i]) || str[i] == '-')) {
			return false;
		}
	}

	return true;
}

bool isIntegerVariable(char* str) {

	for (int i = 0; i < vIndex; i++) {
		if (!strcmp(str, variableList[i])) {
			return true;
		}
	}

	return false;
}

bool isStringConstant(char* str){
    int strLen = strlen(str);
    if (str[0] == '\"' && '\"' == str[strLen-1]){
        return true;
    }
    return false;
}

bool isVariable(char* str)
{
	int len = strlen(str);

	if (len > 20) {
		return false;
	}

	for (int i = 0; i < len; i++) {
		if (str[0] == '_') {
			return false;
		}
		if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] == '_'))) {
			return false;
		}
	}

	return true;
}



bool inLoop = true;
char *loopLine2[1000];
bool loopInteger = false;
int firstTimeLoop = -1;
int entered = 0;
void getLoopCode(char* loop_Code, char* code, int t){

    // this function gets the all words inside of the loop
    //code

    char *loopWords[1000];
    int temp = 0;
    char loop_code[100] = "";

    if (inLoop){
        //inLoop = false;
        char copy[1000];
        char loopCode[1000];
        strcpy(copy,code);
        int loopLineCount = 0;
        char *loopLine[1000];
        char *splitLoopCode = strtok(copy, "[]");

        while (splitLoopCode != NULL) { // split the code at every EOL char "."
            //printf("%s ////////////\n",splitLoopCode);
            loopLine[loopLineCount++] = splitLoopCode;
            splitLoopCode = strtok(NULL, "[]");
        }
        //printf("llc:%d\n",loopLineCount); // -1 al
        strcpy(loopCode,loopLine[t]);
        // loop
        int loopLineCountv2 = 0;


        int loopLineCount2 = 0;


        char *splitLoopCode2 = strtok(loopCode, ".");
        while (splitLoopCode2 != NULL) { // split the code at every EOL char "."
            //printf("splitted loop code : %s\n",splitLoopCode2);
            loopLine2[loopLineCount2++] = splitLoopCode2;
            splitLoopCode2 = strtok(NULL, ".");
        }
        loopLineCountv2 = loopLineCount2;
        int t = 0;
        //printf("ll2size: %d\n",sizeof(*loopLine2));
        while (loopLine2[t] != NULL){
            //strcat(loop_code, " ");
            strcat(loop_code, loopLine2[t]);
            strcat(loop_code, ".");
            //printf("loopLine2 %d:%s , size:%d\n",t,loopLine2[t],sizeof(loopLine2[t]));
            t++;
        }

    }
    strcpy(loop_Code,loop_code);
    //return loop_code;

}

void parse(char* code) {

    char copyCode[1000];
    strcpy(copyCode,code);
    ///////////////////////////////////////////////////////////
	int lineCount = 0;
	char *line[1000];
	char *splitCode = strtok(copyCode, ".");
	while (splitCode != NULL) { // split the code at every EOL char "."
        //printf("Line %d:%s \n",lineCount, splitCode);
		line[lineCount++] = splitCode;
		splitCode = strtok(NULL, ".");
	}
    //printf("linecount:%d\n",lineCount);


   //printf("-----------------------------------------\n");
   //int size.     line = 0
   //int sum.       line = 1
   //move 5 to size. line = 2
   //loop size times line = 3
   //[ out size, newline.  line = 4 // loop un olduğu line numaralarını al if i == lineNum : pass.
   // add size to sum.  line = 5
   //]
   //out newline, "Sum:", sum.
    int index = 0;
    int yy = 0;
    char *allWords[1000];
	for (int i = 0; i < lineCount; i++) {
		int wordCount = 0;
		char *word[1000];
		char *splitLine = strtok(line[i], " \n\r[]\""); // white space, new line, carriage return, double quote for string constants
		while (splitLine != NULL) {
            //printf("::::::%s \n",splitLine);
			word[wordCount++] = splitLine; // words in a line
			allWords[index++] = splitLine;
			splitLine = strtok(NULL, " ,\n\r[]");
		}

		if (wordCount == 0) { return; }

		//Int
		if (!strcmp(word[0], "int")) {
			printf("'int' is a keyword.\n");

			//Variable
			if (word[1] == NULL) {
				printf("Variable is expected (line %i).\n",i+1);
				return 0;
			}
			else if (isKeyword(word[1])) {
				printf("'%s' is a keyword. Variable is expected (line %i).\n", word[1],i+1);
				return 0;
			}
			else if (isVariable(word[1])) {
				printf("'%s' is a variable.\n", word[1]);

				variableList[vIndex++] = word[1];
				integerList[iIndex++] = 0;
			}
			else {
				printf("'%s' is not a variable. Variable is expected. (line %i) \n", word[1],i+1);
				return 0;
			}

			//End Of Line
			if (wordCount == 2) {
				printf("'.' End of Line.\n");
			}
			else {
				printf("End of line is expected. (line %i)\n",i+1);
				return 0;
			}
		}


		//Move
		else if (!strcmp(word[0], "move")) {
			printf("'move' is a keyword.\n");

			//Integer Or Variable
			int moveInt = 0;
			if (word[1] == NULL) {
				printf("Integer or variable are expected. (line %i)\n",i+1);
				return 0;
			}
			else if (isKeyword(word[1])) {
				printf("'%s' is a keyword. Integer or variable are expected. (line %i)\n", word[1],i+1);
				return 0;
			}
			else if (isInteger(word[1])) {
				printf("'%s' is an integer.\n", word[1]);

				moveInt = atoi(word[1]);
			}
			else if (isIntegerVariable(word[1])) {
				printf("'%s' is a variable.\n", word[1]);

				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[1])) {
						moveInt = integerList[i];
						break;
					}
				}
			}
			else {
				printf("'%s' is not a integer or variable. Integer or variable are expected. (line %i)\n", word[1],i+1);
				return 0;
			}

			//To
			if (word[2] == NULL) {
				printf("Keyword 'to' is expected. (line %i)\n",i+1);
				return 0;
			}
			else if (!strcmp(word[2], "to")) {
				printf("'to' is a keyword.\n");
			}
			else {
				printf("Keyword 'to' is expected. (line %i)\n",i+1);
				return 0;
			}

			//Variable
			if (word[3] == NULL) {
				printf("Variable is expected. (line %i)\n",i+1);
				return 0;
			}
			else if (isKeyword(word[3])) {
				printf("'%s' is a keyword. Variable is expected. (line %i)\n", word[3],i+1);
				return 0;
			}
			else if (isIntegerVariable(word[3])) {
				printf("'%s' is a variable.\n", word[3]);

				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[3])) {
						integerList[i] = moveInt;
						break;
					}
				}
			}
			else {
				printf("'%s' is not a variable. Variable is expected. (line %i)\n", word[3],i+1);
				return 0;
			}

			//End Of Line
			if (wordCount == 4) {
				printf("'.' End of Line.\n");
			}
			else {
				printf("End of line is expected. (line %i)\n",i+1);
				return 0;
			}
		}


		//Add
		else if (!strcmp(word[0], "add")) {
			printf("'add' is a keyword.\n");

			//Integer Or Variable
			int addInt = 0;
			if (word[1] == NULL) {
				printf("Integer or variable are expected. (line %i)\n",i+1);
				return 0;
			}
			else if (isKeyword(word[1])) {
				printf("'%s' is a keyword. Integer or variable are expected. (line %i)\n", word[1],i+1);
				return 0;
			}
			else if (isInteger(word[1])) {
				printf("'%s' is an integer.\n", word[1]);

				addInt = atoi(word[1]);
			}
			else if (isIntegerVariable(word[1])) { // add size to sum. # size is integerVariable
				printf("'%s' is a variable.\n", word[1]);

				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[1])) {
						addInt = integerList[i];
						if (loopInteger && firstTimeLoop > 0){
                            integerList[i] -= 1;
                            addInt = integerList[i];
                        }
						break;
					}
				}
			}
			else {
				printf("'%s' is not a integer or variable. Integer or variable are expected. (line %i)\n", word[1],i+1);
				return 0;
			}

			//To
			if (word[2] == NULL) {
				printf("Keyword 'to' is expected. (line %i)\n",i+1);
				return 0;
			}
			else if (!strcmp(word[2], "to")) {
				printf("'to' is a keyword.\n");
			}
			else {
				printf("Keyword 'to' is expected. (line %i)\n",i+1);
				return 0;
			}

			//Variable
			if (word[3] == NULL) {
				printf("Variable is expected. (line %i)\n",i+1);
				return 0;
			}
			else if (isKeyword(word[3])) {
				printf("'%s' is a keyword. Variable is expected. (line %i)\n", word[3],i+1);
				return 0;
			}
			else if (isIntegerVariable(word[3])) {
				printf("'%s' is a variable.\n", word[3]);

				for (int j = 0; j < vIndex; j++) {
					if (!strcmp(variableList[j], word[3])) {
                        //printf("variable list %d:%s --- integer list %d:%d\n",j,variableList[j],j,integerList[j]);
						integerList[j] += addInt;
                        //printf("variable list %d:%s --- integer list %d:%d\n",j,variableList[j],j,integerList[j]);
						break;
					}
				}
			}
			else {
				printf("'%s' is not a variable. Variable is expected. (line %i)\n", word[3],i+1);
				return 0;
			}

			//End Of Line
			if (wordCount == 4) {
				printf("'.' End of Line.\n");
			}
			else {
				printf("End of line is expected. (line %i)\n",i+1);
				return 0;
			}
		}


		//Sub
		else if (!strcmp(word[0], "sub")) {
			printf("'sub' is a keyword.\n");

			//Integer Or Variable
			int subInt = 0;
			if (word[1] == NULL) {
				printf("Integer or variable are expected. (line %i)\n",i+1);
				return 0;
			}
			else if (isKeyword(word[1])) {
				printf("'%s' is a keyword. Integer or variable are expected. (line %i)\n", word[1],i+1);
				return 0;
			}
			else if (isInteger(word[1])) {
				printf("'%s' is an integer.\n", word[1]);

				subInt = atoi(word[1]);
			}
			else if (isIntegerVariable(word[1])) {
				printf("'%s' is a variable.\n", word[1]);

				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[1])) {
						subInt = integerList[i];
						if (loopInteger && firstTimeLoop > 0){
                            integerList[i] -= 1;
                            subInt = integerList[i];
                        }
						break;
					}
				}
			}
			else {
				printf("'%s' is not a integer or variable. Integer or variable are expected. (line %i)\n", word[1],i+1);
				return 0;
			}

			//To
			if (word[2] == NULL) {
				printf("Keyword 'from' is expected. (line %i)\n",i+1);
				return 0;
			}
			else if (!strcmp(word[2], "from")) {
				printf("'from' is a keyword.\n");
			}
			else {
				printf("Keyword 'from' is expected. (line %i)\n",i+1);
				return 0;
			}

			//Variable
			if (word[3] == NULL) {
				printf("Variable is expected. (line %i)\n",i+1);
				return 0;
			}
			else if (isKeyword(word[3])) {
				printf("'%s' is a keyword. Variable is expected. (line %i)\n", word[3],i+1);
				return 0;
			}
			else if (isIntegerVariable(word[3])) {
				printf("'%s' is a variable.\n", word[3]);

				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[3])) {
						integerList[i] -= subInt;
						break;
					}
				}
			}
			else {
				printf("'%s' is not a variable. Variable is expected. (line %i)\n", word[3],i+1);
				return 0;
			}

			//End Of Line
			if (wordCount == 4) {
				printf("'.' End of Line.\n");
			}
			else {
				printf("End of line is expected. (line %i)\n",i+1);
				return 0;
			}
		}

		//Out
		else if (!strcmp(word[0], "out")) {
			printf("'out' is a keyword.\n");
            int x = 1;
            bool girdi = false;
            while (word[x] != NULL){
                //printf("---->%s",word[x]);
                girdi = true;
			//Integer, Variable Or String
			if (word[x] == NULL) {
				printf("Integer, variable or string are expected. (line %i)\n",i+1);
				return 0;
			}
			else if (isInteger(word[x])) {
				printf("'%s' is an integer.\n", word[x]);

				printf("Integer Value: %s\n", word[x]);
			}
			else if (isStringConstant(word[x])){
                printf("String constant (%s)\n",word[x]);
			}
			else if (isIntegerVariable(word[x])) {
				printf("'%s' is a variable.\n", word[x]);

				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[x])) {
						printf("'%s' variable value426: %i\n", word[x], integerList[i]);
						break;
					}
				}
			}
			else {
				printf("%s\n", word[x]); // newline

			}

			//End Of Line
			/*if (wordCount == 2) {426
				printf("'.' End of Line.\n");
			}
			else {
				printf("End of line is expected.\n");
				return 0;
			}*/
			x++;
            }
            if (girdi == false){
                printf("Integer or variable is expected. (line %i)\n",i+1);
            }
		}

		//Loop

		else if (!strcmp(word[0], "loop")) {
            char loop_code[100];
            firstTimeLoop = -1;
            getLoopCode(loop_code, code, entered*2 + 1);
            entered++;
            printf("'loop' is a keyword.\n");



			//Integer Or Variable
			int loopInt = 0;
			if (word[1] == NULL) {
				printf("Integer or variable are expected. (line %i)\n",i+1);
				return 0;
			}
			else if (isKeyword(word[1])) {
				printf("'%s' is a keyword. Integer or variable are expected. (line %i)\n", word[1],i+1);
				return 0;
			}
			else if (isInteger(word[1])) {
				printf("'%s' is an integer.\n", word[1]);

				loopInt = atoi(word[1]);
			}
			else if (isIntegerVariable(word[1])) {
				printf("'%s' is a variable.\n", word[1]);

				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[1])) {
						loopInt = integerList[i];
						break;
					}
				}
			}
			else {
				printf("'%s' is not a integer or variable. Integer or variable are expected. (line %i)\n", word[1],i+1);
				return 0;
			}

			//Times
			if (word[2] == NULL) {
				printf("Keyword 'times' is expected. (line %i)\n",i+1);
				return 0;
			}
			else if (!strcmp(word[2], "times")) {
				printf("'times' is a keyword.\n");
			}
			else {
				printf("Keyword 'times' is expected. (line %i)\n",i+1);
				return 0;
			}

            for (int i = 0; i < loopInt; i++){
                firstTimeLoop++;
                loopInteger = true;
                //printf("New code:%s\n",loop_code);
                parse(loop_code);

            }

            //loopInteger = false;





			//End Of Line
			if (wordCount > 4) {
				printf("'.' End of Line.\n");
			}
			else {
				printf("End of line is expected. (line %i)\n",i+1);
				return 0;
			}
		}

		else {
			printf("%s\n", word[0]);
		}
	}
}

char commentLine[100]; char a[100]; char b[100];
void isComment(char* str) {
	bool trueComment = false;
	int j;
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == '{') {
			for (j = i; j < strlen(str); j++) {
				if (str[j] == '}') {
					substring(str, i + 1, j - i - 1, commentLine);
					printf("Comment Line: '%s'\n", commentLine);

					substring(str, 0, i, a);
					substring(str, j + 1, strlen(str), b);
					strcat(a, b);
					strcpy(str, a);

					trueComment = true;

					isComment(str);
				}
			}
			if (!trueComment) {
				printf("Comment Line is not completed. \n");
				exit(0);
			}
		}
	}
}
int substring(char *source, int from, int n, char *target) {
	int length, i;
	for (length = 0; source[length] != '\0'; length++);
	if (from > length) {
		printf("Starting index is invalid.\n");
		return 1;
	}
	if ((from + n) > length) {
		n = (length - from);
	}
	for (i = 0; i < n; i++) {
		target[i] = source[from + i];
	}
	target[i] = '\0';
	return 0;
}

int main() {
	FILE *fp;
	char file_name[20];
	long lSize;
	char *code;
    int k = 0;
    printf("Enter the name of the file (Example: myscript):");
    scanf("%s",file_name);
    while (file_name[k] != '\0')
        k++;
    file_name[k] = '.';
    file_name[k+1] = 'b';
    file_name[k+2] = 'a';
    file_name[k+3] = '\0';

	fp = fopen(file_name, "rb");

	//if (!fp) perror("myscript.ba"), exit(1);
    //if (!fp) perror(file_name), exit(1);
    if(fp==NULL)//dosya açılmadıysa
        printf("Dosya acilamadi!\n");
    else {
	fseek(fp, 0L, SEEK_END);
	lSize = ftell(fp);
	rewind(fp);

	code = calloc(1, lSize + 1);
	if (!code) fclose(fp), fputs("memory alloc fails", stderr), exit(1);

	if (1 != fread(code, lSize, 1, fp)) {
		fclose(fp), free(code), fputs("entire read fails", stderr), exit(1);
	}

	isComment(code);
	parse(code);
    }
}
