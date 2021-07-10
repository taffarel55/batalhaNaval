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
#define TAM 6                  // Min 6
#define DIFFICULT 30           // Precisão de acerto do computador 
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
    }
};

class Map {
    protected:
        string map[TAM][TAM];
        float score;
    public:
        short win() {
            if (score<QTD*14) return 1;
            else cout<<"Player ganhou"<<endl;
            return 0;
            
        }
        void drawMap(){
            cout<<"  ";
            for (int i=0; i<TAM; i++) cout<<setfill(' ')<<setw(2)<<i; cout<<endl;
            for (int i=0; i<TAM; i++) {
                cout<<char(i+65)<<" ";
                for (int j=0; j<TAM; j++) if (map[i][j]==boat) cout<<sea; else cout<<map[i][j];
                cout<<endl;
            }
        }
        bool shoot(short x, short y) {
            cout<<"Tomei um tiro em "<<x<<","<<y<<endl;
            if (map[x][y]==sea || map[x][y]==boat) {
                if(map[x][y]==sea){
                    cout<<"Errou!"<<endl;
                    map[x][y]=miss;
                } else {
                    cout<<"Acertou!"<<endl;
                    score++;
                    map[x][y]=hit;
                }
                return 0;
            } else return 1;
        }
        Map() {
            score = 0;
            for (int i=0; i<TAM; i++) for (int j=0; j<TAM; j++) map[i][j]=sea;
            for (short len=5; len>1; len--) {
                for(int n=1; n<=QTD; n++) {
                    bool crashShip;
                    short x,y;
                    bool o;
                    do {
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
                        for(int i=ix; i<=fx; i++) for (int j=iy; j<=fy; j++) if (map[j][i]!=sea) crashShip = true;
                    } while (crashShip);
                    if(o) for (int j=y; j<y+len; j++) map[j][x]=boat;
                    else for (int i=x; i<x+len; i++) map[y][i]=boat;
                }
            }
        }
};

class MapPC : public Map {
    protected:
        short guess[TAM][TAM];
    public:
        short win() {
            if (score<QTD*14) return 1;
            else cout<<"Computador ganhou"<<endl;
            return 0;
        }
        short spy() {
            for(int i=0; i<TAM; i++) for(int j=0; j<TAM; j++) if(map[i][j]==boat) return TAM*i+j;
            return -1;
        }
        void drawMap(){
            cout<<"  ";
            for (int i=0; i<TAM; i++) cout<<setfill(' ')<<setw(2)<<i; cout<<endl;
            for (int i=0; i<TAM; i++) {
                cout<<char(i+65)<<" ";
                for (int j=0; j<TAM; j++) cout<<map[i][j];
                cout<<endl;
            }
        }
        bool shoot() {
            short x,y;
            if(rand()%100>=DIFFICULT) {
                x = rand() % TAM;
                y = rand() % TAM;
            } else {
                x = spy() / TAM;
                y = spy() % TAM;
            }  
            cout<<"Computador atirou em "<<x<<","<<y<<endl;
            if (map[x][y]==sea || map[x][y]==boat) {
                if(map[x][y]==sea){
                    cout<<"Errou!"<<endl;
                    map[x][y]=miss;
                } else {
                    cout<<"Acertou!"<<endl;
                    score++;
                    map[x][y]=hit;
                }
                return 0;
            } else return 1;
        }
};

int main() {
    srand((unsigned) time(0));
    Map p1;
    MapPC pc;
    int x;
    while(p1.win() && pc.win()) {
        p1.drawMap();
        pc.drawMap();
        string palp;
        do {
            cout<<"Joga ai o palpite: ";
            cin>>palp; if(palp[0]>96 && palp[0]<=96+TAM) palp[0]-=32;
        } while (p1.shoot(palp[0]-65,palp[1]-48));
        while(pc.shoot());
    }
    return 0;
}