int LDR = 0; //Define a variável LDR como 0.
int VAL = 0; //Define a variável VAL iniciado em 0
 
void setup(){
  Serial.begin(9600); //Velocidade da porta serial
}
 
void loop(){
 
 VAL = analogRead(LDR); /*Faz leitura do pino analogico 0
                          e armazena na variável VAL. */           
 
 Serial.print(VAL); //Manda o valor lido para a porta serial
 
 if (VAL<512)
 {
  Serial.println(": Escuro"); //Escreva no serial Monitor
 }
 else if(VAL > 700)
{
Serial.println(": Claro ");
}
 
  delay(500); //Aguarda meio segundo
 
}
 
