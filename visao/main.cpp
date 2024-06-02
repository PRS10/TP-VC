#include <iostream>
#include <string>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <set> // teste para contar

extern "C" {
#include "Bib.h"
}

void vc_timer(void) {
    static bool running = false;
    static std::chrono::steady_clock::time_point previousTime = std::chrono::steady_clock::now();

    if (!running) {
        running = true;
    } else {
        std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
        std::chrono::steady_clock::duration elapsedTime = currentTime - previousTime;

        // Tempo em segundos.
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(elapsedTime);
        double nseconds = time_span.count();

        std::cout << "Tempo decorrido: " << nseconds << " segundos" << std::endl;
        std::cout << "Pressione qualquer tecla para continuar...\n";
        std::cin.get();
    }
}

int main(void) {
//    int *contador = (int *)malloc(sizeof(int));
//    *contador = 0;
    int contador = 0;
    int contaAnalise = 0;
//    if(contador == NULL){
//        fprintf(stderr, "Erro ao alocar contador em memória\n");
//    }
    
    // Vídeo
    char videofile[65] = "/Users/pedroricardosilva/Desktop/Visao/visao/video_resistors.mp4";
    cv::VideoCapture capture;
    struct {
        int width, height;
        int ntotalframes;
        int fps;
        int nframe;
    } video;
    // Outros
    std::string str;
    int key = 0;

    // Leitura de vídeo de um ficheiro
    capture.open(videofile);

    // Verifica se foi possível abrir o ficheiro de vídeo
    if (!capture.isOpened()) {
        std::cerr << "Erro ao abrir o ficheiro de vídeo!\n";
        return 1;
    }

    // Número total de frames no vídeo
    video.ntotalframes = (int)capture.get(cv::CAP_PROP_FRAME_COUNT);
    // Frame rate do vídeo
    video.fps = (int)capture.get(cv::CAP_PROP_FPS);
    // Resolução do vídeo
    video.width = (int)capture.get(cv::CAP_PROP_FRAME_WIDTH);
    video.height = (int)capture.get(cv::CAP_PROP_FRAME_HEIGHT);

    // Cria uma janela para exibir o vídeo
    cv::namedWindow("VC - VIDEO", cv::WINDOW_AUTOSIZE);

    // Inicia o timer
    vc_timer();
    std::string outputVideoFile = "/Users/pedroricardosilva/Desktop/Visao/visao/output_video.avi";
    cv::VideoWriter outputVideo;
    outputVideo.open(outputVideoFile, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), video.fps, cv::Size(video.width, video.height));

    cv::Mat frame;
    while (key != 'q') {
        // Leitura de uma frame do vídeo
        capture.read(frame);

        // Verifica se conseguiu ler a frame
        if (frame.empty()) break;

        // Número da frame a processar
        video.nframe = (int)capture.get(cv::CAP_PROP_POS_FRAMES);

        // Cria uma nova imagem IVC
        IVC *image = vc_image_new(video.width, video.height, 3, 255);
        IVC *image2 = vc_image_new(video.width, video.height, 3, 255);
        IVC *image3 = vc_image_new(video.width, video.height, 3, 255);
        IVC *imageLab = vc_image_new(video.width, video.height, 1, 255);
        IVC *imageLab2 = vc_image_new(video.width, video.height, 1, 255);
        

        // Copia dados de imagem da estrutura cv::Mat para uma estrutura IVC
        memcpy(image->data, frame.data, video.width * video.height * 3);
        memcpy(image2->data, frame.data, video.width * video.height * 3);

        vc_rgb_to_hsv2(image3);
        vc_rgb_to_hsv2(image);
        vc_hsv_segmentation(image, 30, 250, 40, 100, 32, 100);//geral

        
        vc_3chanels_to_1(image, imageLab2);

        // Converte IVC para cv::Mat
        cv::Mat matImageLab2(video.height, video.width, CV_8UC1, imageLab2->data);
        cv::Mat matDilated;

        // Cria um elemento estruturante (kernel)
        int kernelSize = 41;
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));

        // Aplica a função cv::dilate
        cv::dilate(matImageLab2, matDilated, kernel);

        // Converte de volta para IVC
        memcpy(imageLab->data, matDilated.data, video.width * video.height);

        

        // Processo de detecção de blobs
        int nblobs;
        OVC *blobs = vc_binary_blob_labelling(imageLab, imageLab2, &nblobs);

        // Extração de informações dos blobs
        vc_binary_blob_infoTeste(imageLab2, blobs, nblobs);

        int listaBlobs[nblobs];
       
        // Desenho das bounding boxes
        vc_draw_bounding_box(image2, blobs, nblobs, -20, -20, video.nframe, listaBlobs, image3, &contador, &contaAnalise);
        
              


        // Copia dados de imagem da estrutura IVC para uma estrutura cv::Mat
        memcpy(frame.data, image2->data, video.width * video.height * 3);

        // Liberta a memória da imagem IVC que havia sido criada
        vc_image_free(image);
        
        
        
        
        for(int i = 0; i < nblobs; i++){
            if(video.nframe > 716) break;
            if(blobs[i].width > 150 &&
               blobs[i].area > 11000 && blobs[i].area < 21000 &&
               blobs[i].height > 80 && blobs[i].height <= 115)
            {
                
                
                // Define as informações do blob
//                std::string widthStr = "Largura: " + std::to_string(blobs[i].width) + " px";
//                std::string heightStr = "Altura: " + std::to_string(blobs[i].height) + " px";
//                std::string areaStr = "Area: " + std::to_string(blobs[i].area) + " px";
//                std::string perimeterStr = "Perimetro: " + std::to_string(blobs[i].perimeter) + " px";

                int n = blobs[i].terceiro;
                
                std::string terceiro(n, '0');// converter para 'zeros' o mesmo numero de vezes que o terceiro tem.

                std::string x = "Valor: " + std::to_string(blobs[i].primeiro - 1) + std::to_string(blobs[i].segundo - 1) + terceiro + " ohms"; 

                
                // Define a posição base para o texto (acima da bounding box do blob)
                int baseY = blobs[i].y - 100; // Ajuste para ter espaço para várias linhas
                
                // Insere o texto na imagem, linha por linha
//                cv::putText(frame, widthStr, cv::Point(blobs[i].x, baseY), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 1);
//                cv::putText(frame, heightStr, cv::Point(blobs[i].x, baseY + 25), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 1);
//                cv::putText(frame, areaStr, cv::Point(blobs[i].x, baseY + 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 1);
//                cv::putText(frame, perimeterStr, cv::Point(blobs[i].x, baseY + 75), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 1);
                cv::putText(frame, x, cv::Point(blobs[i].x, baseY + 220), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 1);

            }
        }
        
//        int conta = *contador;

        
        // Exemplo de inserção texto na frame
//        str = std::string("RESOLUCAO: ").append(std::to_string(video.width)).append("x").append(std::to_string(video.height));
//        cv::putText(frame, str, cv::Point(20, 25), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
//        cv::putText(frame, str, cv::Point(20, 25), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 0, 0), 1);
//        str = std::string("TOTAL DE FRAMES: ").append(std::to_string(video.ntotalframes));
//        cv::putText(frame, str, cv::Point(20, 50), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
//        cv::putText(frame, str, cv::Point(20, 50), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);
        str = std::string("FRAME RATE: ").append(std::to_string(video.fps));
        cv::putText(frame, str, cv::Point(20, 75), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
        cv::putText(frame, str, cv::Point(20, 75), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);
        str = std::string("N. DA FRAME: ").append(std::to_string(video.nframe));
        cv::putText(frame, str, cv::Point(20, 100), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
        cv::putText(frame, str, cv::Point(20, 100), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);
        
        str = std::string("Contador resistencias: ").append(std::to_string(contador));
        cv::putText(frame, str, cv::Point(20, 125), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
        cv::putText(frame, str, cv::Point(20, 120), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);
        
        
        

        // Salva a imagem com a resolução sobreposta se a condição for atendida
            std::string filename = "/Users/pedroricardosilva/Desktop/Visao/visao/Prints/frame" + std::to_string(video.nframe) + ".jpg";
            imwrite(filename, frame);
        
        
        
        // Exibe a frame
        cv::imshow("VC - VIDEO", frame);
        outputVideo.write(frame);

        // Sai da aplicação, se o utilizador premir a tecla 'q'
        key = cv::waitKey(1);
    }

    // Para o timer e exibe o tempo decorrido
    vc_timer();

    // Fecha a janela
    cv::destroyWindow("VC - VIDEO");

    // Fecha o ficheiro de vídeo
    capture.release();

    return 0;
}


// Funçao para segmentar e identificar os blobs

