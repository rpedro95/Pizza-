/* 
 * File:   main.c
 * Programa desenvolvido por: Pedro Ribeiro e André Nogueira
 *
 * Data de inicio - 15 de Dezembro de 2016, 10:48
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



//Estrutura referente aos ingredientes, sendo que vai guardar um número identificador único de cada ingrediente, o nome do ingrediente, e
//a quantidade do ingrediente no stock
 
typedef struct ingrediente{
  int id_ingrediente; 			//Identificador único do ingrediente para facilitar as pesquisas de ingredientes, ou seja em vez de armazenarmos todas as informações referentes ao igrediente, basta armazenar este número
  char nome[100]; 				//Nome do ingrediente , como por exemplo Bacon
  int quantidade;				//quantidade que existe no armazem, este valor vai sendo constantemente atualizado a medida de cada venda de pizza.Esta váriavel vai permitir gerir o stock, e informar o vendedor se temos os ingredientes suficientes para fazer as pizzas
}INGREDIENTE;

//Estrutura referente aos clientes, sendo que vai guardar um número identificador único de cada cliente, o seu nome, a sua morada,o seu NIF, o seu telemóvel e o seu endereço eletronico

typedef struct cliente{
    int id_cliente; 			//Identificador único do cliente para facilitar as pesquisas/ligações dos clientes , para poupar espaço
    char nome[250]; 			//Nome do cliente, como por exemplo Justina
    char morada[250]; 			//Morada do cliente , como por exemplo: Lote 3, Rua do Tojal Real Martinho 4700-139 Braga
    char nif[20]; 				//NIF do cliente, como por exemplo: 500001758 
    char telemovel[20];			//Número do telemóvel como por exemplo: 938432721
    char mail[100];			    //Endereço eletrónico referente ao utilizador como por exemplo: zecaAntonio@bragaganhou.com
}CLIENTE;

//Estrutura referente as pizzas, sendo que vai guardar um número identificador único de cada pizza, o seu nome, o seu preço,
//um array bi-dimensional que vai armazenar os números que identificam os ingredientes e a respetiva quantidade desses ingredientes que foi usada na pizza 

typedef struct pizza{
    int id_pizza; 			//Identificador único da pizza para facilitar as pesquisas/ligações das pizzas, de forma a apenas passar o identificador da pizza
    char nome[100];			//Nome da pizza , como por exemplo Napolitana
    float preco; 			//Preço da pizza
    int totalIng;			//vai dizer quantos ingredientes temos no programa
    int estado; 			//Por  defeito a pizza esta disponivel, se for 0, quer dizer que n se pode vender
    int ing[12][2];			//uma pizza pode ter ate 12 ingredientes, e nos vamos guardar o ID desses ingredientes na coluna [0] e a quantidade dos mesmos ingredientes encomendados na coluna[1]
}PIZZA; 

//Estrutura referente ao pedido efetuado pelo utilizador, sendo que vai guardar um número identificador único de cada pedido

typedef struct pedido{
    
    int id_pedido;									//Identificador único do pedido para facilitar as pesquisas referentes aos pedidos
    int id_cliente; 								//Esta variavél vai armazenar o número identificador do cliente que fez a encomenda
    float valorPagar;								//Esta variavél vai guardar o valor total da encomenda, optamos por guardar este valor assim para evitar fazer o processamento do valor total a pagar cada vez que fossemos a ter que o listar, optando por ocupar um bocado mais de espaço para poupar poder de processamento
    int metodoPagamento;							//Esta variavél vai guardar o método de pagamento escolhido pelo utilizador, caso o valor seja 1 quer dizer que o cliente pediu para pagar por Multibanco, se o valor for 2 significa que o cliente vai pagar com dinheiro monetário  
    int metodoEncomenda;							//Esta variavél vai guardar o método de encomenda utilizado pelo utilizador, caso o valor seja 1 quer dizer que o cliente fez a encomenda via telefónica, se o valor for 2 significa que o cliente fez a encomenda via internet[email] 
    int totalPizzas ; 								//quantidade de pizzas a serem encomendadas
    int estado; 									//por defeito nada esta entregue
    int pizzas[15][3];
       
	    /* Para 15 pizzas que é o máximo que se pode encomendar:
     Na primeira coluna vamos  armazenar os números identificadores da pizza de maneira a não estarmos a armazenar toda a informação referente a pizza dentro desta struct, apenas usando o número da pizza
      Na segunda coluna guardamos o preco referente a cada pizza , para evitar que ao ser alterado o preço ao longo do tempo, o histórico de compras fique com informações erradas
   	Na terceira coluna guardamos a quantidade dos ingredientes da pizza..?
   OUTDATED:
    Na terceira coluna guardamos a quantidade de pizzas associadas a tal pizza que foi encomendada porque ele pode encomedar tipo 2 napolitanas
   
      */
    
    /*WARNING: DANGER
     Exemplo: Cada Pizza tem no máximo 12 ingredientes , e cada um destes ingredientes armazena 2 números 
     ou seja 15 -> 12 -> 2
     */	
     
    int pizzasEncomendadasIngredientes[15][12][2];	//Este array tri-dimensional vai armazenar para cada uma das pizzas, e para cada um dos 12 ingredientes, 2 valores númericos, sendo o primeiro o identificador do ingrediente e o segundo a quantidade desse ingrediente utilizada na respectiva pizza

}PEDIDO;

void escreveLinha(){
	printf("*******************************************************************\n");
}

float checkPreco(char preco[100]){  //recebe um array de caracteres
	int i=0,contaNaoDigito=0,contaVP=0;
	float ref=0;
	for(i=0;i<strlen(preco);i++){ //percorre esse array
		if(!isdigit(preco[i]) && preco[i] != '.' && preco[i] != ','){ // se nao for um número, nao tiver pontos e nao tiver vírgulas
			contaNaoDigito++; //sabemos que nao é digito, logo nao pode ser um preço e nao faz nada
		}
			if(preco[i] == '.' || preco[i] == ','){ //se o caracter analisado for vírgula ou ponto
				contaVP++; //incrementamos a variavel contaVP
				preco[i]= '.'; //e mudamos o caracter para ponto (para ficar sempre pontoo)
			}
	}
	
	if(contaNaoDigito != 0 || contaVP > 1 ){ //Caso o contador de erros seja diferente de 0 ou exista mais do que um ponto ou vírgula
		return -2; //dá erro porque isto não é um preço válido
	}else{ // se continuar vamos converter isto para um float
		ref = strtof(preco,NULL) ; 
			if(ref > 0){ //se for um preco válido
				return ref; //retorna o preço
			}else{
				return -2; //se for inválido retorna -2
			}
	}
}

int checkDesconto(float desconto){
	if (desconto<=(-100) || desconto >=100){
		printf("Desconto Invalido!\n");
		return -2;
	}
	else{
		return 1;
	}
}

int checkInteiro(char numero[100]){ 
	int i=0,contaNaoDigito=0,num=0;
	
	if(strlen(numero) < 1){
		system("cls");
		return -2;
	}
	for(i=0;i<strlen(numero);i++){//Percorrer o array de caracteres
		if(!isdigit(numero[i])){//se nao for digito
			contaNaoDigito++; //incrementa a variavel contaNaoDigito, ou seja, não é um interio
		}
	}
	if(contaNaoDigito != 0 ){
		return -2; //deu erro porque isto nao � inteiro
	}else{ // se continuar vamos converter isto para um int
		num = strtol(numero,NULL, 10);	 
			return num;
		}
	}

int checkInteiroNegativo(char numero[100]){ 
	int i=0,contaNaoDigito=0,num=0;
	
	if(strlen(numero) < 1){
		return -765412;
	}
	for(i=0;i<strlen(numero);i++){//Percorrer o array de caracteres
		if(!isdigit(numero[i])){//se nao for digito
			contaNaoDigito++; //incrementa a variavel contaNaoDigito, ou seja, não é um interio
		}
	}
	if(numero[0] == '-' && strlen(numero) > 1 && contaNaoDigito == 1){
		num = strtol(numero,NULL, 10);	 
			return num;
	}
	if(contaNaoDigito != 0 ){
		return -765412; //deu erro porque isto n é inteiro
	}else{ // se continuar vamos converter isto para um int
		num = strtol(numero,NULL, 10);	 
			return num;
		}
	}

void checkNIF (CLIENTE array_Clientes[], int idGlobalClientes){
	int i, valido=-1;
	char texto[50];
	
	do{
		valido=1;
		printf("Introduza o NIF do cliente:\n");
		gets(texto);
		if (strlen(texto)!=9){
			valido=0;	
		}	
		
		for(i=0; i<strlen(texto); i++){
			if(!isdigit(texto[i])){
				valido=2;
			}
		}
			
		for(i=0; i<idGlobalClientes;i++){
			if(stricmp(texto, array_Clientes[i].nif)==0){
				valido=3;
			}  
		}
			
		if(valido==3){
			escreveLinha();
			printf("Este NIF ja existe nos nossos registos.\n");
			escreveLinha();
			printf("\n\n");
		}else{
			if(valido==2){
				escreveLinha();
				printf("O NIF so pode conter digitos.\n");
				escreveLinha();
				printf("\n\n");
			} else{
				if (valido==0){
					escreveLinha();
					printf ("Erro de NIF. O NIF deve conter 9 digitos.\n");
					escreveLinha();
					printf("\n\n");
				}
			}
		}
	}while (valido!=1); 
	strcpy(array_Clientes[idGlobalClientes].nif, texto); 
}

void checkTelemovel(CLIENTE array_Clientes[], int idGlobalClientes){
	int i, valido=-1;
	char texto[50];
	
	do{
		valido=1;
		printf("Introduza o Telemovel do cliente:\n");
		gets(texto);
		
		if (strlen(texto)!=9){
			valido=0;	
		}	
		
		for(i=0; i<strlen(texto); i++){
			if(!isdigit(texto[i])){
				valido=2;
			}
		}
			
		for(i=0; i<idGlobalClientes;i++){
			if(stricmp(texto,array_Clientes[i].telemovel)==0){
				valido=3;
			}
		}
		
	if(valido==3){
			escreveLinha();
			printf("Este Telemovel ja existe nos nossos registos.\n");
			escreveLinha();
			printf("\n\n");
		}else{
			if(valido==2){
				escreveLinha();
				printf("O Telemovel so pode conter digitos.\n");
				escreveLinha();
				printf("\n\n");
			} else{
				if (valido==0){
					escreveLinha();
					printf ("O Telemovel deve conter 9 digitos.\n");
					escreveLinha();
					printf("\n\n");
				}
			}
		}
			
	}while (valido!=1); 
	strcpy(array_Clientes[idGlobalClientes].telemovel, texto);  
}


int menu(){
    char opc[50];
    int qtdLida;
    int num=-1;
    while(num != 1 && num != 2 && num != 0){
    do{
	printf ("MENU PRINCIPAL\n");
    escreveLinha();
    printf("1 - Gerir Pizzaria\n");
    printf("2 - Fazer encomenda\n"); 
    printf("0 - Sair\n");
    }while((num = checkInteiro(gets(opc))) == -2);//enquanto nao meter um numero
    }
    return num;
    } 

int menuAdmin(){
    char opcAdmin[50];
    int num=-1;
    do{
    printf ("MENU GERIR PIZZARIA\n");
    escreveLinha();
    printf("1 - Gerir Clientes\n");
    printf("2 - Gerir Pizzas\n");
	printf("3 - Gerir Ingredientes\n");
	printf("0 - Voltar ao menu anterior\n");
    num = checkInteiro(gets(opcAdmin));
    }while(num  == -2 && num !=0);//enquanto nao meter um numero
    return num;
    } 

int menuCliente(){
    char opcCliente[50];
    int num=-1;
    do{
    printf ("MENU GERIR CLIENTE\n");
    escreveLinha();
    printf("1 - Inserir dados de um cliente\n");
    printf("2 - Listar todos os clientes\n");
    printf("3 - Listar informacoes sobre os clientes que pediram uma determinada pizza\n");
    printf("4 - Listar informacao sobre o(s) cliente(s) que efetuou o pedido mais caro\n");
    printf("0 - Voltar ao menu anterior\n");
	num = checkInteiro(gets(opcCliente));
    }while(num  == -2 && num !=0);//enquanto nao meter um numero
    return num;
    } 

int menuPizza(){
    char opcPizza[50];
    int num=-1;
    do{
   	printf ("MENU GERIR PIZZAS\n");
    escreveLinha();
   	printf("1 - Acrescentar uma pizza ao cardapio\n");
    printf("2 - Listar informacao sobre todas as pizas (incluindo a sua disponibilidade)\n");
    printf("3 - Alterar disponibilidade de uma pizza\n");
    printf("4 - Listar informacoes sobre a pizza com maior preco\n");
	printf("5 - Atualizar todos os precos das pizzas\n");
    printf("6 - Atualizar os precos das pizzas com um determinado ingrediente\n");
    printf("7 - Saber o valor total recebido pela pizzaria ate ao momento\n");
    printf("8 - Listar informacoes sobre a(s) pizza(s) mais vendida(s)\n");
	printf("0 - Voltar ao menu anterior\n");
    num = checkInteiro(gets(opcPizza));
    }while(num  == -2 && num !=0);//enquanto nao meter um numero
    return num;
    } 


int menuEncomendas(){
    char opcEncomendas[50];
    int idGlobal, idPizza, idCliente;
    int num=-1;
    while(num != 1 && num != 2 && num != 3 && num != 4 && num != 0){
    do{
    printf ("MENU GERIR ENCOMENDAS\n");
    escreveLinha();
    printf("1 - Receber Encomenda\n");
    printf("2 - Estado da Encomenda\n");
    printf("3 - Listar informacoes sobre todos os pedidos\n");
    printf("4 - Listar informacoes sobre os pedidos pendentes de entrega\n");
    printf("0 - Voltar ao menu anterior\n");
    }while((num = checkInteiro(gets(opcEncomendas))) == -2);//enquanto nao meter um numero
    }
    return num;
    } 


int inserirCliente(CLIENTE array_Clientes[], int idGlobalClientes){
	
	int id=idGlobalClientes+1;
	int nif=0;
	int telefone;
 	
	checkNIF(array_Clientes, idGlobalClientes); 
 	
 	checkTelemovel(array_Clientes, idGlobalClientes); 
 	
	printf ("Introduza o nome do cliente:\n");
	gets (array_Clientes[idGlobalClientes].nome);
	

	printf ("Introduza a morada do cliente:\n");
	gets (array_Clientes[idGlobalClientes].morada);

	
	printf ("Introduza o e-mail do cliente:\n");
	gets(array_Clientes[idGlobalClientes].mail);	
	
	array_Clientes[idGlobalClientes].id_cliente=id;
 
 return 1;
}

void listarTodosClientes(CLIENTE array_Clientes[], int idGlobalClientes){
	int i=0;
	
	for(i=0; i<idGlobalClientes; i++){
		escreveLinha();
		printf("NUMERO DE CLIENTE: %i\n",array_Clientes[i].id_cliente);
		printf("\tNIF: "); puts(array_Clientes[i].nif);
		printf("\tNome: "); puts(array_Clientes[i].nome);
		printf("\tMorada: "); puts(array_Clientes[i].morada);
		printf("\tTelemovel: "); puts(array_Clientes[i].telemovel);
		printf("\tMail: "); puts(array_Clientes[i].mail);
		escreveLinha();
	}
};

int getCliente(CLIENTE array_Clientes[], int idGlobalClientes,char opcCliente[]){
	int i; 
	int contador=0;
	int idCliente;
	char texto[200];
	int opcClienteNum = checkInteiro(opcCliente);
	switch(opcClienteNum){
  		case 1: //caso queira procurar por NIF
					do{ 
		printf("Introduza o NIF['sair' para sair]:\n");
		gets(texto);
		if(stricmp(texto,"sair") == 0){
			return -1;
		}
		
		for(i=0; i<idGlobalClientes; i++){
			if(stricmp(texto,array_Clientes[i].nif) == 0 ){ 
			escreveLinha();
			printf("NUMERO DE CLIENTE: %i\n",array_Clientes[i].id_cliente);
			printf("\tNIF: "); puts(array_Clientes[i].nif);
			printf("\tNome: "); puts(array_Clientes[i].nome);
			printf("\tMorada: "); puts(array_Clientes[i].morada);
			printf("\tTelemovel: "); puts(array_Clientes[i].telemovel);
			printf("\tMail: "); puts(array_Clientes[i].mail);
			idCliente = array_Clientes[i].id_cliente ;
			escreveLinha();
			contador++;
			}
			
		}
			if(contador == 0){
				printf("Nenhum cliente com esse NIF encontrado!\n");
			}
			
				}while(contador == 0); 
			
		 if(contador==1){
				return idCliente;
			} 
			break;
		case 2: //caso queira procurar por Telemovel
						do{ 
		printf("Introduza o telemovel['sair' para sair]:\n");
		gets(texto);
			if(stricmp(texto,"sair") == 0){
			return -1;
		}
		for(i=0; i<idGlobalClientes; i++){
			if(stricmp(texto,array_Clientes[i].telemovel) == 0 ){ 
			escreveLinha();
			printf("NUMERO DE CLIENTE: %i\n",array_Clientes[i].id_cliente);
			printf("\tNIF: "); puts(array_Clientes[i].nif);
			printf("\tNome: "); puts(array_Clientes[i].nome);
			printf("\tMorada: "); puts(array_Clientes[i].morada);
			printf("\tTelemovel: "); puts(array_Clientes[i].telemovel);
			printf("\tMail: "); puts(array_Clientes[i].mail);
			idCliente = array_Clientes[i].id_cliente ;
			escreveLinha();
			contador++;
			}
		}
			if(contador == 0){
				printf("Nenhum cliente com esse Telemovel encontrado!\n");
			}
			
				}while(contador == 0); 
			
		 if(contador==1){
				return idCliente;
			} 
			break; 
		case 3://caso queira procurar por Mail
						do{ 
		printf("Introduza o Mail['sair' para sair]:\n");
		gets(texto);
			if(stricmp(texto,"sair") == 0){
			return -1;
		}
		for(i=0; i<idGlobalClientes; i++){
			if(stricmp(texto,array_Clientes[i].mail) == 0 ){ 
			escreveLinha();
			printf("NUMERO DE CLIENTE: %i\n",array_Clientes[i].id_cliente);
			printf("\tNIF: "); puts(array_Clientes[i].nif);
			printf("\tNome: "); puts(array_Clientes[i].nome);
			printf("\tMorada: "); puts(array_Clientes[i].morada);
			printf("\tTelemovel: "); puts(array_Clientes[i].telemovel);
			printf("\tMail: "); puts(array_Clientes[i].mail);
			idCliente = array_Clientes[i].id_cliente ;
			escreveLinha();
			contador++;
			}
		}
			if(contador == 0){
				printf("Nenhum cliente com esse Mail encontrado!\n");
			}
			
				}while(contador == 0); 
			
		 if(contador==1){
				return idCliente;
			} 
			break;
	}
}
int menuIngredientes(){
    char opcIngredientes[50];
    int qtdLida;
    int num=-1;
    while(num != 1 && num != 2 && num != 3 && num != 4 && num != 5 && num != 6 && num != 0){
    do{
    printf ("MENU GERIR INGREDIENTES\n");
    escreveLinha();
    printf("1 - Criar Ingrediente\n");
    printf("2 - Listar todos os Ingredientes\n");
    printf("3 - Listar um ingrediente dado o seu nome\n");
    printf("4 - Listar ingredientes com o menor stock\n");
    printf("5 - Listar ingredientes sem stock\n");
    printf("6 - Alterar Stock de Ingrediente\n");
    printf("0 - Voltar ao menu anterior\n");
    num = checkInteiro(gets(opcIngredientes));
    }while(num  == -2 && num !=0);//enquanto nao meter um numero
    }
    return num;
    }

int checkIngrediente(INGREDIENTE array_Ingredientes[250],int idGlobalIngredientes,char ingredienteNovo[100]){
    //Vamos ver se o ingrediente já existe
    int i = 0;
    for(i=0;i<=idGlobalIngredientes-1;i++){
        if(stricmp(array_Ingredientes[i].nome,ingredienteNovo) == 0){ //depois meter o stricmpi no PC do ANDRE
             return array_Ingredientes[i].id_ingrediente;
          //isto quer dizer que ja existe
        }
        
        
} return -1;//isto quer dizer que não existe
}


int checkPizza(PIZZA array_Pizzas[250],int idGlobalPizzas,char pizzaNova[100]){
    //Vamos ver se a pizza já existe
    int i = 0;
    for(i=0;i<=idGlobalPizzas-1;i++){
        if(stricmp(array_Pizzas[i].nome,pizzaNova) == 0){ //depois meter o stricmpi no PC do ANDRE
             return array_Pizzas[i].id_pizza;
          //isto quer dizer que ja existe
        }
        
        
} return -1;//isto quer dizer que não existe
}



int addIngrediente(INGREDIENTE array_Ingredientes[250],int idGlobalIngredientes){
	int qtd;
	char quantidadeIngrediente[50];
    int jaExiste = -1;
    
    //Vamos verificar se o limite dos Ingredientes do programa ainda não foi atngido
    if(idGlobalIngredientes == 250){//Quer dizer que atinjimos o limite de ingredientes que podemos armazenar no nosso programa
        return 0; //ou seja não incrementamos nada e saiamos do programa
    }
    array_Ingredientes[idGlobalIngredientes].id_ingrediente = idGlobalIngredientes + 1;
    do{
    	printf("Introduza o nome do ingrediente [Escreva 'sair' para sair]:\n");
    	gets(array_Ingredientes[idGlobalIngredientes].nome); 
    	if(stricmp(array_Ingredientes[idGlobalIngredientes].nome,"sair") == 0){
        	return 0; //ou seja ele ja n quer nada disto
    	}
    	jaExiste = checkIngrediente(array_Ingredientes,idGlobalIngredientes,array_Ingredientes[idGlobalIngredientes].nome);
    	if(jaExiste != -1){
        	printf("Ja existe o ingrediente inserido.\n");
    	}
    }while(strlen(array_Ingredientes[idGlobalIngredientes].nome) == 0 || jaExiste != -1);
    printf("Introduza a quantidade de %s em stock:\n",array_Ingredientes[idGlobalIngredientes].nome);
   
    while((qtd = checkInteiro(gets(quantidadeIngrediente))) == -2){//enquanto n introduzir uma quantidade valida
    	printf("Por favor introduza uma quantidade:\n");
	}
    array_Ingredientes[idGlobalIngredientes].quantidade = qtd; 
    return 1; 
}

void listarIngredientes(INGREDIENTE array_Ingredientes[250],int idGlobalIngredientes){
    int i;
	
	printf("Listagem de todos os Ingredientes:\n\n\n");
    for(i=0;i<idGlobalIngredientes;i++){
    	escreveLinha();
    	puts(array_Ingredientes[i].nome);
    	printf("Quantidade em stock: %d\n", array_Ingredientes[i].quantidade);
    	escreveLinha();
    } 
}

void listarIngrediente(INGREDIENTE array_Ingredientes[250],int idIngrediente){
    
	printf("Listagem do Ingrediente:\n\n\n");
    idIngrediente--;//porque vai estar sempre uma casa atras
	escreveLinha(); 
    puts(array_Ingredientes[idIngrediente].nome);
    printf("Quantidade em stock: %d\n", array_Ingredientes[idIngrediente].quantidade);
	escreveLinha();
	}

void listarIngredientesSemStock(INGREDIENTE array_Ingredientes[250],int idGlobalIngredientes){      
    int i;
    
	for(i=0;i<idGlobalIngredientes;i++){
    	if(array_Ingredientes[i].quantidade == 0){
    		escreveLinha();
    		puts(array_Ingredientes[i].nome); 
    		escreveLinha();
    	} 
	}
}

void listarIngredientesMenorStock(INGREDIENTE array_Ingredientes[250],int idGlobalIngredientes){
      
    int i,menor=50000;
    //Vamos ver qual é a quantidade mais baixa no stock
    
	for(i=0;i<idGlobalIngredientes;i++){
    	if(array_Ingredientes[i].quantidade < menor && array_Ingredientes[i].quantidade != 0){
    	menor = array_Ingredientes[i].quantidade;
    	}
	}
    //E agora imprimimos essa cena
    for(i=0;i<idGlobalIngredientes;i++){
       	if(array_Ingredientes[i].quantidade == menor){
       		escreveLinha();
            puts(array_Ingredientes[i].nome); 
            printf("Quantidade em stock: %d\n", array_Ingredientes[i].quantidade);
            escreveLinha();
		}
	}
}

void alterarQuantidadeIngrediente(INGREDIENTE array_Ingredientes[250],int idGlobalIngredientes){
	
	char ingredienteAProcurar[100];
	int idIngredienteAListar;
	char quantidadeIngrediente[100];
	int qtd;
						 
	

           do{
    		printf("Introduza o ingrediente cuja quantidade deseja alterar:\n");
    		gets(ingredienteAProcurar);
		} while((idIngredienteAListar = checkIngrediente(array_Ingredientes,idGlobalIngredientes,ingredienteAProcurar)) == -1);//enquanto n introduzir um ingrediente que existe
	 
    if(idIngredienteAListar != -1){//aqui vamos perguntar se quer mudar o stock diretamente ou ...
        printf("Quantidade em stock:%d\n", array_Ingredientes[idIngredienteAListar-1].quantidade);
		a:
       do{
    		printf("Por favor introduza uma quantidade:\n");
		} while((qtd = checkInteiroNegativo(gets(quantidadeIngrediente))) == -765412);//enquanto n introduzir uma quantidade valida
		if((array_Ingredientes[idIngredienteAListar-1].quantidade + qtd) < 0 ){
			printf("Stock nao pode ser negativo!\n");
		  	goto a;
		}
        array_Ingredientes[idIngredienteAListar - 1].quantidade += qtd;
        printf("Quantidade atualizada de %s : %d\n", array_Ingredientes[idIngredienteAListar - 1].nome, array_Ingredientes[idIngredienteAListar - 1].quantidade);
        getchar();
	}else{
        printf("Nao existe nenhum ingrediente com esse nome.\n");
    }
}


//ADICIONAR UNA PIZA # PIZA # PIZA



int addPizza(PIZZA array_Pizza[250],int idGlobalPizzas,INGREDIENTE array_Ingredientes[250],int idGlobalIngredientes){
	float preco;
	char precoString[50];
    int jaExiste = -1;
    
    //Vamos verificar se o limite das pizzas  do programa ainda não foi atngido
    if(idGlobalPizzas == 250){//Quer dizer que atinjimos o limite de pizzas que podemos armazenar no nosso programa[na 'ementa']
        return 0; //ou seja não incrementamos nada e saiamos do programa
    } 
    do{
    	printf("Introduza o nome da Pizza [Escreva 'sair' para sair]:\n");
    	gets(array_Pizza[idGlobalPizzas].nome); 
    	if(stricmp(array_Pizza[idGlobalPizzas].nome,"sair") == 0){
        	return 0; //ou seja ele ja n quer nada disto
    	}
		jaExiste = checkPizza(array_Pizza,idGlobalPizzas,array_Pizza[idGlobalPizzas].nome);
    	if(jaExiste != -1){
        	printf("Ja existe a pizza inserida.\n");
    	}
    }while(strlen(array_Pizza[idGlobalPizzas].nome) == 0 || jaExiste != -1);
  
  	// preco,ingredientes
    printf("Introduza o preco da %s:\n",array_Pizza[idGlobalPizzas].nome);
    while((preco = checkPreco(gets(precoString))) == -2){//enquanto n introduzir um preco valido
    	printf("Por favor introduza um preco valido:\n");
	}
	
	array_Pizza[idGlobalPizzas].preco = preco;
	array_Pizza[idGlobalPizzas].estado =1;
	
    array_Pizza[idGlobalPizzas].id_pizza = idGlobalPizzas + 1 ; 
    
    printf("Quantos ingredientes pertende inserir?\n");
   	char quantidadeIngrediente[100];
   	int qtd;
   	int qtdDoIngrediente;
    while((qtd = checkInteiro(gets(quantidadeIngrediente))) == -2){//enquanto n introduzir uma quantidade valida
   	printf("Por favor introduza uma quantidade correta de ingredientes a inserir:\n");
	}
	array_Pizza[idGlobalPizzas].totalIng = qtd; //isto vai guardar a qtd dos ingredientes que temos na pizza
	char ingredienteAProcurar[250]; 
	int idIngredienteAListar, estado = 0,i,j;
	
	for(i=0;i<array_Pizza[idGlobalPizzas].totalIng;i++){ //alem de introduzirmos ingredientes..temos que ver se o que ele esta a introduzir ja n esta introduzido
		printf("Insercao do %do Ingrediente:\n",i+1);
		do{
			printf("Introduza o nome do ingrediente[Escreva 'sair' para sair]:\n");
        	gets(ingredienteAProcurar);
       	if(stricmp(ingredienteAProcurar,"sair") == 0){
        	return 0; //ou seja ele ja n quer nada disto
    	}
            idIngredienteAListar = checkIngrediente(array_Ingredientes,idGlobalIngredientes,ingredienteAProcurar);
            	
            if(idIngredienteAListar != -1){
            	estado = 1;
                array_Pizza[idGlobalPizzas].ing[i][0] = idIngredienteAListar - 1 ;
                printf("Quantidade do ingrediente:\n");
				while((qtdDoIngrediente = checkInteiro(gets(quantidadeIngrediente))) == -2){//enquanto n introduzir uma quantidade valida
   				printf("Por favor introduza uma quantidade correta do ingrediente:\n");
				}
				array_Pizza[idGlobalPizzas].ing[i][1] = qtdDoIngrediente;
		 
            }else{
               	estado = 0;
				printf("Nao existe nenhum ingrediente com esse nome.\n");
            }  
		}while(estado != 1);   
		
		for(j=0;j<i;j++){ //com isto podemos garantir que n ha 'valores repetidos ao longo do array'
            	 if(array_Pizza[idGlobalPizzas].ing[j][0] == idIngredienteAListar -1){ //se o ingrediente que ele quiser meter ja existir
            	 	array_Pizza[idGlobalPizzas].ing[j][1] += qtdDoIngrediente;
            	 	array_Pizza[idGlobalPizzas].totalIng--;
            	  	i--;
				 }
				}    
	}									  
    return 1; 
}



void listarPizzas(PIZZA array_Pizza[250],int idGlobalPizzas,INGREDIENTE array_Ingredientes[250],int idGlobalIngredientes){
	int i,j,idPizza;
	system("cls");
	for(i=0;i<idGlobalPizzas;i++){
		escreveLinha();
		
		printf("Pizza ID %d: \tNome: %s \t- Preco: %.2f\t",array_Pizza[i].id_pizza,array_Pizza[i].nome,array_Pizza[i].preco );
		if(array_Pizza[i].estado == 1){
			printf("[Disponivel]\n\n");
		}else{
				printf("[Nao Disponivel]\n\n");
		}
		//	while(array_Pizza[i].ing[j] != '\0'){
		printf("Temos %d ingrediente(s) \n",array_Pizza[i].totalIng);
	
		for(j=0;j<array_Pizza[i].totalIng;j++){ 
			idPizza  = array_Pizza[i].ing[j][0];
			printf("%d x de %s\n",array_Pizza[i].ing[j][1] , array_Ingredientes[idPizza].nome);
 		}
		escreveLinha();
	}
	printf("Pressione ENTER para sair da listagem\n");
}

int listarPizzasVenda(PIZZA array_Pizza[250],int idGlobalPizzas,INGREDIENTE array_Ingredientes[250],int idGlobalIngredientes,int s,int num){
	int i,j,z,idPizza,h=0;
	int idPizzaPossiveis[250];
	int possoEncomendar = 1; //por defeito pode
	int opc=-2;
	char opcString[250];

	printf("CARDAPIO:\n\n");
	for(i=0;i<idGlobalPizzas;i++){
	 
			if(array_Pizza[i].estado == 1){ //se a pizza estiver disponivel   
 
				for(j=0;j<array_Pizza[i].totalIng;j++){ 
			idPizza  = array_Pizza[i].ing[j][0];
			//se o stock for maior que aos ingredientes necessarios
			if( array_Ingredientes[idPizza].quantidade < array_Pizza[i].ing[j][1]){ //quer dizer que n temos  stock suficiente para produzir a pizza
				possoEncomendar = 0;
			} 
 		}
 			if(possoEncomendar == 1){
 				
				 printf("Identificador %d:\nNome: %s\nPreco: %.2f\n",array_Pizza[i].id_pizza,array_Pizza[i].nome,array_Pizza[i].preco ); 
				 idPizzaPossiveis[h] = array_Pizza[i].id_pizza;
				 	if(array_Pizza[i].totalIng != 0){
				 		printf("\tIngredientes:\n");
					
				 		for(z=0;z<array_Pizza[i].totalIng;z++){  //Para cada ingrediente
								//array_Pedidos[i].pizzasEncomendadasIngredientes[j][z][0] isto é o ID do ingrediente
									printf("\t%s x%d\n\n", array_Ingredientes[array_Pizza[i].ing[z][0]].nome,array_Pizza[i].ing[z][1]);
							}
						 }else{ 
								printf("\tSem ingredientes\n\n");
							}
				 
				 
				 h++;
			 }
			 possoEncomendar = 1; //para ver se pode encomendar a proxima pizza ..
 			
	 //deixo-o escolher aqui uma destas pizzas..e depois no addPedido listo os ingredientes, pergunto se ta tudo ok, e se tiver vou para a proxima..?
	}
			} 


	do{
	printf("Escolheu encomendar um total de %d pizza(s). Qual a %da pizza que deseja?\n",num, s+1);
	gets(opcString);
	opc = checkInteiro(opcString);
			possoEncomendar = 0;
		for(i=0;i<h;i++){
			if(opc == idPizzaPossiveis[i]){//opc valida
				possoEncomendar = 1;
			}
		}
	}while( opc == -2 || possoEncomendar == 0 ); //enquanto o numero n for inteiro
		system("cls");
	printf("Encomendar  %s\n\n",array_Pizza[opc-1].nome);
	return opc;
}

void pizzaMaisCara(PIZZA array_Pizzas[], int idGlobalPizza){
	int i;
	float maior=0;
	system("cls");
	printf("Pizzas com o preco mais elevado ");
	for(i=0; i<idGlobalPizza;i++){
		if(array_Pizzas[i].preco>maior){
			maior=array_Pizzas[i].preco;
		}
	}
	printf("(%.2f Euros):\n", maior);
	for(i=0; i<idGlobalPizza;i++){
		if(maior==array_Pizzas[i].preco){
			puts(array_Pizzas[i].nome);
		}
	}
	printf("\nCarregue em ENTER para sair da listagem\n");
	getchar();
}
void mudarEstadoPizza(PIZZA array_Pizza[],int idGlobalPizzas){
	int jaExiste;
	char pizzaProcurar[150] = "";
	 do{
	 	system("cls");
    	printf("Introduza o nome da Pizza [Escreva 'sair' para sair]:\n");
    	gets(pizzaProcurar); 
    	if(stricmp(pizzaProcurar,"sair") == 0){
        	return ; //ou seja ele ja n quer nada disto
    	}
		jaExiste = checkPizza(array_Pizza,idGlobalPizzas,pizzaProcurar);
    	if(jaExiste == -1){
        	printf("Essa pizza nao esta inserida.\n"); //jaExiste -  1 = indice do arrayPizza
    	}
    }while(strlen(pizzaProcurar) == 0 || jaExiste == -1);//enquanto nao inserir nada, e a pizza inserida nao existir 
  	if(array_Pizza[jaExiste - 1 ].estado == 0){
  		array_Pizza[jaExiste - 1 ].estado = 1;
  		printf("%s adicionada as opcoes do menu!\n", array_Pizza[jaExiste-1].nome);
  		printf("\nCarregue em ENTER para sair da listagem\n");  
	}else{
	  	array_Pizza[jaExiste - 1 ].estado = 0;
		  	printf("%s removida das opcoes do menu!\n", array_Pizza[jaExiste-1].nome);
			printf("\nCarregue em ENTER para sair da listagem\n");
	  }
getchar();

}

void pizzaAdicionarValor(PIZZA array_Pizza[],int idGlobalPizzas){
	//Primeiro vemos se quer usar % ou somar um valor direto
	int i;
	int opc;
	char opcString[150];
	float preco;
	char precoString[150];
 	float desconto;
 	char descontoString[150];
 	char alterar [5];
	do{
		system("cls");
		printf("1 - Adicionar valor numerario  a TODAS as pizzas\n");
		printf("2 - Adicionar valor percentual a TODAS as pizzas\n");
		printf("0 - Voltar ao menu anterior\n");
	}while((opc = checkInteiro(gets(opcString))) == -2 && opc != 1 && opc != 2 && opc != 0);
	
	
	if(opc == 1){
		system ("cls");
		 //temos que ver se e um preco valido
		 first:
		  printf("Introduza o preco a alterar:\n");
    while((preco = checkPreco(gets(precoString))) == -2){//enquanto n introduzir um preco valido
    	printf("Por favor introduza um preco valido:\n");
	}
	
	before:
	system ("cls");
	printf("Deseja adicionar ou retirar %.2f Euro(s)? (Introduza '+' ou '-')\n", preco);
	gets(alterar);
	
	if (stricmp(alterar,"+")==0 || stricmp(alterar,"-")==0){
		goto next;
	}else{
		printf("Opcao Invalida!\n\n");
		goto before;
	}
	next:
		
	if (stricmp("-", alterar)==0){
	for(i=0;i<idGlobalPizzas;i++){
		if(array_Pizza[i].preco-preco < 0){
			printf("Nao podemos retirar esse valor ao preco das pizzas senao estas ficam com valores negativos!\n");
			goto first;
		}else{
			array_Pizza[i].preco -= preco;
		}
	}
	system ("cls");
	printf ("Preco de todas as pizzas alterado (-%.2f Euro(s))\n\n", preco);
	printf("\nCarregue em ENTER para voltar ao menu\n");
	getchar();
	}
	
	if (stricmp("+", alterar)==0){
	for(i=0;i<idGlobalPizzas;i++){
		array_Pizza[i].preco += preco;
	}
	system ("cls");
	printf ("Preco de todas as pizzas alterado (+%.2f Euro(s))\n\n", preco);
	printf("\nCarregue em ENTER para voltar ao menu\n");
			getchar();
	}
	
			
			
	}else if(opc == 2){ //quer dizer que ele quer adicionar uma percentagem
		system("cls");
		z:
		printf("Introduza o desconto a efetuar (Entre -100%% e 100%%):\n");
        scanf("%f", &desconto);
        
        if (checkDesconto(desconto)==-2){
        	goto z;
		}
		
        
		/*	
			while((desconto = checkInteiro(gets(descontoString))) == -2 &&){//enquanto n introduzir um preco valido
    		printf("Por favor introduza um desconto valido:\n");
		*/
	for(i=0;i<idGlobalPizzas;i++){
		array_Pizza[i].preco +=  (array_Pizza[i].preco * (desconto/100));
	}
	system ("cls");
	printf ("Preco de todas as pizzas aumentado em %.2f%%\n\n", desconto);
	printf("\nCarregue em ENTER para voltar ao menu\n");
	fflush(stdin);
			getchar();
	}

}

void pizzaAdicionarValorIngrediente(PIZZA array_Pizza[250],int idGlobalPizzas,INGREDIENTE array_Ingredientes[250],int idGlobalIngredientes){
	int idIngredienteAListar;
	int estado = 0;
	int i,j,h;
	int encontrou = 0;//isto vai permitir saber se a pizza tem o tal ingrediente
	
	
	float preco;
	char precoString[150];
 	int desconto;
 	char descontoString[150];
	int opc;
	char opcString[150],alterar[50];
 	
 	
	char ingredienteAProcurar[150];
		do{
			system("cls");
			printf("Introduza o nome do ingrediente['sair' - para sair]:\n");
        	gets(ingredienteAProcurar);
         	if(stricmp("sair",ingredienteAProcurar) == 0){
         		return; //quer dizer que ele quer sair
			 }
            idIngredienteAListar = checkIngrediente(array_Ingredientes,idGlobalIngredientes,ingredienteAProcurar);
            if(idIngredienteAListar != -1){//caso encontre o ingrediente
            	estado = 1; //para sair do while que ta sempre a perguntar o ingrediente ate ele introduzir o cenas certo
            
            	
   
		 //temos que ver se e um preco valido
		  printf("Introduza o preco \n");
    while((preco = checkPreco(gets(precoString))) == -2){//enquanto n introduzir um preco valido
    	printf("Por favor introduza um preco valido:\n");
	}   	
		before:
	system ("cls");
	printf("Deseja adicionar ou retirar %.2f Euro(s)? (Introduza '+' ou '-')\n", preco);
	gets(alterar);
	
	if (stricmp(alterar,"+")==0 || stricmp(alterar,"-")==0){
		goto next;
	}else{
		printf("Opcao Invalida!\n\n");
		goto before;
	}
	next:
            	
            	//vamos percorrer todas as pizzas
            		for(i=0;i<idGlobalPizzas;i++){
            				encontrou = 0;
            				for(j=0;j<array_Pizza[i].totalIng;j++){//vamos percorrer todos os ingredientes da pizza
            						if(array_Pizza[i].ing[j][0] == idIngredienteAListar - 1){
            							encontrou = encontrou + array_Pizza[i].ing[j][1];
									}			
							}  //caso tenha encontrado o tal ingrediente vamos fazer assim
							if(encontrou != 0){
					if (stricmp("+", alterar)==0){
						array_Pizza[i].preco  = array_Pizza[i].preco  +  (preco * encontrou);
						
					}else{	array_Pizza[i].preco  = array_Pizza[i].preco  -  (preco * encontrou);
						 						
					}					
						 	}
					} 
            		 	//se tiver o ingrediente vamos alterear o valor da pizza
                 printf("Todas as pizzas com %s tiveram o seu preco aumentado!\n",ingredienteAProcurar);
            }else{
               	estado = 0;
				printf("Nao existe nenhum ingrediente com esse nome.\n");
            }  
		}while(estado != 1);
			printf("\nCarregue em ENTER para voltar ao menu\n");
		getchar();
}

void removerIngredientesPedidoPizza(PEDIDO array_Pedidos[],int idGlobalPedidos,PIZZA array_Pizza[],int idGlobalPizza,INGREDIENTE array_Ingredientes[],int idGlobalIngredientes,int i){
		int num1 = -1,j,idIngredienteAListar,z,h,qtdARemover,checkIngredienteExiste; //para entrar no ciclo 
		char opc[100],ingredienteAProcurar[100];
    while(num1 >array_Pedidos[idGlobalPedidos].pizzas[i][2] || num1 < 0 ){
    do{
	printf("Quantos ingredientes deseja remover[0 - %d]?\n",array_Pedidos[idGlobalPedidos].pizzas[i][2]);
    }while((num1 = checkInteiro(gets(opc))) == -2);//enquanto nao meter um numero
    }
    
	//a medida que vou listando pergunto , quer remover este?e este? e quando o num chegar a 0 xau ai
	//ou entao vou perguntando qual quer remover
	printf("Vamos remover %d ingredientes.\n",num1);
	for(j=0;j<num1;j++){	 //ou entao, para cada um dos ingredientes que deseja remover
	//peço o ingrediente
	pedeOutro:
	do{ 
	   printf("Introduza o ingrediente que deseja eliminar:\n");
       gets(ingredienteAProcurar);
       //vejo se o ingrediente existe
        idIngredienteAListar = checkIngrediente(array_Ingredientes,idGlobalIngredientes,ingredienteAProcurar);
      
		}while(idIngredienteAListar == -1);
		idIngredienteAListar -=1;
		
		printf("ID Do Ingrediente: %d %s \n",idIngredienteAListar,array_Ingredientes[idIngredienteAListar].nome);
	
	//vejo se o ingrediente faz parte da lista de ingredientes da pizza
	 checkIngredienteExiste = -1;
	 qtdARemover = 0;
	for(z=0;z<array_Pedidos[idGlobalPedidos].pizzas[i][2];z++){  
			if(array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][z][0] == idIngredienteAListar){
				checkIngredienteExiste = z; //guardamos o ID do ingrediente na pizza no array, ou seja sabemos que temos que tirar isto daqui
				if(array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][z][1] == 1){
				printf("\n %s removido da pizza!\n", array_Ingredientes[array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][z][0]].nome,array_Ingredientes[idIngredienteAListar].nome);
			array_Pedidos[idGlobalPedidos].pizzas[i][2] -=1;
				}else{
					//se a quantidade for superior a 1 perguntamos qt � que ele quer remover
					    while(qtdARemover >array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][z][1] || qtdARemover < 1 ){
   						 do{
				printf("Introduza a quantidade do ingrediente que deseja remover:[1-%d]\n",array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][z][1]);
   				 }while((qtdARemover = checkInteiro(gets(opc))) == -2);//enquanto nao meter um numero
   					 }
				}
			
			}
		}
		
		
	if(checkIngredienteExiste == -1){ //o ingrediente introduzido não existe na pizza
		goto pedeOutro;
	}else{ 
		//	array_Pedidos[idGlobalPedidos].pizzas[i][0] = 	array_Pedidos[idGlobalPedidos].pizzas[i+1][0]; //id
		//	array_Pedidos[idGlobalPedidos].pizzas[i][1] = 	array_Pedidos[idGlobalPedidos].pizzas[i+1][1]; //preco
			 //a quantidade de ingredientes baixa
	if(qtdARemover != 0 ){ //quer dizer que ele n foi removido
	 	if(qtdARemover == array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][checkIngredienteExiste][1]){ //e para remover tudo
			printf("\n %s removido da pizza!\n", array_Ingredientes[array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][checkIngredienteExiste][0]].nome);
				for(h=checkIngredienteExiste;h<array_Pedidos[idGlobalPedidos].pizzas[i][2];h++){ //para todos os ingredientes que estao a frente, vao descer um indice
			
			
			array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][h][0] = array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][h+1][0];
			array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][h][1] = array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][h+1][1]; 	
		} 
		array_Pedidos[idGlobalPedidos].pizzas[i][2] -=1;
		
		}else{//� s� para mudar a quantidade
	//	printf("%d - %d = ",array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][checkIngredienteExiste][1],qtdARemover);
			array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][checkIngredienteExiste][1] = array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][checkIngredienteExiste][1] - qtdARemover;
	//	printf("%d\n",	array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][checkIngredienteExiste][1] );
			 printf("\n Quantidade do ingrediente na pizza atualizada!\n \t%s x%d!\n\n", array_Ingredientes[array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][checkIngredienteExiste][0]].nome,array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][checkIngredienteExiste][1]);
			
		}
	 
		}
	} 
		}
		if(num1 != 0){ 
		getchar(); 
		}
}

void adicionarIngredientesPedidoPizza(PEDIDO array_Pedidos[],int idGlobalPedidos,PIZZA array_Pizza[],int idGlobalPizza,INGREDIENTE array_Ingredientes[],int idGlobalIngredientes,int i){
		int num1 = -1,j,idIngredienteAListar,z,h,checkIngredienteExiste; //para entrar no ciclo 
		char opc[100],ingredienteAProcurar[100];
     while(num1 > (12 - array_Pedidos[idGlobalPedidos].pizzas[i][2]) || num1 < 0 ){
    do{
	printf("Quantos ingredientes deseja adicionar[0 - %d]?\n",12 - array_Pedidos[idGlobalPedidos].pizzas[i][2]);
    }while((num1 = checkInteiro(gets(opc))) == -2);//enquanto nao meter um numero
    }
    	for(j=0;j<num1;j++){//para cada ingrediente que ele quer introduzir
    	
    	
    	
    	 
	do{ 
	   printf("Introduza o ingrediente que deseja introduzir:\n");
       gets(ingredienteAProcurar);
       //vejo se o ingrediente existe
        idIngredienteAListar = checkIngrediente(array_Ingredientes,idGlobalIngredientes,ingredienteAProcurar);
      
		}while(idIngredienteAListar == -1);
		idIngredienteAListar -=1;
		 
	
	//vejo se o ingrediente faz parte da lista de ingredientes da pizza
	 checkIngredienteExiste = -1; 
	for(z=0;z<array_Pedidos[idGlobalPedidos].pizzas[i][2];z++){  
			if(array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][z][0] == idIngredienteAListar){
				checkIngredienteExiste = z; //guardamos o ID do ingrediente na pizza no array, ou seja sabemos que temos que tirar isto daqui
			   	}
				
				
		} 
		
	if(checkIngredienteExiste == -1){ //o ingrediente introduzido não existe na pizza
	//vamos adicionar o ingrediente e incrementar o crl do limite
	
	if(array_Ingredientes[idIngredienteAListar].quantidade < 1){
		printf("Sem stock disponivel desse ingrediente\n");
		goto noStockPedeOutro; 
	}
	array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][array_Pedidos[idGlobalPedidos].pizzas[i][2]][0] = idIngredienteAListar;
	array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][array_Pedidos[idGlobalPedidos].pizzas[i][2]][1] = 1;
	array_Pedidos[idGlobalPedidos].pizzas[i][2]++; 
		 printf("\n Ingrediente adicionado!\n \t%s x%d!\n\n",array_Ingredientes[array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][array_Pedidos[idGlobalPedidos].pizzas[i][2]-1][0]].nome,array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][array_Pedidos[idGlobalPedidos].pizzas[i][2]-1][1]);
			
	}else{ 
		//	array_Pedidos[idGlobalPedidos].pizzas[i][0] = 	array_Pedidos[idGlobalPedidos].pizzas[i+1][0]; //id
		//	array_Pedidos[idGlobalPedidos].pizzas[i][1] = 	array_Pedidos[idGlobalPedidos].pizzas[i+1][1]; //preco
			 //a quantidade de ingredientes baixa 
	  		//� s� para mudar a quantidade
	  		if((array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][checkIngredienteExiste][1] + 1) > array_Ingredientes[idIngredienteAListar].quantidade ){
	  				printf("Sem stock disponivel desse ingrediente\n");
		goto noStockPedeOutro;
			  }
 			array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][checkIngredienteExiste][1] = array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][checkIngredienteExiste][1] + 1;
 			 printf("\n Quantidade do ingrediente na pizza atualizada!\n \t%s x%d!\n\n", array_Ingredientes[array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][checkIngredienteExiste][0]].nome,array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][checkIngredienteExiste][1]);
		 
		 
	}  
    	noStockPedeOutro:;
		}
			if(num1 != 0){ 
		getchar(); 
		}


}

int addPedido(PEDIDO array_Pedidos[],int idGlobalPedidos,PIZZA array_Pizza[],int idGlobalPizza,INGREDIENTE array_Ingredientes[],int idGlobalIngredientes,CLIENTE array_Clientes[], int idGlobalClientes){
 	system ("cls");              
 	int idCliente, qtdARemover = 0;
 	int qtdIngInicial;
    char opc[50]; 
    int num=-1,i;
    int opcFinal;
    if(idGlobalClientes == 0){
    	printf("Nenhum cliente registado!\n");
    	return 0;
	}
	
	if(idGlobalPedidos >= 250){
		printf("Limite alcancado");
		return 0; //maximo de pedidos alcançado
	}
	array_Pedidos[idGlobalPedidos].id_pedido = idGlobalPedidos + 1;
	char encontraCliente[100],opcCliente[100];
	 
	do{
		
		printf("Informacao para encontrar o cliente:[Escreva 'sair' para cancelar o pedido]\n");
		printf("1 - NIF\n");
		printf("2 - Telemovel\n");
		printf("3 - Mail\n");
		gets(opcCliente);
		//caso ele queira sair da encomenda
		if(stricmp(opcCliente,"sair") == 0){
			return 0;
		}
	}while(stricmp(opcCliente,"1") != 0 && stricmp(opcCliente,"2") != 0 && stricmp(opcCliente,"3") != 0 && stricmp(opcCliente,"4") != 0);
		system ("cls");
     idCliente = getCliente(array_Clientes,idGlobalClientes,opcCliente);
	if(idCliente == -1){
		printf("Encomenda cancelada!\n");
	return 0;
	}
	
	
	//PARTE DO CLIENTE FEITA
	//------------------------------------------------------------------------------------------------------------------------
	array_Pedidos[idGlobalPedidos].id_cliente = idCliente;
	//------------------------------------------------------------------------------------------------------------------------
    //Metodo pagamento:
    	system ("cls");
    while(num != 1 && num != 2 && num != 0){
    do{
	printf("Como pretende efetuar o seu pagamento?\n");
    printf("1 - Multibanco\n");
    printf("2 - Numerario\n"); 
    printf("0 - Cancelar Pedido\n");
    }while((num = checkInteiro(gets(opc))) == -2);//enquanto nao meter um numero
    }
    if(num == 0){ //Ele ja nao quer fazer a encomenda
    	return 0;
	}
   array_Pedidos[idGlobalPedidos].metodoPagamento = num;
   num = -2; //limpar a escolha do utilizador
   //Metodo Encomenda:
   	system ("cls");
       while(num != 1 && num != 2 && num != 0){
    do{
	printf("Pedido realizado via\n");
    printf("1 - Telefone\n");
    printf("2 - E-Mail\n"); 
    printf("0 - Cancelar Pedido\n");
    }while((num = checkInteiro(gets(opc))) == -2);//enquanto nao meter um numero
    }
    if(num == 0){ //Ele ja nao quer fazer a encomenda
    	return 0;
	}
    array_Pedidos[idGlobalPedidos].metodoEncomenda = num;
	array_Pedidos[idGlobalPedidos].estado = 0;
    
	  num = -2; //limpar a escolha do utilizador

	
   //Qtd de pizzas:
   	system ("cls");
       while(num >15 || num < 0 ){
    do{
	printf("Introduza a quantidade de pizzas a encomendar:[0-15]\n");
    }while((num = checkInteiro(gets(opc))) == -2);//enquanto nao meter um numero
    }
    if(num == 0){ //Ele ja nao quer fazer a encomenda
    	return 0;
	}
	
	
	
	 int checkIngredienteExiste = 0;
	 char ingredienteAProcurar[100];
	  int idPizza;
	  int j,z,idIngredienteAListar,h;
	  int qtdRemover = 0;
	  int totalIngSoma;
	  	array_Pedidos[idGlobalPedidos].totalPizzas = num; //vamos guardar o total de pizzas para os ciclos..
	  	
int num1;

array_Pedidos[idGlobalPedidos].valorPagar = 0;
	for(i=0;i<num;i++){ //Para cada pizza que o cliente deseja encomendar
		system ("cls"); 
			
		array_Pedidos[idGlobalPedidos].pizzas[i][0] = listarPizzasVenda(array_Pizza, idGlobalPizza,array_Ingredientes,idGlobalIngredientes,i,num); //Na primeira coluna vamos  armazenar os números identificadores da pizza de maneira a não estarmos a armazenar toda a informação referente a pizza dentro desta struct, apenas usando o número da pizza
 	 //preco é automatico
		array_Pedidos[idGlobalPedidos].pizzas[i][1] = array_Pizza[array_Pedidos[idGlobalPedidos].pizzas[i][0] - 1].preco ;//vamos buscar o preco
	//total de ingredientes de cada pizza..?
		array_Pedidos[idGlobalPedidos].pizzas[i][2] = array_Pizza[array_Pedidos[idGlobalPedidos].pizzas[i][0] - 1].totalIng ;//vamos buscar o total de ingredientes da tal pizza
 		
 	 qtdIngInicial = 0;
 	//agora listamos os ingredientes e pergunta se é isto que quer...caso ele faça alguma adição de ingredientes
	 	//caso a pizza tenha 0 ingredientes posso passar esta parte a frente..
 
		//printf("Listagem dos %d ingredientes:\n\n",array_Pedidos[idGlobalPedidos].pizzas[i][2] ); 
	
		for(j=0;j<array_Pedidos[idGlobalPedidos].pizzas[i][2];j++){ //para cada ingrediente..vamos listar-lo
			//aqui pergunto se deseja ter este ingrediente
   		   //se quiser, pergunto se é esta quantidade ou quer mais 
   		    //Se não quiser, tenho que fazer com que todos os ingredientes a frente venham um indice para tras e tenho que mudar o totalIng e o totalPizzas
			array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][j][0] = array_Pizza[array_Pedidos[idGlobalPedidos].pizzas[i][0] - 1].ing[j][0];   //id do ingrediente
			array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][j][1] = array_Pizza[array_Pedidos[idGlobalPedidos].pizzas[i][0] - 1].ing[j][1];   //qtd usada na pizza
			qtdIngInicial += array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][j][1];
	//		printf("\t%s x%d\n",array_Ingredientes[array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][j][0]].nome, array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][j][1]);
	
		}
		printf("\n");
		notOver:
		opcFinal = -1;
	
	     while(opcFinal != 1 && opcFinal != 2 && opcFinal != 3  ){
	     	system ("cls"); 	
	     	
    do{
	printf("Escoheu a %s.\nA %s tem %d ingredientes e custa %.2f Euro(s):\n\nLista de Ingredientes da %s:\n",array_Pizza[array_Pedidos[idGlobalPedidos].pizzas[i][0] - 1].nome,array_Pizza[array_Pedidos[idGlobalPedidos].pizzas[i][0] - 1].nome, qtdIngInicial,array_Pizza[array_Pedidos[idGlobalPedidos].pizzas[i][0] - 1].preco,array_Pizza[array_Pedidos[idGlobalPedidos].pizzas[i][0] - 1].nome);
 
	totalIngSoma = 0;
		for(j=0;j<array_Pedidos[idGlobalPedidos].pizzas[i][2];j++){ 
		printf("\t%s x%d\n",array_Ingredientes[array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][j][0]].nome, array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][j][1]);
		totalIngSoma += array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][j][1];
	}
	
	
 	if(qtdIngInicial < totalIngSoma){
 		printf("Preco da pizza atual com %d ingredientes extra: %.2f euros\n",(totalIngSoma - qtdIngInicial),(array_Pizza[array_Pedidos[idGlobalPedidos].pizzas[i][0] - 1].preco)+ (2* (totalIngSoma - qtdIngInicial)));
	 
	 }else if(totalIngSoma < qtdIngInicial){
	 	printf("Pode introduzir %d ingredientes sem alterar o valor base da sua pizza !\n",(qtdIngInicial - totalIngSoma));
	 	array_Pedidos[idGlobalPedidos].pizzas[i][1] = (array_Pizza[array_Pedidos[idGlobalPedidos].pizzas[i][0] - 1].preco);
	 }
	 
 	//
 	
	printf("\n1 - Remover Ingredientes\n");
	printf("2 - Adicionar Ingredientes [2 euros cada ingrediente extra]\n");
	printf("3 - Finalizar Pizza\n"); 
    }while((opcFinal = checkInteiro(gets(opc))) == -2);//enquanto nao meter um numero
    }
    if(opcFinal == 1){
    		removerIngredientesPedidoPizza(array_Pedidos,idGlobalPedidos,array_Pizza,idGlobalPizza,array_Ingredientes,idGlobalIngredientes,i);
			
			goto notOver;
	}else if(opcFinal == 2){
		adicionarIngredientesPedidoPizza(array_Pedidos,idGlobalPedidos,array_Pizza,idGlobalPizza,array_Ingredientes,idGlobalIngredientes,i);
	 
		goto notOver;
	}else if(opcFinal == 3){
			for(j=0;j<array_Pedidos[idGlobalPedidos].pizzas[i][2];j++){ //vamos percorrer todos os ingredientes..paraaaaaaaaa meter os stocks direitos
			array_Ingredientes[array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][j][0]].quantidade -= array_Pedidos[idGlobalPedidos].pizzasEncomendadasIngredientes[i][j][1];
		}
		array_Pedidos[idGlobalPedidos].valorPagar = array_Pedidos[idGlobalPedidos].valorPagar + (array_Pizza[array_Pedidos[idGlobalPedidos].pizzas[i][0] - 1].preco)+ (2* (totalIngSoma - qtdIngInicial));
		printf("Pizza adicionada a encomenda!\n");
		printf("Preco da encomenda: %.2f",	array_Pedidos[idGlobalPedidos].valorPagar );
 
	} 

	  
	}	//fecha cada pizza
    
   	printf("\nPedido realizado!\n");
	return 1;
	
 
 	
/*	
	
	array_Pedidos[idGlobalPedidos].valorPagar = vai ser igual ao valor da soma do total das pizzas
	
	 
	
	array_Pedidos[idGlobalPedidos].pizzas[i][0] = id da pizza
	array_Pedidos[idGlobalPedidos].pizzas[i][1] = quantidade dessa  pizza que foi encomendada
	array_Pedidos[idGlobalPedidos].pizzas[i][2] = o preco na altura dessa pizza
	 
    
    /*WARNING: DANGER
     Exemplo: Cada Pizza tem no máximo 12 ingredientes , e cada um destes ingredientes armazena 2 números 
     ou seja 15 -> 12 -> 2
    
    int pizzasEncomendadasIngredientes[15][12][2];//Este array tri-dimensional vai armazenar para cada uma das pizzas, e para cada um dos 12 ingredientes, 2 valores númericos, sendo o primeiro o identificador do ingrediente e o segundo a quantidade desse ingrediente utilizada na respectiva pizza
 */
	
}


void listarTodosPedidos(PEDIDO array_Pedidos[],int idGlobalPedidos,PIZZA array_Pizzas[],int idGlobalPizza,INGREDIENTE array_Ingredientes[],int idGlobalIngredientes,CLIENTE array_Clientes[], int idGlobalClientes){
				int i,j,z;
				int contaPedidos = 0;
					system ("cls");
					printf("Pedidos por entregar:\n");
						for(i=0;i<idGlobalPedidos;i++){ //Para cada Pedido  
							if(array_Pedidos[i].estado == 0)  {
							printf("\n\nPedido : %d ",(array_Pedidos[i].id_pedido));
							if(array_Pedidos[i].metodoEncomenda == 0){
								printf("feito por email: %s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].mail);
							}else{
								printf("feito por telemovel : %s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].telemovel);
							}
							if(array_Pedidos[i].metodoPagamento == 0){
								printf("Pagamento por numerario\n");
							}else{
								printf("Pagamento por multibanco\n");
							} 
							printf("Nome:%s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].nome);
							printf("Morada:%s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].morada);
							printf("NIF:%s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].nif);
					     	printf("Valor total: %.2f\n\n",array_Pedidos[i].valorPagar); 
						 	for(j=0;j<array_Pedidos[i].totalPizzas;j++){ //para cada Pizza
  								printf("Pizza %d : %s\n",j+1,array_Pizzas[array_Pedidos[i].pizzas[j][0] - 1].nome);			
  								printf("\tA pizza tem %d ingredientes\n",array_Pedidos[i].pizzas[j][2]);
  								
								for(z=0;z<array_Pedidos[i].pizzas[j][2];z++){  //Para cada ingrediente
								//array_Pedidos[i].pizzasEncomendadasIngredientes[j][z][0] isto é o ID do ingrediente
									printf("\t\t %s x%d\n", array_Ingredientes[array_Pedidos[i].pizzasEncomendadasIngredientes[j][z][0]].nome,array_Pedidos[i].pizzasEncomendadasIngredientes[j][z][1]);
							}
							}
 									printf("\n\n"); 
									 contaPedidos++; 
						}
							}
						if(contaPedidos == 0){
							
						}else{
					
						printf("[enter para continuar listagem]");
						getchar();	}
						
						contaPedidos = 0;
						
											system ("cls");
					printf("Pedidos entregues:\n");
						for(i=0;i<idGlobalPedidos;i++){ //Para cada Pedido  
							if(array_Pedidos[i].estado == 1)  {
								contaPedidos++;
								printf("\n\nPedido : %d ",(array_Pedidos[i].id_pedido));
							if(array_Pedidos[i].metodoEncomenda == 0){
								printf("feito por email: %s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].mail);
							}else{
								printf("feito por telemovel : %s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].telemovel);
							}
							if(array_Pedidos[i].metodoPagamento == 0){
								printf("Pagamento por numerario\n");
							}else{
								printf("Pagamento por multibanco\n");
							} 
							printf("Nome:%s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].nome);
							printf("Morada:%s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].morada);
							printf("NIF:%s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].nif);
					     	printf("Valor total: %.2f\n\n",array_Pedidos[i].valorPagar); 
							 	for(j=0;j<array_Pedidos[i].totalPizzas;j++){ //para cada Pizza
  								printf("Pizza %d : %s\n",j+1,array_Pizzas[array_Pedidos[i].pizzas[j][0] - 1].nome);			
  								printf("\tA pizza tem %d ingredientes\n",array_Pedidos[i].pizzas[j][2]);
  								
								for(z=0;z<array_Pedidos[i].pizzas[j][2];z++){  //Para cada ingrediente
								//array_Pedidos[i].pizzasEncomendadasIngredientes[j][z][0] isto é o ID do ingrediente
									printf("\t\t %s x%d\n", array_Ingredientes[array_Pedidos[i].pizzasEncomendadasIngredientes[j][z][0]].nome,array_Pedidos[i].pizzasEncomendadasIngredientes[j][z][1]);
							}
							}
 									printf("\n\n"); 	 
						}
							}
						if(contaPedidos == 0){
							
						}else{
					
						printf("[enter para continuar listagem]");
						getchar();	}
					contaPedidos = 0;
										system ("cls");
					printf("Pedidos cancelados:\n");
						for(i=0;i<idGlobalPedidos;i++){ //Para cada Pedido  
							if(array_Pedidos[i].estado == 2)  {
								contaPedidos++;
							printf("\n\nPedido : %d ",(array_Pedidos[i].id_pedido));
							if(array_Pedidos[i].metodoEncomenda == 0){
								printf("feito por email: %s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].mail);
							}else{
								printf("feito por telemovel : %s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].telemovel);
							}
							if(array_Pedidos[i].metodoPagamento == 0){
								printf("Pagamento por numerario\n");
							}else{
								printf("Pagamento por multibanco\n");
							} 
							printf("Nome:%s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].nome);
							printf("Morada:%s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].morada);
							printf("NIF:%s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].nif);
					     	printf("Valor total: %.2f\n\n",array_Pedidos[i].valorPagar); 
						 	for(j=0;j<array_Pedidos[i].totalPizzas;j++){ //para cada Pizza
  								printf("Pizza %d : %s\n",j+1,array_Pizzas[array_Pedidos[i].pizzas[j][0] - 1].nome);			
  								printf("\tA pizza tem %d ingredientes\n",array_Pedidos[i].pizzas[j][2]);
  								
								for(z=0;z<array_Pedidos[i].pizzas[j][2];z++){  //Para cada ingrediente
								//array_Pedidos[i].pizzasEncomendadasIngredientes[j][z][0] isto é o ID do ingrediente
									printf("\t\t %s x%d\n", array_Ingredientes[array_Pedidos[i].pizzasEncomendadasIngredientes[j][z][0]].nome,array_Pedidos[i].pizzasEncomendadasIngredientes[j][z][1]);
							}
							}
 									printf("\n\n"); 	 
						}
							}
								if(contaPedidos == 0){
							
						}else{
					 
						getchar();	}
					 
				
}

void listarTodosPedidosNEntregues(PEDIDO array_Pedidos[],int idGlobalPedidos,PIZZA array_Pizzas[],int idGlobalPizza,INGREDIENTE array_Ingredientes[],int idGlobalIngredientes,CLIENTE array_Clientes[], int idGlobalClientes){
	
				int i,j,z;
				int nEntregues = 0;
						system ("cls");
						printf("Listagem de todos os pedidos que ainda nao foram entregues:\n\n");
						for(i=0;i<idGlobalPedidos;i++){ //Para cada Pedido
							if(array_Pedidos[i].estado == 0){ //quer dizer que ainda nao foi entregue
								printf("\n\nPedido : %d ",(array_Pedidos[i].id_pedido));
							if(array_Pedidos[i].metodoEncomenda == 0){
								printf("feito por email: %s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].mail);
							}else{
								printf("feito por telemovel : %s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].telemovel);
							}
							if(array_Pedidos[i].metodoPagamento == 0){
								printf("Pagamento por numerario\n");
							}else{
								printf("Pagamento por multibanco\n");
							} 
							printf("Nome:%s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].nome);
							printf("Morada:%s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].morada);
							printf("NIF:%s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].nif);
					     	printf("Valor total: %.2f\n\n",array_Pedidos[i].valorPagar); 	
  							for(j=0;j<array_Pedidos[i].totalPizzas;j++){ //para cada Pizza
  								printf("Pizza %d : %s\n",j+1,array_Pizzas[array_Pedidos[i].pizzas[j][0] - 1].nome);	
							 	printf("\tA pizza tem %d ingredientes\n",array_Pedidos[i].pizzas[j][2]);
  								
								for(z=0;z<array_Pedidos[i].pizzas[j][2];z++){  //Para cada ingrediente
								//array_Pedidos[i].pizzasEncomendadasIngredientes[j][z][0] isto é o ID do ingrediente
									printf("\t\t %s x%d\n", array_Ingredientes[array_Pedidos[i].pizzasEncomendadasIngredientes[j][z][0]].nome,array_Pedidos[i].pizzasEncomendadasIngredientes[j][z][1]);
			}
		}
		printf("\n\n");
		nEntregues++;
	} }
	if(nEntregues == 0){
		printf("***Nenhum pedido por entregar!***\n");
	}
	getchar();
}
void printTotalRecebido(PEDIDO array_Pedidos[],int idGlobalPedidos){
	int i = 0;
	system ("cls");
	float total=0;
	for(i=0;i<idGlobalPedidos;i++){
		if(array_Pedidos[i].estado == 1){
			
		total += array_Pedidos[i].valorPagar;
		}
	}
	printf("Total recebido: %.2f euros!\n",total);
	getchar();
}
void alteraEstadoPedido(PEDIDO array_Pedidos[],int idGlobalPedidos,PIZZA array_Pizzas[],int idGlobalPizza,INGREDIENTE array_Ingredientes[],int idGlobalIngredientes,CLIENTE array_Clientes[], int idGlobalClientes){
				int i,j,z,opcEstado;
				char opcText[100];
			 		system ("cls");
						for(i=0;i<idGlobalPedidos;i++){ //Para cada Pedido
							if(array_Pedidos[i].estado == 0){ //quer dizer que ainda nao foi entregue
								
							
							printf("\n\nPedido : %d ",(array_Pedidos[i].id_pedido));
							if(array_Pedidos[i].metodoEncomenda == 0){
								printf("feito por email: %s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].mail);
							}else{
								printf("feito por telemovel : %s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].telemovel);
							}
							if(array_Pedidos[i].metodoPagamento == 0){
								printf("Pagamento por numerario\n");
							}else{
								printf("Pagamento por multibanco\n");
							} 
							printf("Nome:%s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].nome);
							printf("Morada:%s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].morada);
							printf("NIF:%s\n",array_Clientes[array_Pedidos[i].id_cliente - 1].nif);
					     	printf("Valor total: %.2f\n\n",array_Pedidos[i].valorPagar);  
  							for(j=0;j<array_Pedidos[i].totalPizzas;j++){ //para cada Pizza
  								printf("Pizza %d : %s\n",j+1,array_Pizzas[array_Pedidos[i].pizzas[j][0] - 1].nome);			
  								printf("\tA pizza tem %d ingredientes\n",array_Pedidos[i].pizzas[j][2]);
  								
								for(z=0;z<array_Pedidos[i].pizzas[j][2];z++){  //Para cada ingrediente
								//array_Pedidos[i].pizzasEncomendadasIngredientes[j][z][0] isto é o ID do ingrediente
									printf("\t\t %s x%d\n", array_Ingredientes[array_Pedidos[i].pizzasEncomendadasIngredientes[j][z][0]].nome,array_Pedidos[i].pizzasEncomendadasIngredientes[j][z][1]);
			}
		}
		printf("\n\n");
			//fazer um switch a ver se foi entregue ou se nao foi entregue
			
			 		opcEstado = -1; //para entrar no ciclo
    while(opcEstado != 1 && opcEstado != 2 && opcEstado != 3 && opcEstado != 0){
    do{
 
		printf("1 - Entregue\n");
		printf("2 - Cancelada\n");
		printf("3 - Por Entregar\n");
		printf("0 - Sair\n");
 
    }while((opcEstado = checkInteiro(gets(opcText))) == -2);//enquanto nao meter um numero
    }
    		
    		if(opcEstado == 3){ //nao tem que fazer nada visto que ainda nem foi entregue nem cancelada..mas se ele quiser meter outra que vem a frente como entregue..zau
    			
			}else if(opcEstado == 0){
				//aqui ele quer sair para o menu por isso acabamos o ciclo
		i=idGlobalPedidos;
			}else{
					array_Pedidos[i].estado = opcEstado;
			}
		 
			
							  }
						}
}
void pizzasMaisVendidas(PEDIDO array_Pedidos[],int idGlobalPedidos,PIZZA array_Pizzas[], int idGlobalPizza){
	//primeiro contamos a pizza que foi mais vendida
		system ("cls");
	int contador[idGlobalPizza];
	int indicePizza;
	int i,j,z;
	int maior = 0;
	for(i=0;i<idGlobalPizza;i++){ //para cada pizza
		contador[i] = 0;
			for(j=0;j<idGlobalPedidos;j++){//para cada pedido
				for(z=0;z<array_Pedidos[j].totalPizzas;z++){//Para cada pizza do Pedido
					if(array_Pizzas[i].id_pizza == array_Pedidos[j].pizzas[z][0]){
						contador[i]++;
							if(contador[i] > maior){
								maior = contador[i];
							}
					}
				}
			}
	}
	 
	 printf("Pizzas vendidas %d vezes:\n",maior);
	 for(i=0;i<idGlobalPizza;i++){
	 	if(contador[i] == maior){
	 	 //imprimimos a pizza e os ingredientes
	 	 	 	printf("Pizza  %s\n",array_Pizzas[i].nome);			
  				printf("Preco base: %.2f\n\n",array_Pizzas[i].preco);
		}
		 }
		 
		 getchar();
	 }
	void informacoesClientesPizza(PEDIDO array_Pedidos[],int idGlobalPedidos,PIZZA array_Pizzas[], int idGlobalPizza,CLIENTE array_Clientes[],int idGlobalClientes){
		//pedimos a pizza...
		//vamos a todos os clientes
		//vemos se o cliente fez algum pedido
		//se o cliente fez o pedido, vemos se encomendou aquela pizza
		//se sim, imprimimos as suas informa��es sabe la deus porque porra � que alguem quer saber esta puta desta merda mas prontos la na cabe�a de alguem deve fazer sentido uma empresa que analisa
		//os clientes de forma individual....s� a puta da chapada
		system ("cls"); 
		int i,z,j,h=0,opc,possoEncomendar;
	 
	 char opcString[100];
	 
	 
	 for(i=0;i<idGlobalPizza;i++){
	 	printf("%d - %s\n",i+1,array_Pizzas[i].nome);
	 }
	do{
	 
	printf("Qual a pizza cuja informacoes dos clientes deseja saber?\n");
	gets(opcString);
	opc = checkInteiro(opcString);
			possoEncomendar = 0;
	 
			if(opc <= idGlobalPizza && opc > 0){//opc valida
				possoEncomendar = 1;
			}
	 
	}while( opc == -2 || possoEncomendar == 0 ); //enquanto o numero n for inteiro
	for(i=0;i<idGlobalClientes;i++){ //para cada cliente
		for(j=0;j<idGlobalPedidos;j++){//para cada Pedido
		if(array_Pedidos[j].id_cliente == array_Clientes[i].id_cliente){//se o pedido for do cliente
	
			for(z=0;z<array_Pedidos[j].totalPizzas;z++){ //para cada pizza encomendada
				if(array_Pedidos[j].pizzas[z][0] == opc){
					//printamos o cliente
						printf("Nome:%s\n",array_Clientes[i].nome);
						printf("Email: %s\n",array_Clientes[i].mail);
						printf("Telemovel : %s\n",array_Clientes[i].telemovel);
						printf("Morada:%s\n",array_Clientes[i].morada);
						printf("NIF:%s\n\n",array_Clientes[i].nif);
						 z=array_Pedidos[j].totalPizzas; //para sair dos ciclos visto que ja printou este cliente
						 j = idGlobalPedidos;
						 h++;
				}
				
				
			}
				}
				
		}
	}
	if(h>0){ 
	getchar();
	}else{
		printf("Nenhum cliente comprou essa pizza!");
		getchar();
	}
	}
 
 
 void informacoesClientesPizzaMaisCara(PEDIDO array_Pedidos[],int idGlobalPedidos,PIZZA array_Pizzas[], int idGlobalPizza,CLIENTE array_Clientes[],int idGlobalClientes){
	
		system ("cls");
		int i,z,j,h=0,opc,possoEncomendar;
	 float maior =0;
	 char opcString[100];
	 
	 for(i=0;i<idGlobalPizza;i++){ //n posso ir buscar a pizza mais cara..porque vamos imaginar que a pizza mais cara ainda n foi comparada..tipo tenho que percorrer os pedidos -> pizzas
	  	
		  if(array_Pizzas[i].preco > maior){
	 		maior = array_Pizzas[i].preco;
		 }
		 
	 }

	 
 //se a pizza com aquele id tiver o preco = ao maior

		printf("As pizzas mais caras custaram %.2f Euros e foram pedidas pelos seguintes clientes:\n",maior);
	for(i=0;i<idGlobalClientes;i++){ //para cada cliente
		for(j=0;j<idGlobalPedidos;j++){//para cada Pedido
		if(array_Pedidos[j].id_cliente == array_Clientes[i].id_cliente){//se o pedido for do cliente
			for(z=0;z<array_Pedidos[j].totalPizzas;z++){ //para cada pizza encomendada
				if(array_Pizzas[array_Pedidos[j].pizzas[z][0] - 1].preco == maior){
			
					//printamos o cliente
						printf("Nome:%s\n",array_Clientes[i].nome);
						printf("Email: %s\n",array_Clientes[i].mail);
						printf("Telemovel : %s\n",array_Clientes[i].telemovel);
						printf("Morada:%s\n",array_Clientes[i].morada);
						printf("NIF:%s\n\n",array_Clientes[i].nif);
						 z=array_Pedidos[j].totalPizzas; //para sair dos ciclos visto que ja printou este cliente
						 j = idGlobalPedidos; 
						 h++;
				} 
				
				
			}
				}
				
		}
	}
	if(h>0){ 
	getchar();
	}else{
		printf("Nenhum cliente comprou essa pizza!");
		getchar();
	}
	
}

int main() {
	CLIENTE array_Clientes[250];
	PEDIDO array_Pedidos[250];
	INGREDIENTE array_Ingredientes [250];
	PIZZA array_Pizzas[250];
    int opc=-1;
    int opcAdmin = -1;
    int opcEncomendas =-1;
    int opcIngredientes =-1;
    int idGlobalClientes=0;
    int idGlobalPizza=0;
    int idGlobalIngredientes=0;
    int idGlobalPedidos = 0;
    int opcCliente=-1;
    int opcPizza=-1;
    int idIngredienteAListar=0;
    char ingredienteAProcurar[150];
    int i,j,z;  
    
	//Vamos inserir por base os seguintes ingredientes
    array_Ingredientes[0].id_ingrediente = 1; 
    array_Ingredientes[0].quantidade = 35; 
    strcpy(array_Ingredientes[0].nome,"Queijo");
    
    
    array_Ingredientes[1].id_ingrediente = 2;
    array_Ingredientes[1].quantidade = 20; 
    strcpy(array_Ingredientes[1].nome,"Bacon");
    
    array_Ingredientes[2].id_ingrediente = 3;
    array_Ingredientes[2].quantidade = 10; 
    strcpy(array_Ingredientes[2].nome,"Fiambre");
    
    array_Ingredientes[3].id_ingrediente = 4;
    array_Ingredientes[3].quantidade = 30; 
    strcpy(array_Ingredientes[3].nome,"Cogumelos");
    
    array_Ingredientes[4].id_ingrediente = 5;
    array_Ingredientes[4].quantidade = 40; 
    strcpy(array_Ingredientes[4].nome,"Pimentos");
    
    array_Ingredientes[5].id_ingrediente = 6;
    array_Ingredientes[5].quantidade = 30; 
    strcpy(array_Ingredientes[5].nome,"Chourico");
	
	array_Ingredientes[6].id_ingrediente = 7;
    array_Ingredientes[6].quantidade = 30; 
    strcpy(array_Ingredientes[6].nome,"Atum");
    
    array_Ingredientes[7].id_ingrediente = 8;
    array_Ingredientes[7].quantidade = 30; 
    strcpy(array_Ingredientes[7].nome,"Frango");
	
	array_Ingredientes[8].id_ingrediente = 9;
    array_Ingredientes[8].quantidade = 24; 
    strcpy(array_Ingredientes[8].nome,"Pepperoni");
	
	array_Ingredientes[9].id_ingrediente = 10;
    array_Ingredientes[9].quantidade = 19; 
    strcpy(array_Ingredientes[9].nome,"Azeitonas");
    
    array_Ingredientes[10].id_ingrediente = 11;
    array_Ingredientes[10].quantidade = 25; 
    strcpy(array_Ingredientes[10].nome,"Ananas");
	
	array_Ingredientes[11].id_ingrediente = 12;
    array_Ingredientes[11].quantidade = 29; 
    strcpy(array_Ingredientes[11].nome,"Camarao");
			
    idGlobalIngredientes = 12;
    
    //Vamos inserir por base os seguintes clientes
    
    array_Clientes[0].id_cliente = 1;
    strcpy(array_Clientes[0].mail,"passos.coelho@psd.pt");
    strcpy(array_Clientes[0].morada,"Rua do Clero N.3 R/C ESQ, 4935-001 Viana do Castelo");
    strcpy(array_Clientes[0].nif,"556622014");
    strcpy(array_Clientes[0].nome,"Pedro Passos Coelho");
    strcpy(array_Clientes[0].telemovel,"933478478");
    idGlobalClientes = 1;
    
     array_Clientes[1].id_cliente = 2;
    strcpy(array_Clientes[1].mail,"maria_leal@hotmail.com");
    strcpy(array_Clientes[1].morada,"Rua Antonio Saraiva, N.27b, 4935-541 Viana do Castelo");
    strcpy(array_Clientes[1].nif,"341214441");
    strcpy(array_Clientes[1].nome,"Maria Leal");
    strcpy(array_Clientes[1].telemovel,"912124121");
    idGlobalClientes = 2;
    
     array_Clientes[2].id_cliente = 3;
    strcpy(array_Clientes[2].mail,"smendes@gmail.com");
    strcpy(array_Clientes[2].morada,"Praceta Raul Borges N.12 4DIR, 4935-323 Viana do Castelo");
    strcpy(array_Clientes[2].nif,"225588774");
    strcpy(array_Clientes[2].nome,"Shawn Mendes");
    strcpy(array_Clientes[2].telemovel,"963852741");
    idGlobalClientes = 3;
    
    array_Clientes[3].id_cliente = 4;
    strcpy(array_Clientes[3].mail,"mail1");
    strcpy(array_Clientes[3].morada,"morada1");
    strcpy(array_Clientes[3].nif,"123456789");
    strcpy(array_Clientes[3].nome,"nome1");
    strcpy(array_Clientes[3].telemovel,"123456789");
    idGlobalClientes = 4;

    //Vamos inserir por base as seguintes Pizzas
    array_Pizzas[0].estado = 1;
    array_Pizzas[0].id_pizza = 1;
    strcpy(array_Pizzas[0].nome,"Pizza a tua maneira");
    array_Pizzas[0].preco = 6.99;
    array_Pizzas[0].totalIng = 0;
   
    array_Pizzas[1].estado = 1;
    array_Pizzas[1].id_pizza = 2;
    strcpy(array_Pizzas[1].nome ,"Pizza Marguerita");
    array_Pizzas[1].preco = 10;
    array_Pizzas[1].totalIng = 2;
	array_Pizzas[1].ing[0][0] =2;
    array_Pizzas[1].ing[0][1] =2;
    array_Pizzas[1].ing[1][0] =0;
    array_Pizzas[1].ing[1][1] =1;
    
    
	array_Pizzas[2].estado = 1;
    array_Pizzas[2].id_pizza = 3;
   	strcpy( array_Pizzas[2].nome ,"Pizza Funny Bacon");
    array_Pizzas[2].preco = 12;
    array_Pizzas[2].totalIng = 3;
	array_Pizzas[2].ing[0][0] =1;
    array_Pizzas[2].ing[0][1] =1;
	array_Pizzas[2].ing[1][0] =1;
    array_Pizzas[2].ing[1][1] =1;
    array_Pizzas[2].ing[2][0] =0;
    array_Pizzas[2].ing[2][1] =1;
    
    array_Pizzas[3].estado = 1;
    array_Pizzas[3].id_pizza = 4;
    strcpy(array_Pizzas[3].nome ,"Pizza Avatar");
    array_Pizzas[3].preco = 14;
    array_Pizzas[3].totalIng = 4;
    array_Pizzas[3].ing[0][0] =8;
    array_Pizzas[3].ing[0][1] =1;
    array_Pizzas[3].ing[1][0] =3;
    array_Pizzas[3].ing[1][1] =1;
    array_Pizzas[3].ing[2][0] =6;
    array_Pizzas[3].ing[2][1] =1;
    array_Pizzas[3].ing[3][0] =10;
    array_Pizzas[3].ing[3][1] =1;
    
    array_Pizzas[4].estado = 1;
    array_Pizzas[4].id_pizza = 5;
    strcpy(array_Pizzas[4].nome ,"Pizza Havaiana");
    array_Pizzas[4].preco = 12;
    array_Pizzas[4].totalIng = 3;
    array_Pizzas[4].ing[0][0] =6;
    array_Pizzas[4].ing[0][1] =1;
    array_Pizzas[4].ing[1][0] =10;
    array_Pizzas[4].ing[1][1] =1;
    array_Pizzas[4].ing[2][0] =11;
    array_Pizzas[4].ing[2][1] =2;
    
    
    array_Pizzas[5].estado = 1;
    array_Pizzas[5].id_pizza = 6;
    strcpy(array_Pizzas[5].nome ,"Pizza de Atum");
    array_Pizzas[5].preco = 10;
    array_Pizzas[5].totalIng = 2;
    array_Pizzas[5].ing[0][0] =6;
    array_Pizzas[5].ing[0][1] =2;
    array_Pizzas[5].ing[1][0] =9;
    array_Pizzas[5].ing[1][1] =1;
	
	array_Pizzas[6].estado = 1;
    array_Pizzas[6].id_pizza = 7;
    strcpy(array_Pizzas[6].nome ,"Pizza Americana");
    array_Pizzas[6].preco = 14;
    array_Pizzas[6].totalIng = 4;
    array_Pizzas[6].ing[0][0] =3;
    array_Pizzas[6].ing[0][1] =2;
    array_Pizzas[6].ing[1][0] =4;
    array_Pizzas[6].ing[1][1] =1;
    array_Pizzas[6].ing[2][0] =5;
    array_Pizzas[6].ing[2][1] =1;
    array_Pizzas[6].ing[3][0] =8;
    array_Pizzas[6].ing[3][1] =1;
	
	idGlobalPizza = 7;
    	

    do{ //MENU PRINCIPAL
    	system ("cls"); 
    	opc = menu();
        switch(opc){
        	system ("cls");
            case 1:
                do{
                	system("cls");
                	opcAdmin=menuAdmin(); //Aqui vamos fazer a parte de gerir a Pizzaria[ADMIN]
					switch(opcAdmin){
					system("cls");
					case 1: 	
						do{
							system("cls");	
							opcCliente=menuCliente();
							switch(opcCliente){
							system("cls");
									
							case 1: 
								system("cls");
								idGlobalClientes+=inserirCliente(array_Clientes, idGlobalClientes);
								break;
									
							case 2:
									system("cls");
								listarTodosClientes(array_Clientes, idGlobalClientes);
								printf("\nCarregue em ENTER para sair da listagem\n");
								getchar();
								break;
									
							case 3:
								// Listar informacoes sobre os clientes que pediram uma determinada pizza
								informacoesClientesPizza(array_Pedidos,idGlobalPedidos,array_Pizzas, idGlobalPizza,array_Clientes, idGlobalClientes);
								break;
									
							case 4:
								informacoesClientesPizzaMaisCara(array_Pedidos,idGlobalPedidos,array_Pizzas, idGlobalPizza,array_Clientes, idGlobalClientes);
								break;
									
							default:
								break;
							}	
						}while(opcCliente!=0); //fecha o "do" menuCliente
						break; //break case 1 menuAdmin
						
						case 2:
							do{
								system("cls");
								opcPizza=menuPizza();
								switch(opcPizza){
		
							case 1:
										system ("cls");
								idGlobalPizza+=addPizza(array_Pizzas, idGlobalPizza,array_Ingredientes,idGlobalIngredientes);
									break;
									
								case 2:
									listarPizzas(array_Pizzas, idGlobalPizza,array_Ingredientes,idGlobalIngredientes);
									getchar();
									break;
									
								case 3:
									mudarEstadoPizza(array_Pizzas,idGlobalPizza);
							 	break;
									
								case 4:
									pizzaMaisCara(array_Pizzas, idGlobalPizza);
									break;
									
								case 5:
									pizzaAdicionarValor(array_Pizzas,idGlobalPizza);
								
									break;
									
								case 6:
									pizzaAdicionarValorIngrediente(array_Pizzas, idGlobalPizza,array_Ingredientes,idGlobalIngredientes);
									
									break;
									
								case 7:
								printTotalRecebido(array_Pedidos,idGlobalPedidos);
									break;
								
								case 8:
									pizzasMaisVendidas(array_Pedidos,idGlobalPedidos,array_Pizzas, idGlobalPizza);
									break;
								
									default:
										break;
							}
						}while(opcPizza!=0);
						break;  //break case 2 menuAdmin
							
					case 3:
                    	do{  
					        system("cls");
                    		opcIngredientes=menuIngredientes();
                    		system("cls");
							switch(opcIngredientes){
	                    case 1: 
                    	    //caso consiga introduzir o ingrediente vai dedolver 1 , senão devolve 0
                   	    	idGlobalIngredientes = idGlobalIngredientes +  addIngrediente(array_Ingredientes,idGlobalIngredientes);
                        	break;
             
			            case 2:
			            	system("cls");
                        	listarIngredientes(array_Ingredientes,idGlobalIngredientes);
                        	printf("Pressione ENTER para fechar a listagem\n");
            				getchar();
			            	break;
             
			            case 3:
			            	system ("cls");
                    	    printf("Introduza o ingrediente que deseja pesquisar:");
                            gets(ingredienteAProcurar);
                            idIngredienteAListar = checkIngrediente(array_Ingredientes,idGlobalIngredientes,ingredienteAProcurar);
                            if(idIngredienteAListar != -1){
                 	           listarIngrediente(array_Ingredientes,idIngredienteAListar);
                            }else{
                       			printf("Nao existe nenhum ingrediente com esse nome.\n");
                            }
                            printf("Pressione ENTER para fechar a listagem\n");
            				getchar();
                        	break;
                    
					    case 4:
        					printf("Ingredientes com o menor stock:\n ");
                       		listarIngredientesMenorStock(array_Ingredientes,idGlobalIngredientes); 
                       			getchar();
                       		break;
       
                        case 5:
                      	 	printf("Ingredientes sem stock:\n");
                          	listarIngredientesSemStock(array_Ingredientes,idGlobalIngredientes);
                          		getchar();
                     		break;
                     	
                     	case 6:
                     		alterarQuantidadeIngrediente(array_Ingredientes,idGlobalIngredientes);
                     		break;
                     	default:
                     		
                     		break;
                         }
                        }while(opcIngredientes != 0);
                        break;//break case 3 menuAdmin
                        
                        default:
                        	break;
                	}
                }while(opcAdmin!=0);
        		break;	//break MA
        	//case 2 do PRIMEIRO MENU
            case 2:
            
            	do{
            		system ("cls");
                	opcEncomendas=menuEncomendas(); //Aqui vamos fazer a parte de fazer encomendas[CLIENTE]
					switch(opcEncomendas){
						case 1: 
						idGlobalPedidos += addPedido(array_Pedidos,idGlobalPedidos,array_Pizzas,idGlobalPizza,array_Ingredientes,idGlobalIngredientes,array_Clientes, idGlobalClientes);
						
						getchar();
							break;
			
						case 2: 
							alteraEstadoPedido(array_Pedidos,idGlobalPedidos,array_Pizzas,idGlobalPizza,array_Ingredientes,idGlobalIngredientes,array_Clientes, idGlobalClientes);
								
						 
							break;
						
						case 3:  
					listarTodosPedidos(array_Pedidos,idGlobalPedidos,array_Pizzas,idGlobalPizza,array_Ingredientes,idGlobalIngredientes,array_Clientes, idGlobalClientes);
									break;
			
						case 4: 
					listarTodosPedidosNEntregues(array_Pedidos,idGlobalPedidos,array_Pizzas,idGlobalPizza,array_Ingredientes,idGlobalIngredientes,array_Clientes, idGlobalClientes);
							
						
							break;
						default:
							break;
					}	
                }while(opcEncomendas!=0);
                break;
			
			default:
				break;      
    
}
    }while(opc!=0);
    return 0;
}
