
#include "calc.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
typedef struct expr{
    int n;
    char op;
    int isNumber; 
}expr; 
int numHead = -1;
int opHead = -1;
char operatorStack[10000];
int numberStack[10000];
expr ex[100000];
char expression[10000];
char prompt[10000];
int total = 0;
int getPrecedence(char c){
    if(c<='9' && c>='0'){ // is number
        return 0; 
    }
    switch(c){
        case '+' : 
        case '-' : return 1;
        case '*' : 
        case '/' : return 2; 
        case '^' : return 3; 
        default : printf("Wrong operator %c!\n",c); 
                  return -1; 
    }
    
}

int seperate(char * expression){
    int previousNum = 0; 
    int previousIndex = 0;
    int isNegative = 1;
    int precendence;
    int num = 0;
    int consec = 0;
    expr e; 
    for(int i = 0; i < strlen(expression); i++){
        char c = expression[i];
        precendence = getPrecedence(c);
        if(precendence == -1 ){
            return 0; 
        }
        if(!precendence){ // is a number 
            previousNum = 1;
            num*=10;
            num+=atoi(&c);
            consec = 0; 
        }else{ // operator
            if(previousNum){ // store number 
                e.isNumber = 1;
                e.n = num * isNegative;
                isNegative = 1; 
                ex[total++] = e; 
                num = 0;
            }
            if(consec == 2 ){ // more than 3 operators in consecutive order
                printf("Error! Format of operation\n");
                return 0;
            }

            if((i==0 || getPrecedence(expression[i-1])) && expression[i] == '-') {
                isNegative = -1; 
            }else{
                if(i!=strlen(expression)-1 && expression[i+1] == '*' && expression[i]=='*'){
                    e.op='^';
                    e.isNumber = 0; 
                    ex[total++]=e;
                    i+=1;
                }else{
                    e.op=c;
                    e.isNumber = 0;
                    ex[total++]=e;
                }
            }
            previousNum = 0;
            consec += 1; 
        }
    }
    if(precendence){
        printf("Error! expression cannot end with an operator\n");
        return 0;
    }
    e.isNumber = 1;
    e.n = num * isNegative;
    ex[total++]=e;
    if(total==1){
        printf("Error! No operator given.\n");
        return 0;
    }
    return 1;
}

void calculator_prog_1(char *host)
{
	CLIENT *clnt;
	int  *result_1;
    char op;
    int n1,n2;
	inputs  server_calculator_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, CALCULATOR_PROG, CALCULATOR_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
    for(int i = 0 ; i < total; i++){
        if(ex[i].isNumber){
            numberStack[++numHead] = ex[i].n;
        }else{
            char c = ex[i].op;
            while(opHead >= 0 && getPrecedence(operatorStack[opHead]) >= getPrecedence(c)){
                if(numHead <= 0 ) {
                    printf("Error! operation format!\n");
                }
                n2 = numberStack[numHead--];
                n1 = numberStack[numHead--]; 
                op = operatorStack[opHead--]; 
                if(op=='/' && n2==0){
                    printf("Error! Cannot divide by zero\n");
                    return; 
                }
                server_calculator_1_arg.n2 = n2; 
                server_calculator_1_arg.n1 = n1; 
                server_calculator_1_arg.operator = op;
                result_1 = server_calculator_1(&server_calculator_1_arg,clnt);
                if(result_1 == (int*)NULL){
                    printf("call failed\n");
                    clnt_perror(clnt,"call failed");
                    return; 
                }
                
                numberStack[++numHead] = *result_1;
            }
            operatorStack[++opHead]= ex[i].op; 
        }
        }
        while(opHead>=0){
            if(numHead<=0){
                printf("Error! in operation format\n");
                return;
            }
            server_calculator_1_arg.n2 = numberStack[numHead--];
            server_calculator_1_arg.n1 = numberStack[numHead--];
            server_calculator_1_arg.operator = operatorStack[opHead--];
            if(server_calculator_1_arg.n2 == 0 && server_calculator_1_arg.operator =='/'){
                printf("Error! Cannot divide by zero\n");
                return;
            }
            result_1 = server_calculator_1(&server_calculator_1_arg,clnt);
            numberStack[++numHead] = *result_1;
        }

        printf("the answer is %d\n", numberStack[numHead]);
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}

void initializeStack(){
    for(int i = 0; i < 10000; i++){
        numberStack[i] = 0;
        operatorStack[i]='\0';
        expression[i]='\0';
        prompt[i]='\0';
    }
    numHead = -1;
    opHead = -1;
    total = 0; 
}
int main (int argc, char *argv[])
{
	char *host;
	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
    host = argv[1];
    initializeStack();
    while(1){
        initializeStack(); 
        printf("assignment > ");
         scanf("%s",prompt);
         if(strcmp(prompt,"exit")==0){
             exit(0);
         }
        if(strcmp(prompt,"test")!=0){
            printf("Please enter test to start calculation or exit to quit\n");
        }else{
            scanf("%s",expression);
            if(seperate(expression))
                calculator_prog_1(host);
        }
    }
        
exit (0);
}  
