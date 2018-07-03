char newMemory[5000];
int newMemoryIndex = 0;
char newRegisters[8];



struct intermediateCodeTable *loadIntermediateCode(){
	FILE *fp = fopen("intermediateCode.txt", "r");
	int number = 0, i = 0;

	fscanf(fp, "%d", &number);

	struct intermediateCodeTable *intercode = (struct intermediateCodeTable *)malloc(sizeof(struct intermediateCodeTable));
	intercode->number_of_instructions = number;
	intercode->tuples = (struct intermediateCode **)malloc(sizeof(struct intermediateCode *)*number);
	for (int i = 0; i < number; i++){
		intercode->tuples[i] = (struct intermediateCode *)malloc(sizeof(struct intermediateCode));
		intercode->tuples[i]->parameters = (int *)malloc(sizeof(int) * 4);
	}

	for(int i=0;i<number;i++){

		fscanf(fp, "%d %d %d %d %d %d", &intercode->tuples[i]->ino, &intercode->tuples[i]->opCode, &intercode->tuples[i]->parameters[0], &intercode->tuples[i]->parameters[1], &intercode->tuples[i]->parameters[2], &intercode->tuples[i]->parameters[3]);

	}
	return intercode;
}


struct symbolTable *loadSymbols(){

	FILE *fp = fopen("symbols.txt", "r");
	int number = 0;
	fscanf(fp, "%d", &number);
	

	struct symbolTable *symbolsTable = (struct symbolTable*)malloc(sizeof(struct symbolTable));
	symbolsTable->number_of_symbols = 0;
	symbolsTable->capacity = 26;
	symbolsTable->symbols = (struct symbol **)malloc(sizeof(struct symbol *) * 26);
	for (int i = 0; i < 26; i++)
		symbolsTable->symbols[i] = (struct symbol *)malloc(sizeof(struct symbol));

	for (int i = 0; i < number; i++){
		char ch,dump;
		int temp1, temp2;
		fscanf(fp,"%c", &dump);
		fscanf(fp,"%c %d %d",&ch, &temp1, &temp2);
		symbolsTable->symbols[ch - 'A']->address = temp1;
		symbolsTable->symbols[ch - 'A']->size = temp2;
		symbolsTable->number_of_symbols++;
	}
	return symbolsTable;
}

struct labelTable *loadLabelTable(){

	int number = 0;
	FILE *fp = fopen("labeltable.txt", "r");
	fscanf(fp, "%d", &number);
	
	struct labelTable *labels = (struct labelTable *)malloc(sizeof(struct labelTable));
	labels->capacity = 50;
	labels->labels = (struct label **)malloc(sizeof(struct label*) * 26);
	for (int i = 0; i < 26; i++)
		labels->labels[i] = (struct label *)malloc(sizeof(struct label));
	labels->labels_count = 0;

	for (int i = 0; i < number; i++){
		char dump;
		fscanf(fp, "%d", &dump);
		fscanf(fp, "%c %d", &labels->labels[i]->name, &labels->labels[i]->address);
		labels->labels_count++;
	}
	return labels;
}


int conditionCheck(int a, int b, int op){
	switch (op){
	case 8:
		return a == b;
		break;
	case 9:
		return a < b;
		break;
	case 10:
		return a > b;
		break;
	case 11:
		return a <= b;
		break;
	case 12:
		return a >= b;
		break;
	}
}
void loadMemory(){

	FILE *fp = fopen("memory.txt", "r");
	int number = 0;
	fscanf(fp, "%d", &number);
	for (int i = 0; i < number; i++)
		fscanf(fp, "%d", &newMemory[newMemoryIndex++]);
}
void executeIntermediateCode(){

	struct intermediateCodeTable *intercode = loadIntermediateCode();
	/*printf("\n\nIntermediate Code\n");
	printf("____________________________\n");
	printf("Ino\tOpcode\tp1\tp2\tp3\tp4\n");
	for (int i = 0; i < intercode->number_of_instructions; i++){
		printf("%d\t%d\t", intercode->tuples[i]->ino, intercode->tuples[i]->opCode);
		for (int j = 0; j < 4; j++)
			printf("%d\t", intercode->tuples[i]->parameters[j]);
		printf("\n");
	}*/

	struct symbolTable *symbolsTable = loadSymbols();

	/*printf("Symbol Table\n");
	printf("____________________________\n");
	printf("Symbol\tAddress\tSize\n");
	for (int i = 0; i < 26; i++){
		if (symbolsTable->symbols[i]->address >= 0)
			printf("%c\t%d\t%d\n", i + 'A', symbolsTable->symbols[i]->address, symbolsTable->symbols[i]->size);

	}*/

	struct labelTable *labels = loadLabelTable();


	loadMemory();

	/*printf("\n\nLabel Table\n");
	printf("____________________________\n");
	printf("Name\tAddress\n");
	for (int i = 0; i < labels->labels_count; i++)
		printf("%c\t%d", labels->labels[i]->name, labels->labels[i]->address);
	printf("\n");
*/

	printf("\nExecution Started\n");

	int i = 0;
	int iter = 0;
	while(i<intercode->number_of_instructions){

		if (intercode->tuples[i]->opCode == 0){
			i++;
		}
		else if (intercode->tuples[i]->opCode == 1){
			newMemory[intercode->tuples[i]->parameters[0]] = newRegisters[intercode->tuples[i]->parameters[1]];
			i++;
		}
		else if (intercode->tuples[i]->opCode == 2){
			newRegisters[intercode->tuples[i]->parameters[0]] = newMemory[intercode->tuples[i]->parameters[1]];
			i++;
		}
		else if (intercode->tuples[i]->opCode == 3){
			newRegisters[intercode->tuples[i]->parameters[0]] = newRegisters[intercode->tuples[i]->parameters[1]] + newRegisters[intercode->tuples[i]->parameters[2]];
			i++;
		}
		else if (intercode->tuples[i]->opCode == 4){
			newRegisters[intercode->tuples[i]->parameters[0]] = newRegisters[intercode->tuples[i]->parameters[1]] - newRegisters[intercode->tuples[i]->parameters[2]];
			i++;
		}
		else if (intercode->tuples[i]->opCode == 5){
			newRegisters[intercode->tuples[i]->parameters[0]] = newRegisters[intercode->tuples[i]->parameters[1]] * newRegisters[intercode->tuples[i]->parameters[2]];
			i++;
		}
		else if (intercode->tuples[i]->opCode == 6){
			i =intercode->tuples[i]->parameters[0];
		}
		
		else if (intercode->tuples[i]->opCode == 7)
		{
			if (conditionCheck(newRegisters[intercode->tuples[i]->parameters[0]], newRegisters[intercode->tuples[i]->parameters[1]],intercode->tuples[i]->parameters[2]))
			{
				intercode->tuples[intercode->tuples[i]->parameters[0]]->parameters[2] = 1;
				i++;
			}
			else{
				i = intercode->tuples[i]->parameters[3];
				intercode->tuples[intercode->tuples[i]->parameters[0]]->parameters[2] = 0;
			}
		}
		else if (intercode->tuples[i]->opCode == 13){
			printf("%d\n",newRegisters[intercode->tuples[i]->parameters[0]]);
			i++;
		}
		else if (intercode->tuples[i]->opCode == 14){
			scanf("%d", &newRegisters[intercode->tuples[i]->parameters[0]]);
			i++;
		}
		else if (intercode->tuples[i]->opCode == 15){
			if (intercode->tuples[i]->parameters[1])
				i = intercode->tuples[i]->parameters[0];
			else
				i++;
		}
		else if (intercode->tuples[i]->opCode == 16)
			break;
	}


	printf("\nFinal Registers\n");
	for (int i = 0; i < 8; i++)
		printf("%d\t", newRegisters[i]);

	printf("\nMemory\n");

	for (int i = 0; i < newMemoryIndex;i++)
		printf("%d\t",(int) newMemory[i]);
	printf("\n");

	printf("Execution Done\n");

}