//Programa : Teste Modulo Rele Arduino 2 canais - Lampadas
//Autor : JuniorEC
 
//Porta ligada ao pino IN1 do modulo
int porta_rele1 = 7;

 
void setup()
{
  //Define pinos para o rele como saida
  pinMode(porta_rele1, OUTPUT); 
  
}
  
void loop()
{
  digitalWrite(porta_rele1, LOW);  //Liga rele 1
  delay(2000);
  digitalWrite(porta_rele1, HIGH); //Desliga rele 1
  delay(2000);
}
