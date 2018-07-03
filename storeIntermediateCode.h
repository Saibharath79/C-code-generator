void writeIntermediateCodeToFile(struct intermediateCodeTable *intermediatecode){
	FILE *fp=fopen("intermediateCode.txt","w");
	fprintf(fp,"%d\n", intermediatecode->number_of_instructions);
	for (int i = 0; i < intermediatecode->number_of_instructions; i++){
		fprintf(fp, "%d %d ", intermediatecode->tuples[i]->ino, intermediatecode->tuples[i]->opCode);
		for (int j = 0; j < 4; j++)
			fprintf(fp, "%d ", intermediatecode->tuples[i]->parameters[j]);
		fprintf(fp, "\n");
	}
	fclose(fp);
}

void writeLabelTable(struct labelTable *labels){

	FILE *fp = fopen("labeltable.txt", "w");
	fprintf(fp,"%d\n", labels->labels_count);
	for (int i = 0; i < labels->labels_count; i++){
		fprintf(fp,"%c %d\n", labels->labels[i]->name, labels->labels[i]->address);
	}
	fclose(fp);
}


void storeMemory(){
	FILE *fp = fopen("memory.txt", "w");
	fprintf(fp,"%d\n", memoryIndex);
	for (int i = 0; i < memoryIndex; i++)
		fprintf(fp, "%d\n", memory[i]);
	fclose(fp);
}

void storeSymbolTable(struct symbolTable *symbolstable){
	FILE *fp = fopen("symbols.txt", "w");
	fprintf(fp, "%d\n", symbolstable->number_of_symbols);
	for (int i = 0; i < 26; i++)
	{
		if (symbolstable->symbols[i]->address>=0)
			fprintf(fp, "%c %d %d\n",i+'A',symbolstable->symbols[i]->address, symbolstable->symbols[i]->size);
	}
	fclose(fp);
}

void storeIntermediateCode(struct intermediateCodeTable *intercode, struct labelTable *labels, struct symbolTable *symbolstable){
	storeSymbolTable(symbolstable);
	storeMemory();
	writeLabelTable(labels);
	writeIntermediateCodeToFile(intercode);
}