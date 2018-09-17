#include <Fuzzy.h>
#include <FuzzyComposition.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzyOutput.h>
#include <FuzzyRule.h>
#include <FuzzyRuleAntecedent.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzySet.h>

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
  FuzzyInput* anguloVeiculo = new FuzzyInput(2);

  //Partições nebulosas: inferiorDireito, superiorDireito, verticalDireito, vertical, verticalEsquerda, superiorEsqueda, inferiorEsquerda

  FuzzySet* inferiorDireito = new FuzzySet(-90,-45,-45,15);
  anguloVeiculo->addFuzzySet(inferiorDireito);
  FuzzySet* superiorDireito = new FuzzySet(-15,30,30,60);
  anguloVeiculo->addFuzzySet(superiorDireito);
  FuzzySet* verticalDireito = new FuzzySet(45,67,67,90);
  anguloVeiculo->addFuzzySet(verticalDireito);
  FuzzySet* vertical = new FuzzySet(75,90,90,105);
  anguloVeiculo->addFuzzySet(vertical);
  FuzzySet* verticalEsquerda = new FuzzySet(90,112,112,135);
  anguloVeiculo->addFuzzySet(verticalEsquerda);
  FuzzySet* superiorEsqueda = new FuzzySet(120,150,150,195);
  anguloVeiculo->addFuzzySet(superiorEsqueda);
  FuzzySet* inferiorEsquerda = new FuzzySet(165,225,225,270);
  anguloVeiculo->addFuzzySet(inferiorEsquerda);

  fuzzy->addFuzzyInput(anguloVeiculo);
  
  //-------------------------------------------

  //Criando um FuzzyOutput de saída
  //anguloRoda
  FuzzyOutput* anguloRoda = new FuzzyOutput(1);

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

  //--------------------------------------------
  
  //Base de Regras
  
  //Se posição X = lateralEsquerda E anguloVeiculo = inferiorDireito ENTÃO angulo da roda = PositivoPequeno
  FuzzyRuleAntecedent* antecedente1 = new FuzzyRuleAntecedent();
  antecedente1->joinWithAND(lateralEsquerda, inferiorDireito);
  
  FuzzyRuleConsequent* consequente1 = new FuzzyRuleConsequent();
  consequente1->addOutput(positivoPequeno);

  FuzzyRule* rule1 = new FuzzyRule(1, antecedente1, consequente1);
  
  FuzzyRuleAntecedent* antecedente2 = new FuzzyRuleAntecedent();
  antecedente2->joinWithAND(lateralEsquerda,superiorDireito);

  FuzzyRuleConsequent* consequente2 = new FuzzyRuleConsequent();
  consequente2->addOutput(negativoPequeno);
  
  //Se posição X = lateralEsquerda E anguloVeiculo = superiorDireito ENTÂO angulo da roda = negativoPequeno
  FuzzyRule* rule2 = new FuzzyRule(2,antecedente2,consequente2);

  FuzzyRuleAntecedent* antecedente3 = new FuzzyRuleAntecedent();
  antecedente3->joinWithAND(lateralEsquerda,verticalDireito);

  FuzzyRuleConsequent* consequente3 = new FuzzyRuleConsequent();
  consequente3->addOutput(negativoMedio);

  //Se posição X = lateralEsquerda E anguloVeiculo = verticalDireito ENTÃO angulo da roda = negativoMedio
  FuzzyRule* rule3 = new FuzzyRule(3,antecedente3,consequente3);

  FuzzyRuleAntecedent* antecedente4 = new FuzzyRuleAntecedent();
  antecedente4->joinWithAND(lateralEsquerda,vertical);

  FuzzyRuleConsequent* consequente4 = new FuzzyRuleConsequent();
  consequente4->addOutput(negativoMedio);

  //Se posição X = lateralEsquerda E anguloVeiculo = vertical ENTÃO angulo da roda = negativoMedio
  FuzzyRule* rule4 = new FuzzyRule(4,antecedente4,consequente4);

  FuzzyRuleAntecedent* antecedente5 = new FuzzyRuleAntecedent();
  antecedente->joinWithAND(lateralEsquerda,verticalEsquerda);

  FuzzyRuleConsequent* consequente5 = new FuzzyRuleConsequent();
  consequente5->addOutput(negativoGrande);

  //Se posição X = lateralEsquerda E anguloVeiculo = verticalEsquerda ENTÃO angulo da roda = negativoGrande
  FuzzyRule* rule5 = new FuzzyRule(5,antecedente5,consequente5);

  FuzzyRuleAntecedent* antecedente6 = new FuzzyRuleAntecedent();
  antecedente6->joinWithAND(lateralEsquerda,superiorEsquerda);

  FuzzyRuleConsequent* consequente6 = new FuzzyRuleConsequent();
  consequente6->addOutput(negativoGrande);

  //Se posição X = lateralEsquerda E anguloVeiculo = superiorEsquerda ENTÃO angulo da roda = negativoGrande
  FuzzyRule* rule6 = new FuzzyRule(6,antecedente6,consequente6);

  FuzzyRuleAntecedent* antecedente7 = new FuzzyRuleAntecedent();
  antecedente7->joinWithAND(lateralEsquerda,inferiorEsquerda);

  FuzzyRuleConsequent* consequente7 = new FuzzyRuleConsequent();
  consequente7->addOutput(negativoGrande);

  //SE posição X = lateralEsquerda E anguloVeiculo = inferiorEsquerda ENTÃO angulo da roda = negativoGrande
  FuzzyRule* rule7 = new FuzzyRule(7,antecedente7,consequente7);

  FuzzyRuleAntecedet* antecedente8 = new FuzzyRuleAntecedent();
  antecedente8->joinAND(centroEsquerda,inferiorDireito);

  FuzzyRuleConsequent* consequente8 = new FuzzyRuleConsequent();
  consequente8->addOutput(positivoMedio);

  //Se posição X = centroEsquerda E anguloVeiculo = inferiorDireito ENTÃO angulo da roda = positivoMedio
  FuzzyRule* rule8 = new FuzzyRule(8,antecedente8,consequente8);

  FuzzyRuleAntecedent* antecedente9 = new FuzzyRuleAntecedent();
  antecedente9->joinAND(centroEsquerda,superiorDireito);

  FuzzyRuleConsequent* consequente9 = new FuzzyRuleConsequent();
  consequente9->addOutput(positivoPequeno);

  //Se posição X = centroEsquerda E anguloVeiculo = superiorDireito ENTÃO angulo da roda = positivoPequeno
  FuzzyRule* rule9 = new FuzzyRule(9,antecedente9,consequente9);

  FuzzyRuleAntecedent* antecedente10 = new FuzzyRuleAntecedent();
  antecedente10->joinWithAND(centroEsquerda,verticalDireito);

  FuzzyRuleConsequent* consequente10 = new FuzzyRuleConsequent();
  consequente10->addOutput(negativoPequeno);

  //Se posição X = centroEsquerda E anguloVeiculo = verticalDireito ENTÃO angulo da roda = negativoPequeno
  FuzzyRule* rule10 = new FuzzyRule(10,antecedente10,consequente10);

  FuzzyRuleAntecedent* antecedente11 = new FuzzyRuleAntecedent();
  antecedente11->joinAND(centroEsquerda,vertical);

  FuzzyRuleConsequent* consequente11 = new FuzzyRuleConsequent();
  consequente11->addOutput(negativoMedio);

  //Se posição X = centroEsquerda E anguloVeiculo = vertical ENTÃO angulo da roda = negativoMedio
  FuzzyRule* rule11 = new FuzzyRule(11,antecedente11,consequente11);

  FuzzyRuleAntecedent* antecedente12 = new FuzzyRuleAntecedent();
  antecedente12->joinAND(centroEsquerda,verticalEsquerda);

  FuzzyRuleConsequent* consequente12 = new FuzzyRuleConsequent();
  consequente12->addOutput(negativoMedio);

  //Se posição X = centroEsquerda E anguloVeiculo = verticalEsquerda ENTÂO angulo da roda = negativoMedio
  FuzzyRule* rule12 = new FuzzyRule(12,antecedente12,consequente12);

  FuzzyRuleAntecedent* antecedente13 = new FuzzyRuleAntecedent();
  antecedent13->joinAND(centroEsquerda,superiorEsquerda);

  FuzzyRuleConsequent* consequente13 = new FuzzyRuleConsequent();
  consequente13->addOutput(negativoGrande);

  //Se posição X = centroEsquerda E anguloVeiculo = superiorEsquerda ENTÃO angulo da roda = negativoGrande
  FuzzyRule* rule13 = new FuzzyRule(13,antecedente13,consequente13);

  FuzzyRuleAntecedent* antecedente14 = new FuzzyRuleAntecedent();
  antecedente14->joinWithAND(centroEsquerda,inferiorEsquerda);

  FuzzyRuleConsequent* consequente14 = new FuzzyRuleConsequent();
  consequente14->addOutput(negativoGrande);

  //Se posição X = centroEsquerda E anguloVeiculo = inferiorEsquerda ENTÃO angulo da roda = negativoGrande
  FuzzyRule* rule14 = new FuzzyRule(14,antecedente14,consequente14);

  FuzzyRuleAntecedent* antecedente15 = new FuzzyRuleAntecedent();
  antecedente15->joinAND(centro,inferiorDireito);

  FuzzyRuleConsequent* consequente15 = new FuzzyRuleConsequent();
  consequente15->addOutput(positivoMedio);

  //Se posição X = centro E anguloVeiculo = inferiorDireito ENTÃO angulo da roda = positivoMedio
  FuzzyRule* rule15 = new FuzzyRule(15,antecedente15,consequente15);

  FuzzyRuleAntecedent* antecedente16 = new FuzzyRuleAntecedent();
  antecedente16->joinAND(centro,superiorDireito);

  FuzzyRuleConsequent* consequente16 = new FuzzyRuleConsequent();
  consequente16->addOutput(positivoMedio);

  //Se posição X = centro E anguloVeiculo = superiorDireito ENTÃO angulo da roda = positivoMedio
  FuzzyRule* rule16 = new FuzzyRule(16,antecedente16,consequente16);

  FuzzyRuleAntecedent* antecedente17 = new FuzzyRuleAntecedent();
  antecedente17->joinAND(centro,verticalDireito);

  FuzzyRuleConsequent* consequente17 = new FuzzyRuleConsequent();
  consequent->addOutput(positivoPequeno);

  //Se posição X = centro E anguloVeiculo = verticalDireito ENTÃO angulo da roda = positivoPequeno
  FuzzyRule* rule17 = new FuzzyRule(17,antecedente17,consequente17);

  FuzzyRuleAntecedent* antecedente18 = new FuzzyRuleAntecedent();
  antecedente18->joinAND(centro,vertical);

  FuzzyRuleConsequent* consequente18 = new FuzzyRuleConsequent();
  consequente18->addOutput(zero);

  //Se posição X = centro E anguloVeiculo = vertical ENTÃO angulo da roda = zero
  FuzzyRule* rule18 = new FuzzyRule(18,antecedente18,consequente18);
}

void loop() {
  // put your main code here, to run repeatedly:

}
