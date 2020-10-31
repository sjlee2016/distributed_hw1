struct inputs{
	int n1;
	int n2;
	char operator;
};
program CALCULATOR_PROG{
	version CALCULATOR_VERS{
			int server_calculator(inputs)=1;
	}=1;
}=0x31111111;


