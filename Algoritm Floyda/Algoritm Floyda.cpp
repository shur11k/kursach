#include<stdio.h>
#include <iostream>
#include <string>
using namespace std;

/* Определите бесконечность как достаточно большое значение.
Это значение будет использоваться для вершин, не связанных друг с другом */
#define INF 99999

void toString(int** Matrix, int n); // предикатор функции

// Решает проблему кратчайшего пути для всех пар с использованием алгоритма Флойда Уоршалла
void floydWarshall(int** graph, int n)
{
    // dist[][] будет выходной матрицей, которая в конечном итоге будет иметь кратчайшие
    // расстояния между каждой парой вершин

    int** dist = (int**)malloc(sizeof(int*) * n);
    for (int i = 0; i < n; i++) {
        dist[i] = (int*)malloc(sizeof(int*) * n);
        for (int j = 0; j < n; j++)
        {
            dist[i][j] = graph[i][j];
            /* Инициализирую матрицу решения так же, как входную матрицу графика. Или
            можем сказать, что начальные значения кратчайших расстояний основаны
             на кратчайших путях, не учитывающих промежуточную вершину. */
        }
    }

    /* Добавил все вершины одну за другой к набору промежуточных вершин.
     ---> Перед началом итерации у нас есть кратчайшие расстояния между всеми
     парами вершин, такие, что на кратчайших расстояниях рассматриваются только
     вершины в наборе {0, 1, 2, .. k-1} в качестве промежуточных вершин.
     ----> После окончания итерации вершина № k добавляется к набору
    промежуточных вершин, и набор становится {0, 1, 2, .. k}
    */
    for (int k = 0; k < n; k++)
    {
        // Выбрал все вершины в качестве источника одну за другой
        for (int i = 0; i < n; i++)
        {
            // Выбрал все вершины в качестве места назначения для
            // выбранного выше источника
            for (int j = 0; j < n; j++)
            {
                // Если вершина k находится на кратчайшем пути от
                // i в j, затем обновил значение dist[i][j]

                /*if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j]; */ // реализация алгоритма флойда-уоршера

                    /*
                    if (dist[i][k] < INF && dist[k][j] < INF && dist[i][k] + dist[k][j] < dist[i][j])
                        dist[i][j] = dist[i][k] + dist[k][j];*/

                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]); // просто флойд

            }
        }
    }

    // Вывести матрицу кратчайшего расстояния
    printf("\nСледующая матрица показывает кратчайшие расстояния между каждой парой вершин \n");
    toString(dist, n);
}

// Функция для печати матрицы
void toString(int** Matrix, int n) {
    FILE* f;
    fopen_s(&f, "answer.txt", "a"); // файл для записи результата
    fprintf(f, "Ishodnaya \n");
    fprintf(f, "Ответ \n");
    for (int i = 0; i < n; i++)
    {
        printf("%d : ", i);
        fprintf(f, "%d : ", i);
        for (int j = 0; j < n; j++)
        {
            if (Matrix[i][j] == INF) {
                printf("%7s", "INF");
                fprintf(f, "%7s", "INF");
            }
            else {
                printf("%7d", Matrix[i][j]);
                fprintf(f, "%7d", Matrix[i][j]);
            }
        }
        printf("\n");
        fprintf(f, "\n");
    }
    fclose(f);
}

// программа драйвера для тестирования вышеуказанной функции
int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "RUS");
    FILE* file;
    fopen_s(&file, "matrix.txt", "r"); // файл для чтения матрицы из txt
    while (true) { // пока не выйдем сами

        int n;
        system("cls");// очистка консоли
        printf(" Введите кол-во вершин графа: "); scanf_s("%d", &n);

        int** Matrix = (int**)malloc(sizeof(int*) * n);
        for (int i = 0; i < n; i++) {
            Matrix[i] = (int*)malloc(sizeof(int*) * n);
            for (int j = 0; j < n; j++)
            {
                Matrix[i][j] = INF; // создание матрицы смежности( INF-бесконечность )
            }
        }

        string s; // используем стринг для защиты от (дурака)
        printf("\n Выберете способ построения графа: \n 1) Случайное заполнение \n");
        printf(" 2) Считать граф из файла \n 3) Выход\n Введите: "); cin >> s;

        switch (atoi(s.c_str()))
        {
        case 1:
            system("cls");

            int r;
            printf("Введите вероятность присутствия ребра(чем больше, тем выше вероятность): ");
            scanf_s("%d", &r);
            for (int i = 0; i < n; i++)
            {
                for (int j = i + 1; j < n; j++)
                {
                    if (1 <= (rand() % r)) {
                        int u = rand() % 3;
                        u == 0 ? Matrix[i][j] = rand() % 50 : u == 1 ? Matrix[j][i] = rand() % 50 : Matrix[i][j] = Matrix[j][i] = rand() % 50;
                        // случайное заполнение графа(взвешенного ориентированного)
                        // u - служит для способа добавления в граф значения, будет ли она обоюдной или нет
                    }
                }
            }
            printf("----------------- \n Матрица смежности: \n");
            toString(Matrix, n); // вывод получившейся матрицы
            printf("\n-----------------\n Ответ: \n");

            floydWarshall(Matrix, n); // вызов решения

            system("pause");
            break;
        case 2:

            system("cls");

            if (!file)  // проверка на существование файла
                printf("%Файл не был найден");
            else {
                for (int i = 0; i < n && !feof(file); i++) { // до n или пока не кончится файл
                    for (int j = 0; j < n && !feof(file); j++) {
                        int rd;
                        fscanf_s(file, "%d", &rd);
                        rd == -1 ? Matrix[i][j] = INF : Matrix[i][j] = rd;
                        printf("%d ", Matrix[i][j]);
                    }
                }
                putchar('\n');
            }

            printf("----------------- \n Матрица смежности: \n");
            toString(Matrix, n);
            printf("\n-----------------\n Ответ: \n");

            floydWarshall(Matrix, n);
            system("pause");
            break;
        case 3:
            exit(0);
            break;
        }

        for (int i = 0; i < n; i++) // очитска динамической памяти
            free(Matrix[i]);
        free(Matrix);

        system("cls");
    }
    fclose(file);
    return 0;
}