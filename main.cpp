/* include */
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <cstring>

/* Character classes */

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
#define EOE -1


/* Token codes */
#define SPACE 9
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25 // (
#define RIGHT_PAREN 26 // )
#define POWER 27 // ^


using namespace std;

struct Lex{
        int token;
        string lexeme;
        };

/* Function declarations */
    int getChar(char nextLex);
    char* getNonBlank(char* expression);
    int lookup(char expressionSansEspace);
    int lexe(char* expressionSansEspace, Lex* lex);
    int expr(int lexOrder,Lex* lex);
    int term(int lexOrder,Lex* lex);
    int factor(int lexOrder,Lex* lex);
    void display(int lexOrder,Lex* lex);



/*------------------------------------------------
Analyseur lexical traduit
une séquence de caractères
(le programme source) en une séquence
de tokens ( via l'annalyse des lexèmes)
--------------------------------------------------*/


using namespace std;


/*************************MAIN*************************/
int main()
{
/* DECLARATION */
    /* VARIABLES */
    char* expression;

/* FIN DECLARATION*/
        cout << "ALBAN BERTOLINI"<< endl;
    /* Expression */
        expression = "2^n";
        expression = getNonBlank(expression); //Expression sans les espaces ! A utiliser pour le lecteur syntaxique

    /* Supprimer les espaces de la chaine de caractère , retourne la chaine sans espace */

    if (expression[0]=='\0'){
        cout <<"Error - Your expression is null"<<endl;
    }
    else{
        /* Déterminer les lexemes et les tokens de l'expression */
        Lex* lex = new Lex[100]; // Déclaration de lex ( pointeur d'un tableau de lex de taille 100)
        int lexSize = lexe(expression, lex);

        int a;
        a = lexSize;
        lexSize = 0;
        /*Affichage*/
        while(lexSize <= a)
        {
            if(lex[lexSize].token != 9)
            {
                display(lexSize,lex);
            }
            else{
                /*Affichage des espaces*/
                cout << "* : " ;
                cout << lexSize<< endl;
            }
            lexSize++;
        }


        /*****************Analyseur syntaxique*************************/
        int lexOrder=0;
        cout << "-------------Start Syntax Analysis --------------"<< endl;
        display(lexOrder,lex);
        expr(lexOrder,lex);
        delete[] lex;
    }
    return 0;
}// end of main

/* function dislay permet */
void display(int lexOrder,Lex* lex)
{
    cout << " Next token is : ";
    cout << lex[lexOrder].token;
    cout << " And next lexeme is : ";
    cout << lex[lexOrder].lexeme << endl;
}
/***********************************************************************/
/* function expr */
/** expr -> term +|- term */
int expr(int lexOrder,Lex* lex){

    cout << "Enter <expr>" <<endl;

    /* parse the first term */
    lexOrder = term(lexOrder,lex);
    int nextToken = lex[lexOrder].token;

    while (nextToken == ADD_OP || nextToken == SUB_OP)
    {
        lexOrder +=1;
        nextToken = lex[lexOrder].token;
        display(lexOrder,lex);
        lexOrder = term(lexOrder,lex);
    }
    cout <<"Exit <expr>"<< endl;
    return lexOrder;
}/*enf of fuction expr*/

/* function term */
/** term -> factor *|/ factor */
int term(int lexOrder,Lex* lex){
    cout << "Enter <term>" <<endl;
    /* Parse the first factor */
    lexOrder = factor(lexOrder,lex);
    int nextToken = lex[lexOrder].token;

    while(nextToken == MULT_OP || nextToken == DIV_OP || nextToken == POWER){
        lexOrder +=1;
        nextToken = lex[lexOrder].token;
        display(lexOrder,lex);
        lexOrder = factor(lexOrder,lex);
    }
    cout << "Exit <term>" << endl;
    return lexOrder;
}/* end of fuction term*/

/* function factor */
/** factor -> id | constant | expr */
int  factor(int lexOrder,Lex* lex){
    cout << "Enter <factor>" <<endl;
    int nextToken = lex[lexOrder].token;
    /*derterminate which RHS */
    if (nextToken == IDENT || nextToken ==INT_LIT){
        lexOrder +=1;
        nextToken = lex[lexOrder].token;
        display(lexOrder,lex);
    }
    else{
        if(nextToken == LEFT_PAREN){
            lexOrder +=1;
            nextToken = lex[lexOrder].token;
            display(lexOrder,lex);
            lexOrder = expr(lexOrder,lex);
            nextToken = lex[lexOrder].token;

            if(nextToken == RIGHT_PAREN){
                lexOrder +=1;
                nextToken = lex[lexOrder].token;
                display(lexOrder,lex);
            }
            else{
                cout << "Error"<<endl;
                cout << "Erreur avec: "<<endl;
                display(lexOrder,lex);
            }
        }
        else{
            cout << "Error"<<endl;
            cout << "Erreur avec : "<<endl;
            display(lexOrder,lex);
        }
    }
    cout << "Exit <factor>"<< endl;
    return lexOrder;
}

/**********************************************************/
/* function lexe */
int lexe(char* expressionSansEspace, Lex* lex){
        /*Déclaration variables*/
        int a = 0;
        string nextLexSauvegarde;
        int order = 0;
        char nextLexeme;
        int charClass;
        int nextToken;

        while(charClass!=EOF){
            nextLexeme = expressionSansEspace[a];
            charClass = getChar(nextLexeme);
                switch(charClass)
                {
                    case SPACE :
                    {
                        nextToken = SPACE;
                        lex[order].token = nextToken;
                        lex[order].lexeme = "Espace";
                        order +=1;
                        a++;
                        break;
                    }
                    /* Séparer les caractères UNKNOWN et les caractères DIGIT/LETTER */
                    case UNKNOWN :
                    {
                        /* Enregistrement du lexeme UNKNOWN */
                        nextToken = lookup(nextLexeme);
                        lex[order].token = nextToken;
                        lex[order].lexeme = nextLexeme;
                        order +=1;
                        a++;

                        break;
                    }

                    case LETTER :
                    {
                        while(charClass == LETTER || charClass == DIGIT){
                            nextLexeme = expressionSansEspace[a];
                            charClass = getChar(nextLexeme);
                            if(charClass != UNKNOWN && charClass !=SPACE)
                            {
                                nextToken = lookup(nextLexeme);
                                nextLexSauvegarde += nextLexeme;
                                a++;
                            }
                        }
                        nextToken = IDENT;
                        lex[order].token = nextToken;
                        lex[order].lexeme = nextLexSauvegarde;
                        nextLexSauvegarde.clear();// réinitialisation du lex de sauvegarde
                        order +=1;
                        break;
                    }

                    case DIGIT :
                    {
                        while(charClass == DIGIT){
                            nextLexeme = expressionSansEspace[a];
                            charClass = getChar(nextLexeme);
                            if(charClass != UNKNOWN && charClass !=SPACE)
                            {
                                nextToken = lookup(nextLexeme);
                                nextLexSauvegarde += nextLexeme;
                                a++;
                            }
                        }
                        nextToken = INT_LIT;
                        lex[order].token = nextToken;
                        lex[order].lexeme = nextLexSauvegarde;
                        nextLexSauvegarde.clear();// réinitialisation du lex de sauvegarde
                        order +=1;
                        break;
                    }
                }
            }

            /* Enregistrement du lexeme EOE */
            nextToken = EOE;
            lex[order].token = nextToken;
            lex[order].lexeme = "END";

            return order;
}// end of lexe


/*****************************************************************************************/
/* function getChar */
int getChar(char nextLexeme){
int charClass;
    if((nextLexeme) != '\0'){
        if(isspace(nextLexeme)){
        charClass = SPACE;
        }
        else if(isalpha(nextLexeme)){
        charClass = LETTER;
        }
        else if(isdigit(nextLexeme)) {
            charClass = DIGIT;
        }
        else{
            charClass = UNKNOWN;
        }
    }
    else{
        charClass = EOE; // end of expression
    }
 return charClass;
}

/**********************************************************/
/* getNonBlank */

 char* getNonBlank(char* expression){
    char c;
    int a = 0;
    int i = 0;
    int taille; // taille du tableau sans espace
    int compteur = 0;// compte le nombre d'espace
    char *expressionSansEspace;

    /* Compter le nombre d'espace dans la chaine de caractère expression */
    while (expression[i])
    {
        c=expression[i];
        if (isspace(c))
        {
        compteur +=1;
        }
        i++;
    }

    taille =  i - compteur;

    expressionSansEspace = new char[taille]; // Déclarer la taille du tableau de char sans espace
    i = 0;
    /* Insérer le tableau expression sans les espaces dans le tableauSansEspace */
    while(expression[i])
    {
       if (!isspace(expression[i]))
       {
        expressionSansEspace[a] = expression[i];
        a++;
       }
       i++;
    }
    return expressionSansEspace;
}// end of getChar

/********************************************************************/
/* function lookup */

int lookup(char expressionSansEspace){
int nextToken;
switch(expressionSansEspace){
    case '(':
        nextToken = LEFT_PAREN;
        break;
    case ')':
        nextToken = RIGHT_PAREN;
        break;
    case '+':
        nextToken = ADD_OP;
        break;
    case '-':
        nextToken = SUB_OP;
        break;
    case '*':
        nextToken = MULT_OP;
        break;
    case '/':
        nextToken = DIV_OP;
        break;
    case '^':
        nextToken = POWER;
        break;
    default:
        nextToken = '\0';
        break;
    }
    return nextToken;
}// enf of lookup



