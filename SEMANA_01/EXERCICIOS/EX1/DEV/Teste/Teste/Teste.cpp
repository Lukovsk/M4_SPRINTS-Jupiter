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

// Fun��o que altera o valor da maior dist�ncia e retorna o sentido que aponta essa dist�ncia
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

// Fun��o que verifica se o mapeamento continua ou n�o
bool ler() {
	char continuar;

	cout << "Deseja continuar com o mapeamento? Y/N" << endl;
	cin >> continuar;
	
	if (toupper(continuar) == 'N') {
		return false;
	}
	return true;
}

// Fun��o que l� os sensores, muda o vetor global e retorna a quantidade de vezes que o sensor foi lido
int dirige(int* vetorMov, int maxVetor) {
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

// fun��o que "anda o robo", verificando o tamanho percorrido para ver quantas vezes os sensores leram e retornando o quanto ele andou em cada itera��o
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