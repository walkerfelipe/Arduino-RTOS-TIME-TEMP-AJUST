#include <Arduino_FreeRTOS.h>
#include <LiquidCrystal.h>
//https://create.arduino.cc/projecthub/feilipu/using-freertos-multi-tasking-in-arduino-ebc3cc

// define two tasks for Blink & AnalogRead
void TaskConta( void *pvParameters );
void TaskAnalogRead( void *pvParameters );
void TaskDisplay( void *pvParameters );
void TaskBotao( void *pvParameters );

// Variaveis Globais para contagem do relogio
int segundos = 57, minutos = 59, horas = 22;
const int LM35 = A0; // Define o pino que lera a saída do LM35
float temperatura; // Variável que armazenará a temperatura medida
int buttonState = 0; // salva estado do botão
const int buttonPin1 = 0; // botão de ajuste do relogio
const int buttonPin2 = 1; // botão de ajuste do relogio

// Pinos usado pelo ldc
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
   pinMode(buttonPin1, INPUT);
   pinMode(buttonPin2, INPUT);
  //Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);
  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskConta
    ,  (const portCHAR *)"Conta"   // A name just for humans
    ,  128  // Tamanho da pilha
    ,  NULL
    ,  1  // Prioridade
    ,  NULL );
  

  xTaskCreate(
    TaskAnalogRead
    ,  (const portCHAR *) "AnalogRead"
    ,  128 
    ,  NULL
    ,  2  // Prioridade
    ,  NULL );
  xTaskCreate(
    TaskDisplay
    ,  (const portCHAR *)"Display"   
    ,  128  // Tamanho da pilha
    ,  NULL
    ,  3  // prioridade
    ,  NULL );
 xTaskCreate(
    TaskBotao
    ,  (const portCHAR *)"Botao"   
    ,  128  // Tamanho da pilha
    ,  NULL
    ,  3  // prioridade
    ,  NULL );
 // Agora, o agendador de tarefas, que assume o controle do agendamento de tarefas individuais, 
 // é iniciado automaticamente.
}
void atualizatela(){
  lcd.clear();
  //Posiciona o cursor na coluna 3, linha 0;
  lcd.setCursor(3, 0);
  //Envia o texto entre aspas para o LCD
  if ( horas < 10)
    lcd.print("0");
  lcd.print(horas);
  lcd.print(":");
  if ( minutos < 10)
    lcd.print("0");
  lcd.print(minutos);
  lcd.print(":");
  if ( segundos < 10)
    lcd.print("0");
  lcd.print(segundos);
  //lcd.print("Temperatura: ");
  lcd.setCursor(3, 1);
 lcd.print(temperatura);
  }

void contaseg()
{
  if (segundos < 59)segundos++;
  else if (minutos < 59) {
    minutos++;
    segundos = 0;
  }
  else if (horas < 23) {
    horas++;
    minutos = 0;
    segundos = 0;
  }
  else {
    segundos = 0;
    minutos = 0;
    horas = 0;
  }


}


void contamin()
{
  if (minutos < 59) {
    minutos++;
    
  }
  else if (horas < 23) {
    horas++;
    minutos = 0;
    segundos = 0;
  }
  else {
    segundos = 0;
    minutos = 0;
    horas = 0;
  }
}


void contahora()
{

  if (horas < 23) {
    horas++;
  }
  else {
    segundos = 0;
    minutos = 0;
    horas = 0;
  }
}
void loop()
{
  // As tarefas serão realizadas pelo agendador de tarefas
  // aqui fica vazio
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskConta(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  for (;;) // A Task shall never return or exit.
  {
  //função de contagem do relogio
  contaseg(); 
  vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}

void TaskAnalogRead(void *pvParameters)  // Task 1 Leitura da temperatura
{
  (void) pvParameters;
  for (;;)
  {   
    temperatura = (float(analogRead(LM35)) * 5 / (1023)) / 0.01;
    vTaskDelay(10);  // Delay de (15ms) entre leituras para gerar estabilidade.
  }
}
void TaskDisplay(void *pvParameters)  // Task 2 Atualizar tela
{
  (void) pvParameters;
  for (;;)
  {
     lcd.clear();
  //Posiciona o cursor na coluna 3, linha 0;
  lcd.setCursor(3, 0);
  //Envia o texto entre aspas para o LCD
  if ( horas < 10)
    lcd.print("0");
  lcd.print(horas);
  lcd.print(":");
  if ( minutos < 10)
    lcd.print("0");
  lcd.print(minutos);
  lcd.print(":");
  if ( segundos < 10)
    lcd.print("0");
  lcd.print(segundos);
  //lcd.print("Temperatura: ");
  lcd.setCursor(3, 1);
  lcd.print(temperatura);
   vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
    
  }}

void TaskBotao(void *pvParameters)  // Leitura do botão
{
  (void) pvParameters;
  for (;;) // A Task shall never return or exit.
  {
   buttonState = digitalRead(buttonPin1);
  if (buttonState == LOW) {
    contamin();
    atualizatela();
    vTaskDelay(10);
  }
  buttonState = digitalRead(buttonPin2);
  if (buttonState == LOW) {
    contahora();
    atualizatela();
    vTaskDelay(10);
  }
 vTaskDelay(1); 
  }
}

  

