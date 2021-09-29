#include "helper_functions.h"

#include <libutils/rasserts.h>


cv::Mat makeAllBlackPixelsBlue(cv::Mat image) {
    // TODO реализуйте функцию которая каждый черный пиксель картинки сделает синим

    // ниже приведен пример как узнать цвет отдельного пикселя - состоящий из тройки чисел BGR (Blue Green Red)
    // чем больше значение одного из трех чисел - тем насыщеннее его оттенок
    // всего их диапазон значений - от 0 до 255 включительно
    // т.е. один байт, поэтому мы используем ниже тип unsigned char - целое однобайтовое неотрицательное число

    for(int j=0;j<101;j++){
        for(int i=0;i<101;i++){
            cv::Vec3b color = image.at<cv::Vec3b>(j, i); // взяли и узнали что за цвет в пикселе в 14-ом ряду (т.к. индексация с нуля) и 6-ой колонке
            unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
            unsigned char green = color[1];
            unsigned char red = color[2];
            if((blue==0)&&(green==0)&&(red==0)){
               red = 0;
               green = 0;
               blue =255;
            }
            image.at<cv::Vec3b>(j, i) = cv::Vec3b(blue, green, red);
        }
    }
    // как получить белый цвет? как получить черный цвет? как получить желтый цвет?
    // поэкспериментируйте! например можете всю картинку заполнить каким-то одним цветом

    // пример как заменить цвет по тем же координатам

    // запустите эту версию функции и посмотрите на получившуюся картинку - lesson03/resultsData/01_blue_unicorn.jpg
    // какой пиксель изменился? почему он не чисто красный?

    return image;
}

cv::Mat invertImageColors(cv::Mat image) {
    // TODO реализуйте функцию которая каждый цвет картинки инвертирует:
    for(int j=0;j<101;j++){
        for(int i=0;i<101;i++){
            cv::Vec3b color = image.at<cv::Vec3b>(j, i); // взяли и узнали что за цвет в пикселе в 14-ом ряду (т.к. индексация с нуля) и 6-ой колонке
            unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
            unsigned char green = color[1];
            unsigned char red = color[2];
                red = 255-red;
                green = 255-green;
                blue =255-blue;
            image.at<cv::Vec3b>(j, i) = cv::Vec3b(blue, green, red);
        }
    }
    // т.е. пусть ночь станет днем, а сумрак рассеется
    // иначе говоря замените каждое значение яркости x на (255-x) (т.к находится в диапазоне от 0 до 255)

    return image;
}

cv::Mat addBackgroundInsteadOfBlackPixels(cv::Mat object, cv::Mat background) {
    // TODO реализуйте функцию которая все черные пиксели картинки-объекта заменяет на пиксели с картинки-фона
    // т.е. что-то вроде накладного фона получится
    rassert(object.cols == background.cols, 341241251251351);
    rassert(object.rows == background.rows, 341241251251351);
    for(int j=0;j<101;j++){
        for(int i=0;i<101;i++){
            cv::Vec3b color = object.at<cv::Vec3b>(j, i); // взяли и узнали что за цвет в пикселе в 14-ом ряду (т.к. индексация с нуля) и 6-ой колонке
            unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
            unsigned char green = color[1];
            unsigned char red = color[2];
            cv::Vec3b color1 = background.at<cv::Vec3b>(j, i); // взяли и узнали что за цвет в пикселе в 14-ом ряду (т.к. индексация с нуля) и 6-ой колонке
            unsigned char blue1 = color1[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
            unsigned char green1 = color1[1];
            unsigned char red1 = color1[2];
            if((red==0)&&(green==0)&&(blue==0)){
                red = red1;
                green = green1;
                blue =blue1;
            }
            object.at<cv::Vec3b>(j, i) = cv::Vec3b(blue, green, red);
        }
    }
    // гарантируется что размеры картинок совпадают - проверьте это через rassert, вот например сверка ширины:


    return object;
}

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground) {
    // теперь вам гарантируется что largeBackground гораздо больше - добавьте проверок этого инварианта (rassert-ов)
    for(int j=0;j<101;j++){
        for(int i=0;i<101;i++){
            cv::Vec3b color = object.at<cv::Vec3b>(j, i); // взяли и узнали что за цвет в пикселе в 14-ом ряду (т.к. индексация с нуля) и 6-ой колонке
            unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
            unsigned char green = color[1];
            unsigned char red = color[2];
            cv::Vec3b color1 = largeBackground.at<cv::Vec3b>(j+245, i+586); // взяли и узнали что за цвет в пикселе в 14-ом ряду (т.к. индексация с нуля) и 6-ой колонке
            unsigned char blue1 = color1[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
            unsigned char green1 = color1[1];
            unsigned char red1 = color1[2];
            red1=red;
            blue1=blue;
            green1=green;
            largeBackground.at<cv::Vec3b>(j+245, i=586) = cv::Vec3b(blue1, green1, red1);
        }
    }
    // TODO реализуйте функцию так, чтобы нарисовался объект ровно по центру на данном фоне, при этом черные пиксели объекта не должны быть нарисованы

    return largeBackground;
}
