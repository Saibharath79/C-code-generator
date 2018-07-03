struct intermediateCodeTable{

	int number_of_instructions;
	int capacity;
	struct intermediateCode **tuples;
};


struct intermediateCode{

	int ino;
	int opCode;
	int *parameters;
};


struct labelTable{
	int capacity = 50;
	int labels_count;
	struct label **labels;
};

struct label{
	char name;
	int address;
};

struct symbolTable{

	int number_of_symbols;
	int capacity;
	struct symbol **symbols;
};

struct symbol{
	int address;
	int size;
};