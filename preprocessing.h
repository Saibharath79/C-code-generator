#define OPCODE_SIZE 17
char registers[8];
char memory[5000];
int memoryIndex = 0;
void pushToSymbolTable(struct symbolTable *symbolsTable,char *line){
	int i = 0;
	while (i<strlen(line)){

		while (line[i] == ' ')
			i++;
		char *dataOrConst = (char *)malloc(sizeof(char) * 20);
		int index = 0;
		while (line[i] != ' ')
			dataOrConst[index++] = line[i++];
		dataOrConst[index] = '\0';
		if (strcmp(dataOrConst, "DATA") == 0)
		{
			index = 0;
			while (line[i] == ' ')
				i++;

			while (line[i] != '\0')
				dataOrConst[index++] = line[i++];
			dataOrConst[index] = '\0';


			symbolsTable->symbols[dataOrConst[0] - 'A']->address=memoryIndex;
			if (strlen(dataOrConst) == 1){
				memoryIndex++;
				symbolsTable->symbols[dataOrConst[0] - 'A']->size = 1;
			}
			else{
				index = 0;
				while (dataOrConst[index] != '[')
					index++;
				index++;

				symbolsTable->symbols[dataOrConst[0] - 'A']->size = 0;
				while (dataOrConst[index] != ']')
					symbolsTable->symbols[dataOrConst[0] - 'A']->size = symbolsTable->symbols[dataOrConst[0] - 'A']->size * 10 + dataOrConst[index++] - '0';
				index++;
				memoryIndex = memoryIndex + (symbolsTable->symbols[dataOrConst[0] - 'A']->size);
			}
		}
		else if (strcmp(dataOrConst, "CONST") == 0){

			index = 0;
			while (line[i] == ' ')
				i++;

			while (line[i] != '\0')
				dataOrConst[index++] = line[i++];
			dataOrConst[index] = '\0';

			symbolsTable->symbols[dataOrConst[0] - 'A']->size = 0;
			symbolsTable->symbols[dataOrConst[0] - 'A']->address = memoryIndex++;

			index = 4;
			int value = 0;
			while (dataOrConst[index] != '\0')
				value = value * 10 + dataOrConst[index++] - '0';
			memory[symbolsTable->symbols[dataOrConst[0] - 'A']->address] = value;

		}
		symbolsTable->number_of_symbols++;
		if (symbolsTable->capacity < symbolsTable->number_of_symbols){
			symbolsTable->symbols = (struct symbol **)realloc(symbolsTable->symbols, symbolsTable->capacity * 2);
			for (int i = 0; i < symbolsTable->number_of_symbols; i++)
				if (symbolsTable->symbols[i] == NULL)
					symbolsTable->symbols[i] = (struct symbol *)malloc(sizeof(struct symbol));
			symbolsTable->capacity = symbolsTable->capacity * 2;


		}
	}
}

int is_label(char *label){
	if (strlen(label) != 2)
		return 0;
	if (label[1] != ':')
		return 0;
	return 1;
}

int is_register(char *registerName){

	if (strlen(registerName) != 2)
		return 0;

	if (registerName[1] == 'X' && registerName[0] >= 'A' && registerName[0] <= 'Z')
		return 1;
	return 0;

}

void pushToIntermediateCodeTable(struct intermediateCodeTable *intercode, struct labelTable *labels, struct symbolTable *symbolsTable,struct stack *s,char *line){
	char opcodes[OPCODE_SIZE][6] = { "LABEL", "MOV", "fsdf", "ADD", "SUB", "MUL", "JMP", "IF", "EQ", "LT", "GT", "LTEQ", "GTEQ", "PRINT", "READ", "ELSE","END" };
	int i = 0;

	while (i < strlen(line)){

		while (line[i] == ' ')
			i++;

		intercode->tuples[intercode->number_of_instructions]->ino = intercode->number_of_instructions;//ino entry done


		char *operation = (char *)malloc(sizeof(char) * 6);
		int index = 0;
		while (line[i] != ' ' && i < strlen(line))
			operation[index++] = line[i++];
		operation[index] = '\0';




		for (int j = 0; j < OPCODE_SIZE; j++){
			if (strcmp(opcodes[j], operation) == 0)
				intercode->tuples[intercode->number_of_instructions]->opCode = j;//Opcode Entry Done
		}
	

		while (line[i] == ' ')
			i++;

		if (strcmp(operation, "PRINT") == 0 || strcmp(operation, "READ") == 0){
			char *registerName = (char *)malloc(sizeof(char) * 2);
			int index = 0;
			while (line[i] != '\0')
				registerName[index++] = line[i++];
			registerName[index] = '\0';
			intercode->tuples[intercode->number_of_instructions]->parameters[0] = registerName[0] - 'A';
			//free(registerName);

		}
		else if (strcmp(operation, "ADD") == 0 || strcmp(operation, "SUB") == 0 || strcmp(operation, "MUL") == 0){
			char *destRegister = (char *)malloc(sizeof(char) * 2);
			char *source1Register = (char *)malloc(sizeof(char) * 2);
			char *source2Register = (char *)malloc(sizeof(char) * 2);

			int index = 0;
			while (line[i] != ',')
				destRegister[index++] = line[i++];
			i++;//ADD OPERATION
			destRegister[index] = '\0';

			while (line[i] == ' ')
				i++;

			index = 0;
			while (line[i] != ',')
				source1Register[index++] = line[i++];
			i++;
			source1Register[index] = '\0';

			while (line[i] == ' ')
				i++;

			index = 0;
			while (line[i] != '\0')
				source2Register[index++] = line[i++];
			source2Register[index] = '\0';

			intercode->tuples[intercode->number_of_instructions]->parameters[0] = destRegister[0] - 'A';
			intercode->tuples[intercode->number_of_instructions]->parameters[1] = source1Register[0] - 'A';
			intercode->tuples[intercode->number_of_instructions]->parameters[2] = source2Register[0] - 'A';
		}
		else if (strcmp(operation, "JMP") == 0){
			char *label = (char *)malloc(sizeof(char) * 20);
			int index = 0;
			while (line[i] != '\0')
				label[index++] = line[i++];
			label[index] = '\0';
			for (int i = 0; i < labels->labels_count; i++){
				if (label[0] == labels->labels[i]->name)
					intercode->tuples[intercode->number_of_instructions]->parameters[0] = labels->labels[i]->address;
			}
			free(label);
		}
		else if (strcmp(operation, "IF") == 0){

			char *register1 = (char *)malloc(sizeof(char) * 2);
			char *register2 = (char *)malloc(sizeof(char) * 2);
			char *comparision = (char *)malloc(sizeof(char) * 4);


			int index = 0;
			while (line[i] != ' ')
				register1[index++] = line[i++];
			register1[index] = '\0';
			while (line[i] == ' ')
				i++;

			index = 0;
			while (line[i] != ' ')
				comparision[index++] = line[i++];
			comparision[index] = '\0';
			while (line[i] == ' ')
				i++;

			index = 0;
			while (line[i] != ' ')
				register2[index++] = line[i++];
			register2[index] = '\0';

			while (line[i] != '\0')
				i++;

			for (int j = 8; j < 13; j++){
				if (strcmp(comparision, opcodes[j]) == 0)
					intercode->tuples[intercode->number_of_instructions]->parameters[2] = j;
			}
			intercode->tuples[intercode->number_of_instructions]->parameters[0] = register1[0] - 'A';
			intercode->tuples[intercode->number_of_instructions]->parameters[1] = register2[0] - 'A';

			push(s, intercode->number_of_instructions);

		//	free(register1);
		//	free(register2);
		//	free(comparision);
		}
		else if (strcmp(operation, "MOV") == 0){
			char *destRegister = (char *)malloc(sizeof(char) * 2);
			char *sourceRegister = (char *)malloc(sizeof(char) * 2);

			int index = 0;
			while (line[i] != ',')
				destRegister[index++] = line[i++];
			i++;//ADD OPERATION
			destRegister[index] = '\0';

			while (line[i] == ' ')
				i++;

			index = 0;
			while (line[i] != '\0')
				sourceRegister[index++] = line[i++];
			i++;
			sourceRegister[index] = '\0';

			if (is_register(sourceRegister) == 1 && is_register(destRegister) == 0){
				intercode->tuples[intercode->number_of_instructions]->opCode = 1;
				intercode->tuples[intercode->number_of_instructions]->parameters[1] = sourceRegister[0] - 'A';
				int offset = 0;
				if (strlen(destRegister) >= 3)
				{
					index = 0;
					while (destRegister[index] != '[')
						index++;
					index++;
					while (destRegister[index] != ']')
						offset = offset * 10 + (destRegister[index++]) - '0';
				}
				intercode->tuples[intercode->number_of_instructions]->parameters[0] = symbolsTable->symbols[destRegister[0] - 'A']->address+offset;

			}
			else if (is_register(sourceRegister) == 0 && is_register(destRegister) == 1){
				intercode->tuples[intercode->number_of_instructions]->opCode = 2;
				intercode->tuples[intercode->number_of_instructions]->parameters[0] = destRegister[0] - 'A';
				int offset = 0;
				if (strlen(sourceRegister) >= 3)
				{
					index = 0;
					while (sourceRegister[index] != '[')
						index++;
					index++;
					while (sourceRegister[index] != ']')
						offset = offset * 10 + (sourceRegister[index++]) - '0';
				}
				intercode->tuples[intercode->number_of_instructions]->parameters[1] = symbolsTable->symbols[sourceRegister[0] - 'A']->address+offset;
			}
		//	free(sourceRegister);
		//	free(destRegister);
		}
		else if (is_label(operation)){
			
			labels->labels[labels->labels_count]->name = operation[0];
			labels->labels[labels->labels_count]->address = intercode->number_of_instructions;
			intercode->tuples[intercode->number_of_instructions]->parameters[0]=labels->labels_count++;
			intercode->tuples[intercode->number_of_instructions]->opCode=0;
			
			//intercode->number_of_instructions--;

		}
		else if (strcmp(operation, "ELSE") == 0)
		{
			intercode->tuples[pop(s)]->parameters[3] = intercode->number_of_instructions+1;
			push(s, intercode->number_of_instructions);
		}
		else if (strcmp(operation, "ENDIF") == 0){
			intercode->tuples[pop(s)]->parameters[0] = intercode->number_of_instructions;
			intercode->number_of_instructions--;
		}
		else if (strcmp(operation, "END"))
			return;
	//	free(operation);

	}
	intercode->number_of_instructions++;
	if (intercode->capacity < intercode->number_of_instructions){
		intercode->tuples = (struct intermediateCode **)realloc(intercode->tuples, intercode->capacity * 2);
		for (int i = 0; i < intercode->number_of_instructions; i++)
			if (intercode->tuples[i] == NULL)
				intercode->tuples[i] = (struct intermediateCode *)malloc(sizeof(struct intermediateCode));
		intercode->capacity = intercode->capacity * 2;
	}
}