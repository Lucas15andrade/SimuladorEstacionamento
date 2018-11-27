#include <Fuzzy.h>
#include <FuzzyComposition.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzyOutput.h>
#include <FuzzyRule.h>
#include <FuzzyRuleAntecedent.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzySet.h>]
#include <Ultrasonic.h>
#include <Servo.h>

#define pino_trigger 12
#define pino_echo 13


//Instanciando um objeto da biblioteca
Fuzzy* fuzzy = new Fuzzy();
Ultrasonic ultrasonic(pino_trigger, pino_echo);
Servo servo;

int eixo = 30;
int angulo = 80;
const int w = 1;
long distancia;

int pino1 = 3;
int pino2 = 4;

int pino3 = 6;
int pino4 = 7;

int pino5 = 9;
int pino6 = 10;


void setup() {
  //Iniciando comunicação serial
  Serial.begin(115200);
  //randomSeed(analogRead(0));
  servo.attach(11);

  servo.write(0);

  pinMode(pino1, OUTPUT);
  pinMode(pino2, OUTPUT);
  pinMode(pino3, OUTPUT);
  pinMode(pino4, OUTPUT);
  pinMode(pino5, OUTPUT);
  pinMode(pino6, OUTPUT);

  analogWrite(pino5, 100);
  analogWrite(pino6, 100);

  digitalWrite(pino1, HIGH);
  digitalWrite(pino2, LOW);

  digitalWrite(pino3, HIGH);
  digitalWrite(pino4, LOW);


  //Criando um FuzzyInput de entrada
  //Posição X = Posição do veículo em relação ao eixo horizontal X [0 a 100]
  FuzzyInput* posicaoX = new FuzzyInput(1);

  //Partições nebulosas: Esquerda, CentroEsqueda, Centro, CentroDireita, Direita


  FuzzySet* esquerda = new FuzzySet(0, 0, 10, 35);
  posicaoX->addFuzzySet(esquerda);
  FuzzySet* centroEsquerda = new FuzzySet(30, 40, 40, 50);
  posicaoX->addFuzzySet(centroEsquerda);
  FuzzySet* centro = new FuzzySet(45, 50, 50, 55);
  posicaoX->addFuzzySet(centro);
  FuzzySet* centroDireita = new FuzzySet(50, 60, 60, 70);
  posicaoX->addFuzzySet(centroDireita);
  FuzzySet* direita = new FuzzySet(65, 90, 100, 100);
  posicaoX->addFuzzySet(direita);


  fuzzy->addFuzzyInput(posicaoX);

  //-------------------------------------------

  //Criando um FuzzyInput de entrada
  //Posição Y = Angulo do veículo [-90 a 270]
  FuzzyInput* anguloVeiculo = new FuzzyInput(2);

  //Partições nebulosas: inferiorDireito, superiorDireito, verticalDireito, vertical, verticalEsquerda, superiorEsqueda, inferiorEsquerda

  FuzzySet* inferiorDireito = new FuzzySet(-90, -45, -45, 15);
  anguloVeiculo->addFuzzySet(inferiorDireito);
  FuzzySet* superiorDireito = new FuzzySet(-15, 30, 30, 60);
  anguloVeiculo->addFuzzySet(superiorDireito);
  FuzzySet* verticalDireito = new FuzzySet(45, 67, 67, 90);
  anguloVeiculo->addFuzzySet(verticalDireito);
  FuzzySet* vertical = new FuzzySet(75, 90, 90, 105);
  anguloVeiculo->addFuzzySet(vertical);
  FuzzySet* verticalEsquerda = new FuzzySet(90, 112, 112, 135);
  anguloVeiculo->addFuzzySet(verticalEsquerda);
  FuzzySet* superiorEsquerda = new FuzzySet(120, 150, 150, 195);
  anguloVeiculo->addFuzzySet(superiorEsquerda);
  FuzzySet* inferiorEsquerda = new FuzzySet(165, 225, 225, 270);
  anguloVeiculo->addFuzzySet(inferiorEsquerda);


  fuzzy->addFuzzyInput(anguloVeiculo);

  //-------------------------------------------

  //Criando um FuzzyOutput de saída
  //anguloRoda
  FuzzyOutput* anguloRoda = new FuzzyOutput(1);

  //Partições nebulosas: negativoGrande, negativoMedio, negativoPequeno, zero, positivoPequeno, positivoMedio, positivoGrande

  FuzzySet* negativoGrande = new FuzzySet(-30, -30, -15, -15);
  anguloRoda->addFuzzySet(negativoGrande);
  FuzzySet* negativoMedio = new FuzzySet(-28, -15, -15, -5);
  anguloRoda->addFuzzySet(negativoMedio);
  FuzzySet* negativoPequeno = new FuzzySet(-10, -5, -5, 0);
  anguloRoda->addFuzzySet(negativoPequeno);
  FuzzySet* zero = new FuzzySet(-5, 0, 0, 5);
  anguloRoda->addFuzzySet(zero);
  FuzzySet* positivoPequeno = new FuzzySet(0, 5, 5, 10);
  anguloRoda->addFuzzySet(positivoPequeno);
  FuzzySet* positivoMedio = new FuzzySet(5, 15, 15, 25);
  anguloRoda->addFuzzySet(positivoMedio);
  FuzzySet* positivoGrande = new FuzzySet(15, 15, 30, 30);
  anguloRoda->addFuzzySet(positivoGrande);


  fuzzy->addFuzzyOutput(anguloRoda);

  //Conjuntos de entrada e saída criados.

  //--------------------------------------------

  //Base de Regras


  //Consequentes
  FuzzyRuleConsequent* pS = new FuzzyRuleConsequent();
  pS->addOutput(positivoPequeno);

  FuzzyRuleConsequent* pM = new FuzzyRuleConsequent();
  pM->addOutput(positivoMedio);

  FuzzyRuleConsequent* pB = new FuzzyRuleConsequent();
  pB->addOutput(positivoGrande);

  FuzzyRuleConsequent* zE = new FuzzyRuleConsequent();
  zE->addOutput(zero);

  FuzzyRuleConsequent* nS = new FuzzyRuleConsequent();
  nS->addOutput(negativoPequeno);

  FuzzyRuleConsequent* nM = new FuzzyRuleConsequent();
  nM->addOutput(negativoMedio);

  FuzzyRuleConsequent* nB = new FuzzyRuleConsequent();
  nB->addOutput(negativoGrande);


  //Se posição X = esquerda E anguloVeiculo = inferiorDireito ENTÃO angulo da roda = PositivoPequeno


  FuzzyRuleAntecedent* antecedente1 = new FuzzyRuleAntecedent();
  antecedente1->joinWithAND(esquerda, inferiorDireito);

  //Se posição X = esquerda E anguloVeiculo = inferiorDireito ENTÃO angulo da roda = PositivoPequeno
  FuzzyRule* rule1 = new FuzzyRule(1, antecedente1, pS);

  fuzzy->addFuzzyRule(rule1);

  FuzzyRuleAntecedent* antecedente2 = new FuzzyRuleAntecedent();
  antecedente2->joinWithAND(esquerda, superiorDireito);

  //Se posição X = esquerda E anguloVeiculo = superiorDireito ENTÂO angulo da roda = negativoPequeno
  FuzzyRule* rule2 = new FuzzyRule(2, antecedente2, nS);

  fuzzy->addFuzzyRule(rule2);

  FuzzyRuleAntecedent* antecedente3 = new FuzzyRuleAntecedent();
  antecedente3->joinWithAND(esquerda, verticalDireito);

  //Se posição X = esquerda E anguloVeiculo = verticalDireito ENTÃO angulo da roda = negativoMedio
  FuzzyRule* rule3 = new FuzzyRule(3, antecedente3, nM);

  fuzzy->addFuzzyRule(rule3);

  FuzzyRuleAntecedent* antecedente4 = new FuzzyRuleAntecedent();
  antecedente4->joinWithAND(esquerda, vertical);

  //Se posição X = esquerda E anguloVeiculo = vertical ENTÃO angulo da roda = negativoMedio
  FuzzyRule* rule4 = new FuzzyRule(4, antecedente4, nM);

  fuzzy->addFuzzyRule(rule4);

  FuzzyRuleAntecedent* antecedente5 = new FuzzyRuleAntecedent();
  antecedente5->joinWithAND(esquerda, verticalEsquerda);

  //Se posição X = esquerda E anguloVeiculo = verticalEsquerda ENTÃO angulo da roda = negativoGrande
  FuzzyRule* rule5 = new FuzzyRule(5, antecedente5, nB);

  fuzzy->addFuzzyRule(rule5);

  FuzzyRuleAntecedent* antecedente6 = new FuzzyRuleAntecedent();
  antecedente6->joinWithAND(esquerda, superiorEsquerda);

  //Se posição X = esquerda E anguloVeiculo = superiorEsquerda ENTÃO angulo da roda = negativoGrande
  FuzzyRule* rule6 = new FuzzyRule(6, antecedente6, nB);

  fuzzy->addFuzzyRule(rule6);

  FuzzyRuleAntecedent* antecedente7 = new FuzzyRuleAntecedent();
  antecedente7->joinWithAND(esquerda, inferiorEsquerda);

  //SE posição X = esquerda E anguloVeiculo = inferiorEsquerda ENTÃO angulo da roda = negativoGrande
  FuzzyRule* rule7 = new FuzzyRule(7, antecedente7, nB);

  fuzzy->addFuzzyRule(rule7);

  FuzzyRuleAntecedent* antecedente8 = new FuzzyRuleAntecedent();
  antecedente8->joinWithAND(centroEsquerda, inferiorDireito);

  //Se posição X = centroEsquerda E anguloVeiculo = inferiorDireito ENTÃO angulo da roda = positivoMedio
  FuzzyRule* rule8 = new FuzzyRule(8, antecedente8, pM);

  fuzzy->addFuzzyRule(rule8);

  FuzzyRuleAntecedent* antecedente9 = new FuzzyRuleAntecedent();
  antecedente9->joinWithAND(centroEsquerda, superiorDireito);

  //Se posição X = centroEsquerda E anguloVeiculo = superiorDireito ENTÃO angulo da roda = positivoPequeno
  FuzzyRule* rule9 = new FuzzyRule(9, antecedente9, pS);

  fuzzy->addFuzzyRule(rule9);

  FuzzyRuleAntecedent* antecedente10 = new FuzzyRuleAntecedent();
  antecedente10->joinWithAND(centroEsquerda, verticalDireito);

  //Se posição X = centroEsquerda E anguloVeiculo = verticalDireito ENTÃO angulo da roda = negativoPequeno
  FuzzyRule* rule10 = new FuzzyRule(10, antecedente10, nS);

  fuzzy->addFuzzyRule(rule10);

  FuzzyRuleAntecedent* antecedente11 = new FuzzyRuleAntecedent();
  antecedente11->joinWithAND(centroEsquerda, vertical);

  //Se posição X = centroEsquerda E anguloVeiculo = vertical ENTÃO angulo da roda = negativoMedio
  FuzzyRule* rule11 = new FuzzyRule(11, antecedente11, nM);

  fuzzy->addFuzzyRule(rule11);

  FuzzyRuleAntecedent* antecedente12 = new FuzzyRuleAntecedent();
  antecedente12->joinWithAND(centroEsquerda, verticalEsquerda);

  //Se posição X = centroEsquerda E anguloVeiculo = verticalEsquerda ENTÂO angulo da roda = negativoMedio
  FuzzyRule* rule12 = new FuzzyRule(12, antecedente12, nM);

  fuzzy->addFuzzyRule(rule12);

  FuzzyRuleAntecedent* antecedente13 = new FuzzyRuleAntecedent();
  antecedente13->joinWithAND(centroEsquerda, superiorEsquerda);

  //Se posição X = centroEsquerda E anguloVeiculo = superiorEsquerda ENTÃO angulo da roda = negativoGrande
  FuzzyRule* rule13 = new FuzzyRule(13, antecedente13, nB);

  fuzzy->addFuzzyRule(rule13);

  FuzzyRuleAntecedent* antecedente14 = new FuzzyRuleAntecedent();
  antecedente14->joinWithAND(centroEsquerda, inferiorEsquerda);

  //Se posição X = centroEsquerda E anguloVeiculo = inferiorEsquerda ENTÃO angulo da roda = negativoGrande
  FuzzyRule* rule14 = new FuzzyRule(14, antecedente14, nB);

  fuzzy->addFuzzyRule(rule14);

  FuzzyRuleAntecedent* antecedente15 = new FuzzyRuleAntecedent();
  antecedente15->joinWithAND(centro, inferiorDireito);

  //Se posição X = centro E anguloVeiculo = inferiorDireito ENTÃO angulo da roda = positivoMedio
  FuzzyRule* rule15 = new FuzzyRule(15, antecedente15, pM);

  fuzzy->addFuzzyRule(rule15);

  FuzzyRuleAntecedent* antecedente16 = new FuzzyRuleAntecedent();
  antecedente16->joinWithAND(centro, superiorDireito);

  //Se posição X = centro E anguloVeiculo = superiorDireito ENTÃO angulo da roda = positivoMedio
  FuzzyRule* rule16 = new FuzzyRule(16, antecedente16, pM);

  fuzzy->addFuzzyRule(rule16);

  FuzzyRuleAntecedent* antecedente17 = new FuzzyRuleAntecedent();
  antecedente17->joinWithAND(centro, verticalDireito);

  //Se posição X = centro E anguloVeiculo = verticalDireito ENTÃO angulo da roda = positivoPequeno
  FuzzyRule* rule17 = new FuzzyRule(17, antecedente17, pS);

  fuzzy->addFuzzyRule(rule17);

  FuzzyRuleAntecedent* antecedente18 = new FuzzyRuleAntecedent();
  antecedente18->joinWithAND(centro, vertical);

  //Se posição X = centro E anguloVeiculo = vertical ENTÃO angulo da roda = zero
  FuzzyRule* rule18 = new FuzzyRule(18, antecedente18, zE);

  fuzzy->addFuzzyRule(rule18);

  FuzzyRuleAntecedent* antecedente19 = new FuzzyRuleAntecedent();
  antecedente19->joinWithAND(centro, verticalEsquerda);

  //Se posição x = centro e auguloVeículo = verticalEsquerda Então angulo da roda = negativoPequeno
  FuzzyRule* rule19 = new FuzzyRule(19, antecedente19, nS);

  fuzzy->addFuzzyRule(rule19);

  FuzzyRuleAntecedent* antecedente20 = new FuzzyRuleAntecedent();
  antecedente20->joinWithAND(centro, superiorEsquerda);

  //Se posição x = centro e auguloVeículo = seperiorEsquerda Então angulo da roda = negativoGrande
  FuzzyRule* rule20 = new FuzzyRule(20, antecedente20, nB);

  fuzzy->addFuzzyRule(rule20);

  FuzzyRuleAntecedent* antecedente21 = new FuzzyRuleAntecedent();
  antecedente21->joinWithAND(centro, inferiorEsquerda);

  //Se posição x = centro e auguloVeículo = inferiorEsquerda Então angulo da roda = negativoMedio
  FuzzyRule* rule21 = new FuzzyRule(21, antecedente21, nM);

  fuzzy->addFuzzyRule(rule21);

  FuzzyRuleAntecedent* antecedente22 = new FuzzyRuleAntecedent();
  antecedente22->joinWithAND(centroDireita, inferiorEsquerda);

  //Se posição x = centroDireita e auguloVeículo = inferiorEsquerda Então angulo da roda = positivoGrande
  FuzzyRule* rule22 = new FuzzyRule(22, antecedente22, pB);

  fuzzy->addFuzzyRule(rule22);

  FuzzyRuleAntecedent* antecedente23 = new FuzzyRuleAntecedent();
  antecedente23->joinWithAND(centroDireita, superiorDireito);

  //Se posição x = centroDireita e auguloVeículo = superiorDireito Então angulo da roda = positivoGrande
  FuzzyRule* rule23 = new FuzzyRule(23, antecedente23, pB);

  fuzzy->addFuzzyRule(rule23);

  FuzzyRuleAntecedent* antecedente24 = new FuzzyRuleAntecedent();
  antecedente24->joinWithAND(centroDireita, verticalDireito);

  //Se posição x = centroDireita e auguloVeículo = verticalDireito Então angulo da roda = positivoMedio
  FuzzyRule* rule24 = new FuzzyRule(24, antecedente24, pM);

  fuzzy->addFuzzyRule(rule24);

  FuzzyRuleAntecedent* antecedente25 = new FuzzyRuleAntecedent();
  antecedente25->joinWithAND(centroDireita, vertical);

  //Se posição x = centroDireita e auguloVeículo = vertical Então angulo da roda = positivoMedio
  FuzzyRule* rule25 = new FuzzyRule(25, antecedente25, pM);

  fuzzy->addFuzzyRule(rule25);

  FuzzyRuleAntecedent* antecedente26 = new FuzzyRuleAntecedent();
  antecedente26->joinWithAND(centroDireita, verticalEsquerda);

  //Se posição x = centroDireta e auguloVeículo = verticalEsquerda Então angulo da roda = positivoPequeno
  FuzzyRule* rule26 = new FuzzyRule(26, antecedente26, pS);

  fuzzy->addFuzzyRule(rule26);

  FuzzyRuleAntecedent* antecedente27 = new FuzzyRuleAntecedent();
  antecedente27->joinWithAND(centroDireita, superiorEsquerda);

  //Se posição x = centroDireita e auguloVeículo = seperiorEsquerda Então angulo da roda = negativoPequeno
  FuzzyRule* rule27 = new FuzzyRule(27, antecedente27, nS);

  fuzzy->addFuzzyRule(rule27);

  FuzzyRuleAntecedent* antecedente28 = new FuzzyRuleAntecedent();
  antecedente28->joinWithAND(centroDireita, inferiorEsquerda);

  //Se posição x = centroDireita e auguloVeículo = inferiorEsquerda Então angulo da roda = negativoMedio
  FuzzyRule* rule28 = new FuzzyRule(28, antecedente28, nM);

  fuzzy->addFuzzyRule(rule28);

  FuzzyRuleAntecedent* antecedente29 = new FuzzyRuleAntecedent();
  antecedente29->joinWithAND(direita, inferiorDireito);

  //Se posição x = direita e auguloVeículo = inferiorDireito Então angulo da roda = positivoGrande
  FuzzyRule* rule29 = new FuzzyRule(29, antecedente29, pB);

  fuzzy->addFuzzyRule(rule29);


  FuzzyRuleAntecedent* antecedente30 = new FuzzyRuleAntecedent();
  antecedente30->joinWithAND(direita, superiorDireito);

  //Se posição x = dereito e auguloVeículo = seperiorDireito Então angulo da roda = positivoGrande
  FuzzyRule* rule30 = new FuzzyRule(30, antecedente30, pB);

  fuzzy->addFuzzyRule(rule30);

  FuzzyRuleAntecedent* antecedente31 = new FuzzyRuleAntecedent();
  antecedente31->joinWithAND(direita, verticalDireito);

  //Se posição x = direita e auguloVeículo = verticalDireito Então angulo da roda = positivoGrande
  FuzzyRule* rule31 = new FuzzyRule(31, antecedente31, pB);

  fuzzy->addFuzzyRule(rule31);

  FuzzyRuleAntecedent* antecedente32 = new FuzzyRuleAntecedent();
  antecedente32->joinWithAND(direita, vertical);

  //Se posição x = direita e auguloVeículo = vertical Então angulo da roda = positivoMedio
  FuzzyRule* rule32 = new FuzzyRule(32, antecedente32, pM);

  fuzzy->addFuzzyRule(rule32);

  FuzzyRuleAntecedent* antecedente33 = new FuzzyRuleAntecedent();
  antecedente33->joinWithAND(direita, verticalEsquerda);

  //Se posição x = direita e auguloVeículo = verticalEsquerda Então angulo da roda = positivoMedio
  FuzzyRule* rule33 = new FuzzyRule(33, antecedente33, pM);

  fuzzy->addFuzzyRule(rule33);

  FuzzyRuleAntecedent* antecedente34 = new FuzzyRuleAntecedent();
  antecedente34->joinWithAND(direita, superiorEsquerda);

  //Se posição x = direita e auguloVeículo = seperiorEsquerda Então angulo da roda = positivoPequeno
  FuzzyRule* rule34 = new FuzzyRule(34, antecedente34, pS);

  fuzzy->addFuzzyRule(rule34);

  FuzzyRuleAntecedent* antecedente35 = new FuzzyRuleAntecedent();
  antecedente35->joinWithAND(direita, inferiorEsquerda);

  //Se posição x = direita e auguloVeículo = inferiorEsquerda Então angulo da roda = negativoPequeno
  FuzzyRule* rule35 = new FuzzyRule(35, antecedente35, nS);

  fuzzy->addFuzzyRule(rule35);

}


void loop() {
  //int r1 = random(100);
  //int r2 = random(-90, 270);

  //eixo: Posição do veículo em relação ao eixo x
  //angulo: Angulo do veículo em relação a vaga (90° graus)


  //fuzzy->setInput(1, eixo);

  //fuzzy->setInput(2, angulo);

  //fuzzy->fuzzify();

  //float output = fuzzy->defuzzify(1);

  //Implementar movimento do servoMotor com a saída da defuzificação

  //angulo = angulo + output;
  //eixo = eixo + w * cos(angulo);
  //Serial.println(output);
  //Serial.println(eixo);

  /*
    Serial.print("posicao: ");
    Serial.println(eixo);
    Serial.print("angulo veiculo: ");
    Serial.println(angulo);
    Serial.println("saida: ");
    Serial.println(output);
  */

  //distancia = ultrasonic.Ranging(CM);// ultrassom.Ranging(CM) retorna a distancia em
  // centímetros(CM) ou polegadas(INC)
  //Serial.print(distancia); //imprime o valor da variável distancia
  //Serial.println("cm");

  /*
    if(distancia <= 100){
    servo.write(120);
    delay(1200);
    //servo.write(90);
    }else if(distancia > 100){

    //for(int x = distancia; x < 90; x++){
      servo.write(90);
      delay(150);
    //}

    }
  */


  if (Serial.available() > 0){
    // Lê toda string recebida
    String recebido = leStringSerial();
      
    
  }
  

  /*
  if(Serial.available() > 0){
    String dado = leStringSerial();
    Serial.print("dado: ");
    Serial.println(dado);
  }
  */

}
