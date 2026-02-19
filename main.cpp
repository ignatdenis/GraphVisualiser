#include <graphics.h>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <cfloat>

using namespace std;

int SCREEN_W = 1000;
int SCREEN_H = 600;
int SIDEBAR_W = 280;
int GRAPH_W = SCREEN_W - SIDEBAR_W;

struct Functie
{
    string expresie;
    int culoare;

};

vector<Functie> listaFunctii;
vector<string> raport_analiza;

string input_functie = "";
string input_a1 = "-2";
string input_b1 = "2";
string input_eval_x = "0";
string mesaj_status = "";

int focus_element = 0;
int culoare_curenta = 12;

bool interval_activ = false;
bool afisare_rezultate = false;

double a = -5.0, b = 5.0;
double c = -5.0, d = 5.0;

double val_a1 = -2.0;
double val_b1 = 2.0;

double get_nan()
{
    return sqrt(-1.0);
}

bool is_nan(double x)
{
    return (x != x);
}

double aplica_op(double a, double b, char op)
{
    switch(op)
    {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return (a == 0 || b == 0) ? 0 : a * b;
    case '/':
        return b != 0 ? a / b : get_nan();
    case '^':
        return pow(a, b);
    case '>':
        return a >= b;
    case '<':
        return a < b;
    case '=':
        return a == b;
    case '#':
        return a != b;
    default:
        return 0;
    }
}

double aplica_functie(double val, char func)
{
    switch(func)
    {
    case 's':
        return sin(val);
    case 'c':
        return cos(val);
    case 'e':
        return exp(val);
    case 'l':
        return (val <= 0) ? get_nan() : log(val);
    case 'a':
        return fabs(val);
    case 'q':
        return (val < 0) ? get_nan() : sqrt(val);
    default:
        return val;
    }
}

bool e_functie(char op)
{
    return (strchr("scelaq", op) != NULL);
}

bool e_operator(char op)
{
    return (strchr("+-*/^><=#", op) != NULL);
}

int prioritate(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    if (op == '^')
        return 3;
    return 0;
}

bool valideaza_expresie(string expr)
{
    if (expr.empty())
    {
        mesaj_status = "Expresie vida!";
        return false;
    }
    int paranteze = 0;
    char last_char = '(';
    for (int i = 0; i < expr.length(); i++)
    {
        char ch = expr[i];
        if (ch == ' ')
            continue;
        if (ch == '(')
            paranteze++;
        if (ch == ')')
            paranteze--;
        if (paranteze < 0)
        {
            mesaj_status = "Paranteze gresite!";
            return false;
        }
        if (e_operator(ch))
        {
            if (e_operator(last_char))
            {
                mesaj_status = "Operatori consecutivi!";
                return false;
            }
            if (last_char == '(' && ch != '-')
            {
                mesaj_status = "Operator la start!";
                return false;
            }
        }
        if(e_operator(last_char) && ch == ')')
            {
                mesaj_status = "Operator la final";
                return false;
            }
        last_char = ch;
    }
    if (paranteze != 0)
    {
        mesaj_status = "Paranteze neechilibrate!";
        return false;

        if (e_operator(last_char))
        {
            mesaj_status = "Se termina cu operator!";
            return false;
        }
        mesaj_status = "";
        return true;
    }
    return true;
}
double evaluare_expresie(double x, string expresie_curenta)
{
    stack<double> Opd;
    stack<char> Op;
    Op.push('(');
    string expr = expresie_curenta + ")";
    for (unsigned int i = 0; i < expr.length(); i++)
    {
        if (expr[i] == ' ')
            continue;
        if (isdigit(expr[i]) || expr[i] == '.')
        {
            string numStr = "";
            while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.'))
            {
                numStr += expr[i];
                i++;
            }
            i--;
            Opd.push(atof(numStr.c_str()));
        }
        else if (expr[i] == 'x')
            Opd.push(x);
        else if (isalpha(expr[i]))
        {
            string nume = "";
            while (i < expr.length() && isalpha(expr[i]))
            {
                nume += expr[i];
                i++;
            }
            i--;
            if (nume == "sin")
                Op.push('s');
            else if (nume == "cos")
                Op.push('c');
            else if (nume == "exp")
                Op.push('e');
            else if (nume == "ln")
                Op.push('l');
            else if (nume == "abs")
                Op.push('a');
            else if (nume == "sqr")
                Op.push('q');
        }
        else if (expr[i] == '(' && expr[i+1] == '-')
        {
            Op.push('(');
            Op.push('-');
            Opd.push(0.0);
            i++;
        }
        else if (expr[i] == '(')
            Op.push('(');
        else if (expr[i] == ')')
        {
            while (!Op.empty() && Op.top() != '(')
            {
                if (Opd.size() < 2)
                    break;
                double val2 = Opd.top();
                Opd.pop();
                double val1 = Opd.top();
                Opd.pop();
                char op = Op.top();
                Op.pop();
                Opd.push(aplica_op(val1, val2, op));
            }
            if (!Op.empty())
                Op.pop();
            if (!Op.empty() && e_functie(Op.top()))
            {
                if (!Opd.empty())
                {
                    char func = Op.top();
                    Op.pop();
                    double val = Opd.top();
                    Opd.pop();
                    Opd.push(aplica_functie(val, func));
                }
            }
        }
        else if (strchr("+-*/^><=#", expr[i]))
        {
            while (!Op.empty() && !e_functie(Op.top()) && Op.top() != '(' &&
                    prioritate(Op.top()) >= prioritate(expr[i]))
            {
                double val2 = Opd.top();
                Opd.pop();
                double val1 = Opd.top();
                Opd.pop();
                char op = Op.top();
                Op.pop();
                Opd.push(aplica_op(val1, val2, op));
            }
            Op.push(expr[i]);
        }
    }
    if (!Opd.empty())
        return Opd.top();
    return 0.0;

}

double f(double x, string expresie)
{
    return evaluare_expresie(x, expresie);
}

void salvare_fisiere()
{
    ofstream out("functii_salvate.txt");
    out << listaFunctii.size() << endl;
    for(size_t i=0; i<listaFunctii.size(); i++)
    {
        out << listaFunctii[i].culoare << endl;
        out << listaFunctii[i].expresie << endl;
    }
    out.close();
    mesaj_status = "Functii salvate!";
}

void incarcare_fisiere()
{
    ifstream in("functii_salvate.txt");
    if(!in.is_open())
    {
        mesaj_status = "Fisier lipsa!";
        return;
    }
    int n;
    in >> n;
    listaFunctii.clear();
    for(int i=0; i<n; i++)
    {
        Functie f;
        in >> f.culoare;
        in.ignore();
        getline(in, f.expresie);
        listaFunctii.push_back(f);
    }
    in.close();
    mesaj_status = "Functii incarcate!";

}

int x_to_px(double x)
{
    return SIDEBAR_W + (x - a) / (b - a) * GRAPH_W;
}

int y_to_px(double y)
{
    return SCREEN_H - (y - c) / (d - c) * SCREEN_H;
}

double calc_pas(double x, double y)
{
    double interval = y - x;
    double pas = interval / 10.0;
    return 5.0 * pow(10, floor(log10(pas)));
}

void calculeaza_puncte_extreme()
{
    raport_analiza.clear();
    double start_x, end_x;
    if (interval_activ)
    {
        start_x = val_a1;
        end_x = val_b1;
    }
    else
    {
        start_x = a;
        end_x = b;
    }

    double step = (end_x - start_x) / 1500.0;

    for(int i=0; i<listaFunctii.size(); i++)
    {
        string expr = listaFunctii[i].expresie;
        char header[50];
        string short_expr = expr;
        if(short_expr.length() > 12)
            short_expr = short_expr.substr(0,12) + "..";
        sprintf(header, "--- F%d: %s ---", i+1, short_expr.c_str());
        raport_analiza.push_back(string(header));
        double min_y = DBL_MAX;
        double max_y = -DBL_MAX;
        double x_at_min = 0;
        double x_at_max = 0;
        bool found_any_valid = false;
        for(double x = start_x; x <= end_x; x += step)
        {
            double y = f(x, expr);
            if(!is_nan(y) && y > -DBL_MAX && y < DBL_MAX)
            {
                    found_any_valid = true;
                    if(y < min_y)
                    {
                        min_y = y;
                        x_at_min = x;
                    }
                    if(y > max_y)
                    {
                        max_y = y;
                        x_at_max = x;
                    }
            }
        }
        if(!found_any_valid)
            raport_analiza.push_back("  Nedefinita in interval.");
        else
        {
            char buf_min[64];
            sprintf(buf_min, "  Min: %.2f (x=%.2f)", min_y, x_at_min);
            raport_analiza.push_back(string(buf_min));
            char buf_max[64];
            sprintf(buf_max, "  Max: %.2f (x=%.2f)", max_y, x_at_max);
            raport_analiza.push_back(string(buf_max));
        }
    }
}

void deseneaza_sidebar()
{
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(0, 0, SIDEBAR_W, SCREEN_H);
    setcolor(BLACK);
    line(SIDEBAR_W, 0, SIDEBAR_W, SCREEN_H);
    setbkcolor(LIGHTGRAY);
    setcolor(BLACK);
    outtextxy(10, 35, "Functie f(x)=");
    setfillstyle(SOLID_FILL, WHITE);
    setcolor(focus_element == 1 ? RED : BLACK);
    rectangle(10, 55, SIDEBAR_W - 10, 85);
    bar(11, 56, SIDEBAR_W - 11, 84);
    setbkcolor(WHITE);
    setcolor(BLACK);
    string text_vizibil = input_functie;
    if (input_functie.length() > 28)
        text_vizibil = input_functie.substr(input_functie.length() - 28);
    outtextxy(15, 60, (char*)text_vizibil.c_str());
    if(mesaj_status != "")
    {
        setbkcolor(LIGHTGRAY);
        setcolor(RED);
        outtextxy(10, 88, (char*)mesaj_status.c_str());
    }

    setbkcolor(LIGHTGRAY);
    setcolor(BLACK);
    outtextxy(10, 110, "Interval [a, b]:");
    rectangle(150, 110, 165, 125);
    if(interval_activ)
    {
        setcolor(BLUE);
        line(150, 110, 165, 125);
        line(150, 125, 165, 110);
        setcolor(BLACK);
    }
    outtextxy(170, 110, "Activ");
    setcolor(focus_element == 2 ? RED : BLACK);
    setfillstyle(SOLID_FILL, WHITE);
    bar(10, 130, 80, 155);
    rectangle(10, 130, 80, 155);
    setbkcolor(WHITE);
    setcolor(BLACK);
    outtextxy(15, 135, (char*)input_a1.c_str());
    setcolor(focus_element == 3 ? RED : BLACK);
    bar(90, 130, 160, 155);
    rectangle(90, 130, 160, 155);
    outtextxy(95, 135, (char*)input_b1.c_str());

    setbkcolor(LIGHTGRAY);
    setcolor(BLACK);
    outtextxy(10, 165, "Culoare:");
    int culori[] = {12, 10, 9, 14, 13, 11, 3, 5, 6, 0};
    for(int i=0; i<10; i++)
    {
        setfillstyle(SOLID_FILL, culori[i]);
        int x1 = 10 + i * 25;
        bar(x1, 185, x1 + 20, 205);
        if(culori[i] == culoare_curenta)
        {

            setcolor(WHITE);
            rectangle(x1+2, 187, x1+18, 203);
            setcolor(BLACK);
            rectangle(x1, 185, x1+20, 205);
        }
    }
    setcolor(BLACK);
    setbkcolor(LIGHTGRAY);
    setfillstyle(SOLID_FILL, GREEN);
    bar(10, 220, 90, 250);

    setcolor(WHITE);
    rectangle(10, 220, 90, 250);
    setbkcolor(GREEN);
    outtextxy(25, 228, "ADD");

    setfillstyle(SOLID_FILL, RED);
    bar(100, 220, 180, 250);

    setcolor(WHITE);
    rectangle(100, 220, 180, 250);
    setbkcolor(RED);
    outtextxy(115, 228, "DEL");

    setfillstyle(SOLID_FILL, afisare_rezultate ? YELLOW : DARKGRAY);
    bar(190, 220, 270, 250);

    setcolor(WHITE);
    rectangle(190, 220, 270, 250);

    setbkcolor(afisare_rezultate ? YELLOW : DARKGRAY);
    setcolor(afisare_rezultate ? BLACK : WHITE);

    outtextxy(200, 228, "ANALIZA");

    setfillstyle(SOLID_FILL, BLUE);
    bar(10, 260, 90, 290);

    setcolor(WHITE);
    rectangle(10, 260, 90, 290);
    setbkcolor(BLUE);
    outtextxy(25, 268, "SAVE");

    setfillstyle(SOLID_FILL, MAGENTA);
    bar(100, 260, 180, 290);

    setcolor(WHITE);
    rectangle(100, 260, 180, 290);
    setbkcolor(MAGENTA);
    outtextxy(115, 268, "LOAD");

    setbkcolor(LIGHTGRAY);
    setcolor(BLACK);
    outtextxy(10, 305, "Evaluare X=");
    setfillstyle(SOLID_FILL, WHITE);
    setcolor(focus_element == 4 ? RED : BLACK);
    rectangle(100, 300, 200, 325);
    bar(101, 301, 199, 324);
    setbkcolor(WHITE);
    setcolor(BLACK);
    outtextxy(105, 305, (char*)input_eval_x.c_str());
    setbkcolor(LIGHTGRAY);
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, WHITE);
    bar(10, 340, SIDEBAR_W - 10, SCREEN_H - 10);
    rectangle(10, 340, SIDEBAR_W - 10, SCREEN_H - 10);

    int y_pos = 350;

    if(!afisare_rezultate)
    {
        outtextxy(10, 340 - 15, "Functii active:");
        setbkcolor(WHITE);

        double val_x_calc = atof(input_eval_x.c_str());
        for(int i=0; i<listaFunctii.size(); i++)
        {
            if(y_pos > SCREEN_H - 20)
                break;
            setcolor(listaFunctii[i].culoare);
            string display_str = listaFunctii[i].expresie;

            double rezultat = f(val_x_calc, display_str);
            char val_buf[30];
            if(is_nan(rezultat))
                sprintf(val_buf, " err");
            else
                sprintf(val_buf, " %.2f", rezultat);

            if(display_str.length() > 15)
                display_str = display_str.substr(0, 12) + "...";
            char buffer[100];
            sprintf(buffer, "%d. %s | =%s", i+1, display_str.c_str(), val_buf);
            outtextxy(15, y_pos, buffer);
            y_pos += 20;
        }
    }
    else
    {
        outtextxy(10, 340 - 15, "Rezultate Analiza:");
        setbkcolor(WHITE);
        setcolor(BLACK);
        for(int i=0; i<raport_analiza.size(); i++)
        {
            if(y_pos > SCREEN_H - 20)
                break;
            if(raport_analiza[i].rfind("---", 0) == 0)
                setcolor(BLUE);
            else
                setcolor(BLACK);
            outtextxy(15, y_pos, (char*)raport_analiza[i].c_str());
            y_pos += 20;
        }
    }
}

void deseneaza_grafic()
{
    setfillstyle(SOLID_FILL, WHITE);
    bar(SIDEBAR_W + 1, 0, SCREEN_W, SCREEN_H);
    int ox = x_to_px(0.0);
    int oy = y_to_px(0.0);
    setcolor(BLACK);
    bool axa_y_vizibila = (ox > SIDEBAR_W && ox < SCREEN_W);
    bool axa_x_vizibila = (oy > 0 && oy < SCREEN_H);
    if(axa_y_vizibila)
        line(ox, 0, ox, SCREEN_H);
    if(axa_x_vizibila)
        line(SIDEBAR_W, oy, SCREEN_W, oy);
    setbkcolor(WHITE);

    if (axa_x_vizibila)
        setcolor(BLACK);
    else
        setcolor(DARKGRAY);

    int anchor_y_text;
    if(axa_x_vizibila)
        anchor_y_text = oy + 5;
    else if(oy <= 0)
        anchor_y_text = 5;
    else
        anchor_y_text = SCREEN_H - 20;
    int anchor_y_tick;
    if(axa_x_vizibila)
        anchor_y_tick = oy;
    else if(oy <= 0)
        anchor_y_tick = 0;
    else
        anchor_y_tick = SCREEN_H;
    double pas = calc_pas(a, b);
    for (double i = ceil(a / pas) * pas; i <= b; i += pas)
    {
        if (abs(i) < 1e-9)
            i = 0.0;
        int px = x_to_px(i);
        if(px > SIDEBAR_W + 5 && px < SCREEN_W)
        {
            line(px, anchor_y_tick - 3, px, anchor_y_tick + 3);
            char buf[16];
            sprintf(buf, "%.4g", i);
            outtextxy(px - 5, anchor_y_text, buf);

        }
    }

    if (axa_y_vizibila)
        setcolor(BLACK);

    else
        setcolor(DARKGRAY);

    int anchor_x_text;

    if(axa_y_vizibila)
        anchor_x_text = ox + 5;
    else if(ox <= SIDEBAR_W)
        anchor_x_text = SIDEBAR_W + 5;
    else
        anchor_x_text = SCREEN_W - 50;
    int anchor_x_tick;
    if(axa_y_vizibila)
        anchor_x_tick = ox;
    else if(ox <= SIDEBAR_W)
        anchor_x_tick = SIDEBAR_W;
    else
        anchor_x_tick = SCREEN_W;
    pas = calc_pas(c, d);
    for (double i = ceil(c / pas) * pas; i <= d; i += pas)
    {
        if (abs(i) < 1e-9)
            i = 0.0;
        if(i != 0)
        {
            int py = y_to_px(i);
            line(anchor_x_tick - 3, py, anchor_x_tick + 3, py);
            char buf[16];
            sprintf(buf, "%.4g", i);
            outtextxy(anchor_x_text, py - 5, buf);
        }
    }

    setcolor(BLACK);
    if(interval_activ)
    {

        setcolor(BLUE);
        setlinestyle(DOTTED_LINE, 0, 1);
        int px_a1 = x_to_px(val_a1);
        int px_b1 = x_to_px(val_b1);
        if(px_a1 > SIDEBAR_W && px_a1 < SCREEN_W)
            line(px_a1, 0, px_a1, SCREEN_H);
        if(px_b1 > SIDEBAR_W && px_b1 < SCREEN_W)
            line(px_b1, 0, px_b1, SCREEN_H);
        setlinestyle(SOLID_LINE, 0, 1);
    }
    if(!input_eval_x.empty())
    {
        double val_x_eval = atof(input_eval_x.c_str());
        int px_eval = x_to_px(val_x_eval);
        if(px_eval > SIDEBAR_W && px_eval < SCREEN_W)
        {
            setcolor(RED);
            setlinestyle(DOTTED_LINE, 0, 1);
            line(px_eval, 0, px_eval, SCREEN_H);
            setlinestyle(SOLID_LINE, 0, 1);
        }
    }
    double step = (b - a) / GRAPH_W;

    double start_x = interval_activ ? max(a, val_a1) : a;
    double end_x = interval_activ ? min(b, val_b1) : b;

    for(int i=0; i<listaFunctii.size(); i++)
    {
        setcolor(listaFunctii[i].culoare);
        string expr = listaFunctii[i].expresie;
        bool prima = true;
        int prev_px = 0, prev_py = 0;
        double preprev_y = 0, prev_y = 0;
        for(double x = start_x; x <= end_x; x += step)
        {
            double y = f(x, expr);
            if(!is_nan(y))
            {
                int px = x_to_px(x);
                int py = SCREEN_H - (y - c) / (d - c) * SCREEN_H;
                if(px > SIDEBAR_W && px < SCREEN_W)
                {
                    if(preprev_y == 0 && prev_y == 0)
                        prima = true;
                    if(!prima)
                        if((abs(py - prev_py) < SCREEN_H * 0.6 ) && y!=0)
                            line(prev_px, prev_py, px, py);
                    preprev_y = prev_y;
                    prev_y = y;
                    prev_px = px;
                    prev_py = py;
                    prima = false;

                }
            }
        }
        double val_x_eval = atof(input_eval_x.c_str());
        if(val_x_eval >= start_x && val_x_eval <= end_x)
        {
            double val_y_eval = f(val_x_eval, expr);
            if(!is_nan(val_y_eval))
            {
                int px_pt = x_to_px(val_x_eval);
                int py_pt = y_to_px(val_y_eval);
                if(px_pt > SIDEBAR_W && px_pt < SCREEN_W && py_pt > 0 && py_pt < SCREEN_H)
                {
                    setfillstyle(SOLID_FILL, listaFunctii[i].culoare);
                    fillellipse(px_pt, py_pt, 4, 4);

                }
            }
        }
    }
}

void deseneaza_tot()
{
    deseneaza_sidebar();
    deseneaza_grafic();
}

void reset()
{
    a = -5.0, b = 5.0;
    c = -5.0, d = 5.0;
}

void verifica_mouse()
{
    if(ismouseclick(WM_LBUTTONDOWN))
    {
        int mx, my;
        getmouseclick(WM_LBUTTONDOWN, mx, my);
        if(mx >= 10 && mx <= SIDEBAR_W - 10 && my >= 55 && my <= 85)
            focus_element = 1;
        else if(mx >= 10 && mx <= 80 && my >= 130 && my <= 155)
            focus_element = 2;
        else if(mx >= 90 && mx <= 160 && my >= 130 && my <= 155)
            focus_element = 3;
        else if(mx >= 100 && mx <= 200 && my >= 300 && my <= 325)
            focus_element = 4;
        else if(mx < SIDEBAR_W)
            focus_element = 0;
        if(mx >= 150 && mx <= 165 && my >= 110 && my <= 125)
        {
            interval_activ = !interval_activ;
            val_a1 = atof(input_a1.c_str());
            val_b1 = atof(input_b1.c_str());
            if(afisare_rezultate)
                calculeaza_puncte_extreme();
        }
        if(my >= 185 && my <= 205)
        {
            int paleta[] = {12, 10, 9, 14, 13, 11, 3, 5, 6, 0};
            for(int i=0; i<10; i++)
            {
                int x1 = 10 + i * 25;
                if(mx >= x1 && mx <= x1 + 20)
                    culoare_curenta = paleta[i];
            }
        }
        if(mx >= 10 && mx <= 90 && my >= 220 && my <= 250)
        {
            if(valideaza_expresie(input_functie))
            {
                Functie f;
                f.expresie = input_functie;
                f.culoare = culoare_curenta;
                listaFunctii.push_back(f);
                mesaj_status = "";
                if(afisare_rezultate)
                    calculeaza_puncte_extreme();
            }
        }
        if(mx >= 100 && mx <= 180 && my >= 220 && my <= 250)
        {
            if(!listaFunctii.empty())
                listaFunctii.pop_back();
            if(afisare_rezultate)
                calculeaza_puncte_extreme();
        }
        if(mx >= 190 && mx <= 270 && my >= 220 && my <= 250)
        {
            afisare_rezultate = !afisare_rezultate;
            if(afisare_rezultate)
                calculeaza_puncte_extreme();
        }
        if(mx >= 10 && mx <= 90 && my >= 260 && my <= 290)
            salvare_fisiere();
        if(mx >= 100 && mx <= 180 && my >= 260 && my <= 290)
            incarcare_fisiere();
        deseneaza_tot();
    }
}

void verifica_tastatura()
{
    if(kbhit())
    {
        char ch = getch();
        if(focus_element > 0)
        {
            string* tinta = NULL;
            int limita = 20;
            if(focus_element == 1)
            {
                tinta = &input_functie;
                limita = 200;
            }
            if(focus_element == 2)
                tinta = &input_a1;
            if(focus_element == 3)
                tinta = &input_b1;
            if(focus_element == 4)
                tinta = &input_eval_x;
            if(ch == 8)
            {
                if(tinta->length() > 0)
                    tinta->resize(tinta->length() - 1);
            }

            else if(ch >= 32 && ch <= 126)
            {
                if(tinta->length() < limita)
                    *tinta += ch;
            }
            if(focus_element == 2 || focus_element == 3)
            {
                val_a1 = atof(input_a1.c_str());
                val_b1 = atof(input_b1.c_str());

            }
            deseneaza_tot();
        }
        else
        {

            double dx = (b - a) * 0.1;
            double dy = (d - c) * 0.1;
            if (ch == 114)
            {
                reset();
            }
            else if (ch == 75)
            {
                a -= dx;
                b -= dx;
            }
            else if (ch == 77)
            {
                a += dx;
                b += dx;
            }
            else if (ch == 72)
            {
                c += dy;
                d += dy;
            }
            else if (ch == 80)
            {
                c -= dy;
                d -= dy;
            }
            else if (ch == '=')
            {
                double cx = (a + b) / 2, cy = (c + d) / 2;
                double lx = (b - a) * 0.4, ly = (d - c) * 0.4;
                a = cx - lx;
                b = cx + lx;
                c = cy - ly;
                d = cy + ly;
            }
            else if (ch == '-')
            {
                double cx = (a + b) / 2, cy = (c + d) / 2;
                double lx = (b - a) * 0.625, ly = (d - c) * 0.625;
                a = cx - lx;
                b = cx + lx;
                c = cy - ly;
                d = cy + ly;
            }
            if(!interval_activ && afisare_rezultate)
                calculeaza_puncte_extreme();
            deseneaza_tot();
        }
    }
}

int main()
{
    initwindow(SCREEN_W, SCREEN_H, "Grafic Functii");
    deseneaza_tot();
    while(true)
    {
        verifica_mouse();
        verifica_tastatura();
        delay(10);
    }
    closegraph();
    return 0;

}
