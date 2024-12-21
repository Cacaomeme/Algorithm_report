#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ノードの構造体定義
typedef struct Node {
    int value;
    struct Node *left;
    struct Node *right;
} Node;

// ノードを作成
Node* create_node(int value) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// BST に挿入
Node* insert_bst(Node* root, int value) {
    if (root == NULL) {
        return create_node(value);
    }
    if (value < root->value) {
        root->left = insert_bst(root->left, value);
    } else {
        root->right = insert_bst(root->right, value);
    }
    return root;
}

// BST の高さを計算 (0から始まる)
int calculate_height(Node* root) {
    if (root == NULL) {
        return -1; // 空の木は高さ -1
    }
    int left_height = calculate_height(root->left);
    int right_height = calculate_height(root->right);
    return 1 + (left_height > right_height ? left_height : right_height);
}

// BST を解放
void free_bst(Node* root) {
    if (root != NULL) {
        free_bst(root->left);
        free_bst(root->right);
        free(root);
    }
}

// BST の形状キーを生成 (事前順序走査を文字列化)
void generate_shape_key(Node* root, char* key) {
    if (root == NULL) {
        strcat(key, "N"); // NULL を "N" で表現
        return;
    }
    char tmp[10] ;
    sprintf(tmp, "%d", root->value);
    strcat(key, tmp); // ノードが存在する部分を "X" で表現
    generate_shape_key(root->left, key);
    generate_shape_key(root->right, key);
}

// 配列を交換
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 完全二分木かどうかを判定する関数
int is_complete(Node* root, int* difference){
    if (root == NULL){
        return 0;
    }

    int left_count = 0;
    int right_count = 0;
    left_count = is_complete(root->left, difference);
    right_count = is_complete(root->right, difference);
    if (abs((right_count - left_count)) > 1){
        *difference = 1;
    }
    return left_count + right_count + 1;
}

// AVL木かどうかを判定する関数
int is_avl(Node* root, int* height) {
    if (root == NULL) {
        *height = -1; // 空の木の高さは -1
        return 1;     // 空の木はAVL木
    }

    int left_height = 0, right_height = 0;

    // 左右の部分木がAVL木かチェックしつつ高さを取得
    int left_avl = is_avl(root->left, &left_height);
    int right_avl = is_avl(root->right, &right_height);

    // 現在のノードがAVL木である条件
    *height = 1 + (left_height > right_height ? left_height : right_height);
    if (abs(left_height - right_height) > 1) {
        return 0; // 高さの差が1を超えている場合、AVL木ではない
    }

    return left_avl && right_avl; // 左右部分木もAVL木であるか
}

// ブランチの表示
void print_branch(char step[100], int height, int now){
    if (height - now > 1){
        for (int i = 0; i < (3 * (int)pow(2.0, (double)(height - now - 1))) / 2 - 1; i++){
            for(int j = 0; j < (int)pow(2.0, (double)now); j++){
                for (int k = 0; k < (3 * (int)pow(2.0, (double)(height - now - 1))) - 1 - i; k++){
                    printf(" ");
                }
                if (step[2 * j] != 'N' && step[2 * j] != 0){
                    printf("/");  // 左枝を表示
                }else{
                    printf(" ");
                }
                for (int k = 0; k < 2 * i + 1; k++){
                    printf(" ");
                }
                if (step[2 * j + 1] != 'N' && step[2 * j + 1] != 0){
                    printf("\\");  // 右枝を表示
                }else{
                    printf(" ");
                }
                for (int k = 0; k < (3 * (int)pow(2.0, (double)(height - now - 1))) - 2 - i; k++){
                    printf(" ");
                }
            }
            printf("\r\n");
        }
    } else {
        for(int j = 0; j < (int)pow(2.0, (double)now); j++){
            printf("  ");
            if (step[2 * j] != 'N' && step[2 * j] != 0){
                printf("/");  // 左枝
            } else {
                printf(" ");
            }
            printf(" ");
            if (step[2 * j + 1] != 'N' && step[2 * j + 1] != 0){
                printf("\\");  // 右枝
            } else {
                printf(" ");
            }
            printf(" ");
        }
        printf("\r\n");
    }
}

// BST を表示
void print_bst(char table[10][100], int height){
    for (int i = 0; i <= height; i++){
        for(int j = 0; j < (int)pow(2.0, (double)i); j++){
            if (i != height){
                for (int k = 0; k < 3 * (int)pow(2.0, (double)(height - i - 1)); k++){
                    printf(" ");
                }
                if (table[i][j] != 'N' && table[i][j] != 0){
                    printf("%c", table[i][j]);
                } else {
                    printf(" ");
                }
                for (int k = 1; k < 3 * (int)pow(2.0, (double)(height - i - 1)); k++){
                    printf(" ");
                }
            } else {
                if ((j % 2) == 0){
                    printf(" ");
                    if (table[i][j] != 'N' && table[i][j] != 0){
                        printf("%c", table[i][j]);
                    } else {
                        printf(" ");
                    }
                    printf("   ");
                    if (table[i][j + 1] != 'N' && table[i][j + 1] != 0){
                        printf("%c", table[i][j + 1]);
                    } else {
                        printf(" ");
                    }
                }
            }
        }   
        printf("\r\n");  
        if(i != height){
            print_branch(table[i + 1], height, i);
        }  
    }
}

void insert_and_shift(char arr[], int *size, int max_size, int index, char value) {
    if (*size >= max_size) {
        printf("配列が一杯で、挿入できません。\n");
        return;
    }
    if (index < 0 || index > *size) {
        printf("無効なインデックスです。\n");
        return;
    }

    // 要素を右にシフト
    for (int i = *size; i > index; i--) {
        arr[i] = arr[i - 1];
    }

    // 新しい値を挿入
    arr[index] = value;

    // サイズを更新
    (*size)++;
    arr[*size] = '\0'; // 終端文字を更新
}

// 順列を生成し、それぞれの BST を処理
void generate_permutations(int arr[], int start, int n, 
                           double* height_sum, double* height_sq_sum, 
                           int* unique_count, int* total_permutations,
                           double* avl_height_sum, double* avl_height_sq_sum,
                           int* avl_count, double* complete_height_sum,
                           double* complete_height_sq_sum, int* complete_count) {
    static char shapes[10000][100]; // 形状キーのリスト
    static int shape_count = 0;    // 登録された形状の数
    static int heights[10000];     // 各形状の高さ

    if (start == n) {
        for(int i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\r\n");

        // BST を構築
        Node* root = NULL;
        for (int i = 0; i < n; i++) {
            root = insert_bst(root, arr[i]);
        }

        // 形状キーを生成
        char key[100] = "";
        generate_shape_key(root, key);

        for (int i = 0; i < 100; i++){
            printf("%c", key[i]);
        }printf("\n");

        // 深さごとに数字を仕分けるためのテーブル
        char table[10][100] = {0}; // 各行に文字列を格納
        int row = 0;
        int check = 0;

        for (int i = 0; i < strlen(key); i++) {
            if (key[i] != 'N') {
                char temp[2] = {key[i], '\0'}; // key[i] を文字列に変換
                strcat(table[row], temp); // 現在の行に追加
                //if(check % 2 == 0 || key[i-1] != 'N' || (key[i-3]==key[i-2]==key[i-1]=='N')){
                //    check = 0;
                //}
                check = 0;
                row += 1; // 次の行に移動
            } else {
                char temp[2] = {key[i], '\0'}; // key[i] を文字列に変換
                strcat(table[row], temp); // 現在の行に追加
                check += 1;
                if (check > 1) {
                    if (key[i-1] == 'N' && key[i-2] == 'N' && key[i-3] != 'N' && key[i-4] == 'N' &&  key[i-5] != 'N'){
                        row -= 1;
                    }else if (key[i-1] == 'N' && key[i-2] == 'N' && key[i-3] != 'N' && key[i-4] == 'N'){
                        row -= 2;
                    }else if (key[i-1] == 'N' && key[i-2] == 'N' && key[i-3] != 'N' && key[i-4] != 'N' && key[i-5] == 'N'){
                        row -= 2;
                    }else if (key[i-1] == 'N' && key[i-2] != 'N' && key[i-3] == 'N' && key[i-4] == 'N'){
                        row -= 2;
                    }else if (key[i-1] == 'N' && key[i-2] != 'N' && key[i-3] == 'N' && key[i-4] != 'N' && key[i-5] == 'N' && key[i-6] != 'N'){
                        row -= 3;
                    }else if (key[i-1] == 'N' && key[i-2] != 'N' && key[i-3] == 'N' && key[i-4] != 'N'){
                        row -= 2;
                    }else{
                        row -= 1; // 'N' なら一つ前の行に戻る
                    }
                }
            }
        }

        // 高さを計算
        int height = calculate_height(root);

        // 結果をテーブルの形で表示
        printf("table\r\n");


        for (int i = 0; i < 10; i++) {
            if (table[i][0] != '\0') {
                printf("%s \n", table[i]);
            }
        }
        printf("table\r\n");
        for (int i = 0; i < 10; i++) {
            if (table[i][0] != '\0') {
                int row_size = strlen(table[i]); // 行の長さを取得
            for (int j = 0; j < row_size; j++) { // 各列について
                for (int k = i - 1; k >= 0; k--) { // i より小さい k
                    if (table[i][j] != 'N' && table[k][j] != 'N' && table[k][j] != '\0' && table[i][j] > table[k][j]) {
                        int size = strlen(table[i]); // 行の長さを更新
                        //for (int t = 0; t < height / 2 - j - 1; t++){
                            for (int s = i; s < height+1; s++){
                                insert_and_shift(table[s], &size, 100, j, 'N'); // 'N' を挿入
                                insert_and_shift(table[s], &size, 100, j, 'N'); // 'N' を挿入
                            }
                        //}
                        break; // 1回挿入したら次の列へ
                    }
                }
            }
                printf("%s \n", table[i]);
            }
        }


        // AVL木かどうか判定
        int avl_height = 0;
        int is_avl_tree = is_avl(root, &avl_height);

        // 完全二分木かどうか判定
        int difference = 0;
        int is_complete_tree = is_complete(root, &difference);

        // 新しい形状なら登録
        int found = 0;
        for (int i = 0; i < shape_count; i++) {
            if (strcmp(shapes[i], key) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(shapes[shape_count], key);
            heights[shape_count] = height;
            shape_count++;
            *unique_count += 1;
            *height_sum += height;
            *height_sq_sum += height * height;
            print_bst(table, height);

            // AVL木ならマーク表示
            if (is_avl_tree) {
                *avl_count += 1;
                *avl_height_sum += height;
                *avl_height_sq_sum += height * height;
                printf("This tree is an AVL tree!\n");
            }

            if (!difference){
                *complete_count += 1;
                *complete_height_sum += height;
                *complete_height_sq_sum += height * height;
                printf("This tree is a complete binary tree!\r\n");
                exit(1);
            }
        }

        // 総順列数をカウント
        *total_permutations += 1;

        // BST を解放
        free_bst(root);

        return;
    }

    for (int i = start; i < n; i++) {
        swap(&arr[start], &arr[i]);
        generate_permutations(arr, start + 1, n, height_sum, height_sq_sum, unique_count, total_permutations, avl_height_sum, avl_height_sq_sum, avl_count, complete_height_sum, complete_height_sq_sum, complete_count);
        swap(&arr[start], &arr[i]);
    }
}

int main() {
    int n;

    printf("整数 n を入力してください: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("n は正の整数である必要があります。\n");
        return 1;
    }

    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("メモリの確保に失敗しました。\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }

    double height_sum = 0.0;
    double avl_height_sum = 0.0;
    double complete_height_sum = 0.0;
    double height_sq_sum = 0.0;
    double avl_height_sq_sum = 0.0;
    double complete_height_sq_sum = 0.0;
    int unique_count = 0;
    int avl_count = 0;
    int complete_count = 0;
    int total_permutations = 0;

    generate_permutations(arr, 0, n, &height_sum, &height_sq_sum, &unique_count, &total_permutations, 
                        &avl_height_sum, &avl_height_sq_sum, &avl_count, 
                        &complete_height_sum, &complete_height_sq_sum, &complete_count);

    double mean_height = height_sum / unique_count;
    double variance_height = (height_sq_sum / unique_count) - (mean_height * mean_height);
    double mean_avl_height = avl_height_sum / avl_count;
    double variance_avl_height = (avl_height_sq_sum / avl_count) - (mean_avl_height * mean_avl_height);
    double mean_complete_height = complete_height_sum / complete_count;
    double variance_complete_height = (complete_height_sq_sum / complete_count) - (mean_complete_height * mean_complete_height);

    printf("順列の総数: %d\n", total_permutations);
    printf("相異なるBSTの数: %d\n", unique_count);
    printf("BSTの高さの平均: %.5f\n", mean_height);
    printf("BSTの高さの分散: %.5f\n", variance_height);
    printf("AVL木の数: %d\n", avl_count);
    printf("AVL木の高さの平均: %.5f\n", mean_avl_height);
    printf("AVL木の高さの分散: %.5f\n", variance_avl_height);
    printf("完全二分木の数: %d\n", complete_count);
    printf("完全二分木の高さの平均: %.5f\n", mean_complete_height);
    printf("完全二分木の高さの分散: %.5f\n", variance_complete_height);

    free(arr);
    return 0;
}
