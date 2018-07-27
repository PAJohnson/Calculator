#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"

//basic calculator
//It works! next step - macro expander
//Macro expander, to remain compatible with the PSoC, should rely
//on static macro memory. In the PC version, this will be a simple
//text file. In the PSoC, it could be an SD card or flash chip or
//something.

//Macros?
//name(input1,input2...)=expression
//have the macros stored in a file somewhere, retrieve them.
//scan through user input, expand macros while equation is still
//in a text format. Once macros are expanded, parse then input
//and evaluate as usual.

//for macro retrieval - malloc space to store them. They will reside
//in allocated memory until the program is turned off - no problem
//for the PSoC.

//example macro: sind(x)=sin(x*2*3.14159/260)
//wherever sind(x) is found, sin(x*2*3.14159/360) will be substituted
//don't bother checking the macro arguments for correctness - 
//errors will be found during the parsing and evaluating stage.

//temporary macro for testing!
char *sind = "sind(x)=sin(x*2*3.14159/360)";

#define DEPTH 1000
#define INPUT_DEPTH 800
#define MACRO_DEPTH 100

char *help = "help";  
char *quit = "quit";  

enum operation{
	Blank,
    Null,
    Add,
    Subtract,
    Multiply,
    Divide,
    OpenParen,
    CloseParen,
    Power,
    Sine,			
    Cosine,
    Tangent,
    Arcsin,			
    Arccos,
    Arctan,
    Exp,			
    Ln
};

struct entry{
    enum operation operator;
    double data;
    int data_flag;
    int op_flag;
};

struct eqn{
    int tree_ptr;
    struct entry tree[DEPTH];
};

char ** macros[MACRO_DEPTH];

void add_entry(struct eqn * equation ,enum operation op, double data);
void strip_whitespace(char * input);
void parse(char * input, struct eqn * equation);
void evaluate_eqn(struct eqn * equation);
void print_eqn(struct eqn * equation);
void clear_eqn(struct eqn * equation);
int is_number(char input);
int is_operator(char input);
void squish_eqn(struct eqn * equation);
void swap_entry(struct entry * left, struct entry * right);
int empty_entry(struct entry * ptr);
void clear_macros(char ** macros);
void get_macros(char ** macros);
void expand_macros(char * input, char ** macros);

int main(){

    char input[INPUT_DEPTH];
    struct eqn equation;
    int i = 0;
    clear_eqn(&equation);
    for(i = 0; i < INPUT_DEPTH; i++){
        input[i] = 0;
    }
    printf("Scientific Calculator 0.1\n");
    printf("Type 'help' for more information\n");

    while(1){
        printf(": ");
        fgets(input,INPUT_DEPTH,stdin);
        strip_whitespace(input);
        if(strcmp(input,help) == 0){
			printf("Supported operations: \n");
			printf("+ - * / ^ sin() cos() tan() asin() acos() atan() exp() ln()\n\n");
			printf("For negative numbers: (-10) evaluates to negative 10\n\n");
			printf("Trigonometric operations assume input in radians\n\n");
			printf("type 'quit' to exit program\n\n");
			printf("Example usage: sin(90*(2*3.14159/360)) evaluates to 1.000000\n");
		}
		else if(strcmp(input,quit) == 0){
			return EXIT_SUCCESS;
		}
		else{
			parse(input,&equation);      
			squish_eqn(&equation);
			while(equation.tree_ptr > 1){
				evaluate_eqn(&equation);
			}
			print_eqn(&equation);
			clear_eqn(&equation);       //clear out tree
		}	


    }

}

void clear_macros(char ** macros){
	int i = 0;
	for(i = 0; i < MACRO_DEPTH; i++){
		macros[i] = NULL;
	}
}

void get_macros(char ** macros){
	
}


void expand_macros(char * input, char ** macros){
	
}

void add_entry(struct eqn * equation, enum operation op, double data){

    if(equation->tree_ptr < DEPTH){

        if(op == Null){
            equation->tree[equation->tree_ptr].data = data;
            equation->tree[equation->tree_ptr].data_flag = 1;
            equation->tree_ptr++;
        }
		else if(op == Blank){
            equation->tree[equation->tree_ptr].data = 0.0;
            equation->tree[equation->tree_ptr].data_flag = 0;
            equation->tree[equation->tree_ptr].op_flag = 0;
            equation->tree_ptr++;
        }
        else{
			equation->tree[equation->tree_ptr].operator = op;
			equation->tree[equation->tree_ptr].op_flag = 1;
			equation->tree_ptr++;
        }
    }
    return;
}

void parse(char * input,struct eqn * equation){
    int i = 0;
    int j = 0;
    char temp[80];
    enum operation tempOp = Blank;
    while(input[i] != '\0'){
		tempOp = Blank;
        for(j = 0; j < 80; j++){
            temp[j] = 0;
        }

        j = 0;

        while(is_number(input[i])){
            temp[j] = input[i];
            i++;
            j++;
        }

        if(temp[0] != 0){
            add_entry(equation,Null,atof(temp));
        }

        if(is_operator(input[i])){
			j = 0;
            if(input[i] == '+'){
                tempOp = Add;
            }
            else if(input[i] == '-'){
                tempOp = Subtract;
            }
            else if(input[i] == '*'){
                tempOp = Multiply;
            }
            else if(input[i] == '/'){
                tempOp = Divide;
            }
            else if(input[i] == '('){
                tempOp = OpenParen;
            }
            else if(input[i] == ')'){
                tempOp = CloseParen;
            }
            else if(input[i] == '^'){
				tempOp = Power;
			}
			else if(input[i] == 's' && input[i+1] == 'i' && input[i+2] == 'n'){
				tempOp = Sine;
				i = i+2;
			}
			else if(input[i] == 'c' && input[i+1] == 'o' && input[i+2] == 's'){
				tempOp = Cosine;
				i = i+2;
			}
			else if(input[i] == 't' && input[i+1] == 'a' && input[i+2] == 'n'){
				tempOp = Tangent;
				i = i+2;
			}
			else if(input[i] == 'a' && input[i+1] == 's' && input[i+2] == 'i'
					&& input[i+3] == 'n'){
				tempOp = Arcsin;
				i = i+3;
			}
			else if(input[i] == 'a' && input[i+1] == 'c' && input[i+2] == 'o'
					&& input[i+3] == 's'){
				tempOp = Arccos;
				i = i+3;
			}
			else if(input[i] == 'a' && input[i+1] == 't' && input[i+2] == 'a'
					&& input[i+3] == 'n'){
				tempOp = Arctan;
				i = i+3;
			}
			else if(input[i] == 'e' && input[i+1] == 'x' && input[i+2] == 'p'){
				tempOp = Exp;
				i = i+2;
			}
			else if(input[i] == 'l' && input[i+1] == 'n'){
				tempOp = Ln;
				i = i+1;
			}
			else{
				tempOp = Blank;
			}
            add_entry(equation,tempOp,0.0);
        }
        i++;
    }

    return;

}

int is_operator(char input){
    int condition;
    if((input >= '(' && input <= '/') || (input >= 'A' && input <= 'z')){
        condition = 1;
    }
    else{
        condition = 0;
    }
    return condition;
}

int is_number(char input){
    int condition;
    if((input >= '0' && input <= '9') || input == '.'){
        condition = 1;
    }
    else{
        condition = 0;
    }
    return condition;
}

void evaluate_eqn(struct eqn * equation){
	//parens first
	//then unary operators
	//then power
	//then multiply divide
	//then add subtract
	struct eqn tempEq;
	struct eqn * temp_ptr = &tempEq;
	clear_eqn(temp_ptr);
	
    int md = 0;	//multiply or divide 	* /
    int as = 0; //add or subtract 		+ -
    int op = 0;	//open paren			(
    int cp = 0; //close paren			)
    int pwr = 0; //power operator 		^
    int i = 0;
    int unary = 0;
    int paren_begin = 0;
    int paren_end = 0;
    for(i = 0; i < equation->tree_ptr; i++){

        if(equation->tree[i].op_flag == 1){
            if(equation->tree[i].operator == Multiply || \
                equation->tree[i].operator == Divide){
                md++;
            }
            if(equation->tree[i].operator == Add || \
                equation->tree[i].operator == Subtract){
                as++;
            }
            if(equation->tree[i].operator == OpenParen){
				op++;
			}
			if(equation->tree[i].operator == CloseParen){
				cp++;
			}
			if(equation->tree[i].operator == Power){
				pwr++;
			}
			if(equation->tree[i].operator == Sine || 
			equation->tree[i].operator == Cosine || 
			equation->tree[i].operator == Tangent ||
			equation->tree[i].operator == Arcsin ||
			equation->tree[i].operator == Arccos ||
			equation->tree[i].operator == Arctan ||
			equation->tree[i].operator == Exp ||
			equation->tree[i].operator == Ln){
				unary++;
			}
        }
    }
    
	if(cp != op){
		printf("Missing parentheses\n");
		clear_eqn(equation);
		return;
	}

    i = 0;
    
    if(cp != 0){
		while(equation->tree[paren_end].operator != CloseParen){
			paren_end++;
		}
		
		paren_begin = paren_end;
		
		while(equation->tree[paren_begin].operator != OpenParen){
			paren_begin--;
		}
		
		equation->tree[paren_begin].data = 0.0;
		equation->tree[paren_begin].operator = Null;
		equation->tree[paren_begin].op_flag = 0;
		equation->tree[paren_begin].data_flag = 0;

		equation->tree[paren_end].data = 0.0;
		equation->tree[paren_end].operator = Null;
		equation->tree[paren_end].op_flag = 0;
		equation->tree[paren_end].data_flag = 0;
		
		for(i = paren_begin+1; i < paren_end; i++){
			tempEq.tree[i-paren_begin-1].data = equation->tree[i].data;
			tempEq.tree[i-paren_begin-1].operator = equation->tree[i].operator;
			tempEq.tree[i-paren_begin-1].op_flag = equation->tree[i].op_flag;
			tempEq.tree[i-paren_begin-1].data_flag = equation->tree[i].data_flag;
			equation->tree[i].data = 0.0;
			equation->tree[i].operator = Null;
			equation->tree[i].op_flag = 0;
			equation->tree[i].data_flag = 0;
			tempEq.tree_ptr++;
		}
		
		evaluate_eqn(temp_ptr);
		equation->tree[paren_begin].data = tempEq.tree[0].data;
		equation->tree[paren_begin].operator = tempEq.tree[0].operator;
		equation->tree[paren_begin].data_flag = tempEq.tree[0].data_flag;
		equation->tree[paren_begin].op_flag = tempEq.tree[0].op_flag;
		squish_eqn(equation);
		return;
		
	}

	i = 0;
	
	while(unary != 0){
		if(equation->tree[i].op_flag == 1){
			if(equation->tree[i].operator == Sine){
				equation->tree[i+1].data = sin(equation->tree[i+1].data);
				equation->tree[i].data_flag = 0;
                equation->tree[i].op_flag = 0;
				squish_eqn(equation);
				unary--;
				i--;
			}
			if(equation->tree[i].operator == Cosine){
				equation->tree[i+1].data = cos(equation->tree[i+1].data);
				equation->tree[i].data_flag = 0;
                equation->tree[i].op_flag = 0;
				squish_eqn(equation);
				unary--;
				i--;
			}
			if(equation->tree[i].operator == Tangent){
				equation->tree[i+1].data = tan(equation->tree[i+1].data);
				equation->tree[i].data_flag = 0;
                equation->tree[i].op_flag = 0;
				squish_eqn(equation);
				unary--;
				i--;
			}
			if(equation->tree[i].operator == Arcsin){
				equation->tree[i+1].data = asin(equation->tree[i+1].data);
				equation->tree[i].data_flag = 0;
                equation->tree[i].op_flag = 0;
				squish_eqn(equation);
				unary--;
				i--;
			}
			if(equation->tree[i].operator == Arccos){
				equation->tree[i+1].data = acos(equation->tree[i+1].data);
				equation->tree[i].data_flag = 0;
                equation->tree[i].op_flag = 0;
				squish_eqn(equation);
				unary--;
				i--;
			}
			if(equation->tree[i].operator == Arctan){
				equation->tree[i+1].data = atan(equation->tree[i+1].data);
				equation->tree[i].data_flag = 0;
                equation->tree[i].op_flag = 0;
				squish_eqn(equation);
				unary--;
				i--;
			}
			if(equation->tree[i].operator == Exp){
				equation->tree[i+1].data = exp(equation->tree[i+1].data);
				equation->tree[i].data_flag = 0;
                equation->tree[i].op_flag = 0;
				squish_eqn(equation);
				unary--;
				i--;
			}
			if(equation->tree[i].operator == Ln){
				equation->tree[i+1].data = log(equation->tree[i+1].data);
				equation->tree[i].data_flag = 0;
                equation->tree[i].op_flag = 0;
				squish_eqn(equation);
				unary--;
				i--;
			}
		}
		i++;
	}

	i = 0;
	while(pwr != 0){
		if(equation->tree[i].op_flag == 1){
            if(equation->tree[i].operator == Power){
                equation->tree[i-1].data = pow(equation->tree[i-1].data,equation->tree[i+1].data);
                equation->tree[i].data_flag = 0;
                equation->tree[i].op_flag = 0;
                equation->tree[i+1].data_flag = 0;
                equation->tree[i+1].op_flag = 0;
                squish_eqn(equation);
                pwr--;
                i--;
            }
        }
        i++;
	}
	i = 0;
    while(md != 0){
        if(equation->tree[i].op_flag == 1){
            if(equation->tree[i].operator == Multiply){
                equation->tree[i-1].data = equation->tree[i+1].data * equation->tree[i-1].data;
                equation->tree[i].data_flag = 0;
                equation->tree[i].op_flag = 0;
                equation->tree[i+1].data_flag = 0;
                equation->tree[i+1].op_flag = 0;
                squish_eqn(equation);
                md--;
                i--;
            }
            if(equation->tree[i].operator == Divide){
                equation->tree[i-1].data = equation->tree[i-1].data / equation->tree[i+1].data;
                equation->tree[i].data_flag = 0;
                equation->tree[i].op_flag = 0;
                equation->tree[i+1].data_flag = 0;
                equation->tree[i+1].op_flag = 0;
                squish_eqn(equation);
                md--;
                i--;
            }
        }
        i++;
    }

    i = 0;
    while(as != 0){
        if(equation->tree[i].op_flag == 1){
            if(equation->tree[i].operator == Add){
                equation->tree[i-1].data = equation->tree[i+1].data + equation->tree[i-1].data;
                equation->tree[i].data_flag = 0;
                equation->tree[i].op_flag = 0;
                equation->tree[i+1].data_flag = 0;
                equation->tree[i+1].op_flag = 0;
                squish_eqn(equation);
                as--;
                i--;
            }
            if(equation->tree[i].operator == Subtract){
				if(equation->tree[i-1].data_flag == 1){
					equation->tree[i-1].data = equation->tree[i-1].data - equation->tree[i+1].data;
					equation->tree[i].data_flag = 0;
					equation->tree[i].op_flag = 0;
					equation->tree[i+1].data_flag = 0;
					equation->tree[i+1].op_flag = 0;
					squish_eqn(equation);
					as--;
					i--;
				}
				else{
					equation->tree[i].data = 0.0 - equation->tree[i+1].data;
					equation->tree[i].data_flag = 1;
					equation->tree[i].operator = Null;
					equation->tree[i].op_flag = 0;	
					equation->tree[i+1].data_flag = 0;
					equation->tree[i+1].op_flag = 0;	
					squish_eqn(equation);
					as--;
					i--;			
				}
            }
        }
        i++;
    }

    return;

}

void swap_entry(struct entry * left, struct entry * right){
    //This function swaps the "left" and "right" entries

    struct entry temp;
    temp.operator = Null;
    temp.data = 0.0;
    temp.data_flag = 0;
    temp.op_flag = 0;

    temp.operator       = right->operator;
    temp.data           = right->data;
    temp.data_flag      = right->data_flag;
    temp.op_flag        = right->op_flag;

    right->operator     = left->operator;
    right->data         = left->data;
    right->data_flag    = left->data_flag;
    right->op_flag      = left->op_flag;

    left->operator      = temp.operator;
    left->data          = temp.data;
    left->data_flag     = temp.data_flag;
    left->op_flag       = temp.op_flag;

}

void squish_eqn(struct eqn * equation){
    //This function eliminates blank entries in an equation.
    //What it should do - count blank entries all the way
    //to the end of the eqn (as marked by tree_ptr).
    //Then copy the equation, skipping the blanks, to the temp
    //equation.

    struct eqn tempEq;
    int blanks = 0;
    int i;
    int j = 0;

    for(i = 0; i < equation->tree_ptr; i++){
        if(empty_entry(&(equation->tree[i]))){
            blanks++;
        }
    }
    //copy filled entries from equation to tempEq

    for(i = 0; i < equation->tree_ptr; i++){
        if(!empty_entry(&(equation->tree[i]))){
            tempEq.tree[j].data = equation->tree[i].data;
            tempEq.tree[j].operator = equation->tree[i].operator;
            tempEq.tree[j].data_flag = equation->tree[i].data_flag;
            tempEq.tree[j].op_flag = equation->tree[i].op_flag;
            j++;
        }
    }

    tempEq.tree_ptr = j;
    equation->tree_ptr = tempEq.tree_ptr;

    for(i = 0; i < tempEq.tree_ptr; i++){
        equation->tree[i].data = tempEq.tree[i].data;
        equation->tree[i].operator = tempEq.tree[i].operator;
        equation->tree[i].data_flag = tempEq.tree[i].data_flag;
        equation->tree[i].op_flag = tempEq.tree[i].op_flag;
    }

    //equation should now be a valid, contiguous math expression
    //up to tree_ptr;

}

int empty_entry(struct entry * ptr){
    //If the entry is empty, as evidenced by data_flag and
    //op_flag being false, return true. Otherwise, the entry
    //is useful.

    if(ptr->data_flag == 0 && ptr->op_flag == 0){
        return 1;
    }
    else{
        return 0;
    }
}

void print_eqn(struct eqn * equation){
    int i;
    for(i = 0; i < equation->tree_ptr; i++){
        if(equation->tree[i].data_flag){
			if(equation->tree[i].data < 0.0000001){
				printf("%e",equation->tree[i].data);
			}
			else{
				printf("%f",equation->tree[i].data);
			}
        }
        else if(equation->tree[i].op_flag){
            if(equation->tree[i].operator == Add){
                printf("+");
            }
            if(equation->tree[i].operator == Subtract){
                printf("-");
            }
            if(equation->tree[i].operator == Multiply){
                printf("*");
            }
            if(equation->tree[i].operator == Divide){
                printf("/");
            }
            if(equation->tree[i].operator == OpenParen){
                printf("(");
            }
            if(equation->tree[i].operator == CloseParen){
                printf(")");
            }
            if(equation->tree[i].operator == Power){
				printf("^");
			}
			if(equation->tree[i].operator == Sine){
				printf("sin");
			}
        }
        else{
            printf("_");
        }
    }
    printf("\n");
}

void clear_eqn(struct eqn * equation){
    int i;
    equation->tree_ptr = 0;
    for(i = 0; i < DEPTH; i++){
        equation->tree[i].operator = Null;
        equation->tree[i].data = 0.0;
        equation->tree[i].data_flag = 0;
        equation->tree[i].op_flag = 0;
    }
}

void strip_whitespace(char * input){
    int i = 0;
    int j = 0;
    char newstring[INPUT_DEPTH];
    int length = 0;
    while(input[length] != '\0'){
        length++;
    }
    //at this point, length whill be the place in input where
    //the null terminator is

    for(i = 0; i <= length; i++){
        if(input[i] == ' ' || input[i] == '\r' || input[i] == '\n'){

        }
        else{
            newstring[j] = input[i];
            j++;
        }
    }
    for(i = 0; i < j; i++){
        input[i] = newstring[i];
    }
    return;
}
