#include <iostream>
#include <string>
using namespace std;

// Função que recebe uma medida e coloca ela percentualmente entre dois outros valores inputados
double medidaRelativa(double medida, double min, double max) {
	double porcentagem{ 0 }; 
	if ((max - min) != 0) { 
		porcentagem = (medida - min) / (max - min);
	}
	return porcentagem*100;
}

// Função que capta algum número do sensor
int captaSensor(string direção) {
	int numero;		// cria uma variável
	cout << "Digite um numero para o sensor " << direção << ": ";
	cin >> numero;	// armazena o valor da variável no que alguém digitou
	return numero;
}

// Função que e armazena um valor em uma posição vetorial e retorna a próxima posição de um vetor
int append(int valor, int tamanho, int posição, int* vetor) {
	if (posição < tamanho) {		// verifica se é possível fazer a alteração
		vetor[posição] = valor;		// coloca o valor na posição desejada
		return posição + 1;			// o retorno é a posição seguinte
	}
	return posição;				// posição inalterada
}

// Função que altera o valor da maior distância e retorna o sentido que aponta essa distância
const char* menorCaminho(int* vetor, int* distância) {

	const char* sentidos[] = { "Direita", "Esquerda", "Frente", "Tras" };

	int sentido = 0;							// sentido escolhido
	int maior = vetor[0];						// maior valor do vetor

	// percorre os quatro sentidos
	for (int i = 0; i < 4; i++) {
		if (vetor[i] > maior) {					// verifica se o valor do vetor é o maior
			maior = vetor[i];					
			sentido = i;
		}
	}
	*distância = maior;							// faz o ponteiro apontar a distância pro maior valor
	return sentidos[sentido];					// retorna o sentido da maior distância
}

// Função que verifica se o mapeamento continua ou não
bool ler() {
	char continuar;

	cout << "Deseja continuar com o mapeamento? Y/N" << endl;
	cin >> continuar;
	
	if (toupper(continuar) == 'N') {
		return false;
	}
	return true;
}

// Função que lê os sensores, muda o vetor global e retorna a quantidade de vezes que o sensor foi lido
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

// função que "anda o robo", verificando o tamanho percorrido para ver quantas vezes os sensores leram e retornando o quanto ele andou em cada iteração
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