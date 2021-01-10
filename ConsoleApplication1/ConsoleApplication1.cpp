#include <iostream>
using namespace std;

// "#" 代表墙壁、"_"代表空间、"."代表目的地、"o"代表箱子、"p"代表玩家
const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";
const int gStageWidth = 8;
const int gStageHeight = 5;

// 用枚举类来表示场景中的状态 
enum Object
{
    OBJ_WALL,
    OBJ_SPACE,
    OBJ_BLOCK,
    OBJ_BLOCK_ON_GOAL,
    OBJ_GOAL,
    OBJ_MAN,
    OBJ_MAN_ON_GOAL,

    OBJ_UNKNOWN,
};

// 函数声明
void initialize(Object *state, int w, int h, const char* stageData);
void draw(const Object *state, int w, int h);
void update(Object *state, char input, int w, int h);
bool checkClear(const Object *state, int w, int h);

int main()
{
    // 创建状态数组
    Object *state = new Object[gStageWidth * gStageHeight]; // 枚举类型是一种用于列举的类型，所以可以用类的方法new来生成
    initialize(state, gStageWidth, gStageHeight, gStageData);
    // 主循环
    while (true)
    {
        // 绘制场景
        draw(state, gStageWidth, gStageHeight);
        // 通关检测
        if (checkClear(state, gStageWidth, gStageHeight)) {
            break;
        }
        // 获取输入
        cout << "a:left d:right w:up s:down. command?" << endl;
        char input;
        cin >> input;
        // 更新
        update(state, input, gStageWidth, gStageHeight);
    }
    // 主循环结束，游戏通关
    cout << "Congratulation's! you win." << endl;
    delete[] state; // 通过new创建的而数组不能用delete,而应用delete[]
    state = 0;

    while (true) {
        ;
    }
    return 0;
}

void initialize(Object *state, int w, int h, const char *stageData) {
    const char *d = stageData;
    int x = 0;
    int y = 0;
    while (*d != '\0') {
        Object t;
        switch (*d){
            case '#': t = OBJ_WALL; break;
            case ' ': t = OBJ_SPACE; break;
            case 'o': t = OBJ_BLOCK; break;
            case 'O': t = OBJ_BLOCK_ON_GOAL; break;
            case '.': t = OBJ_GOAL; break;
            case 'p': t = OBJ_MAN; break;
            case 'P': t = OBJ_MAN_ON_GOAL; break;
            case '\n': // 换行
                x = 0;
                ++y;
                t = OBJ_UNKNOWN;
                break;
            default: t = OBJ_UNKNOWN; break;
        }
        ++d;
        // 遇到unknown则无视
        if (t != OBJ_UNKNOWN) {
            state[y * w + x] = t; // 写入
            ++x;
        }
    }
}

void draw(const Object* state, int w, int h) {
    const char font[] = { '#', ' ','o','O','.','p','P' }; // font的顺序需要与枚举类型Object的顺序相同
    for (int y = 0; y < h;++y)
    {
        for (int x = 0; x < w; ++x)
        {
            Object o = state[y * w + x];
            cout << font[o]; //枚举类型变量可以作为数组下标

        }
        cout << endl;
    }
}

void update(Object* state, char input, int w, int h) {

    // 将输入转换为移动量
    int dx = 0;
    int dy = 0;
    switch (input) {
    case 'w':dy = -1; break;
    case 's':dy = 1; break;
    case 'a':dx = -1; break;
    case 'd':dx = 1; break;
    }

    // 查找玩家坐标
    int i = -1;
    for (i = 0; i < w*h; ++i){
        if (state[i] == OBJ_MAN || state[i] == OBJ_MAN_ON_GOAL){
            break;
        }
    }
    int x = i % w;
    int y = i / w;

    // 移动后的坐标
    int tx = x + dx;
    int ty = y + dy;
    // 检查是否符合坐标的最大值与最小值
    if (tx < 0 || ty < 0 || tx >= w || ty >= h){
        return;
    }

    int p = y * w + x;
    int tp = ty * w + tx;

    if (state[tp] == OBJ_SPACE || state[tp] == OBJ_GOAL) {
        // 如果是目的地，就设置为目的地的玩家
        state[tp] = (state[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
        // 如果已经在目的地，就设置玩家的位置为目的地
        state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
    }
    // 要移动的位置有箱子，如果沿该方向的下一个网格是空白或目的地，则移动
    else if (state[tp] == OBJ_BLOCK || state[tp] == OBJ_BLOCK_ON_GOAL) {
        int tx2 = tx + dx;
        int ty2 = ty + dy;
        if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) {
            return;
        }
        int tp2 = (ty + dy) * w + (tx + dx);
        if (state[tp2] == OBJ_SPACE || state[tp2] == OBJ_GOAL) {
            state[tp2] = (state[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
            state[tp] = (state[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
            state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
        }
    }
}

bool checkClear(const Object* state, int w, int h) {
    for (int i = 0; i < w*h; i++)
    {
        if (state[i] == OBJ_BLOCK)
        {
            return false;
        }
    }
    return true;
}
