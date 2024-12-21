#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// 配列を交換
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 順列を生成し、それぞれの BST を処理
void generate_permutations(int arr[], int start, int n) {
    if (start == n) {
        for(int i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\r\n");
        return;
    }

    for (int i = start; i < n; i++) {
        swap(&arr[start], &arr[i]);
        generate_permutations(arr, start + 1, n);
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

    generate_permutations(arr, 0, n);

    free(arr);
    return 0;
}
