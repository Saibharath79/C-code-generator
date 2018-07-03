void writeCfile(struct intermediateCodeTable *intercode, struct symbolTable *symbols, struct labelTable *labels){

	char opcodes[OPCODE_SIZE][6] = { "dfhf", "MOV", "fsdf", "ADD", "SUB", "MUL", "JMP", "IF", "EQ", "LT", "GT", "LTEQ", "GTEQ", "PRINT", "READ", "ELSE", "END" };

	FILE *fp=fopen("outputCode.c", "w");
	fprintf(fp, "#include<stdio.h>\n");
	fprintf(fp, "#include<malloc.h>\n");

	fprintf(fp, "int main(){\n");

	fprintf(fp,"int ");
	for (int i = 0; i < 26; i++)
	{
			if (symbols->symbols[i]->size == 1)
				fprintf(fp, "%c,", i + 'A');
	}
	for (int i = 0; i < 7; i++)
	{
		fprintf(fp, "%cX,", i + 'A');
	}
	fprintf(fp,"HX;\n");

	for (int i = 0; i < 26; i++){
		if (symbols->symbols[i]->size>1)
			fprintf(fp, "int %c[%d];\n", i + 'A', symbols->symbols[i]->size);
	}

	for (int i = 0; i < 26; i++)
	{
		if (symbols->symbols[i]->size == 0){
			if (symbols->symbols[i]->address >= 0)
				fprintf(fp, "const int %c=%d;\n", i + 'A', memory[symbols->symbols[i]->address]);
		}
	}


	for (int i = 0; i < 26; i++){
		if (symbols->symbols[i] >= 0){
			if (symbols->symbols[i]->size == 1){
				fprintf(fp, "%c=%d;\n", i + 'A', memory[symbols->symbols[i]->address]);
			}
			else if (symbols->symbols[i]->size > 1){
				for (int j = 0; j < symbols->symbols[i]->size; j++){
					fprintf(fp, "%c[", i + 'A');
					fprintf(fp, "%d]=%d;\n", j, memory[symbols->symbols[i]->address + j]);
				}
			}
		}
	}
	//fprintf(fp, "%d")

		
	int *closingBracketBit = (int *)malloc(sizeof(int)*(intercode->number_of_instructions));
	for (int i = 0; i < intercode->number_of_instructions; i++)
	{
		closingBracketBit[i] = 0;
	}

	for (int i = 0; i < intercode->number_of_instructions; i++){

		if (closingBracketBit[i] == 1)
			fprintf(fp, "}");


		if (intercode->tuples[i]->opCode == 0){
			fprintf(fp, "%c:\n",labels->labels[intercode->tuples[i]->parameters[0]]->name);
		}

		else if (intercode->tuples[i]->opCode == 1){
			char arg1, arg2;
			for (int j = 0; j < 26; j++)
				if (symbols->symbols[i]->address == intercode->tuples[i]->parameters[0])
					arg1 = (char)(j + 'A');
			arg2 = (char)(intercode->tuples[i]->parameters[1]+'A');
			fprintf(fp, "%c=%cX;\n", arg1, arg2);
		}
		else if (intercode->tuples[i]->opCode == 2){
			
			char arg1, arg2;
			for (int j = 0; j < 26; j++){
				if (symbols->symbols[j]->address == intercode->tuples[i]->parameters[1])
					arg1 = (char)(j + 'A');
			}
			arg2 = (char)(intercode->tuples[i]->parameters[0] + 'A');
			fprintf(fp, "%cX=%c;\n", arg2, arg1);

		}
		else if (intercode->tuples[i]->opCode == 3){

			char arg1 = (char)(intercode->tuples[i]->parameters[0] + 'A');
			char arg2 = (char)(intercode->tuples[i]->parameters[1] + 'A');
			char arg3 = (char)(intercode->tuples[i]->parameters[2] + 'A');

			fprintf(fp, "%cX=%cX+%cX;\n", arg1, arg2, arg3);
		}
		else if (intercode->tuples[i]->opCode == 4){

			char arg1 = (char)(intercode->tuples[i]->parameters[0] + 'A');
			char arg2 = (char)(intercode->tuples[i]->parameters[1] + 'A');
			char arg3 = (char)(intercode->tuples[i]->parameters[2] + 'A');

			fprintf(fp, "%cX=%cX-%cX;\n", arg1, arg2, arg3);

		}
		else if (intercode->tuples[i]->opCode == 5){

			char arg1 = (char)(intercode->tuples[i]->parameters[0] + 'A');
			char arg2 = (char)(intercode->tuples[i]->parameters[1] + 'A');
			char arg3 = (char)(intercode->tuples[i]->parameters[2] + 'A');

			fprintf(fp, "%cX=%cX*%cX;\n", arg1, arg2, arg3);

		}
		else if (intercode->tuples[i]->opCode == 6){

			for (int j = 0; j <26; j++){

				if (labels->labels[j]->address >= 0){
					if (intercode->tuples[i]->parameters[0] == labels->labels[j]->address)
						fprintf(fp,"goto %c;\n", labels->labels[j]->name);
				}
			}

		}
		else if (intercode->tuples[i]->opCode == 7){

			char arg1 = intercode->tuples[i]->parameters[0]+'A';
			char arg2 = intercode->tuples[i]->parameters[1]+'A';
			fprintf(fp, "if(%cX",arg1);
			
			if (strcmp(opcodes[intercode->tuples[i]->parameters[2]], "LT") == 0)
				fprintf(fp, "<");

			else if (strcmp(opcodes[intercode->tuples[i]->parameters[2]], "GT") == 0)
				fprintf(fp, ">");

			else if (strcmp(opcodes[intercode->tuples[i]->parameters[2]], "LTEQ") == 0)
				fprintf(fp, "<=");

			else if (strcmp(opcodes[intercode->tuples[i]->parameters[2]], "GTEQ") == 0)
				fprintf(fp, ">=");

			else if (strcmp(opcodes[intercode->tuples[i]->parameters[2]], "EQ") == 0)
				fprintf(fp, "==");

			fprintf(fp,"%cX){\n",arg2);

		}
		else if (intercode->tuples[i]->opCode == 13){
			char arg1 = intercode->tuples[i]->parameters[0] + 'A';
			fprintf(fp, "printf(\"");
			fprintf(fp, "%%");
			fprintf(fp, "d\\n\",");
			fprintf(fp, "%cX);\n", arg1);
		}
		else if (intercode->tuples[i]->opCode == 14){
			char arg1 = intercode->tuples[i]->parameters[0] + 'A';
			fprintf(fp, "scanf(\"");
			fprintf(fp, "%%");
			fprintf(fp, "d\",");
			fprintf(fp, "&%cX);\n", arg1);
		}
		else if (intercode->tuples[i]->opCode == 15){
			fprintf(fp, "}else{\n");
			closingBracketBit[intercode->tuples[i]->parameters[0]] = 1;
		}
		else if (intercode->tuples[i]->opCode == 16){
			fprintf(fp, "return 0;\n");
			fprintf(fp, "}");
		}


		/*if (labelsIter < labels->labels_count && i != (intercode->number_of_instructions - 1) && (i + 1) == labels->labels[labelsIter]->address){

			fprintf(fp, "%c:\n", labels->labels[labelsIter]->name);
			labelsIter++;
		}*/
	}
	fclose(fp);

	
}