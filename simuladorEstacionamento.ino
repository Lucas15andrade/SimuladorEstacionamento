#include <Fuzzy.h>
#include <FuzzyComposition.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzyOutput.h>
#include <FuzzyRule.h>
#include <FuzzyRuleAntecedent.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzySet.h>

//Constante que representa a distância fixa em que o veículo é deslocado a cada passo.
//Definir melhor valor posteriormente.
const int w = 5;

//Instanciando um objeto da biblioteca
Fuzzy* fuzzy = new Fuzzy();

void setup() {
  //Iniciando comunicação serial
  Serial.begin(9600);

  //Criando um FuzzyInput de entrada
  //Posição X = Posição do veículo em relação ao eixo horizontal X
  FuzzyInput* posicaoX = new FuzzyInput(1);

  //Partições nebulosas: LateralEsquerda, CentroEsqueda, Centro, CentroDireita, Direita

  FuzzySet* lateralEsquerda = new FuzzySet(0,0,10,35);
  posicaoX->addFuzzySet(lateralEsquerda);
  FuzzySet* centroEsquerda = new FuzzySet(30,40,40,50);
  posicaoX->addFuzzySet(centroEsquerda);
  FuzzySet* centro = new FuzzySet(45,50,50,55);
  posicaoX->addFuzzySet(centro);
  FuzzySet* centroDireita = new FuzzySet(50,60,60,70);
  posicaoX->addFuzzySet(centroDireita);
  FuzzySet* direita = new FuzzySet(65,90,100,100);
  posicaoX->addFuzzySet(direita);

  fuzzy->addFuzzyInput(posicaoX);

  //-------------------------------------------

  //Criando um FuzzyInput de entrada
  //Posição Y = Rotação do veículo
  FuzzyInput* posicaoY = new FuzzyInput(2);

  //Partições nebulosas: inferiorDireito, superiorDireito, verticalDireito, vertical, verticalEsquerda, superiorEsqueda, inferiorEsquerda

  FuzzySet* inferiorDireito = new FuzzySet(-90,-45,-45,15);
  posicaoY->addFuzzySet(inferiorDireito);
  FuzzySet* superiorDireito = new FuzzySet(-15,30,30,60);
  posicaoY->addFuzzySet(superiorDireito);
  FuzzySet* verticalDireito = new FuzzySet(45,67,67,90);
  posicaoY->addFuzzySet(verticalDireito);
  FuzzySet* vertical = new FuzzySet(75,90,90,105);
  posicaoY->addFuzzySet(vertical);
  FuzzySet* verticalEsquerda = new FuzzySet(90,112,112,135);
  posicaoY->addFuzzySet(verticalEsquerda);
  FuzzySet* superiorEsqueda = new FuzzySet(120,150,150,195);
  posicaoY->addFuzzySet(superiorEsqueda);
  FuzzySet* inferiorEsquerda = new FuzzySet(165,225,225,270);
  posicaoY->addFuzzySet(inferiorEsquerda);

  fuzzy->addFuzzyInput(posicaoY);
  
  //-------------------------------------------

  //Criando um FuzzyOutput de saída
  //anguloRoda
  FuzzyOutput* anguloRoda = new FuzzyOutput(3);

  //Partições nebulosas: negativoGrande, negativoMedio, negativoPequeno, zero, positivoPequeno, positivoMedio, positivoGrande

  FuzzySet* negativoGrande = new FuzzySet(-30,-30,-15,-15);
  anguloRoda->addFuzzySet(negativoGrande);
  FuzzySet* negativoMedio = new FuzzySet(-28,-15,-15,-5);
  anguloRoda->addFuzzySet(negativoMedio);
  FuzzySet* negativoPequeno = new FuzzySet(-10,-5,-5,0);
  anguloRoda->addFuzzySet(negativoPequeno);
  FuzzySet* zero = new FuzzySet(-5,0,0,5);
  anguloRoda->addFuzzySet(zero);
  FuzzySet* positivoPequeno = new FuzzySet(0,5,5,10);
  anguloRoda->addFuzzySet(positivoPequeno);
  FuzzySet* positivoMedio = new FuzzySet(5,15,15,25);
  anguloRoda->addFuzzySet(positivoMedio);
  FuzzySet* positivoGrande = new FuzzySet(15,15,30,30);
  anguloRoda->addFuzzySet(positivoGrande);

  fuzzy->addFuzzyOutput(anguloRoda);

  //Conjuntos de entrada e saída criados.

  //Se posição X = lateralEsquerda E posição Y = inferiorDireito ENTÃO angulo da roda = Positivo Pequeno
  FuzzyRuleAntecedent* posLateralEsquerdaEInferiorDireito = new FuzzyRuleAntecedent();
  posLateralEsquerda->joinWithAND(lateralEsquerda, inferiorDireito);
  
  FuzzyRuleConsequent* angPositivoPequeno = new FuzzyRuleConsequent();
  angPositivoPequeno->addOutput(positivoPequeno);

  FuzzyRule* fuzzyRule01 = new FuzzyRule(1, posLateralEsquerdaEInferiorDireito, angPositivoPequeno);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
