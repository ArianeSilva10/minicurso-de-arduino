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
  Serial.println("Aguardando entrada de dados. Digite um numero no terminal...");
}

void loop() {
  // Verifica se há dados disponíveis no monitor serial
  if (Serial.available() > 0) {
    
    // Lê o número em formato float enviado pelo terminal
    float entrada = Serial.parseFloat();

    // Limpa o buffer do terminal para evitar leitura de 'Enter' (\n ou \r)
    while (Serial.available() > 0) {
      Serial.read();
    }

    Serial.println("\n---------------------------------");
    Serial.print("Entrada recebida: ");
    Serial.println(entrada);

    // Passa o valor recebido para o modelo
    modelSetInput(entrada, 0);

    // Executa a inferência
    if (!modelRunInference()) {
      Serial.println("ERRO: falha na inferencia!");
      return;
    }

    // Obtém o resultado da predição
    float resultado = modelGetOutput(0);

    Serial.print("Saida do modelo: ");
    Serial.println(resultado, 4);

    // Avalia o resultado e controla os LEDs
    if (resultado < 0.5) {
      Serial.println("Classe 0 -> LED VERDE");
      digitalWrite(LED_VERDE, HIGH);
      digitalWrite(LED_VERMELHO, LOW);
    } else {
      Serial.println("Classe 1 -> LED VERMELHO");
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_VERMELHO, HIGH);
    }
    
    Serial.println("Aguardando novo numero...");
  }
}