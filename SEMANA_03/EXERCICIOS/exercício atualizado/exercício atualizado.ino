#define LED_1 36  // yellow LED
#define LED_2 40  // white LED
#define LED_3 2   // red LED
#define LED_4 1   // green LED
#define LED_control 9
#define LDR_Reader 4    // leitor LDR
#define SPEAKER_PIN 38  // buzzer
#define SAVE_BUTTON 5   // botão de guardar
#define PLAY_BUTTON 7   // botão de tocar

// vetor dos leds que acenderão
const int LEDS[4] = { LED_1, LED_2, LED_3, LED_4 };

// Variables
#define time 500
float lux;             // luminosidade que será calculada
int relLux;            // variável que receberá o valor relativo da luminosidade
int binaryForm;        // variável que receberá a forma binária do valor relativo
int musicVector[100];  // vetor com os valores relativos para tocar a música
int counter = 0;       // contador que garante não sobrescrever a nota atual

// LDR Control variables
const float maxLux = 10000;
const float minLux = 100;

// LDR Characteristics
const float GAMMA = 0.7;
const float RL10 = 50;


void setup() {
  Serial.begin(115200);
  pinMode(LDR_Reader, INPUT);
  for (int i = 0; i < 4; i++) { pinMode(LEDS[i], OUTPUT); }
  pinMode(LED_control, OUTPUT);
  pinMode(SAVE_BUTTON, INPUT_PULLUP);
  pinMode(PLAY_BUTTON, INPUT_PULLUP);
}

// função que calcula a luminosidade baseado nas saídas analógicas
float calculateLux() {
  int analogValue = analogRead(LDR_Reader);
  float voltage = analogValue / 1024. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);

  return pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
}

// função que recebe uma luminosidade e converte ela para
// um valor decimal entre 0 e 15
int relativeLux(float lumin) {
  float luminConvert = 0.;
  if (lumin > maxLux) {
    return 15;
  } else if (lumin < minLux) {
    return 0;
  } else {
    luminConvert = (lumin - minLux) / (maxLux - minLux);
    return luminConvert * 15;
  }
}

// função que recebe um número decimal e
// retorna um número decimal em formato binário
// ex.: 13 --> 1101
int binaryReturn(int decimal) {
  int binary = 0;
  int remainder;
  int product = 1;

  while (decimal != 0) {
    remainder = decimal % 2;
    binary = binary + (remainder * product);
    decimal = decimal / 2;
    product *= 10;
  }
  return binary;
}

// função que fica printando log's no formato
// lux --> relLux --> binaryForm --> split binary
void logPrints(float lux, int relLux, int binaryForm) {
  Serial.print(lux);
  Serial.print(" --> ");
  Serial.print(relLux);
  Serial.print(" --> ");
  Serial.print(binaryForm);
  Serial.print(" --> ");

  Serial.print(binaryForm % 10);
  Serial.print(" ");
  Serial.print(binaryForm / 10 % 10);
  Serial.print(" ");
  Serial.print(binaryForm / 100 % 10);
  Serial.print(" ");
  Serial.print(binaryForm / 1000 % 10);
  Serial.print("\n");
}

// função que desliga todos os LEDS musicais
void lightsOff() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(LEDS[i], LOW);
  }
}

// função que liga todos os leds de acordo com o ledsVector
void lightsOn(int *vector) {
  for (int i = 0; i < 4; i++) {
    if (vector[i] == 1) {
      digitalWrite(LEDS[i], HIGH);
    }
  }
}

// função que coloca os valores da forma binária no ledsVector
void pseudoSplit(int *vector, int binaryForm) {
  vector[0] = (binaryForm) % 10;         // 1101 % 10 = 1
  vector[1] = (binaryForm / 10) % 10;    // 110 % 10 = 0
  vector[2] = (binaryForm / 100) % 10;   // 11 % 10 = 1
  vector[3] = (binaryForm / 1000) % 10;  // 1 % 10 = 1
}

// loop padrão
void loop() {
  int ledsVector[4];                  // definição do vetor dos leds que serão acesos
  lux = calculateLux();               // cálculo da luminosidade baseado no que o leitor LDR ler
  relLux = relativeLux(lux);          // transformação dessa luminosidade para um valor entre 0 e 15
  binaryForm = binaryReturn(relLux);  // forma binária do valor relativo

  // agora tocamos a música e ligamos os leds baseado na luminosidade lida atualmente
  pseudoSplit(ledsVector, binaryForm);       // primeiro definimos quais leds serão ativados (isso é feito direto no vetor de leds)
  tone(SPEAKER_PIN, relLux * 70, time * 3);  // depois tocamos a música por 1.5 segundos
  lightsOn(ledsVector);                      // enquanto isso os leds acendem
  delay(time * 3);

  // confirmação dos botões
  digitalWrite(LED_control, HIGH);  // depois de 1.5 segundos, o led azul acende para sinalizar que um botão pode ser apertado
  lightsOff();                       // e os outros leds são desligados
  delay(time);                        // isso dura 0.5 segundos
  digitalWrite(LED_control, LOW);   // o led azul desliga e os botões são verificados

  // verificação dos botões
  // Save Button: salva a luminosidade relativa atual e armazena seu valor no vetor musical
  if (digitalRead(SAVE_BUTTON) == 0) {
    delay(time);                        // espera 0.5 segundos
    digitalWrite(LED_control, HIGH);  // liga o led azul pra mostrar que está "trabalhando nisso"
    musicVector[counter] = relLux;     // armazena a luminosidade relativa
    counter += 1;                      // aumenta o contador para a próxima vez armazenar no espaço seguinte

    // finalização da operação
    digitalWrite(LED_control, LOW);     // desliga o led azul para dizer que a operação de armazenamento acabou
    logPrints(lux, relLux, binaryForm);  // printa a a forma da música atual
  }

  // Play Button: para o loop para tocar a música criada com os valores armazenados e reinicia o vetor e o contador
  if (digitalRead(PLAY_BUTTON) == 0) {
    delay(time/2);  // espera de segurança
    for (int thisNote = 0; musicVector[thisNote] != 0; thisNote++) {
      digitalWrite(LED_control, HIGH);  // liga o led azul para tocar uma nota
      // prints para ajudar a visualização
      Serial.println("Tocando a musica: ");
      logPrints(0, relLux, binaryForm);

      pseudoSplit(ledsVector, binaryReturn(musicVector[thisNote]));  // separa os leds que serão acesos
      tone(SPEAKER_PIN, musicVector[thisNote] * 70, time * 2);       // toca a musica por 1 segundos
      lightsOn(ledsVector);                                          // enquanto isso acende os leds
      delay(time);                                                   // antes da musica acabar apaga os leds
      lightsOff();
      delay(time);
      // depois da musica acabar
      musicVector[thisNote] = 0;        // apaga o valor atual armazenado
      digitalWrite(LED_control, LOW);  // desliga o led azul
      delay(time * 2);                  // espera 1 segundo para a próxima nota
    }
    // depois de todas as notas, reseta o contador
    counter = 0;
  }
}

/*
  Construa um programa que seja capaz de ler os dados de luz de
  um sensor LDR, converter o valor lido em úm número de 0 a 15 e use esse valor
  para acender 4 LEDs, cada um para um dígito binário que irá corresponder a
  conversão do número (0 a 15).
  Além disso:
  1 - Faça uma função que, a cada novo número medido, faça
  a contagem binária nos leds, pausadamente, de 0 até o número medido.
  2 - Acrescente "toque" usando um Buzzer que corresponda
  a uma nota diferente para cada número. Ao contar de 0 até o
  número, o buzzer irá tocando todas as notas correspondentes.
  3 - Acrescente um "push button", tal que, ao ser pressionado,
  armazene o valor medido em um vetor.
  4 - Acrescente um SEGUNDO push button que, ao ser pressionado,
  toque todos os tons correspondentes ao número armazenado.
  5 - Ponto de desafio: Acrescente um display ao seu sistema que
  vá indicando as medidas que vao ocorrendo, assim como as notas tocadas.

  A - Utilize a interface IDE 2.0 do Arduino para construcao do codigo
  B - Monte o respectivo projeto e faça o funcionar.
  C - Faça uma versão simulada no Wowki
*/