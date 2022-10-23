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

// 4 - Faça uma função que recebe um vetor com 4 posições que contém
// o valor da distância de um pequeno robô até cada um dos seus 4 lados.
// A função deve retornar duas informações: A primeira é a direção
// de maior distância ("Direita", "Esquerda", "Frente", "Tras") e a
// segunda é esta maior distância.
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

// 5 - Faça uma função que pergunta ao usuário se ele deseja continuar o mapeamento e
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

// 6 - A função abaixo (que está incompleta) vai "dirigindo" virtualmente um robô
// e através de 4 sensores em cada um dos 4 pontos do robo ("Direita", "Esquerda",
// "Frente", "Tras").
//      A cada passo, ele verifica as distâncias aos objetos e vai mapeando o terreno
// para uma movimentação futura.
//      Ele vai armazenando estas distancias em um vetor fornecido como parâmetro
// e retorna a ultima posicao preenchida do vetor.
//      Esta função deve ir lendo os 4 sensores até que um comando de pare seja enviado
//
//      Para simular os sensores e os comandos de pare, use as funções já construídas
// nos ítens anteriores e em um looping contínuo até que um pedido de parada seja
// enviado pelo usuário.
//
//      Complete a função com a chamada das funções já criadas
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

// O trecho abaixo irá utilizar as funções acima para ler os sensores e o movimento
// do robô e no final percorrer o vetor e mostrar o movimento a cada direção baseado 
// na maior distância a cada movimento
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