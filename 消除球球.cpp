//特性：当鼠标在发射口下方时，子弹无效
//建议：鼠标瞄准点击发射子弹时，敌人才消失，（现在在范围内，瞄准敌人就消失）
//想法：可以稍加修改，参照吸血鬼幸存者

//w=x；h=y；
//发射线宽10，方向误差0.05，合适参数(小范围）
//发射线宽1，方向误差0.01，合适参数（大范围）

#include<graphics.h>
#include<conio.h>
#include<math.h>
#include<iostream>
#include<time.h>

using namespace std;

struct game_window
{
    int wide;
    int height;
};

struct launch
{
    int w, h;
};

struct line_sight//瞄准线
{
    int s_w, s_h;
    int e_w, e_h;
    double sin, cos;
    COLORREF color;
};

struct enemy
{
    int w, h;
    double sin, cos;
    COLORREF color;
};


const int e_max = 100;
const int e_a_r = 100;//发射的有效攻击范围
const int line_wide = 10; //发射线宽
const double e_range = 0.05;//方向误差

game_window g_w;
launch lau;
MOUSEMSG m;
line_sight line_aim;//瞄准线实例
enemy e[e_max] = { 0 };

void set();
void show();
void c_line_aim();
void c_e();
void c_line_a_to_e();
int c_e_to_lau(enemy& e);//计算单个敌人对发射口的距离
bool between(enemy e, launch lau, double k);//计算敌人和瞄准线是否在同一个方向上（偏差内）



int main()
{
    set();

    initgraph(g_w.wide, g_w.height);

    while (true)
    {
        show();
        

        c_line_aim();
        c_e();
        c_line_a_to_e();

        Sleep(5);
        cleardevice();
    }

    _getch();

    closegraph();
}

bool between(enemy e, launch lau, double k)
{
    bool f = false;
    //if (e.sin>line_aim.sin-k && e.sin < line_aim.sin + k)
    //{
    //    if (e.cos > line_aim.cos - k && e.cos < line_aim.cos + k)
    //    {
    //        f = true;
    //    }
    //}

    if (fabs(line_aim.sin-e.sin)<k)
    {
        if (fabs(line_aim.cos - e.cos) < k)
        {
            f = true;
        }
    }

    return f;
}

int c_e_to_lau(enemy& e)
{
    int a = e.w - lau.w;//w
    int b = lau.h - e.h;//h
    double c = sqrt(a * a + b * b);
    e.sin = (b / c);
    e.cos = (a / c);
    return c;
}

void c_line_a_to_e()
{
    for (int i = 0; i < e_max; i++)
    {
        //c_e_to_lau(e[i]);
        if (c_e_to_lau(e[i]) < e_a_r)
        {
            if (between(e[i], lau, e_range))
            {
                e[i].w = rand() % g_w.wide;
                e[i].h = rand() % lau.h - lau.h;
            }
        }
    }
}

void c_e()
{
    for (int i = 0; i < e_max; i++)
    {
        if (e[i].h < g_w.height)
        {
            e[i].h++;
        }
        else
        {
            e[i].w = rand() % g_w.wide;
            e[i].h = rand() % lau.h - lau.h;
        }
    }
}

void c_line_aim()
{
    //m = GetMouseMsg();
    PeekMouseMsg(&m);

    if (m.mkLButton)
    {
        line_aim.color = GREEN;
    }
    else
    {
        line_aim.color = WHITE;
    }

    int m_l_lenth = (g_w.wide - lau.w)+(lau.h);

    int a = m.x - lau.w;//w
    int b = lau.h - m.y;//h
    double c = sqrt(a* a + b*b);
    line_aim.sin = (b / c);
    line_aim.cos = (a / c);

    line_aim.e_w = line_aim.s_w + line_aim.cos * m_l_lenth;
    line_aim.e_h = line_aim.s_h - fabs(line_aim.sin) * m_l_lenth;

}

void set()
{
    g_w.wide = 400;
    g_w.height = 600;

    lau.w = g_w.wide / 2;
    lau.h = g_w.height / 3 *2;

    line_aim.s_w = lau.w;
    line_aim.s_h = lau.h;

    srand(time(0));
    for (int i = 0; i < e_max; i++)
    {
        e[i].w = rand()%g_w.wide;
        e[i].h = rand()% lau.h -lau.h;
        e[i].color = WHITE;
    }
}

void show()
{
    BeginBatchDraw();

    //画发射口
    circle(lau.w, lau.h, 10);
    circle(lau.w, lau.h, e_a_r);
    
    //画瞄准线
    LINESTYLE* p=new LINESTYLE;
    getlinestyle(p);
    setlinestyle(NULL, line_wide, NULL, NULL);
    setcolor(line_aim.color);
    line(line_aim.s_w, line_aim.s_h, line_aim.e_w, line_aim.e_h);
    setlinestyle(p);
    setcolor(WHITE);

    //画敌人
    for (int i = 0; i < e_max; i++)
    {
        circle(e[i].w, e[i].h, 5);
    }


    //显示鼠标位置
    WCHAR s[50];
    _stprintf_s(s, _T("[%d,%d]"), m.x, m.y);
    outtextxy(0, 0, s);
    //_stprintf_s(s, _T("瞄准线：[%f,%f]"), line_aim.sin, line_aim.cos);
    //outtextxy(0, 15, s);
    //int T = 1;
    //for (int i = 0; i < e_max; i++)
    //{
    //    if (c_e_to_lau(e[i]) < e_a_r)
    //    {
    //        _stprintf_s(s, _T("瞄准范围内敌人：[%f,%f]"), e[i].sin, e[i].cos);
    //        outtextxy(0, 15+T++*15, s);
    //    }
    //}

    EndBatchDraw();
}
