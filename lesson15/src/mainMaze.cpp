#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <libutils/rasserts.h>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/types.hpp>


struct Edge {
    int u, v; // номера вершин которые это ребро соединяет
    int w; // длина ребра (т.е. насколько длинный путь предстоит преодолеть переходя по этому ребру между вершинами)

    Edge(int u, int v, int w) : u(u), v(v), w(w)
    {}
};

// Эта биективная функция по координате пикселя (строчка и столбик) + размерам картинки = выдает номер вершины
int encodeVertex(int row, int column, int nrows, int ncolumns) {
    rassert(row < nrows, 348723894723980017);
    rassert(column < ncolumns, 347823974239870018);
    int vertexId = row * ncolumns + column;
    return vertexId;
}

// Эта биективная функция по номеру вершины говорит какой пиксель этой вершине соовтетствует (эта функция должна быть симметрична предыдущей!)
cv::Point2i decodeVertex(int vertexId, int nrows, int ncolumns) {

    // TODO: придумайте как найти номер строки и столбика пикселю по номеру вершины (просто поймите предыдущую функцию и эта функция не будет казаться сложной)
    int row = vertexId/ncolumns;
    int column = vertexId-row*ncolumns;

    // сверим что функция симметрично сработала:
    rassert(encodeVertex(row, column, nrows, ncolumns) == vertexId, 34782974923035);

    rassert(row < nrows, 34723894720027);
    rassert(column < ncolumns, 3824598237592030);
    return cv::Point2i(column, row);
}

void run(int mazeNumber) {
    cv::Mat maze = cv::imread("lesson15/data/mazesImages/maze" + std::to_string(mazeNumber) + ".png");
    rassert(!maze.empty(), 324783479230019);
    rassert(maze.type() == CV_8UC3, 3447928472389020);
    std::cout << "Maze resolution: " << maze.cols << "x" << maze.rows << std::endl;

    int nvertices = maze.cols*maze.rows; // TODO

    std::vector<std::vector<Edge>> edges_by_vertex(nvertices);
    for (int j = 0; j < maze.rows; ++j) {
        for (int i = 0; i < maze.cols; ++i) {
            cv::Vec3b color = maze.at<cv::Vec3b>(j, i);
            unsigned char blue = color[0];
            unsigned char green = color[1];
            unsigned char red = color[2];
            if((i>0)&&(j>0)&&(i<maze.cols-1)&&(j<maze.rows-1)){

                edges_by_vertex[j*maze.cols+i].push_back(Edge(j*maze.cols+i, j*maze.cols+i+1, abs(red+green+blue+1-maze.at<cv::Vec3b>(j, i+1)[0]-maze.at<cv::Vec3b>(j, i+1)[1]-maze.at<cv::Vec3b>(j, i+1)[2])));
                edges_by_vertex[j*maze.cols+i].push_back(Edge(j*maze.cols+i, j*maze.cols+i-1, abs(red+green+blue+1-maze.at<cv::Vec3b>(j, i-1)[0]-maze.at<cv::Vec3b>(j, i-1)[1]-maze.at<cv::Vec3b>(j, i-1)[2])));
                edges_by_vertex[j*maze.cols+i].push_back(Edge(j*maze.cols+i, j*maze.cols+i+maze.cols, abs(red+green+blue+1-maze.at<cv::Vec3b>(j+1, i)[0]-maze.at<cv::Vec3b>(j+1, i)[1]-maze.at<cv::Vec3b>(j+1, i)[2])));
                edges_by_vertex[j*maze.cols+i].push_back(Edge(j*maze.cols+i, j*maze.cols+i-maze.cols, abs(red+green+blue+1-maze.at<cv::Vec3b>(j-1, i)[0]-maze.at<cv::Vec3b>(j-1, i)[1]-maze.at<cv::Vec3b>(j-1, i)[2])));
            }
            // TODO добавьте соотвтетсвующие этому пикселю ребра
        }
    }

    int start, finish;
    if (mazeNumber >= 1 && mazeNumber <= 3) { // Первые три лабиринта очень похожи но кое чем отличаются...
        start = encodeVertex(300, 300, maze.rows, maze.cols);
        finish = encodeVertex(0, 305, maze.rows, maze.cols);
    } else if (mazeNumber == 4) {
        start = encodeVertex(154, 312, maze.rows, maze.cols);
        finish = encodeVertex(477, 312, maze.rows, maze.cols);
    } else if (mazeNumber == 5) { // Лабиринт в большом разрешении, добровольный (на случай если вы реализовали быструю Дейкстру с приоритетной очередью)
        start = encodeVertex(1200, 1200, maze.rows, maze.cols);
        finish = encodeVertex(1200, 1200, maze.rows, maze.cols);
    } else {
        rassert(false, 324289347238920081);
    }

    const int INF = std::numeric_limits<int>::max();

    cv::Mat window = maze.clone(); // на этой картинке будем визуализировать до куда сейчас дошла прокладка маршрута

    std::vector<int> distances(nvertices, INF);
    distances[start]=0;
    std::vector<int> q(nvertices, 0);
    std::vector<int> pred(nvertices, -1);
    while (q[finish]==0){
        int min_d=INF;
        int min_v;
        for(int i=0;i<nvertices;i++){
            if((distances[i]<min_d)&&(q[i]==0)){
                min_v=i;
                min_d=distances[i];
            }
        }
        for(int i=0;i<edges_by_vertex[min_v].size();i++){
            if(distances[edges_by_vertex[min_v][i].v]>min_d+edges_by_vertex[min_v][i].w){
                distances[edges_by_vertex[min_v][i].v]=min_d+edges_by_vertex[min_v][i].w;
                pred[edges_by_vertex[min_v][i].v]=min_v;
            }
        }
        q[min_v]=1;
    }

    int a=finish;
    while(a!=-1){
        cv::Point2i p = decodeVertex(a, maze.rows, maze.cols);
            window.at<cv::Vec3b>(p.y, p.x) = cv::Vec3b(0, 0, 255);
            cv::imshow("Maze", window);
            cv::waitKey(1);
        a=pred[a];
    }
    // TODO СКОПИРУЙТЕ СЮДА ДЕЙКСТРУ ИЗ ПРЕДЫДУЩЕГО ИСХОДНИКА

    // TODO в момент когда вершина становится обработанной - красьте ее на картинке window в зеленый цвет и показывайте картинку:
    //    cv::Point2i p = decodeVertex(the_chosen_one, maze.rows, maze.cols);
    //    window.at<cv::Vec3b>(p.y, p.x) = cv::Vec3b(0, 255, 0);
    //    cv::imshow("Maze", window);
    //    cv::waitKey(1);
    // TODO это может тормозить, в таком случае показывайте window только после обработки каждой сотой вершины

    // TODO обозначьте найденный маршрут красными пикселями

    // TODO сохраните картинку window на диск

    std::cout << "Finished!" << std::endl;

    // Показываем результат пока пользователь не насладиться до конца и не нажмет Escape
    while (cv::waitKey(10) != 27) {
        cv::imshow("Maze", window);
    }
}

int main() {
    try {
        int mazeNumber = 1;
        run(mazeNumber);

        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}
