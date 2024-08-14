#include "snake .h"

static Arena* memory_pool = (Arena *) NULL;
static Apple* apple = (Apple *) NULL;
static NodeSnake* snake = (NodeSnake *) NULL;
static Vector2 direction = (Vector2){ 0 };
static unsigned short int size = 0;
static bool alive = true;
static unsigned long long int points = 0;

static void InitGame ( void );
static void UpdateGame ( void );
static void DrawGame ( void );
static void CloseGame ( void );

static void InitNode ( NodeSnake* n, float x, float y );
static float SetDifficulty ( void );
static void SwitchDirection ( void );
static void SnakeDeath ( void );
static void SnakeCollision ( void );  /* Only with walls or itself */
static void MoveSnake ( void );
static void Eat ( void );
static void ReallocApple ( void );

static void DrawSnake ( void );
static void DrawApple ( void );

void MainSnake ( void ) {
    InitGame();
    while ( !WindowShouldClose() && alive ) {
        UpdateGame();
        DrawGame();
    }
    CloseGame();
}

void InitGame ( void ) {
    SetWindowTitle("Snake");
    memory_pool = CreateArena( MAX_SIZE * sizeof(NodeSnake) + sizeof(Apple) );
    apple = ArenaAlloc(memory_pool, sizeof(Apple));
    snake = ArenaAlloc(memory_pool, sizeof(NodeSnake));

    *apple = (Apple) {
            (Rectangle) {
                    (float)GetScreenWidth()/2,
                    (float)GetScreenHeight()/2,
                15.0f,
                15.0f
            },
            RED
    };
    InitNode(snake, 0, 0);
}

void UpdateGame ( void ) {
    SwitchDirection();
    MoveSnake();
    SnakeCollision();
    Eat();
}

void DrawGame ( void ) {
    char points_txt[21];
    sprintf(points_txt, "%llu", points);
    int width = MeasureText(points_txt, 20);
    BeginDrawing();
    ClearBackground( (Color) { 245, 245, 220, 255 } );
    DrawText(points_txt, GetScreenWidth() - 2*width, 30, 20, GRAY);
    DrawSnake();
    DrawApple();
    EndDrawing();
}

void CloseGame ( void ) {
    DestroyArena(memory_pool);
    memory_pool = NULL;
    apple = NULL;
    snake = NULL;
    size = 0;
    points = 0;
    alive = true;
    direction = (Vector2) {0};
}

static void InitNode ( NodeSnake* n, float x, float y ) {
    n->box = (Rectangle) {
        x,
        y,
        15.0f,
        15.0f,
    };
    n->next = NULL;
    n->c = GREEN;
}

static void SwitchDirection ( void ) {
    switch (GetKeyPressed()) {
        default:
            break;
        case KEY_LEFT:
            if ( direction.x != +15 ) direction = (Vector2) { -15, 0 };
            break;
        case KEY_RIGHT:
            if ( direction.x != -15 ) direction = (Vector2) { +15, 0 };
            break;
        case KEY_UP:
            if ( direction.y != +15 ) direction = (Vector2) { 0, -15 };
            break;
        case KEY_DOWN:
            if ( direction.y != -15 ) direction = (Vector2) { 0, +15 };
            break;
    }
}

static void SnakeCollision ( void ) {
    if (snake != NULL) {
        /* Check Collision with its body */
        NodeSnake *current = snake->next;
        while (current != NULL) {
            if (CheckCollisionRecs(snake->box, current->box)) {
                SnakeDeath();
                return;
            }
            current = current->next;
        }
        /* Check Collision with walls */
        if ( snake->box.y < 0 || snake->box.y + 15.0f > GetScreenHeight() || snake->box.x < 0 ||
            snake->box.x + 15.0f > GetScreenWidth() )
            SnakeDeath();
    }
}

static void SnakeDeath ( void ) {
    direction = (Vector2) {0, 0};
    char* text = "You Lose";
    int width = MeasureText(text, 30);
    BeginDrawing();
    DrawText(text, (GetScreenWidth() - width) / 2, (GetScreenHeight() - 30)/2, 30, RED);
    EndDrawing();
    WaitTime(3);
    alive = false; /* May add an option to restart the game */
}

static float SetDifficulty ( void ) {
    if ( points < 500 ) return 1.0f;
    else if ( points > 500 && points < 1000 ) return 1.0001f;
    else if ( points > 1000 && points < 1500 ) return 1.001f;
    return 1.1f;
}

static void MoveSnake ( void ) {
    if ( snake != NULL ) {

        Vector2 pre = (Vector2) { 0 };
        Vector2 aux = (Vector2) { 0 };

        pre.x = snake->box.x;
        pre.y = snake->box.y;

        snake->box.x += SetDifficulty()*direction.x;
        snake->box.y += SetDifficulty()*direction.y;

        NodeSnake* current = snake->next;

        while ( current != NULL ) {
            aux.x = current->box.x;
            aux.y = current->box.y;

            current->box.x = pre.x;
            current->box.y = pre.y;

            pre = aux;
            current = current->next;
        }

    }
}

static void Eat ( void ) {
    if ( snake != NULL && CheckCollisionRecs(snake->box, apple->box) && size <= MAX_SIZE ) {
        NodeSnake* new = ArenaAlloc(memory_pool, sizeof(NodeSnake));
        InitNode(new, snake->box.x - direction.x, snake->box.y - direction.y);
        new->next = snake->next;
        snake->next = new;
        points += points < MAX_POINTS ? 100 : 0;
        ReallocApple();
        size++;
    }
}

static void ReallocApple ( void ) {
    apple->box.x = (float)GetRandomValue( 10, 1270 );
    apple->box.y = (float)GetRandomValue( 10, 710 );
}

static void DrawApple ( void ) {
    if (apple != NULL)
        DrawRectangleRec(apple->box, apple->c);
}

static void DrawSnake ( void ) {
    NodeSnake* aux = snake;
    while ( aux != NULL ) {
        DrawRectangleRec(aux->box, aux->c);
        aux = aux->next;
    }
}