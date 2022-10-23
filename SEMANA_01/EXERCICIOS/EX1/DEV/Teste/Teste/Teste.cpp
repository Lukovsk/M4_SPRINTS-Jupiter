#include <iostream>
#include <string>
using namespace std;

// Fun��o que recebe uma medida e coloca ela percentualmente entre dois outros valores inputados
double medidaRelativa(double medida, double min, double max) {
	double porcentagem{ 0 }; 
	if ((max - min) != 0) { 
		porcentagem = (medida - min) / (max - min);
	}
	return porcentagem*100;
}

// Fun��o que capta algum n�mero do sensor
int captaSensor(string dire��o) {
	int numero;		// cria uma vari�vel
	cout << "Digite um numero para o sensor " << dire��o << ": ";
	cin >> numero;	// armazena o valor da vari�vel no que algu�m digitou
	return numero;
}

// Fun��o que e armazena um valor em uma posi��o vetorial e retorna a pr�xima posi��o de um vetor
int append(int valor, int tamanho, int posi��o, int* vetor) {
	if (posi��o < tamanho) {		// verifica se � poss�vel fazer a altera��o
		vetor[posi��o] = valor;		// coloca o valor na posi��o desejada
		return posi��o + 1;			// o retorno � a posi��o seguinte
	}
	return posi��o;				// posi��o inalterada
}

// 4 - Fa�a uma fun��o que recebe um vetor com 4 posi��es que cont�m
// o valor da dist�ncia de um pequeno rob� at� cada um dos seus 4 lados.
// A fun��o deve retornar duas informa��es: A primeira � a dire��o
// de maior dist�ncia ("Direita", "Esquerda", "Frente", "Tras") e a
// segunda � esta maior dist�ncia.
const char* menorCaminho(int* vetor, int* dist�ncia) {

	const char* sentidos[] = { "Direita", "Esquerda", "Frente", "Tras" };

	int sentido = 0;							// sentido escolhido
	int maior = vetor[0];						// maior valor do vetor

	// percorre os quatro sentidos
	for (int i = 0; i < 4; i++) {
		if (vetor[i] > maior) {					// verifica se o valor do vetor � o maior
			maior = vetor[i];					
			sentido = i;
		}
	}
	*dist�ncia = maior;							// faz o ponteiro apontar a dist�ncia pro maior valor
	return sentidos[sentido];					// retorna o sentido da maior dist�ncia
}

// 5 - Fa�a uma fun��o que pergunta ao usu�rio se ele deseja continuar o mapeamento e
// retorna verdadeiro ou falso
bool ler() {
	char continuar;

	cout << "Deseja continuar com o mapeamento? Y/N" << endl;
	cin >> continuar;
	
	if (toupper(continuar) == 'N') {
		return false;
	}
	return true;
}

// 6 - A fun��o abaixo (que est� incompleta) vai "dirigindo" virtualmente um rob�
// e atrav�s de 4 sensores em cada um dos 4 pontos do robo ("Direita", "Esquerda",
// "Frente", "Tras").
//      A cada passo, ele verifica as dist�ncias aos objetos e vai mapeando o terreno
// para uma movimenta��o futura.
//      Ele vai armazenando estas distancias em um vetor fornecido como par�metro
// e retorna a ultima posicao preenchida do vetor.
//      Esta fun��o deve ir lendo os 4 sensores at� que um comando de pare seja enviado
//
//      Para simular os sensores e os comandos de pare, use as fun��es j� constru�das
// nos �tens anteriores e em um looping cont�nuo at� que um pedido de parada seja
// enviado pelo usu�rio.
//
//      Complete a fun��o com a chamada das fun��es j� criadas
int dirige(int* v, int maxv) {
	int maxVetor = maxv;
	int* vetorMov = v;
	int posAtualVetor = 0;
	bool dirigindo = true;

	string sentidos[] = { "Direita", "Esquerda", "Frente", "Tras" };

	while (dirigindo) {
		for (int f = 0; f < 4; f++) {
			posAtualVetor = append(medidaRelativa(captaSensor(sentidos[f]), 0, 830), maxVetor, posAtualVetor, vetorMov);
		}

		dirigindo = ler();
	}
	return posAtualVetor;
}

// O trecho abaixo ir� utilizar as fun��es acima para ler os sensores e o movimento
// do rob� e no final percorrer o vetor e mostrar o movimento a cada dire��o baseado 
// na maior dist�ncia a cada movimento
void percorre(int* v, int tamPercorrido) {
	int* vetorMov = v;
	int maiorDir = 0;

	for (int i = 0; i < tamPercorrido; i += 4) {
		const char* direcao = menorCaminho(&(vetorMov[i]), &maiorDir);
		cout << "Movimentando " << maiorDir << "% da distancia total para " << direcao << ". \n";
	}
}

int main(int argc, char** argv) {
	const int maxVetor = 100;
	int vetorMov[maxVetor * 4];
	int posAtualVet = 0;

	posAtualVet = dirige(vetorMov, maxVetor);
	percorre(vetorMov, posAtualVet);

	return 0;
}