#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"
#include "structures.h"
#include "preprocessing.h"
#include "storeIntermediateCode.h"
#include "executeIntermediateCode.h"
#include "writeCfile.h"

void readLine(FILE *fp, char *str){
	int index = 0;
	char c = fgetc(fp);
	while (c != '\n' && c != EOF)
	{
		str[index++] = c;
		c = fgetc(fp);
		if (feof(fp))
			break;
	}
	str[index] = '\0';
}

int main(){

	/*char *filename = (char *)malloc(sizeof(char) * 260);

	printf("Enter File Name:");
	scanf("%s", filename);
	printf("\n");*/


	FILE *fp = fopen("code1.txt", "r");
	char *line = (char *)malloc(sizeof(char) * 1000);


	//create Symbol Table
	struct symbolTable *symbolsTable = (struct symbolTable*)malloc(sizeof(struct symbolTable));
	symbolsTable->number_of_symbols = 0;
	symbolsTable->capacity = 26;
	symbolsTable->symbols = (struct symbol **)malloc(sizeof(struct symbol *) * 26);
	for (int i = 0; i < 26; i++)
		symbolsTable->symbols[i] = (struct symbol *)malloc(sizeof(struct symbol));
	//Read DATA and CONST and push to symbol table
	while (true){

		readLine(fp, line);
		if (strcmp(line, "START:") == 0)
			break;

		pushToSymbolTable(symbolsTable, line);

	}
	printf("Symbol Table\n");
	printf("____________________________\n");
	printf("Symbol\tAddress\tSize\n");
	for (int i = 0; i < 26; i++){
		if (symbolsTable->symbols[i]->address>=0)
			printf("%c\t%d\t%d\n", i + 'A', symbolsTable->symbols[i]->address, symbolsTable->symbols[i]->size);

	}

	//Create IntermediateCode table
	struct intermediateCodeTable *intercode = (struct intermediateCodeTable *)malloc(sizeof(struct intermediateCodeTable));
	intercode->number_of_instructions = 0;
	intercode->capacity = 200;
	intercode->tuples = (struct intermediateCode **)malloc(sizeof(struct intermediateCode *)*(200));
	for (int i = 0; i < 200; i++){
		intercode->tuples[i] = (struct intermediateCode *)malloc(sizeof(struct intermediateCode));
		intercode->tuples[i]->parameters = (int *)malloc(sizeof(int) * 4);
		for (int j = 0; j < 4; j++)
			intercode->tuples[i]->parameters[j] = -1;
	}

	struct labelTable *labels = (struct labelTable *)malloc(sizeof(struct labelTable));
	labels->capacity = 50;
	labels->labels = (struct label **)malloc(sizeof(struct label*) * 26);
	for (int i = 0; i < 26; i++)
		labels->labels[i] = (struct label *)malloc(sizeof(struct label));
	labels->labels_count = 0;

	struct stack *s = createStack(20);

	while (true){

		readLine(fp, line);
		if (*line == '\0')
			break;

		pushToIntermediateCodeTable(intercode, labels, symbolsTable,s,line);

	}

	printf("\n\nIntermediate Code\n");
	printf("____________________________\n");
	printf("Ino\tOpcode\tp1\tp2\tp3\tp4\n");
	for (int i = 0; i < intercode->number_of_instructions; i++){
		printf("%d\t%d\t", intercode->tuples[i]->ino, intercode->tuples[i]->opCode);
	for (int j = 0; j < 4; j++)
		printf("%d\t", intercode->tuples[i]->parameters[j]);
	printf("\n");
	}

	printf("\n\nLabel Table\n");
	printf("____________________________\n");
	printf("Name\tAddress\n");
	for (int i = 0; i < labels->labels_count; i++){
		printf("%c\t%d\n", labels->labels[i]->name, labels->labels[i]->address);
	}

	storeIntermediateCode(intercode,labels,symbolsTable);
	executeIntermediateCode();
	writeCfile(intercode, symbolsTable, labels);
	free(line);
	/*free(filename);*/
	system("pause");
	return 0;
}