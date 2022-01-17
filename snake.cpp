#include <iostream>
#include <sys/ioctl.h>
#include <termios.h>
#include <stdio.h>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

    const int width = 30;
    const int height = 30;
    int bytes,i,sx,sy,nx,ny,h,w,x,y;
    int dirx = 1;
    int diry = 0;
    int bg[width][height];
    int snake[50][2];
    int food[] = {0,0};
    int score = 0;
    int length = 3;

//Kolory

    #define NC "\e[0m"
    #define RED "\e[0;31m"
    #define GRN "\e[0;32m"
    #define CYN "\e[0;36m"
    #define REDB "\e[41m"
    #define YEL "\e[1;93m"
    #define BLU "\e[0;34m"
    #define BLUB "\e[44m"
    #define CYNB "\e[46m"
    #define GRNB "\e[42m"
    #define YELB "\e[43m"
    #define BHRED "\e[1;91m"
    #define BHBLU "\e[1;94m"
    #define BHGRN "\e[1;92m"
    #define BHCYN "\e[1;96m"
    #define BHMAG "\e[1;95m"
    #define BMAG "\e[1;35m"
    #define BHYEL "\e[1;93m"
    #define MAG "\e[0;35m"

//Wciśnięcie klawisza 

    int _kbhit()
    {
        static const int STDIN = 0;
        static bool initialized = false;
        if (! initialized)
            {
                termios term;
                tcgetattr(STDIN, &term);
                term.c_lflag &= ~ICANON;
                tcsetattr(STDIN, TCSANOW, &term);
                setbuf(stdin, NULL);
                initialized = true;
            }
        ioctl(STDIN, FIONREAD, &bytes);
        return bytes;
    }

//aktualizacja pozycji węża

void update(void)
    {
        sx = snake[0][0];
        sy = snake[0][1];

        snake[0][0] = sx + dirx;
        snake[0][1] = sy + diry;

        nx = snake[0][0];
        ny = snake[0][1];       

        for(i = 1; i < length; i++)
        {
            nx = snake[i][0];
            ny = snake[i][1]; 
            snake[i][0] = sx;
            snake[i][1] = sy;
            sx = nx;
            sy = ny;                                    
        }
    }

//Wąż w odpowiednim miejscu planszy 

void draw_snake_in_bg(const int rev)
    {
        for(i = 0; i < length; i++)
        {
            x = snake[i][0];
            y = snake[i][1];
            if((x != 0) && (y != 0))
            {
                bg[y][x] = rev;
            }
        }
    }

//render planszy 

void initBg(void)
    {
        for(i = 0; i < width; i++){
        bg[0][i] = 3;
        }
        for(i = 0; i < height; i++){
        bg[i][0] = 3;
        }
        for(i = 0; i < height; i++){
        bg[i][width-1] = 3;
        }
        for(i = 0; i < width; i++){
        bg[height - 1][i] = 3;
        }

    }

//Znaki w odpowiednich miejsac

void print(void)
    {
    for(h=0; h<height; h++){
        for(w = 0; w < width; w++){
            i=bg[h][w];
            if(i == 1){
            cout << BHGRN "*"  NC;
            }else if (i == 2){
            cout << REDB "0" NC;
            }else if (i == 3){
            cout << BHCYN "#" NC;
            }else{
            cout << " ";
            }
        }
        cout << endl;
    }  
    }  

//Rozpoznawanie przycisku

void read_key(const char k)
    {
    if(k == 'p' || k == 'P'){
        ofstream file("save.txt");
        cout << BHCYN "\n Gra zostala zapisana \n" NC;
        file << length << " " << score;
        file.close();       
        exit(1); 
    }
    if(k == 'd'){
        dirx = 1;
        diry = 0;
    }else if(k =='D'){
        dirx = 1;
        diry = 0;
    }else if(k =='a'){
        dirx = -1;
        diry = 0;
    }else if(k =='A'){
        dirx = -1;
        diry = 0;
    }else if(k =='w'){
        dirx = 0;
        diry = -1;
    }else if(k =='W'){
        dirx = 0;
        diry = -1;
    }else if(k =='s'){
        dirx = 0;
        diry = 1;
    }else if(k =='S'){
        dirx = 0;
        diry = 1;
    }
    }

//jedzenie

void placeFood(void)
    {
        if (food[0]==0){
        x = rand() % (width - 2) + 1;
        y = rand() % (height - 2) + 1;
        food[0] = x;
        food[1] = y;
        bg[y][x] = 2;
        }
    }

//Zakończenie gry 

void check(void)
    {
        sx = snake[0][0];
        sy = snake[0][1];
        if((sx == 0)||(sx == (width - 1))||(sy == 0)||(sy == (height - 1))){
        cout << BHRED "Koniec gry" NC << endl;
        ofstream file("points.txt", ios::app);    
        file << "score: " << score << "\n";
        file.close();
        exit(0);
        }
    }

//funkcja wywolana w momencie w ktorym waz zjada jedzenie

void eat(void)
    {
        x=food[0];
        y=food[1];
        if ((x == snake[0][0]) && (y == snake[0][1])){
            bg[y][x] = 0;
            food[0] = 0; 
            score++;
            length++;
            placeFood();
        }
    }
        
//petla

void loop(void)
    {
        int frame=  0;
        x = 0;
        y = 0;
        char k;

    
    while(x < 500){
        clock_t time_end;
        time_end = clock() + 200 * CLOCKS_PER_SEC/1000;
        while (clock() < time_end);
        if(_kbhit())   {
            cin >> k; 
            read_key(k);
        }
        system("clear");     
        draw_snake_in_bg(1); 
        print();       
        draw_snake_in_bg(0);            
        update();
        check();      
        placeFood();               
        eat();            
        cout <<  BHGRN "Points: " NC  << score << endl; 
        cout << BHBLU "To save press: " NC BHYEL "P \n" NC ;
        frame++;
        }
    }

void padding(int k){
    for(int i = 0;i < k;i++){
        cout<< " ";
    }
}

void instruction()
    {
        padding(30);
        cout << BHBLU "INSTRUKCJA\n" NC;
        padding(8);
        cout << BHMAG "STEROWANIE ZA POMOCA KLAWISZY " NC REDB " W " NC BLUB" S " NC GRNB" A " NC CYNB" D " NC 
        BHMAG " ORAZ KLAWISZA " NC YELB " P \n" NC;
        padding(15);
        cout << BHMAG "ABY ROZPOCZAC - WYBIERZ OPCJE I KLIKNIJ " NC YELB " P \n" NC;
    }

int initMenu() 
    {
        system("clear");
        padding(30);
        cout<< BHGRN "SNAKE GAME\n" NC;  
        padding(10);
        for(int i = 0; i < 50;i ++){
            cout << YEL "-" NC;
        }
        cout << "\n";
        for(int i = 0;i < 5;i++){
            padding(10);
            cout << YEL "|" NC;
            padding(48);
            cout << YEL "| \n" NC;
    }

    padding(10);
    cout << YEL "|" NC;
    padding(22);
    cout << BHCYN "*Graj" NC;
    padding(21);
    cout << YEL "| \n" NC;
    padding(10);
    cout << YEL "|" NC;
    padding(22);
    cout << CYN "Wczytaj" NC;
    padding(19);
    cout << YEL "| \n" NC;
    padding(10);
    cout << YEL "|" NC;
    padding(22);
    cout << CYN "Wyjdz" NC;
    padding(21);
    cout << YEL "| \n" NC;
    for(int i = 0;i < 5;i++){
            padding(10);
            cout << YEL "|" NC;
            padding(48);
            cout << YEL "| \n" NC;
    }
    padding(10);
    for(int i =0; i < 50;i ++){
        cout << YEL "-" NC;
    }

    cout << "\n";
    instruction();
    int play = 1;

    char k;
    if(_kbhit){
        cin >> k;
    }

    while(k != 'p' && k != 'P'){
        if((k == 'w' && play == 3 ) || (k == 'W' && play == 3 ) || (k == 's' && play == 1) || (k == 'S' && play == 1 )  || ((k == 'w' || k == 's')  && play == 2) || ((k == 'W' || k == 'S')  && play == 2))
            {
            system("clear");  
            padding(30);
            cout << BHGRN "SNAKE GAME\n" NC;  
            padding(10);
            for(int i = 0; i < 50;i ++){
                cout << YEL "-" NC;
            }
            cout << "\n";
            for(int i = 0;i < 5;i++){
                    padding(10);
                    cout << YEL "|" NC;
                    padding(48);
                    cout << YEL "| \n" NC;
            }
            if(k == 's') play++;
            if(k == 'w') play--;
            if(k == 'S') play++;
            if(k == 'W') play--;

            if(play == 1) {
                padding(10);
                cout << YEL "|" NC;
                padding(22);
                cout << BHCYN "*Graj" NC;
                padding(21);
                cout << YEL "| \n" NC;
                padding(10);
                cout << YEL "|" NC;
                padding(22);
                cout << CYN "Wczytaj" NC;
                padding(19);
                cout << YEL "| \n" NC;
                padding(10);
                cout << YEL "|" NC;
                padding(22);
                cout << CYN "Wyjdz" NC;
                padding(21);
                cout << YEL "| \n" NC;
            }

            if(play == 2) {
                padding(10);
                cout << YEL "|" NC;
                padding(22);
                cout << CYN "Graj" NC;
                padding(22);
                cout << YEL "| \n" NC;
                padding(10);
                cout << YEL "|" NC;
                padding(22);
                cout << BHCYN "*Wczytaj" NC;
                padding(18);
                cout << YEL "| \n" NC;
                padding(10);
                cout << YEL "|" NC;
                padding(22);
                cout << CYN "Wyjdz" NC;
                padding(21);
                cout << YEL "| \n" NC;
            }

            if(play == 3) {
                padding(10);
                cout << YEL "|" NC;
                padding(22);
                cout << CYN "Graj" NC;
                padding(22  );
                cout << YEL "| \n" NC;
                padding(10);
                cout << YEL "|" NC;
                padding(22);
                cout << CYN "Wczytaj" NC;
                padding(19);
                cout << YEL "| \n" NC;
                padding(10);
                cout << YEL "|" NC;
                padding(22);
                cout << BHCYN "*Wyjdz" NC;
                padding(20);
                cout << YEL "| \n" NC;
            }
            

            for(int i = 0;i < 5;i++){
                    padding(10);
                    cout << YEL "|" NC;
                    padding(48);
                    cout << YEL "| \n" NC;
            }
            padding(10);
            for(int i = 0; i < 50;i ++){
                cout << YEL "-" NC;
            }
            cout << "\n";
            instruction();
        }
       
        if(_kbhit)  cin >> k;
    } 
    if(play == 3){
        exit(1);
    }
    return play == 2;
}



int main(){

    int save = initMenu(); 

    initBg(); 
    snake[0][0]=3; 
    snake[0][1]=3; 
    snake[1][0]=3+1; 
    snake[1][1]=3; 
    snake[2][0]=3+2;
    snake[2][1]=3; 
    snake[3][0]=3+3; 
    snake[3][1]=3;
    snake[4][0]=3+4;
    snake[4][1]=3;   

    if(save){
        ifstream infile("save.txt");
        if(infile.good()) infile >> length >> score;  
        infile.close();
    }
    
    loop();                 
}

