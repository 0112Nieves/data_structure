#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define max(a, b) ((a) > (b) ? (a) : (b))
typedef struct{
    int row;
    int col;
    int value;
}matrix;
matrix w[105], x[105], b[105], z[105];

matrix* find_matrix(char c){
    if(c == 'w') return w;
    else if(c == 'x') return x;
    else if(c == 'b') return b;
    return NULL;
}

int find_location(matrix* m, int row, int col){
    int cnt = 1;
    while(cnt <= m[0].value){
        if(m[cnt].row == row && m[cnt].col == col) return m[cnt].value;
        else{
            cnt++;
        }
    }
    return -1;
}

void transpose(matrix* n1, matrix* n2){
    int cnt;
    n2[0].row = n1[0].col; n2[0].col = n1[0].row; n2[0].value = n1[0].value;
    if(n1[0].value){
        cnt = 1;
        for(int i = 0; i < n1[0].col; i++){
            for(int j = 1; j <= n1[0].value; j++){
                if(n1[j].col == i){
                    n2[cnt].row = n1[j].col;
                    n2[cnt].col = n1[j].row;
                    n2[cnt].value = n1[j].value;
                    cnt++;
                }
            }
        }
    }
}

void fast_transpose(matrix* n1, matrix* n2){
    int row_terms[105] = {0}, starting_pos[105] = {0}, num_cols = n1[0].col, num_terms = n1[0].value;
    n2[0].row = n1[0].col; n2[0].col = n1[0].row; n2[0].value = n1[0].value;
    // nonzero matrix
    if(num_terms){
        for(int i = 0; i < num_cols; i++) row_terms[i] = 0;
        for(int i = 1; i <= num_terms; i++) row_terms[n1[i].col]++;
        starting_pos[0] = 1;
        for(int i = 1; i < num_cols; i++) starting_pos[i] = starting_pos[i-1] + row_terms[i-1];
        int j;
        for(int i = 1; i <= num_terms; i++){
            j = starting_pos[n1[i].col]++;
            n2[j].row = n1[i].col; n2[j].col = n1[i].row;
            n2[j].value = n1[i].value;
        }
    }
}

void storesum(matrix* m, int* total, int row, int col, int* sum){
    if(*sum != 0){
        m[*total].row = row;
        m[*total].col = col;
        m[*total].value = *sum;
        (*total)++;
    }
    *sum = 0;
}

int compare(int a, int b){
    if(a > b) return 1;
    else if(a == b) return 0;
    else return -1;
}

matrix* sparse_matrix_multiply(matrix* w, matrix* x){
    matrix* result = (matrix*)malloc(105 * sizeof(matrix));
    if (result == NULL){
        printf("Memory allocation failed!\n");
        exit(1);
    }
    memset(result, 0, 105 * sizeof(matrix));
    matrix new_x[105];
    int col, row_begin = 1, sum = 0, row = w[1].row, total = 1;
    if(w[0].col != x[0].row){
        fprintf(stderr, "Incompatible matrices\n");
        exit(1);
    }
    fast_transpose(x, new_x);
    // set boundary condition
    w[w[0].value+1].row = w[0].row;
    new_x[x[0].value+1].row = x[0].col;
    new_x[x[0].value+1].col = 0;
    new_x[x[0].value+1].value = 0;
    for(int i = 1; i <= w[0].value; ){
        col = new_x[1].row;
        for(int j = 1; j <= new_x[0].value+1; ){
            if(w[i].row != row){
                storesum(result, &total, row, col, &sum);
                i = row_begin;
                for(; new_x[j].row == col; j++);
                col = new_x[j].row;
            }
            else if(new_x[j].row != col){
                storesum(result, &total, row, col, &sum);
                i = row_begin;
                col = new_x[j].row;
            }
            else switch(compare(w[i].col, new_x[j].col)){
                case -1:
                    i++;
                    break;
                case 0:
                    sum += (w[i++].value * new_x[j++].value);
                    
                    break;
                case 1:
                    j++;
                    break;
            }
        }      
        for(; w[i].row == row; i++);
        row_begin = i;
        row = w[i].row;
    }
    result[0].row = w[0].row;
    result[0].col = x[0].col;
    result[0].value = total-1;
    return result;
}

matrix* sparse_matrix_add(matrix* y, matrix* b){
    if(y[0].col != b[0].col || y[0].row != b[0].row){
        printf("These two matrices have different dimensions.\n");
        return NULL;
    }
    matrix* result = (matrix*)malloc(105 * sizeof(matrix));
    if (result == NULL){
        printf("Memory allocation failed!\n");
        exit(1);
    }
    result[0].row = b[0].row; 
    result[0].col = b[0].col; 
    int cnt = 1, i = 1, j = 1, sum;
    while (i <= b[0].value || j <= y[0].value) {
        if(i > b[0].value) result[cnt++] = y[j++];
        else if(j > y[0].value) result[cnt++] = b[i++];
        else{
            switch(compare(b[i].row, y[j].row)){
                case -1:
                    storesum(result, &cnt, b[i].row, b[i].col, &b[i].value);
                    i++;
                    break;
                case 0:
                    sum = b[i].value + y[j].value;
                    storesum(result, &cnt, b[i].row, b[i].col, &sum);
                    i++;
                    j++;
                    break;
                case 1:
                    storesum(result, &cnt, y[i].row, y[i].col, &y[j].value);
                    j++;
                    break;
            }
        }
    }
    result[0].value = cnt-1;
    return result;
}

void ReLU(matrix* z){
    int cnt = 0;
    for(int i = 1; i <= z[0].value; i++) z[i].value = max(0, z[i].value);
}

void Sigmoid(matrix* z) {
    for (int i = 1; i <= z[0].value; i++) z[i].value = 1.0 / (1.0 + exp(-z[i].value));
}

void final_result(matrix* w, matrix* x, matrix* b, matrix** z){
    matrix* tmp = (matrix*)malloc(105 * sizeof(matrix));
    tmp = sparse_matrix_multiply(w, x);
    *z = sparse_matrix_add(tmp, b);
    free(tmp);
}

void activate(int opt, matrix* z){
    if(opt == 0) ReLU(z);
    else if(opt == 1) Sigmoid(z);
    else printf("Please choose between 0 and 1.\n");
    return;
}

int main()
{
    printf("Please input total row & total col of matrix W: ");
    scanf("%d%d", &w[0].row, &w[0].col);
    printf("Please input how many number is in the matrix W: ");
    scanf("%d", &w[0].value);
    printf("Please input W matrix(row, col, value):\n");
    for(int i = 1; i <= w[0].value; i++) scanf("%d%d%d", &w[i].row, &w[i].col, &w[i].value);
    printf("Please input total row & total col of matrix X: ");
    scanf("%d%d", &x[0].row, &x[0].col);
    printf("Please input how many number is in the matrix X: ");
    scanf("%d", &x[0].value);
    printf("Please input X matrix(row, col, value):\n");
    for(int i = 1; i <= x[0].value; i++) scanf("%d%d%d", &x[i].row, &x[i].col, &x[i].value);
    printf("Please input total row & total col of matrix B: ");
    scanf("%d%d", &b[0].row, &b[0].col);
    printf("Please input how many number is in the matrix B: ");
    scanf("%d", &b[0].value);
    printf("Please input B matrix(row, col, value):\n");
    for(int i = 1; i <= b[0].value; i++) scanf("%d%d%d", &b[i].row, &b[i].col, &b[i].value);
    int row, col, value, cmd;
    char c;
    matrix* z = (matrix*)malloc(105 * sizeof(matrix));
    matrix* tmp = (matrix*)malloc(105 * sizeof(matrix));
    matrix* result = (matrix*)malloc(105 * sizeof(matrix));
    while(1){
        printf("\nPlease choose what you want to do:\n"
            "0: exit!\n"
            "1: Can output the values ​​of the specified rows and columns of the specified matrix.\n"
            "2: Computes the product W X of the sparse matrix W and the matrix X.\n"
            "3: Calculation of the fully connected layer Z = W X + B.\n"
            "4: Perform ReLU activation function on Z\n"
            "5: Perform Sigmoid activation function on Z\n"
            "6: Multiple fully connected layer (W and B) inputs can be specified, and their dimensions can be different (e.g. m * n, then n * k), and multiple.\n");
        scanf("%d", &cmd);
        if(cmd == 0) break;
        switch(cmd){
            case 1:
                printf("Please enter matrix name(w, b, x, z), row, col:");
                scanf(" %c%d%d", &c, &row, &col);
                value = find_location(find_matrix(c), row, col);
                if(value == -1) printf("The value at (%d, %d) in the matrix %c is 0.\n", row, col, c);
                else printf("The value at (%d, %d) in the matrix %c is %d.\n",row, col, c, value);
                break;
            case 2:
                result = sparse_matrix_multiply(w, x);
                for(int i = 0; i <= result[0].value; i++) printf("%d %d %d\n", result[i].row, result[i].col, result[i].value);
                break;
            case 3:
                final_result(w, x, b, &z);
                for(int i = 0; i <= z[0].value; i++) printf("%d %d %d\n", z[i].row, z[i].col, z[i].value);
                break;
            case 4:
                ReLU(z);
                for(int i = 0; i <= z[0].value; i++) printf("%d %d %d\n", z[i].row, z[i].col, z[i].value);
                break;
            case 5:
                Sigmoid(z);
                for(int i = 0; i <= z[0].value; i++) printf("%d %d %d\n", z[i].row, z[i].col, z[i].value);

                break;
            case 6:
                int layer, opt;
                printf("Please input how many layer do you want: ");
                scanf("%d", &layer);
                printf("Please choose what activate function you want to choose(0: ReLU, 1: Sigmod): ");
                scanf("%d", &opt);
                final_result(w, x, b, &z);
                activate(opt, z);
                result = z;
                for(int i = 1; i < layer; i++){
                    printf("Please input total row & total col of matrix W: ");
                    scanf("%d%d", &w[0].row, &w[0].col);
                    printf("Please input how many number is in the matrix W: ");
                    scanf("%d", &w[0].value);
                    printf("Please input W matrix(row, col, value):\n");
                    for(int i = 1; i <= w[0].value; i++) scanf("%d%d%d", &w[i].row, &w[i].col, &w[i].value);
                    printf("Please input total row & total col of matrix B: ");
                    scanf("%d%d", &b[0].row, &b[0].col);
                    printf("Please input how many number is in the matrix B: ");
                    scanf("%d", &b[0].value);
                    printf("Please input B matrix(row, col, value):\n");
                    for(int i = 1; i <= b[0].value; i++) scanf("%d%d%d", &b[i].row, &b[i].col, &b[i].value);
                    final_result(w, result, b, &z);
                    activate(opt, z);
                    result = z;
                }
                for(int i = 0; i <= z[0].value; i++){
                    if(z[i].value) printf("%d %d %d\n", z[i].row, z[i].col, z[i].value);
                }
                break;
        }
    }
    free(tmp);
    free(result);
    free(z);
    return 0;
}