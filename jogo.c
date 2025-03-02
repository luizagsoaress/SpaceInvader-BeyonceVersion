#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>


#define ALTURA 650
#define LARGURA 800
#define STD_SIZE_X 100
#define STD_SIZE_Y 116

typedef struct{
Texture2D Fundo;
Texture2D Heroi;
Texture2D Nave;
Texture2D BalaNave;
Texture2D BalaHeroi;
Texture2D Earth;
Texture2D ShipEnd;
Texture2D Lua;
Texture2D Barreira;
Texture2D Meteoro;
Texture2D Coracao;
Font Fonte;
Sound Tiro;
}ASSETS;

typedef struct{
 Rectangle pos;
 int Direcao;
}SHIPEND;

typedef struct{
Rectangle pos;
int Direcao;
}METEORO;

typedef struct{
 Rectangle pos;
 int vida;
}BARREIRA;

typedef enum{menu, GAME}TELA;

typedef struct{ 
bool Ativa;
int Velocidade; 
int Direcao;
int Tempo;
Rectangle pos;
Sound Tiro;
}BALA;

typedef struct{
Rectangle pos;
int SizeX, SizeY;
double Atual;
double Tiro;
BALA Bala;
int Vida;
}HEROI;

typedef struct{
Rectangle pos;
int SizeX, SizeY;
double Direcao;
double Atual;
double Tiro;
BALA Bala;
}NAVE;

typedef struct{
Rectangle pos;
int vida;
}CORACAO;

typedef struct{
ASSETS Assets;
TELA EstadoTela;
HEROI Heroi;
NAVE Nave;
BALA Bala;
SHIPEND ShipEnd;
BARREIRA Barreira[3];
METEORO Meteoro;
CORACAO Coracao[3];
}JOGO;

JOGO Jogo;

void IniciaJogo(JOGO *j);
void TelaInicial(JOGO *j);
void CarregaImagens(JOGO *j);
void DescarregaImagens(JOGO *j);
void DesenhaHeroi(JOGO *j);
void DesenhaEnter(JOGO *j);
void DesenhaNave(JOGO *j);
void ColisaoBalasNave(JOGO *j);
void ColisaoBalasHeroi(JOGO *j);
void TelaJogo(JOGO *j);


int main(){
InitAudioDevice(); 
InitWindow(LARGURA, ALTURA, "Beyonce's");

Music Musica = LoadMusicStream("assets/Musica.mp3");
PlayMusicStream(Musica); 




CarregaImagens(&Jogo);
IniciaJogo(&Jogo);
Jogo.EstadoTela = menu;

while(!WindowShouldClose()){
 UpdateMusicStream(Musica);
 switch (Jogo.EstadoTela) {
            case menu:
                TelaInicial(&Jogo);
                break;

            case GAME:
                TelaJogo(&Jogo);
                break;
        }
}
   
  
CloseAudioDevice();
DescarregaImagens(&Jogo);
CloseWindow();
    return 0;
}

void IniciaJogo(JOGO *j){
j->Heroi.Bala.Tiro = LoadSound("assets/Shoot.wav");
j->Heroi.SizeX = 100;
j->Heroi.SizeY = 116;
j->Heroi.pos = (Rectangle) {0 , 540, STD_SIZE_X, STD_SIZE_Y};
j->Nave.pos = (Rectangle) {100 , 116, STD_SIZE_X, STD_SIZE_Y};
j->Nave.pos.x = 0;
j->Nave.pos.y = 0;
j->Nave.Direcao = 1;
j->Heroi.Bala.pos.x = 0;
j->Heroi.Bala.pos.y = 0;
j->Heroi.Bala.Ativa = false;
j->Heroi.Bala.Velocidade = 1;
j->Heroi.Vida = 3;
j->Nave.Bala.pos.x = 0;
j->Nave.Bala.pos.y = 0;
j->Nave.Bala.Ativa = false;
j->Nave.Bala.Velocidade = 1;
j->ShipEnd.pos.x = 0;
j->ShipEnd.Direcao = 1;
j->Meteoro.pos.x = 700;
j->Meteoro.pos.y = 0;
j->Meteoro.Direcao = 10;

int X = 50;
int Y = 425;
for(int i = 0; i < 3; i++){
j->Barreira[i].vida = 1;
j->Barreira[i].pos = (Rectangle){X , Y, 80, 80};
X += 300;
}

int XX = 20;
int YY = 600;
for(int i = 0; i < 3; i++){
j->Coracao[i].vida = 1;
j->Coracao[i].pos = (Rectangle){XX, YY, 80, 80};
XX += 40;
}

}

void CarregaImagens(JOGO *j){
j->Assets.Fundo = LoadTexture("assets/Beyonce_inicio.png");
j->Assets.Fonte = LoadFont("assets/fontes/PressStart2P-Regular.ttf");
j->Assets.Heroi = LoadTexture("assets/Heroi.png");
j->Assets.Nave = LoadTexture("assets/Nave.png");
j->Assets.BalaNave = LoadTexture("assets/Bala_nave.png");
j->Assets.BalaHeroi = LoadTexture("assets/Bala_heroi.png");
j->Assets.Earth = LoadTexture("assets/Earth.png");
j->Assets.ShipEnd = LoadTexture("assets/ShipEnd.png");
j->Assets.Lua = LoadTexture("assets/Lua.png");
j->Assets.Barreira = LoadTexture("assets/Barreira_jogo.png");
j->Assets.Meteoro = LoadTexture("assets/Meteoro.png");
j->Assets.Coracao = LoadTexture("assets/Coracao.png");
}

void DescarregaImagens(JOGO *j){
UnloadTexture(Jogo.Assets.Fundo);
UnloadFont(Jogo.Assets.Fonte);
UnloadTexture(Jogo.Assets.Heroi);
UnloadTexture(Jogo.Assets.Nave);
UnloadTexture(Jogo.Assets.BalaNave);
UnloadTexture(Jogo.Assets.BalaHeroi);
UnloadTexture(Jogo.Assets.Earth);
UnloadTexture(Jogo.Assets.ShipEnd);
UnloadTexture(Jogo.Assets.Lua);
UnloadTexture(Jogo.Assets.Barreira);
UnloadTexture(Jogo.Assets.Meteoro);
UnloadTexture(Jogo.Assets.Coracao);
}

void TelaInicial(JOGO *j){
if (IsKeyPressed(KEY_ENTER)){
Jogo.EstadoTela = GAME;
}
Color PURPLEE = { 50, 20, 80, 255 };
Color NEON = { 255, 0, 150, 255 };
BeginDrawing();
ClearBackground(BLACK);
if(j->Assets.Fundo.id > 0){
DrawTexture(j->Assets.Fundo, 430, 250, WHITE);
}
Rectangle BotaoPlay = {350, 400, 170, 40};
Color REDD = {160, 0, 0, 255};
DrawTextEx(j->Assets.Fonte, "BEYONCE'S", (Vector2){130, 95}, 60, 2, NEON);
DrawTextEx(j->Assets.Fonte, "BEYONCE'S", (Vector2){120, 80}, 60, 2, WHITE);

DesenhaEnter(&Jogo);

bool MouseToque = CheckCollisionPointRec(GetMousePosition(), BotaoPlay);
if (MouseToque && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
 j->EstadoTela = GAME;
}
EndDrawing();
}

void DesenhaHeroi(JOGO *j){
Jogo.Heroi.Atual = GetTime();
 DrawTexture(j->Assets.Heroi, j->Heroi.pos.x, j->Heroi.pos.y, WHITE);
 if(IsKeyDown(KEY_RIGHT) && j->Heroi.pos.x <= 700){
j->Heroi.pos.x += 1;
}
if(IsKeyDown(KEY_LEFT) && j->Heroi.pos.x >= 0){
 j->Heroi.pos.x -= 1;
}

}

void DesenhaEnter(JOGO *j){
static double t = 0;
static double ult = 0;
Color PURPLEE = { 50, 20, 80, 255 };
t = GetTime();
if(t - ult >= 1.0f){
BeginDrawing();
DrawTextEx(j->Assets.Fonte, "PRESS ENTER", (Vector2){300, 407}, 15, 2, WHITE);
if(t - ult >= 2.0f){
EndDrawing();
ult = t;
}
}
}

void DesenhaNave(JOGO *j){
DrawTexture(j->Assets.Nave, j->Nave.pos.x, j->Nave.pos.y, WHITE);
double UltimoMovimento = 0;
double Atual = GetTime();
if(Atual -  UltimoMovimento >= 0.7 && j->Nave.pos.x <= 700){
 j->Nave.pos.x += j->Nave.Direcao;
}if(j->Nave.pos.x == 700){
j->Nave.Direcao *= -1; 
}
if(j->Nave.pos.x <= 0){
j->Nave.Direcao = fabs(j->Nave.Direcao); 
}
UltimoMovimento = Atual;
}

void AtiraBalas(JOGO *j){
Jogo.Nave.Atual = GetTime();
Jogo.Nave.Tiro = 0;
if (!j->Nave.Bala.Ativa && j->Nave.Atual - j->Nave.Tiro >= 1.0f) {
   j->Nave.Bala.Ativa = true;
   j->Nave.Bala.pos.x = j->Nave.pos.x + 50;
   j->Nave.Bala.pos.y = j->Nave.pos.y + 80;
   j->Nave.Tiro = j->Nave.Atual;
PlaySound(j->Heroi.Bala.Tiro);
}
if (IsKeyDown(KEY_SPACE) && !j->Heroi.Bala.Ativa && j->Heroi.Atual - j->Heroi.Tiro >= 0.7F) {
   j->Heroi.Bala.Ativa = true;
   j->Heroi.Bala.pos.x = j->Heroi.pos.x + 55;
   j->Heroi.Bala.pos.y = j->Heroi.pos.y - 45;
   j->Heroi.Tiro = j->Heroi.Atual;
   PlaySound(j->Heroi.Bala.Tiro);
}
}



void DesenhaCoracao(JOGO *j){
for(int i = 0; i < 3; i++){
if(j->Coracao[i].vida > 0){
DrawTexture(j->Assets.Coracao, j->Coracao[i].pos.x, j->Coracao[i].pos.y, WHITE);
}
}
}

void ColisaoCoracao(JOGO *j){
for(int i = 3; i > 0; i--){
if(j->Coracao[i].vida > 0){
if(CheckCollisionRecs(j->Nave.Bala.pos, j->Heroi.pos)){
j->Coracao[i].vida = 0;
j->Nave.Bala.Ativa = false;
 break;  
}
}
}
}

void DesenhaBarreira(JOGO *j){
for(int i = 0; i < 3; i++){
if(j->Barreira[i].vida > 0){
DrawTexture(j->Assets.Barreira, j->Barreira[i].pos.x, j->Barreira[i].pos.y, WHITE);
}
}
}

void ColisaoBarreira(JOGO *j){
for(int i = 0; i < 3; i++){
if(j->Barreira[i].vida > 0){
if(CheckCollisionRecs(j->Nave.Bala.pos, j->Barreira[i].pos)){
j->Barreira[i].vida = 0;
j->Nave.Bala.Ativa = false;
}
}
}
}

void AtualizaBalas(JOGO *j){
if(j->Heroi.Bala.Ativa){
j->Heroi.Bala.pos.y -= j->Heroi.Bala.Velocidade;
if(j->Heroi.Bala.pos.y < 0){
j->Heroi.Bala.Ativa = false;
}
}

if(j->Nave.Bala.Ativa){
j->Nave.Bala.pos.y += j->Nave.Bala.Velocidade;
if(j->Nave.Bala.pos.y > ALTURA){
j->Nave.Bala.Ativa = false;
}
}
}

void DesenhaBala(JOGO *j){
if(j->Heroi.Bala.Ativa){
DrawTexture(j->Assets.BalaHeroi, j->Heroi.Bala.pos.x, j->Heroi.Bala.pos.y, WHITE);
}
if(j->Nave.Bala.Ativa){
DrawTexture(j->Assets.BalaNave, j->Nave.Bala.pos.x, j->Nave.Bala.pos.y, WHITE);
}
}

void VoceGanhou(JOGO *j){
while (!WindowShouldClose()) {
Color NEON = { 255, 0, 150, 255 };
double T = 0;
T = GetTime();
static double Ult = 0;
double Tempo = GetTime();

BeginDrawing();
ClearBackground(BLACK);
Rectangle BotaoYes = {300, 450, 170, 20};
Rectangle BotaoNo = {450, 450, 170, 20};

DrawTextEx(j->Assets.Fonte, "TRY AGAIN", (Vector2){290, 400}, 20, 2, WHITE);
DrawTextEx(j->Assets.Fonte, "YES", (Vector2){300, 450}, 15, 2, NEON);
DrawTextEx(j->Assets.Fonte, "NO", (Vector2){445, 450}, 15, 2, NEON);
bool MouseToqueYes = CheckCollisionPointRec(GetMousePosition(), BotaoYes);

if (MouseToqueYes && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
 j->EstadoTela = menu;
IniciaJogo(&Jogo);
return;
}
bool MouseToqueNo = CheckCollisionPointRec(GetMousePosition(), BotaoNo);
if (MouseToqueNo && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
CloseWindow();
}

DrawTexture(j->Assets.Meteoro, j->Meteoro.pos.x, j->Meteoro.pos.y, WHITE);
if(j->Assets.Meteoro.id > 0 && T - Ult >= 1.0f && j->Meteoro.pos.x > 0){
j->Meteoro.pos.x -= j->Meteoro.Direcao;
j->Meteoro.pos.y += j->Meteoro.Direcao;
Ult = T;
}

DrawTextEx(j->Assets.Fonte, "QUEEN B", (Vector2){190, 80}, 60, 2, NEON);
DrawTextEx(j->Assets.Fonte, "QUEEN B", (Vector2){185, 65}, 60, 2, WHITE);
DrawTextEx(j->Assets.Fonte, "WINNER", (Vector2){220, 160}, 60, 2, NEON);
DrawTextEx(j->Assets.Fonte, "WINNER", (Vector2){215, 145}, 60, 2, WHITE);
DrawTexture(j->Assets.Earth, 450, 290, WHITE);
EndDrawing();
}

CloseWindow();
}



void VocePerdeu(JOGO *j){
while (!WindowShouldClose()) {
Color REDD = {160, 0, 0, 255};
double T = 0;
T = GetTime();
static double Ult = 0;

BeginDrawing();
ClearBackground(BLACK);
Rectangle BotaoYes = {300, 450, 170, 20};
Rectangle BotaoNo = {450, 450, 170, 20};

DrawTextEx(j->Assets.Fonte, "TRY AGAIN", (Vector2){290, 400}, 20, 2, WHITE);
DrawTextEx(j->Assets.Fonte, "YES", (Vector2){300, 450}, 15, 2, REDD);
DrawTextEx(j->Assets.Fonte, "NO", (Vector2){450, 450}, 15, 2, REDD);
bool MouseToqueYes = CheckCollisionPointRec(GetMousePosition(), BotaoYes);

if (MouseToqueYes && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
 j->EstadoTela = menu;
IniciaJogo(&Jogo);
return;
}
bool MouseToqueNo = CheckCollisionPointRec(GetMousePosition(), BotaoNo);
if (MouseToqueNo && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
CloseWindow();
}

DrawTexture(j->Assets.ShipEnd, j->ShipEnd.pos.x, j->ShipEnd.pos.y + 200, WHITE);
if(j->Assets.ShipEnd.id > 0 && T - Ult >= 0.1f && j->ShipEnd.pos.x <= 800){
j->ShipEnd.pos.x += j->ShipEnd.Direcao + 1;
Ult = T;
if(j->ShipEnd.pos.x >= 800){
j->ShipEnd.pos.x = -300;
}
}

DrawTextEx(j->Assets.Fonte, "GAME OVER", (Vector2){130, 110}, 60, 2, REDD);
DrawTextEx(j->Assets.Fonte, "GAME OVER", (Vector2){125, 80}, 60, 2, RED);
DrawTexture(j->Assets.Earth, 450, 290, WHITE);

EndDrawing();
}
CloseWindow();
}


void ColisaoBalasHeroi(JOGO *j){
if(j->Heroi.Bala.Ativa){
if(CheckCollisionRecs(j->Heroi.Bala.pos, j->Nave.pos)){
j->Heroi.Bala.Ativa = false;
 VoceGanhou(&Jogo);
}
}
}

void ColisaoBalasNave(JOGO *j){
if(j->Nave.Bala.Ativa){
if(CheckCollisionRecs(j->Nave.Bala.pos, j->Heroi.pos)){
j->Heroi.Vida--;
j->Nave.Bala.Ativa = false;

if(j->Heroi.Vida == 0){
VocePerdeu(&Jogo);
}
}
}
}

void TelaJogo(JOGO *j){
BeginDrawing();
ClearBackground(BLACK);
DesenhaHeroi(&Jogo);
DesenhaNave(&Jogo);
DesenhaCoracao(&Jogo);
ColisaoCoracao(&Jogo);
DesenhaBarreira(&Jogo);
ColisaoBarreira(&Jogo);
AtiraBalas(&Jogo);
AtualizaBalas(&Jogo);
DesenhaBala(&Jogo);
ColisaoBalasHeroi(&Jogo);
ColisaoBalasNave(&Jogo);
EndDrawing();
}




