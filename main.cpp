/*
 * OpenFit.cpp
 *
 * ! Archive Description.
*
* 
  \Felipe Tabata Fukushima.
  \author Rodrigo Jost.   
* \since 04/03/2022
* \version 1.0
*/

#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/calib3d.hpp>
#include "math.h"
#include "iostream"
#include "stdio.h"

using namespace cv;
using namespace std;

//Carega o video principal
VideoCapture capt("..\\path\\video.mp4");
//vetor de armazenamento dos pontos clicados
vector<Point2f> marcadoresImagem;
//vetor armazenamento dos angulos
vector<vector<double>> angulosMinMax = {{360, -360},{360, -360},{360, -360},{360, -360}};
Mat img;

//funcao callback do maouse
void defPontosMouse(int event, int x, int y, int flags, void* param){
    if (event == EVENT_LBUTTONUP)
        {
        // verifica se est� no momento de calibra��o ou de convers�o
        if (marcadoresImagem.size() < 6)
        {
            // operador >> entrega o pr�ximo frame
            capt >> img;
            imshow( "Gravacao Bike Fit", img);

                    // entra com pontos da imagem
                    marcadoresImagem.push_back(Point2f(x,y));
                    cout << "Ponto " << marcadoresImagem.size() << " informado." << endl;
                    // se j� tiver 6 pontos inicia tracking o tracking
                        if (marcadoresImagem.size() == 6)
                        {
                        cout << "Todos os Pontos Informados, rastreando marcadores..." << endl;

                        int qtdFrames = capt.get(CAP_PROP_FRAME_COUNT);
                        //namedWindow("Imagem Normal", WINDOW_NORMAL);
                        //namedWindow("Image P&B", WINDOW_NORMAL);
                        //namedWindow( "Connected Components", WINDOW_NORMAL);

                        char tecla = ' ';
                        int contaFrames = 0;
                        Mat img, imgPeB;
                        int threshval = 50;
                        while ((contaFrames < qtdFrames) && (tecla != 27))
                        {
                                // operador >> entrega o pr�ximo frame
                                capt >> img;
                                //imshow( "Imagem Normal", img);
                                cvtColor(img, imgPeB,COLOR_BGR2GRAY);
                                //imshow( "Imagem Normal", img);
                                Mat bw(img.size(), CV_8UC1);
                                threshold(imgPeB, bw, threshval, 200, THRESH_BINARY_INV);
                                //imshow( "Image P&B", bw);

                                Mat labelImage(img.size(), CV_32S);
                                Mat stats, centroids;

                                int nLabels = connectedComponentsWithStats(bw, labelImage, stats, centroids, 8);

                                /*
                                std::vector<Vec3b> colors(nLabels);
                                colors[0] = Vec3b(0, 0, 0);//background
                                for(int label = 1; label < nLabels; ++label){
                                    colors[label] = Vec3b( (rand()&255), (rand()&255), (rand()&255) );
                                }
                                Mat dst(img.size(), CV_8UC3);
                                for(int r = 0; r < dst.rows; ++r){
                                    for(int c = 0; c < dst.cols; ++c){
                                        int label = labelImage.at<int>(r, c);
                                        //Vec3b &pixel = dst.at<Vec3b>(r, c);
                                        //pixel = colors[label];
                                        dst.at<Vec3b>(r, c) = colors[label];
                                     }
                                }*/

                                //vetor pra coletar �ndices dos marcadores
                                vector<int> labelsMarcadores = {0,0,0,0,0,0};

                                //laco para encontrar os componentes conectados mais proximos dos clicks
                                for(int i = 0; i<centroids.rows; i++){

                                    double posColunaCandidato = centroids.at<double>(Point(0, i));
                                    double posLinhaCandidato = centroids.at<double>(Point(1, i));

                                    for(int j=0; j<6; j++){

                                        double posColunaClick = marcadoresImagem.at(j).x;
                                        double posLinhaClick = marcadoresImagem.at(j).y;

                                        double posColunaAtual = centroids.at<double>(Point(0, labelsMarcadores[j]));
                                        double posLinhaAtual = centroids.at<double>(Point(1, labelsMarcadores[j]));

                                        int dstEuclidianaCandidato = round(sqrt(pow(abs(posColunaClick - posColunaCandidato), 2)+pow(abs(posLinhaClick - posLinhaCandidato), 2)));
                                        int dstEuclidianaAtual = round(sqrt(pow(abs(posColunaClick - posColunaAtual), 2)+pow(abs(posLinhaClick - posLinhaAtual), 2)));

                                        if (dstEuclidianaCandidato < dstEuclidianaAtual){
                                            labelsMarcadores[j] = i;
                                        }
                                    }
                                }

                                //destacando e desenhando elementos
                                for(int i=0; i<centroids.rows; i++)
                                {
                                  int x = round(centroids.at<double>(Point(0, i))); // label: i
                                  int y = round(centroids.at<double>(Point(1, i)));
                                  //dimensoes dos componentes conectados, eqv diametro
                                  int w = stats.at<int>(Point(2, i));
                                  int h = stats.at<int>(Point(3, i));

                                  for(int j = 0; j <6; j++){
                                     if (i == labelsMarcadores[j]){
                                        int raio = round(((h+w)/4));
                                        circle(img,Point(x, y), raio, Scalar(0,0,255), FILLED, LINE_8);
                                     }
                                     int posColunaMarcadorA = round(centroids.at<double>(Point(0, labelsMarcadores[j])));
                                     int posLinhaMarcadorA = round(centroids.at<double>(Point(1, labelsMarcadores[j])));
                                    if(j < 5){
                                        int posColunaMarcadorB = round(centroids.at<double>(Point(0, labelsMarcadores[j+1])));
                                        int posLinhaMarcadorB = round(centroids.at<double>(Point(1, labelsMarcadores[j+1])));
                                        //line(dst, Point(posColunaMarcadorA,posLinhaMarcadorA), Point(posColunaMarcadorB,posLinhaMarcadorB), Scalar(255, 0, 0), 2), LINE_4;
                                        line(img, Point(posColunaMarcadorA,posLinhaMarcadorA), Point(posColunaMarcadorB,posLinhaMarcadorB), Scalar(255, 0, 0), 2), LINE_4;
                                      }
                                    //atualizando coordenadas dos marcadores para buscar no pr�ximo frame
                                    marcadoresImagem.at(j) = Point(posColunaMarcadorA,posLinhaMarcadorA);

                                  }
                                }
                                //calculando angulos
                                for (int m = 1; m < 5; m++){
                                    double point1X = marcadoresImagem.at(m-1).x;
                                    double point1Y = marcadoresImagem.at(m-1).y;
                                    double point2X = marcadoresImagem.at(m+1).x;
                                    double point2Y = marcadoresImagem.at(m+1).y;
                                    double fixedX = marcadoresImagem.at(m).x;
                                    double fixedY = marcadoresImagem.at(m).y;
                                    double anguloDeg;
                                    double angle1 = atan2(point1Y - fixedY, point1X - fixedX);
                                    double angle2 = atan2(point2Y - fixedY, point2X - fixedX);

                                    if ( m == 1 || m == 3){
                                        anguloDeg = 360 - abs(((angle1 - angle2) * 180.0) / (atan(1)*4));
                                    }
                                    else{
                                        anguloDeg = abs(((angle1 - angle2) * 180.0) / (atan(1)*4));
                                    }
                                    if (angulosMinMax.at(m-1).at(0) > anguloDeg) {
                                        angulosMinMax.at(m-1).at(0) = anguloDeg;
                                    }

                                    if (angulosMinMax.at(m-1).at(1) < anguloDeg) {
                                        angulosMinMax.at(m-1).at(1) = anguloDeg;
                                    }
                                }
                                tecla = waitKey(1);
                                //exibe resultados finais
                                if ((contaFrames + 1 == qtdFrames) || tecla == 'r'){
                                    for (int m = 0; m < 4; m++)
                                    {
                                        if (m == 0){
                                            cout << "--------------------------------------------" << endl;
                                            cout << "----- Angulos Internos -----" << endl;
                                            cout << "Indice 1: cotovelo --- Indice 2: ombro" << endl;
                                            cout << "Indice 3: quadril  --- Indice 4: joelho" << endl;
                                        }
                                    cout << "--------------------------------------------" << endl;
                                    cout << "Indice " << m + 1  << endl;
                                    cout << "-> Angulo Minimo: "<< angulosMinMax.at(m).at(0) << endl;
                                    cout << "-> Angulo Maximo: "<< angulosMinMax.at(m).at(1) << endl;
                                    }
                                }
                                //imshow( "Connected Components", dst );
                                imshow( "Gravacao Bike Fit", img);

                                contaFrames++;
                        }
                }
        }
    }
}

void trackingMarcadores (){

    //define janela principal de visualizacao
    namedWindow("Gravacao Bike Fit", WINDOW_NORMAL);
    //set callback mouse
    setMouseCallback("Gravacao Bike Fit", defPontosMouse);
    cout << "Clique sobre os 6 marcadores na seguinte ordem:" << endl;
    cout << "Mao, cotovelo, ombro, quadril, joelho e tornozelo" << endl;

    // operador >> entrega o primeiro frame
    capt >> img;
    char tecla = ' ';
    while (tecla != 27)
        {
        imshow( "Gravacao Bike Fit", img);
        tecla = waitKey();
        }
    // liberacapturador
    capt.release();
    // destroi todas as janelas abertas
    destroyAllWindows();
}

int main(int argc, char *argv[])
{
    trackingMarcadores();
    return 0;
}
