#include <ArduTFLite.h>

extern const unsigned char model[];
extern const unsigned int model_data_len;

// LEDs
#define LED_VERDE 18
#define LED_VERMELHO 19

// Memória usada pelo TensorFlow Lite Micro
constexpr int kTensorArenaSize = 8*1024;

alignas(16) uint8_t tensor_arena[kTensorArenaSize];

void setup() {

  Serial.begin(115200);
  delay(1000);

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);

  Serial.println();
  Serial.println("=== TinyML ESP32 ===");
  Serial.println("Inicializando modelo...");

  if (!modelInit(model, tensor_arena, kTensorArenaSize)) {
    Serial.println("ERRO: falha ao inicializar o modelo!");
    while (true);
  }

  Serial.println("Modelo inicializado com sucesso!");

  Serial.print("Tamanho do modelo: ");
  Serial.print(model_data_len);
  Serial.println(" bytes");

  Serial.println();
  Serial.println("Executando teste TinyML...");
}

void loop() {

  // Teste 1: valor que deve pertencer à classe 0
  float entrada = 0.2;

  modelSetInput(entrada, 0);

  if (!modelRunInference()) {
    Serial.println("ERRO: falha na inferencia!");
    return;
  }

  float resultado = modelGetOutput(0);

  Serial.print("Entrada: ");
  Serial.print(entrada);
  Serial.print(" | Saida: ");
  Serial.println(resultado, 4);

  if (resultado < 0.5) {

    Serial.println("Classe 0 -> LED VERDE");

    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_VERMELHO, LOW);

  } else {

    Serial.println("Classe 1 -> LED VERMELHO");

    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, HIGH);
  }

  delay(3000);

  // Teste 2: valor que deve pertencer à classe 1
  entrada = 0.7;

  modelSetInput(entrada, 0);

  if (!modelRunInference()) {
    Serial.println("ERRO: falha na inferencia!");
    return;
  }

  resultado = modelGetOutput(0);

  Serial.print("Entrada: ");
  Serial.print(entrada);
  Serial.print(" | Saida: ");
  Serial.println(resultado, 4);

  if (resultado < 0.5) {

    Serial.println("Classe 0 -> LED VERDE");

    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_VERMELHO, LOW);

  } else {

    Serial.println("Classe 1 -> LED VERMELHO");

    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, HIGH);
  }

  delay(3000);
}