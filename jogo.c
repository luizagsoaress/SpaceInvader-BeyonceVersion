#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define altura_bala 30
#define largura_bala 40
#define altura 650
#define largura 800
#define bordax 700
#define borday 536

typedef struct{
Texture2D fundo;
Texture2D heroi;
Texture2D nave;
Texture2D balanave;
Texture2D balaheroi;
Font fonte;
}Assets;

typedef enum{menu, JOGO}Tela;

typedef struct{ 
bool ativa;
int velocidade; 
int direcao;
int tempo;
int posx, posy;
}Bala;

typedef struct{
int posx;
int posy;
int sizex, sizey;
double atual;
double tiro;
Bala bala;
}Heroi;

typedef struct{
int posx;
int posy;
int sizex, sizey;
double direcao;
double atual;
double tiro;
Bala bala;
}Nave;


typedef struct{
Assets assets;
Tela estadotela;
Heroi heroi;
Nave nave;
Bala bala;
}Jogo;

Jogo jogo;


void telainicial(Jogo *j);
void desenhos(Jogo *j);
void desenhaheroi(Jogo *j);
void desenhanave(Jogo *j);
void pagtela(Jogo *j);


int main(){
InitAudioDevice(); 
InitWindow(largura, altura, "Beyonce's");

Music musica = LoadMusicStream("assets/musica.mp3");
PlayMusicStream(musica); 

jogo.heroi.sizex = 100;
jogo.heroi.sizey = 116;
jogo.heroi.posx = 0;
jogo.heroi.posy = 540;
jogo.nave.sizex = 100;
jogo.nave.sizey = 116;
jogo.nave.posx = 0;
jogo.nave.posy = 0;
jogo.nave.direcao = 1;
jogo.heroi.bala.posx = 0;
jogo.heroi.bala.posy = 0;
jogo.heroi.bala.ativa = false;
jogo.heroi.bala.velocidade = 1;
jogo.nave.bala.posx = 0;
jogo.nave.bala.posy = 0;
jogo.nave.bala.ativa = false;
jogo.nave.bala.velocidade = 1;


desenhos(&jogo);
jogo.estadotela = menu;

while(!WindowShouldClose()){
 UpdateMusicStream(musica);
 switch (jogo.estadotela) {
            case menu:
                telainicial(&jogo);
                break;

            case JOGO:
                pagtela(&jogo);
                break;
        }
}
    UnloadTexture(jogo.assets.fundo);
    UnloadTexture(jogo.assets.heroi);
    UnloadTexture(jogo.assets.nave);
    UnloadTexture(jogo.assets.balanave);
    UnloadTexture(jogo.assets.balaheroi);
    UnloadFont(jogo.assets.fonte);
  
CloseAudioDevice();
    CloseWindow();
    return 0;
}

void desenhos(Jogo *j){
j->assets.fundo = LoadTexture("assets/beyonce_inicio.png");
j->assets.fonte = LoadFont("assets/fontes/PressStart2P-Regular.ttf");
j->assets.heroi = LoadTexture("assets/heroi.png");
j->assets.nave = LoadTexture("assets/nave.png");
j->assets.balanave = LoadTexture("assets/bala_nave.png");
j->assets.balaheroi = LoadTexture("assets/bala_heroi.png");
}

void telainicial(Jogo *j){
BeginDrawing();
ClearBackground(WHITE);
if(j->assets.fundo.id > 0){
DrawTexture(j->assets.fundo, 430, 250, WHITE);
}
Rectangle botao = {350, 400, 170, 40};
Color background = {10, 0, 150, 255};
DrawText(">", 333, 400, 30, background);
DrawText(">", 330, 400, 30, background);
DrawTextEx(j->assets.fonte, "Play", (Vector2){350, 400}, 30, 2, background);
bool mousetoque = CheckCollisionPointRec(GetMousePosition(), botao);
if (mousetoque && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
 j->estadotela = JOGO;
}
EndDrawing();
}

void desenhaheroi(Jogo *j){
jogo.heroi.atual = GetTime();
 DrawTexture(j->assets.heroi, j->heroi.posx, j->heroi.posy, WHITE);
 if(IsKeyDown(KEY_RIGHT) && j->heroi.posx <= bordax){
j->heroi.posx += 1;
}
if(IsKeyDown(KEY_LEFT) && j->heroi.posx >= 0){
 j->heroi.posx -= 1;
}

if (IsKeyDown(KEY_SPACE) && !j->heroi.bala.ativa && j->heroi.atual - j->heroi.tiro >= 0.7F) {
   j->heroi.bala.ativa = true;
   j->heroi.bala.posx = j->heroi.posx + 55;
   j->heroi.bala.posy = j->heroi.posy - 45;
   j->heroi.tiro = j->heroi.atual;
}
}

void desenhanave(Jogo *j){
DrawTexture(j->assets.nave, j->nave.posx, j->nave.posy, WHITE);
double ultimomovimento = 0;
double atual = GetTime();
if(atual -  ultimomovimento >= 0.7 && j->nave.posx <= bordax){
 j->nave.posx += j->nave.direcao;
}if(j->nave.posx == bordax){
j->nave.direcao *= -1; 
}
if(j->nave.posx <= 0){
j->nave.direcao = fabs(j->nave.direcao); 
}
ultimomovimento = atual;
}

void atira(Jogo *j){
jogo.nave.atual = GetTime();
jogo.nave.tiro = 0;
if (!j->nave.bala.ativa && j->nave.atual - j->nave.tiro >= 1.0f) {
   j->nave.bala.ativa = true;
   j->nave.bala.posx = j->nave.posx + 50;
   j->nave.bala.posy = j->nave.posy + 80;
   j->nave.tiro = j->nave.atual;
}
}
void atualizabala(Jogo *j){

if(j->heroi.bala.ativa){
j->heroi.bala.posy -= j->heroi.bala.velocidade;
if(j->heroi.bala.posy < 0){
j->heroi.bala.ativa = false;
}
}

if(j->nave.bala.ativa){
j->nave.bala.posy += j->nave.bala.velocidade;
if(j->nave.bala.posy > altura){
j->nave.bala.ativa = false;
}
}
}

void desenhabala(Jogo *j){
if(j->heroi.bala.ativa){
DrawTexture(j->assets.balaheroi, j->heroi.bala.posx, j->heroi.bala.posy, WHITE);
}
if(j->nave.bala.ativa){
DrawTexture(j->assets.balanave, j->nave.bala.posx, j->nave.bala.posy, WHITE);
}
}

void voceganhou(Jogo *j){
double tempo = GetTime();
while (GetTime() - tempo < 5.0f) {
BeginDrawing();
ClearBackground(WHITE);
DrawText("Beyoncé venceu!", 230, 325, 40, GREEN);
EndDrawing();
}
j->estadotela = menu;
}

void voceperdeu(Jogo *j){
double tempo = GetTime();
while (GetTime() - tempo < 5.0f) {
BeginDrawing();
ClearBackground(BLACK);
DrawText("Sasha Fierce venceu!", 190, 325, 40, RED);
EndDrawing();
}
j->estadotela = menu;
}

void check(Jogo *j){
if(j->heroi.bala.ativa){
Rectangle retanguloNave = { j->nave.posx, j->nave.posy, j->nave.sizex, j->nave.sizey };
Rectangle balaHeroi = { j->heroi.bala.posx, j->heroi.bala.posy, largura_bala, altura_bala };

if(CheckCollisionRecs(balaHeroi, retanguloNave)){
j->heroi.bala.ativa = false;
 voceganhou(&jogo);
}
}

if(j->nave.bala.ativa){
Rectangle retanguloHeroi = { j->heroi.posx, j->heroi.posy, j->heroi.sizex, j->heroi.sizey };
Rectangle balaNave = { j->nave.bala.posx, j->nave.bala.posy, largura_bala, altura_bala };

if(CheckCollisionRecs(balaNave, retanguloHeroi)){
j->nave.bala.ativa = false;
 voceperdeu(&jogo);
}
}
}

void pagtela(Jogo *j){
BeginDrawing();
ClearBackground(BLACK);
desenhaheroi(&jogo);
atira(&jogo);
atualizabala(&jogo);
desenhabala(&jogo);
desenhanave(&jogo);
check(&jogo);
EndDrawing();
}




