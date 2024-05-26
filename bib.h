/// \brief Biblioteca de funções de processamento de imagens
/// \author Pedro Silva
/// \date 2023
/// \bug
/// \warning
/// \todo

#include "vc.h"
#include <stdio.h>

//• Construa uma função que calcule o negativo de uma imagem Gray.
int vc_gray_negative(IVC *srcdst);

//• Construa uma função que calcule o negativo de uma imagem RGB.
int vc_rgb_negative(IVC *srcdst);

// Converter RGB para HSV
int vc_rgb_to_hsv(IVC *srcdst);
int vc_rgb_to_hsv2(IVC *srcdst);

int vc_rgb_to_gray(IVC *src, IVC *dst);

int vc_hsv_segmentation(IVC *src, int hmin, int hmax, int smin,
                        int smax, int vmin, int vmax);
int subtrair(IVC *imagem1, IVC *imagem2, IVC *destino);
int vc_scale_gray_to_rgb(IVC* src, IVC* dst);

int vc_gray_to_binary(IVC *srcdst, IVC *dst, int threshold);

int vc_gray_to_binary_global_mean (IVC *srcdst, IVC *dst);

int vc_gray_to_binary_midpoint(IVC *srcdst, IVC *dst, int vizinhos );

int vc_gray_to_binary_Bersen(IVC *src, IVC *dst, int kernel, int c);

int vc_gray_to_binary_Niblack(IVC *src, IVC *dst, int kernel, float k);

int vc_binary_erode(IVC *src, IVC *dst, int kernel);

int vc_binary_dilate(IVC *src, IVC *dst, int kernel);

int vc_binary_open(IVC *src, IVC *dst, int kernelE, int kernelD);

int vc_binary_close(IVC *src, IVC *dst, int kernelE, int kernelD);

int vc_gray_erode(IVC *src, IVC *dst, int kernel);

int vc_gray_dilate(IVC *src, IVC *dst, int kernel);

int vc_binary_sub(IVC *imagem1, IVC *imagem2, IVC *destino);

#pragma region  labelling
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                   ESTRUTURA DE UM BLOB (OBJECTO)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef struct {
	int x, y, width, height;	// Caixa Delimitadora (Bounding Box)
	int area;					// �rea
	int xc, yc;					// Centro-de-massa
	int perimeter;				// Per�metro
	int label;					// Etiqueta
    int valor;
} OVC;


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                    PROT�TIPOS DE FUN��ES
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

OVC* vc_binary_blob_labelling(IVC *src, IVC *dst, int *nlabels);
int vc_binary_blob_info(IVC *src, OVC *blobs, int nblobs);


// feita por mim

int vc_cores_labels(IVC *src, IVC *dst, int nblobs, OVC *blobs);
int vc_bin_labels(IVC *src, IVC *dst, int nblobs, OVC *blobs, int detalhes);

#pragma endregion

int vc_3chanels_to_1 (IVC *src, IVC *dst);

int vc_draw_bounding_box(IVC *src, OVC *blobs, int nblobs, int margemX, int margemY, int frame, int **listaBlobs, IVC *teste, int *contador);
int vc_draw_bounding_box_RGB(IVC *src, OVC *blobs, int nblobs, int margemX, int margemY, int red, int green, int blue);
int vc_draw_center_of_mass(IVC *src, OVC *blobs, int nblobs, int tamanho_alvo, int cor);
int vc_draw_center_of_mass_RGB(IVC *src, OVC *blobs, int nblobs, int tamanho_alvo, int cor, int red, int green, int blue);


int contagemPixeisSup1Inf255(IVC *src, int min, int max);// mhe


int vc_gray_histogram_show(IVC *src, IVC *dst);
int vc_gray_histogram_equalization(IVC *src, IVC *dst);
int vc_hsv_to_rgb(IVC *srcdst);


#pragma region  contornos

int vc_gray_edge_prewitt(IVC *src, IVC *dst, float th);

#pragma endregion


int vc_hsv_segmentationmetade(IVC *src, int hmin, int hmax, int smin,
                              int smax, int vmin, int vmax);
int vc_binary_erodeCentro(IVC *src, IVC *dst, int kernel);
int vc_binary_dilateCentro(IVC *src, IVC *dst, int kernel);

OVC *vc_binary_blob_labellingTeste(IVC *src, IVC *dst, int *nlabels);
int vc_bin_labelsTeste(IVC *src, IVC *dst, int nblobs, OVC *blobs, int detalhes);
int vc_binary_blob_infoTeste(IVC *src, OVC *blobs, int nblobs);

int vc_draw_bounding_boxTeste(IVC *src, OVC *blobs, int nblobs, int margemX, int margemY);


void ValoresRgb_to_hsv(unsigned char red, unsigned char green, unsigned char blue, float *h, float *s, float *v);

