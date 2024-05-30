#include "snake .h"

static unsigned long long int framesCounter = 0, points = 0;

static void ControlPlayer(Snake *S) { /* Manages player input */
    if (IsKeyPressed(KEY_UP) && S->speed.y != 20) S->speed = (Vector2){0, -20};
    else if (IsKeyPressed(KEY_DOWN) && S->speed.y != -20) S->speed = (Vector2){0, +20};
    if (IsKeyPressed(KEY_LEFT) && S->speed.x != 20) S->speed = (Vector2){-20, 0};
    else if (IsKeyPressed(KEY_RIGHT) && S->speed.x != -20) S->speed = (Vector2){+20, 0};
}

static int SetDifficulty(unsigned long long int p) { /* Increases speed of Snake */
    int k = 0;
    if (p < 1000) k = 6;
    else if (p < 1500) k = 5;
    else if (p < 1700) k = 4;
    else if (p < 2500) k = 3;
    else k = 2;
    return k;
}

static bool HeadCollisions(Snake *S, Apple *A) { /* Returns true if head collision with wall or self */
    /* Collision with apple */
    if (CheckCollisionRecs(S->head->box, A->box)) {
        Eat(S); /* Grow Snake */
        ReposApple(A); /* Repos apple Object */
        return false;
    }

    /* Check wall collision */
    if ( S->head->box.y < 0 || S->head->box.y > (float)GetScreenHeight() ) return true;
    if ( S->head->box.x < 0 || S->head->box.x > (float)GetScreenWidth() ) return true;

    /* Check with self collision */
    NodeSnake *current = S->head->next;
    while (current != NULL) {
        if (current->box.x == S->head->box.x && current->box.y == S->head->box.y)
            return true;
        current = current->next;
    }

    return false;
}

static void KillSnake( Snake *S ) { /* Stops snake and Draw a message */
    S->speed = (Vector2){0,0};
    const char *msg = "You Lost";
    BeginDrawing();
    int measure = MeasureText(msg, 50);
    int xPos = (GetScreenWidth()  -  measure)/2;
    DrawText(msg, xPos, GetScreenHeight()/2 - 50, 50, RED);
    EndDrawing();
    WaitTime(5);
}

static void DrawGame( Snake *S, Apple *A ) { /* Draw all the game */
    BeginDrawing();
    ClearBackground((Color){225, 198, 153, 255});

    /* Draw complete snake */
    NodeSnake *current = S->head;
    while (current != NULL) {
        DrawRectangleRec(current->box, current->c);
        current = current->next;
    }

    /* Draw points  */
    char *msg_points = (char *) malloc(21 * sizeof(char));/* still don't figure out how to convert static ull points to char* */
    sprintf(msg_points, "%llu", points);
    int measure = MeasureText(msg_points, 20);
    int xPos = GetScreenWidth() - measure;
    int yPos = 20;
    DrawText(msg_points, xPos - 20, yPos, 20, (Color){0, 0, 0, 150});

    /* Draw apple */
    DrawRectangleRec(A->box, A->c);
    EndDrawing();
    free(msg_points);
}

static void CloseGame(Snake *S, Apple *A) { /* Closes the game and free memory used  */
    SnakeDeath(S);
    DeleteApple(A);
}

static bool UpdateGame(Snake *S, Apple *A) { /* Update game variables */
    if (framesCounter == ULLONG_MAX) framesCounter = 0;
    if (points == ULLONG_MAX) points = 0;
    Move(S);
    return HeadCollisions(S, A);
}

/* Main Method for the game */
void MainSnake(void) {
    Snake *S = InitSnake();
    Apple *A = InitApple();
    SetWindowTitle("Snake");
    SetTargetFPS(60);

    while ( !WindowShouldClose() ) {
        if (UpdateGame(S, A)) {
            KillSnake(S);
            break;
        }
        DrawGame(S, A);
    }

    CloseGame(S, A);
}

/* ========== Snake Implementations ========== */

Snake *InitSnake(void) {
    Snake *S = (Snake *)malloc(sizeof(Snake));
    S->head = InitNode(0, 0, 20, 20);
    S->speed = (Vector2){0, 0};
    S->size = 1;
    return S;
}

void SnakeDeath(Snake *s) {
    NodeSnake *current = s->head;
    while ( current != NULL ) {
        NodeSnake *aux = current->next;
        DeleteNode(current);
        current = aux;
    }
    s->head = NULL;
    s->size = 0;
    free((void *)s);
    s = NULL;
}

void Move(Snake *S) {
    ControlPlayer(S);
    Vector2 pre = (Vector2){};
    Vector2 aux = (Vector2){};

    if (framesCounter % SetDifficulty(points) == 0) {
        /* Head movement */
        pre.x = S->head->box.x;
        pre.y = S->head->box.y;

        S->head->box.x += S->speed.x;
        S->head->box.y += S->speed.y;

        /* Chase the head with the body */
        NodeSnake *current = S->head->next;
        while ( current != NULL ) {
            aux.x = current->box.x;
            aux.y = current->box.y;

            current->box.x = pre.x;
            current->box.y = pre.y;

            pre = aux;
            current = current->next;
        }
    }

    framesCounter++;
}

void Eat(Snake *s) {
    if (s->size < MAX_SIZE) {
        Vector2 NewNodePos = (Vector2) {s->head->box.x - s->speed.x,
                                        s->head->box.y - s->speed.y};
        NodeSnake *new = InitNode(NewNodePos.x, NewNodePos.y,
                                  20, 20);
        new->next = s->head->next;
        s->head->next = new;
        s->size++;
    }
    points += 100;
}

/* =========================================== */

/* ============ Node implementations ========== */

NodeSnake *InitNode(float x, float y, float width, float height) {
    NodeSnake *n = (NodeSnake *)malloc(sizeof(NodeSnake));
    n->box = (Rectangle) {x, y, width, height};
    n->c = (Color){108, 187, 60, 255}; /* Snake's green */
    n->next = NULL;
    return n;
}

void DeleteNode(NodeSnake *n) {
    n->box = (Rectangle){0, 0, 0, 0};
    free((void *)n);
    n = NULL;
}

/* =========================================== */

/* =============== Apple Implementations ============== */

Apple *InitApple(void) {
    Apple *A = (Apple *) malloc(sizeof(Apple));
    A->box = (Rectangle) {(float)(20*(GetRandomValue(20, 1180)) % 1180),
                          (float)(20*(GetRandomValue(20, 780)) % 780),
                          20, 20};
    A->c = (Color){199, 55, 47, 255};
    return A;
}

void ReposApple(Apple *A) {
    A->box.x = (float)(20*(GetRandomValue(20, 1180)) % 1180);
    A->box.y = (float)(20*(GetRandomValue(20, 780)) % 780);
}

void DeleteApple(Apple *A) {
    A->box = (Rectangle){0, 0, 0, 0};
    free((void *)A);
    A = NULL;
}

/* ==================================================== */