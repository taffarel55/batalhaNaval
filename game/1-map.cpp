#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <iomanip>

#define sea  "\u2591\u2591"    // ░░
#define miss "\u2592\u2592"    // ▒▒
#define hit  "\u2593\u2593"    // ▓▓
#define boat "\u2588\u2588"    // ██
#define TAM 10                 // Tamanho maior que 5, multiplo de 5
#define QTD TAM/5

using namespace std;

class Ship {
    private:
        short length;
        short x;
        short y;
        bool orientation;
    public:
        short   getLength(){return length;}
        short   getCordinates(){return TAM*x+y;}
        bool    getOrientation(){return orientation;}
        void    setLength(short l){length=l;}
        void    setCordinates(short cx,short cy){x=cx;y=cy;}
        void    setOrientation(bool o){orientation=o;}
        void    getInfo() {
            string text=orientation?"Vertical":"Horizontal";
            cout<<"Cordenadas=("<<x<<","<<y<<") Orientação="<<text<<endl;
        }
    Ship (short len) {
        setLength(len);
        orientation = rand()%2;
        if (orientation) setCordinates(rand()%TAM,rand()%(TAM-len+1));
        else setCordinates(rand()%(TAM-len+1),rand()%TAM);
        getInfo();
    }
};

class Map {
    private:
        string map[TAM][TAM];
    public:
        void drawMap(){
            // Desenha o estado do Mapa atual na Tela
            cout<<"Desenho atual do mapa:"<<endl;
            cout<<"  ";
            for (int i=0; i<TAM; i++) cout<<setfill(' ')<<setw(2)<<i; cout<<endl;
            for (int i=0; i<TAM; i++) {
                cout<<char(i+65)<<" ";
                for (int j=0; j<TAM; j++) cout<<map[i][j];
                cout<<endl;
            }
        }
        bool shoot(short x, short y) {
            // Tenta atirar em uma coordenada x,y. Se atingir um barco retorna 1.
            return 0;
        }
        Map() {
            // Criando mapa com água
            cout<<"Criando mapa..."<<endl;
            for (int i=0; i<TAM; i++) for (int j=0; j<TAM; j++) map[i][j]=sea;
            drawMap();

            // Barcos: 2 Porta Aviões(5) 2 Cruzadores(4) 2 Destroyers(4) 2 Submarinos(3) 2 Fragatas(2)
            cout<<"Criando os barcos..."<<endl;
            for (short len=5; len>1; len--) {
                srand((unsigned) time(0));
                for(int n=1; n<=QTD; n++) {
                    bool crashShip;
                    short x;
                    short y;
                    bool o;
                    do {
                        cout<<"Criando o barco "<<n+2*(5-len)<<endl; // 5 = len_{max}
                        crashShip=false;
                        Ship pa(len);
                        x = pa.getCordinates() / TAM;
                        y = pa.getCordinates() % TAM;
                        o = pa.getOrientation();
                        short ix,iy,fx,fy;
                        ix = x ? x-1 : x;
                        iy = y ? y-1 : y;
                        if (o) {
                            fy = (y+len<TAM) ? y+len : TAM-1;
                            fx = (x==TAM-1) ? x : x+1;
                        } else {
                            fx = (x+len<TAM) ? x+len : TAM-1;
                            fy = (y==TAM-1) ? y : y+1;
                        }
                        cout<<"Conferir: ("<<ix<<","<<iy<<") ("<<fx<<","<<fy<<") | "<<ix<<"..."<<fx<<", "<<iy<<"..."<<fy<<endl;
                        for(int i=ix; i<=fx; i++) {
                            for (int j=iy; j<=fy; j++) {
                                if (map[j][i]!=sea) {
                                    crashShip = true;
                                    cout<<"Choque de navios!"<<endl;
                                    cout<<"Existe um pedaço de navio na posição ("<<i<<","<<j<<")"<<endl;
                                } 
                            }
                        }
                        
                    } while (crashShip);
                    if(o) for (int j=y; j<y+len; j++) map[j][x]=boat;
                    else for (int i=x; i<x+len; i++) map[y][i]=boat;
                    drawMap();
                }
            }
        }
};

int main() {
    Map m;
    return 0;
}