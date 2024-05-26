/// \brief Biblioteca de funções de processamento de imagens
/// \author Pedro Silva
/// \date 2023
/// \bug
/// \warning
/// \todo

#include "bib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


// Para as novas funções de etiquetagem
#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

// exercicio de espaços de cor

// • Construa uma função que calcule o negativo de uma imagem Gray.
//  Slide VC-4 (Aula 4 dia 04/03/2024)
//  Apenas para imagens em tons de cinza, com um canal
int vc_gray_negative(IVC *srcdst)
{

    if (srcdst->width <= 0 || srcdst->height <= 0)
    {
        printf("(vc_gray_negative) Imagem tem de ter dimensões positivas\n");
        return 0;
    }
    if (srcdst->channels != 1)
    {
        printf("(vc_gray_negative) Imagem tem de ter 1 canal\n");
        return 0;
    }

    for (int y = 0; y < srcdst->height; y++)
    {
        for (int x = 0; x < srcdst->width; x++)
        {
            int pos = y * srcdst->bytesperline + x * srcdst->channels;
            srcdst->data[pos] = 255 - srcdst->data[pos];
        }
    }

    return 1;
}

// • Construa uma função que calcule o negativo de uma imagem RGB.
int vc_rgb_negative(IVC *srcdst)
{

    if (srcdst->width <= 0 || srcdst->height <= 0)
    {
        printf("(vc_rgb_negative) Imagem tem de ter dimensões positivas\n");
        return 0;
    }
    if (srcdst->channels != 3)
    {
        printf("(vc_rgb_negative) Imagem tem de ter 3 canais\n");
        return 0;
    }

    for (int y = 0; y < srcdst->height; y++)
    {
        for (int x = 0; x < srcdst->width; x++)
        {
            int pos = y * srcdst->bytesperline + x * srcdst->channels;
            srcdst->data[pos] = 255 - srcdst->data[pos];
            srcdst->data[pos + 1] = 255 - srcdst->data[pos + 1];
            srcdst->data[pos + 2] = 255 - srcdst->data[pos + 2];
        }
    }
    return 1;
}

/// @brief Apenas para auxilio de calculo
/// @param a
/// @param b
/// @return
float fmaxf(float a, float b)
{
    // verifica qual é o maior valor
    if (a > b)
    {
        return a;
    }
    return b;
}

/// @brief Apenas para auxilio de calculo
/// @param a
/// @param b
/// @return
float fminf(float a, float b)
{
    // verifica qual é o menor valor
    if (a < b)
    {
        return a;
    }
    return b;
}

// Função que converta uma imagem no espaço RGB para uma imagem no espaço HSV
int vc_rgb_to_hsv(IVC *srcdst)
{
    unsigned char *data = (unsigned char *)srcdst->data;
    int width = srcdst->width;
    int height = srcdst->height;
    int bytesperline = srcdst->width * srcdst->channels;
    int channels = srcdst->channels;
    float r, g, b, hue, saturation, value;
    float rgb_max, rgb_min;
    int i, size;

    // Verificação de erros
    if ((srcdst->width) <= 0 || (srcdst->height <= 0) || (srcdst->data == NULL))
    {
        printf("(vc_rgb_to_hsv) Tamanhos inválidos\n");
        return 0;
    }

    if (srcdst->channels != 3)
    {
        printf("(vc_rgb_to_hsv) Imagem tem de ter 3 canais\n");
        return 0;
    }

    size = width * height * channels;

    for (i = 0; i < size; i = i + channels)
    {
        // Mudar esta funcão. O trabalho está em BGR e não em RGB
        // r = (float)data[i] / 255.0f;
        // g = (float)data[i + 1] / 255.0f;
        // b = (float)data[i + 2] / 255.0f;

        r = (float)data[i + 2] / 255.0f;
        g = (float)data[i + 1] / 255.0f;
        b = (float)data[i] / 255.0f;

        rgb_max = fmaxf(fmaxf(r, g), b);
        rgb_min = fminf(fminf(r, g), b);

        // Valor (Value)
        value = rgb_max;

        // Saturação (Saturation)
        if (rgb_max > 0)
            saturation = (rgb_max - rgb_min) / rgb_max;
        else
            saturation = 0;

        // Hue
        if (saturation == 0)
            hue = 0;
        else
        {
            float delta = rgb_max - rgb_min;
            if (r == rgb_max)
                hue = (g - b) / delta;
            else if (g == rgb_max)
                hue = 2 + (b - r) / delta;
            else
                hue = 4 + (r - g) / delta;

            hue *= 60;
            if (hue < 0)
                hue += 360;
        }

        // Atribui os valores HSV de volta à imagem
        data[i] = (unsigned char)(hue / 2);              // Componente H
        data[i + 1] = (unsigned char)(saturation * 255); // Componente S
        data[i + 2] = (unsigned char)(value * 255);      // Componente V
    }

    return 1;
}

int vc_rgb_to_hsv2(IVC *srcdst)
{
    unsigned char *data = (unsigned char *)srcdst->data;
    int width = srcdst->width;
    int height = srcdst->height;
    int bytesperline = srcdst->bytesperline;
    int channels = srcdst->channels;
    float r, g, b, hue, saturation, value;
    float rgb_max, rgb_min;
    int i, size;

    // Verificação de erros
    if ((width <= 0) || (height <= 0) || (data == NULL))
        return 0;
    if (channels != 3)
        return 0;

    size = width * height * channels;

    for (i = 0; i < size; i = i + channels)
    {
        r = (float)data[i];
        g = (float)data[i + 1];
        b = (float)data[i + 2];

        // Calcula valores máximo e mínimo dos canais de cor R, G e B
        rgb_max = (r > g ? (r > b ? r : b) : (g > b ? g : b));
        rgb_min = (r < g ? (r < b ? r : b) : (g < b ? g : b));

        // Value toma valores entre [0,255]
        value = rgb_max;
        if (value == 0.0f)
        {
            hue = 0.0f;
            saturation = 0.0f;
        }
        else
        {
            // Saturation toma valores entre [0,255]
            saturation = ((rgb_max - rgb_min) / rgb_max) * 255.0f;

            if (saturation == 0.0f)
            {
                hue = 0.0f;
            }
            else
            {
                // Hue toma valores entre [0,360]
                if ((rgb_max == r) && (g >= b))
                {
                    hue = 60.0f * (g - b) / (rgb_max - rgb_min);
                }
                else if ((rgb_max == r) && (b > g))
                {
                    hue = 360.0f + 60.0f * (g - b) / (rgb_max - rgb_min);
                }
                else if (rgb_max == g)
                {
                    hue = 120.0f + 60.0f * (b - r) / (rgb_max - rgb_min);
                }
                else
                {
                    hue = 240.0f + 60.0f * (r - g) / (rgb_max - rgb_min);
                }
            }
        }

        // Atribui valores entre [0,255]
        data[i] = (unsigned char)(hue / 360.0f * 255.0f);
        data[i + 1] = (unsigned char)(saturation);
        data[i + 2] = (unsigned char)(value);
    }

    return 1;
}

/// @brief Converte uma imagem RGB para cinzento
/// @param src
/// @param dst
/// @return
int vc_rgb_to_gray(IVC *src, IVC *dst)
{
    unsigned char *data = (unsigned char *)src->data;
    unsigned char *data_dst = (unsigned char *)dst->data;
    int bytesperline_src = src->width * src->channels;
    int bytesperline_dst = dst->width * dst->channels;
    int channels_dst = dst->channels;
    int channels_src = src->channels;
    int width = src->width;
    int height = src->height;
    long int pos_src, pos_dst;
    float r, g, b;

    if ((src->width) <= 0 || (src->height <= 0) || (src->data == NULL))
    {
        printf("(vc_rgb_to_gray) Tamanhos inválidos\n");
        return 0;
    }

    if (dst->channels > 1)
    {
        printf("Imagem de destino tem de ter apenas um canal\n");
        return 0;
    }
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            pos_src = y * bytesperline_src + x * channels_src;
            pos_dst = y * bytesperline_dst + x * channels_dst;

            r = (float)data[pos_src];
            g = (float)data[pos_src + 1];
            b = (float)data[pos_src + 2];

            data_dst[pos_dst] = (unsigned char)((r * 0.299) + (g * 0.587) + (b * 0.114));
        }
    }

    return 1;
}

/// @brief Faz a seguementação de uma imagem no espaço HSV
/// Atenção que a imagem tem de estar no espaço HSV!
/// A nova imagem sai com os pixeis a branco ou preto, mas isso nao altera os canais da imagem original
/// Para alterar também os canais podemos utilizar a função vc_scale_gray_to_rgb, ou vc_3chanels_to_1
/// @param src
/// @param hmin
/// @param hmax
/// @param smin
/// @param smax
/// @param vmin
/// @param vmax
/// @return
int vc_hsv_segmentation(IVC *src, int hmin, int hmax, int smin,
                        int smax, int vmin, int vmax)
{

    unsigned char *data = (unsigned char *)src->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;
    float h, s, v;
    int i, size;

    if ((src->width) <= 0 || (src->height <= 0) || (src->data == NULL))
        return 0;
    if (src->channels != 3)
        return 0;

    size = width * height * channels;

    for (i = 0; i < size; i = i + channels)
    {

        h = (float)data[i] * 360.0f / 255.0f;
        s = (float)data[i + 1] * 100.0f / 255.0f;
        v = (float)data[i + 2] * 100.0f / 255.0f;

        if (h >= hmin && h <= hmax && s >= smin && s <= smax && v >= vmin && v <= vmax)
        {
            data[i] = 255;
            data[i + 1] = 255;
            data[i + 2] = 255;
        }
        else
        {
            data[i] = 0;
            data[i + 1] = 0;
            data[i + 2] = 0;
        }
    }
    return 1;
}

int vc_hsv_segmentationmetade(IVC *src, int hmin, int hmax, int smin, int smax, int vmin, int vmax)
{
    unsigned char *data = (unsigned char *)src->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;
    float h, s, v;
    int x, y;

    if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
        return 0;
    if (src->channels != 3)
        return 0;

    // Calcular os limites para os 40% centrais da imagem
    int x_start = width * 0.3; // 30% da largura da imagem
    int x_end = width * 0.7;   // 70% da largura da imagem

    // Loop para percorrer apenas a faixa central de 40% da imagem
    for (y = 0; y < height; y++)
    {
        for (x = x_start; x < x_end; x++)
        {
            int i = y * bytesperline + x * channels;

            h = (float)data[i] * 360.0f / 255.0f;
            s = (float)data[i + 1] * 100.0f / 255.0f;
            v = (float)data[i + 2] * 100.0f / 255.0f;

            if (h >= hmin && h <= hmax && s >= smin && s <= smax && v >= vmin && v <= vmax)
            {
                data[i] = 255;
                data[i + 1] = 255;
                data[i + 2] = 255;
            }
            else
            {
                data[i] = 0;
                data[i + 1] = 0;
                data[i + 2] = 0;
            }
        }
    }

    return 1;
}


/// @brief Subtrai duas imagens binarias
/// @param imagem1 
/// @param imagem2 
/// @param destino 
/// @return 
int subtrair(IVC *imagem1, IVC *imagem2, IVC *destino){
    unsigned char *data1 = (unsigned char *)imagem1->data;
    unsigned char *data2 = (unsigned char *)imagem2->data;
    unsigned char *data_destino = (unsigned char *)destino->data;
    int width = imagem1->width;
    int height = imagem1->height;
    int bytesperline = imagem1->width * imagem1->channels;
    int channels = imagem1->channels;
    int i, size;

    if ((imagem1->width) <= 0 || (imagem1->height <= 0) || (imagem1->data == NULL)) return 0;
    if ((imagem2->width) <= 0 || (imagem2->height <= 0) || (imagem2->data == NULL)) return 0;
    if ((destino->width) <= 0 || (destino->height <= 0) || (destino->data == NULL)) return 0;
    if (imagem1->channels != 1 || imagem2->channels != 1 || destino->channels != 1) return 0;

    size = width * height * channels;

    for(i = 0; i < size; i = i + channels){
        if(data1[i] > 0 && data2[i] == 255){
            data_destino[i] = data1[i];
        }else{
            data_destino[i] = 0;
        }
    }
    return 1;
}

// TESTE 01/05/2024 para testar os blobs
/// @brief Função que conta o número de pixeis que são passados por parametros
int contagemPixeisSup1Inf255(IVC *src, int min, int max)
{
    unsigned char *data = (unsigned char *)src->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;
    int i, y, pos, size;
    int contagem = 0;
    if ((src->width) <= 0 || (src->height <= 0) || (src->data == NULL))
        return 0;

    for (i = 0; i < width; i++)
    {
        for (y = 0; y < height; y++)
        {
            pos = y * bytesperline + i * channels;
            if (data[pos] > min && data[pos] < max)
            {
                contagem++;
            }
        }
    }
    return contagem;
}

#pragma region Conversão de RGB para Gray (vc_scale_gray_to_rgb)
/// @brief Função de auxilio para a função vc_scale_gray_to_rgb
/// @param data
/// @param i
/// @return
int CheckRed(unsigned char *data, int i)
{
    int intensity = data[i];

    if (intensity < 128)
    {
        return 0;
    }
    else if (intensity < 192)
    {
        return 4 * (intensity - 128);
    }
    else
    {
        return 255;
    }
}
/// @brief Função de auxilio para a função vc_scale_gray_to_rgb
/// @param data
/// @param i
/// @return
int CheckGreen(unsigned char *data, int i)
{
    int intensity = data[i];

    if (intensity < 64)
    {
        return (4 * intensity);
    }
    else if (intensity < 192)
    {
        return 255;
    }
    else
    {
        return 255 - 4 * (intensity - 192);
    }
}
/// @brief Função de auxilio para a função vc_scale_gray_to_rgb
/// @param data
/// @param i
/// @return
int CheckBlue(unsigned char *data, int i)
{
    int intensity = data[i];

    if (intensity < 64)
    {
        return 255;
    }
    else if (intensity < 128)
    {
        return 4 * (intensity - 64) + 255;
    }
    else
    {
        return 0;
    }
}

/// @brief Faz a conversão de uma imagem de cinzentos para RGB
/// @param src
/// @param dst
/// @return
int vc_scale_gray_to_rgb(IVC *src, IVC *dst)
{
    unsigned char *data = (unsigned char *)src->data;
    int width = src->width;
    int height = src->height;

    int bytesperline = src->width * src->channels;
    int channels = src->channels;
    float r, g, b;
    int i, size;

    if ((src->width) <= 0 || (src->height <= 0) || (src->data == NULL))
    {
        printf("(vc_scale_gray_to_rgb) Tamanhos inválidos\n");
        return 0;
    }
    if (src->channels != 3)
    {
        printf("(vc_scale_gray_to_rgb) Imagem tem de ter 3 canais\n");
        return 0;
    }

    size = width * height * channels;
    int d = 0;

    for (i = 0; i < size; i = i++)
    { // isto porque o canal inicial é zero
        r = CheckRed(data, i);
        g = CheckGreen(data, i);
        b = CheckBlue(data, i);

        dst->data[d] = r;
        dst->data[d + 1] = g;
        dst->data[d + 2] = b;
        d += channels;
    }

    return 1;
}

#pragma endregion

#pragma region Segmentação de imagens por theshold (Limiarização)

/// @brief Faz a conversão de uma imagem de cinzentos para binario, passando o theshold (valor) por parametros
/// @param srcdst
/// @param dst
/// @param threshold
/// @return
int vc_gray_to_binary(IVC *srcdst, IVC *dst, int threshold)
{
    unsigned char *data = (unsigned char *)srcdst->data;
    unsigned char *data_dst = (unsigned char *)dst->data;
    int width = srcdst->width;
    int height = srcdst->height;
    int bytesperline = srcdst->width * srcdst->channels;
    int channels = srcdst->channels;
    int i, size;

    if ((srcdst->width) <= 0 || (srcdst->height <= 0) || (srcdst->data == NULL))
    {
        printf("(vc_gray_to_binary) Tamanhos inválidos\n");
        return 0;
    }

    if (dst->channels != 1 || srcdst->channels != 1)
    {
        printf("(vc_gray_to_binary) Imagem tem de ter 1 canal\n");
        return 0;
    }

    size = width * height * channels;

    for (i = 0; i < size; i = i + channels)
    {
        if (data[i] > threshold)
        {
            data_dst[i] = 255;
        }
        else
        {
            data_dst[i] = 0;
        }
    }

    return 1;
}

// Função que converte uma imagem em tons de cinza para uma imagem binária
// mas utiliza um threshold automático, calculado a partir da média dos níveis de cinzento
int vc_gray_to_binary_global_mean(IVC *srcdst, IVC *dst)
{
    unsigned char *data = (unsigned char *)srcdst->data;
    unsigned char *data_dst = (unsigned char *)dst->data;
    int width = srcdst->width;
    int height = srcdst->height;
    int bytesperline = srcdst->width * srcdst->channels;
    int channels = srcdst->channels;
    int i, size, x = 0, y = 0;
    int threshold = 0;
    int total = 0;
    int media = 0;

    if ((srcdst->width) <= 0 || (srcdst->height <= 0) || (srcdst->data == NULL))
    {
        printf("(vc_gray_to_binary_global_mean) Tamanhos inválidos\n");
        return 0;
    }

    if (dst->channels != 1 || srcdst->channels != 1)
    {
        printf("(vc_gray_to_binary_global_mean) Imagem tem de ter 1 canal\n");
        return 0;
    }

    size = width * height * channels;

    for (i = 0; i < size; i = i + channels)
    {
        total += data[i];
    }

    if (total <= 0)
    {
        printf("(vc_gray_to_binary_global_mean) Total dos pixeis é zero\n");
        return 0;
    }

    media = total / (width * height);

    threshold = media;

    int pos = y * bytesperline + x;

    for (i = 0; i < width; i++)
    {
        for (y = 0; y < height; y++)
        {
            pos = y * bytesperline + i * channels;
            if (data[pos] > threshold)
            {
                data_dst[pos] = 255;
            }
            else
            {
                data_dst[pos] = 0;
            }
        }
    }

    // for(i = 0; i < size; i = i + channels){
    //     if(data[i] > threshold){
    //         data_dst[i] = 255;
    //     }else{
    //         data_dst[i] = 0;
    //     }
    // }

    return 1;
}

// procura o maximo e o minimo dos pixels vizinhos
//  Midpoint = (max + min) / 2
int vc_gray_to_binary_midpoint(IVC *src, IVC *dst, int kernel)
{

    unsigned char *datasrc = (unsigned char *)src->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;
    int x, y, kx, ky, min, max;
    long int pos, posk;
    int offset = (kernel - 1) / 2; // Calculo do valor do offset
    float threshold = 0.0;

    unsigned char *datadst = (unsigned char *)dst->data;

    if (src->height <= 0 || src->width <= 0 || src->height <= 0 || src->width <= 0 || src == NULL || dst == NULL)
    {
        printf("(vc_gray_to_binary_midpoint) Tamanhos inválidos\n");
        return 0;
    }
    if (src->channels != 1 || dst->channels != 1)
    {
        printf("(vc_gray_to_binary_midpoint) Imagem tem de ter 1 canal\n");
        return 0;
    }

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            pos = y * bytesperline + x * channels;

            max = 0;
            min = 255;

            for (ky = -offset; ky <= offset; ky++)
            {
                for (kx = -offset; kx <= offset; kx++)
                {
                    if ((y + ky >= 0) && (y + ky < height) && (x + kx >= 0) && (x + kx < width))
                    {
                        posk = (y + ky) * bytesperline + (x + kx) * channels;

                        if (datasrc[posk] > max)
                            max = datasrc[posk];
                        if (datasrc[posk] < min)
                            min = datasrc[posk];
                    }
                }
            }

            threshold = ((float)(min + max) / (float)2); // --> Midpoint
            if (datasrc[pos] <= (unsigned)threshold)
                datadst[pos] = 0;
            else
                datadst[pos] = 255;
        }
    }
    return 1;
}

// Função binarização automática
int vc_gray_to_binary_Bersen(IVC *src, IVC *dst, int kernel, int c)
{

    unsigned char *datasrc = (unsigned char *)src->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;
    int x, y, kx, ky, min, max;
    long int pos, posk;
    int offset = (kernel - 1) / 2; // Calculo do valor do offset
    float threshold = 0.0;

    unsigned char *datadst = (unsigned char *)dst->data;

    if (src->height <= 0 || src->width <= 0 || src->height <= 0 || src->width <= 0 || src == NULL || dst == NULL)
    {
        printf("(vc_gray_to_binary_Bersen) Tamanhos inválidos\n");
        return 0;
    }
    if (src->channels != 1 || dst->channels != 1)
    {
        printf("(vc_gray_to_binary_Bersen) Imagem tem de ter 1 canal\n");
        return 0;
    }

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            pos = y * bytesperline + x * channels;

            max = 0;
            min = 255;

            for (ky = -offset; ky <= offset; ky++)
            {
                for (kx = -offset; kx <= offset; kx++)
                {
                    if ((y + ky >= 0) && (y + ky < height) && (x + kx >= 0) && (x + kx < width))
                    {
                        posk = (y + ky) * bytesperline + (x + kx) * channels;

                        if (datasrc[posk] > max)
                            max = datasrc[posk];
                        if (datasrc[posk] < min)
                            min = datasrc[posk];
                    }
                }
            }

            if (max - min <= c)
                threshold = (unsigned char)(float)src->levels / (float)2;
            else
                threshold = (unsigned char)(float)(max + min) / (float)2;

            if (datasrc[pos] <= (unsigned)threshold)
                datadst[pos] = 0;
            else
                datadst[pos] = 255;
        }
    }
    return 1;
}

// Função binarização automática Niblack
int vc_gray_to_binary_Niblack(IVC *src, IVC *dst, int kernel, float k)
{

    unsigned char *datasrc = (unsigned char *)src->data;
    unsigned char *datadst = (unsigned char *)dst->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;
    int x, y, kx, ky, min, max, contador;
    float media = 0, soma = 0;
    float threshold = 0.0;
    long int pos, posk;
    int offset = (kernel - 1) / 2; // Calculo do valor do offset

    if (src->height <= 0 || src->width <= 0 || src->height <= 0 || src->width <= 0 || src == NULL || dst == NULL)
    {
        printf("(vc_gray_to_binary_Niblack) Tamanhos inválidos\n");
        return 0;
    }
    if (src->channels != 1 || dst->channels != 1)
    {
        printf("(vc_gray_to_binary_Niblack) Imagem tem de ter 1 canal\n");
        return 0;
    }

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            pos = y * bytesperline + x * channels;

            // Reset aos valores
            media = 0.0;
            float desvioPadrao = 0.0;

            for (ky = -offset; ky <= offset; ky++)
            {
                for (kx = -offset; kx <= offset; kx++)
                {
                    if ((y + ky >= 0) && (y + ky < height) && (x + kx >= 0) && (x + kx < width))
                    {
                        posk = (y + ky) * bytesperline + (x + kx) * channels;

                        // Fazer a soma para encontrar a média dos pixels
                        media += (float)datasrc[posk];
                        contador++;
                    }
                }
            }

            // Média dos pixels
            media /= contador;

            // Reset aos valores
            contador = 0;

            // Calcular o desvio padrão
            for (ky = -offset; ky <= offset; ky++)
            {
                for (kx = -offset; kx <= offset; kx++)
                {
                    if ((y + ky >= 0) && (y + ky < height) && (x + kx >= 0) && (x + kx < width))
                    {
                        posk = (y + ky) * bytesperline + (x + kx) * channels;

                        // soma += (float)powf(datasrc[posk] - media, 2);
                        desvioPadrao += powf(((float)datasrc[posk] - media), 2);
                        contador++;
                    }
                }
            }

            // Calcular o desvio padrão
            desvioPadrao = sqrtf(desvioPadrao / contador);

            // Encontrar o threshold
            threshold = (unsigned char)media + k * desvioPadrao;

            if (datasrc[pos] < (unsigned)threshold)
                datadst[pos] = 0;
            else
                datadst[pos] = 255;
        }
    }

    printf(" Niblack feito\n");
    return 1;
}

#pragma endregion

#pragma region Morfologia

#pragma region Binaria

/// @brief Função que faz a erosão de uma imagem binária
/// Basta existir um pixel na vizinhança a zero para que o pixel central ser 255
/// @param src
/// @param dst
/// @param kernel
/// @return
int vc_binary_erode(IVC *src, IVC *dst, int kernel)
{

    unsigned char *datasrc = (unsigned char *)src->data;
    unsigned char *datadst = (unsigned char *)dst->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;
    int x, y, kx, ky;
    long int pos, posk;
    int offset = (kernel - 1) / 2; // Calculo do valor do offset

    if (src->height <= 0 || src->width <= 0 || src->height <= 0 || src->width <= 0 || src == NULL || dst == NULL)
    {
        printf("(vc_binary_erode) Tamanhos inválidos\n");
        return 0;
    }
    if (src->channels != 1 || dst->channels != 1)
    {
        printf("(vc_binary_erode) Imagem tem de ter 1 canal\n");
        // return 0;
    }

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            pos = y * bytesperline + x * channels;

            int flag = 0;

            for (ky = -offset; ky <= offset; ky++)
            {
                for (kx = -offset; kx <= offset; kx++)
                {
                    if ((y + ky >= 0) && (y + ky < height) && (x + kx >= 0) && (x + kx < width))
                    {
                        posk = (y + ky) * bytesperline + (x + kx) * channels;

                        if (datasrc[posk] == 0)
                        {
                            flag = 1;
                            break;
                        }
                    }
                }
            }
            if (flag == 1)
            {
                datadst[pos] = 0;
            }
            else
            {
                datadst[pos] = 255;
            }
        }
    }
    return 1;
}

int vc_binary_erodeCentro(IVC *src, IVC *dst, int kernel)
{
    unsigned char *datasrc = (unsigned char *)src->data;
    unsigned char *datadst = (unsigned char *)dst->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;
    int x, y, kx, ky;
    long int pos, posk;
    int offset = (kernel - 1) / 2; // Calculo do valor do offset

    if (src->height <= 0 || src->width <= 0 || src == NULL || dst == NULL)
    {
        printf("(vc_binary_erode) Tamanhos inválidos\n");
        return 0;
    }
    if (src->channels != 1 || dst->channels != 1)
    {
        printf("(vc_binary_erode) Imagem tem de ter 1 canal\n");
        return 0;
    }

    // Calcular os limites para os 40% centrais da largura da imagem
    int x_start = width * 0.3; // 30% da largura da imagem
    int x_end = width * 0.7;   // 70% da largura da imagem

    // Inicializar a imagem de destino com valor 255 (branco)
    memset(datadst, 255, width * height * sizeof(unsigned char));

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            pos = y * bytesperline + x * channels;

            if (x < x_start || x >= x_end)
            {
                // Definir as áreas fora da região central como preto (valor 0)
                datadst[pos] = 0;
            }
            else
            {
                int flag = 0;

                for (ky = -offset; ky <= offset; ky++)
                {
                    for (kx = -offset; kx <= offset; kx++)
                    {
                        if ((y + ky >= 0) && (y + ky < height) && (x + kx >= 0) && (x + kx < width))
                        {
                            posk = (y + ky) * bytesperline + (x + kx) * channels;

                            if (datasrc[posk] == 0)
                            {
                                flag = 1;
                                break;
                            }
                        }
                    }
                    if (flag == 1)
                    {
                        break;
                    }
                }

                if (flag == 1)
                {
                    datadst[pos] = 0;
                }
                else
                {
                    datadst[pos] = 255;
                }
            }
        }
    }
    return 1;
}


/// @brief Função que faz a dilatação de uma imagem binária
/// Basta existir um pixel na vizinhança a um para que o pixel central ser 255
/// @param src
/// @param dst
/// @param kernel
/// @return
int vc_binary_dilate(IVC *src, IVC *dst, int kernel)
{

    unsigned char *datasrc = (unsigned char *)src->data;
    unsigned char *datadst = (unsigned char *)dst->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;
    int x, y, kx, ky;
    long int pos, posk;
    int offset = (kernel - 1) / 2; // Calculo do valor do offset

    if (src->height <= 0 || src->width <= 0 || src->height <= 0 || src->width <= 0 || src == NULL || dst == NULL)
    {
        printf("(vc_binary_dilate) Tamanhos inválidos\n");
        return 0;
    }
    if (src->channels != 1)
    {
        printf("(vc_binary_dilate) Erro imagem de origem, canais tem de ser 1\n");
        return 0; // Verifica se as imagens têm os canais corretos
    }
    if (dst->channels != 1)
    {
        printf("(vc_binary_dilate) Erro imagem de destino, canais tem de ser 1\n");
        return 0; // Verifica se as imagens têm os canais corretos
    }

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            pos = y * bytesperline + x * channels;

            int flag = 0;

            for (ky = -offset; ky <= offset; ky++)
            {
                for (kx = -offset; kx <= offset; kx++)
                {
                    if ((y + ky >= 0) && (y + ky < height) && (x + kx >= 0) && (x + kx < width))
                    {
                        posk = (y + ky) * bytesperline + (x + kx) * channels;

                        // Fiz com flags, mas podemos fazer de várias formas.
                        if (datasrc[posk] >= 1)
                        { // tecnicamente basta ser diferente de zero, mas pronto.
                            flag = 1;
                            break;
                        }
                    }
                }
            }

            if (flag == 1)
            {
                datadst[pos] = 255;
            }
            else
            {
                datadst[pos] = 0;
            }
        }
    }

    return 1;
}

int vc_binary_dilateCentro(IVC *src, IVC *dst, int kernel)
{
    unsigned char *datasrc = (unsigned char *)src->data;
    unsigned char *datadst = (unsigned char *)dst->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;
    int x, y, kx, ky;
    long int pos, posk;
    int offset = (kernel - 1) / 2; // Calculo do valor do offset

    if (src->height <= 0 || src->width <= 0 || src == NULL || dst == NULL)
    {
        printf("(vc_binary_dilate) Tamanhos inválidos\n");
        return 0;
    }
    if (src->channels != 1)
    {
        printf("(vc_binary_dilate) Erro imagem de origem, canais tem de ser 1\n");
        return 0; // Verifica se as imagens têm os canais corretos
    }
    if (dst->channels != 1)
    {
        printf("(vc_binary_dilate) Erro imagem de destino, canais tem de ser 1\n");
        return 0; // Verifica se as imagens têm os canais corretos
    }

    // Calcular os limites para os 40% centrais da largura da imagem
    int x_start = width * 0.3; // 30% da largura da imagem
    int x_end = width * 0.7;   // 70% da largura da imagem

    // Inicializar a imagem de destino com valor 0 (preto)
    memset(datadst, 0, width * height * sizeof(unsigned char));

    for (y = 0; y < height; y++)
    {
        for (x = x_start; x < x_end; x++)
        {
            pos = y * bytesperline + x * channels;

            int flag = 0;

            for (ky = -offset; ky <= offset; ky++)
            {
                for (kx = -offset; kx <= offset; kx++)
                {
                    if ((y + ky >= 0) && (y + ky < height) && (x + kx >= 0) && (x + kx < width))
                    {
                        posk = (y + ky) * bytesperline + (x + kx) * channels;

                        if (datasrc[posk] >= 1)
                        {
                            flag = 1;
                            break;
                        }
                    }
                }
                if (flag == 1)
                {
                    break;
                }
            }

            if (flag == 1)
            {
                datadst[pos] = 255;
            }
            else
            {
                datadst[pos] = 0;
            }
        }
    }

    return 1;
}


/// @brief Erosão -> Dilatação para imagens binarias
/// @param src
/// @param dst
/// @param kernel
/// @return
int vc_binary_open(IVC *src, IVC *dst, int kernelE, int kernelD)
{
    // precisamos de criar uma imagem temporária para poder passar
    IVC *temp = vc_image_new(dst->width, dst->height, dst->channels, dst->levels);

    vc_binary_erode(src, temp, kernelE);

    vc_binary_dilate(temp, dst, kernelD);

    vc_image_free(temp);
    printf("Abertuta concluida\n");

    return 1;
}

/// @brief Erosão -> Dilatação para cinzentos
/// @param src
/// @param dst
/// @param kernel
/// @return
int vc_gray_open(IVC *src, IVC *dst, int kernelE, int kernelD)
{
    // precisamos de criar uma imagem temporária para poder passar
    IVC *temp = vc_image_new(dst->width, dst->height, dst->channels, dst->levels);

    vc_gray_erode(src, temp, kernelE);

    vc_gray_dilate(temp, dst, kernelD);

    vc_image_free(temp);
    printf("Abertuta concluida\n");

    return 1;
}

/// @brief Dilatação->Erosão para binario
/// @param src
/// @param dst
/// @param kernel
/// @return
int vc_binary_close(IVC *src, IVC *dst, int kernelE, int kernelD)
{

    IVC *temp = vc_image_new(dst->width, dst->height, dst->channels, dst->levels);

    vc_binary_dilate(src, temp, kernelD);

    vc_binary_erode(temp, dst, kernelE);

    vc_image_free(temp);
    printf("Fecho concluido\n");

    return 1;
}

/// @brief Dilatação->Erosão para cinzentos
/// @param src
/// @param dst
/// @param kernel
/// @return
int vc_gray_close(IVC *src, IVC *dst, int kernelE, int kernelD)
{

    IVC *temp = vc_image_new(dst->width, dst->height, dst->channels, dst->levels);

    vc_gray_dilate(src, temp, kernelD);

    vc_gray_erode(temp, dst, kernelE);

    vc_image_free(temp);
    printf("Fecho concluido\n");

    return 1;
}

// Subtração de imagens binárias
int vc_binary_sub(IVC *imagem1, IVC *imagem2, IVC *destino)
{

    unsigned char *data1 = (unsigned char *)imagem1->data;
    unsigned char *data2 = (unsigned char *)imagem2->data;
    unsigned char *data_destino = (unsigned char *)destino->data;
    int width = imagem1->width;
    int height = imagem1->height;
    int bytesperline = imagem1->width * imagem1->channels;
    int channels = imagem1->channels;
    int i, size;

    if ((imagem1->width) <= 0 || (imagem1->height <= 0) || (imagem1->data == NULL))
        return 0;
    if ((imagem2->width) <= 0 || (imagem2->height <= 0) || (imagem2->data == NULL))
        return 0;
    if ((destino->width) <= 0 || (destino->height <= 0) || (destino->data == NULL))
        return 0;
    if (imagem1->channels != 1 || imagem2->channels != 1 || destino->channels != 1)
        return 0;

    size = width * height * channels;

    for (i = 0; i < size; i = i + channels)
    {
        if (data1[i] == 255 && data2[i] == 0)
        {
            data_destino[i] = 255;
        }
        else
        {
            data_destino[i] = 0;
        }
    }
    return 1;
}

#pragma endregion

#pragma region grey

/// @brief Função que faz a erosão de uma imagem em escala de cinzentos
/// Basta existir um pixel na vizinhança a zero para que o pixel central ser 255
/// @param src
/// @param dst
/// @param kernel
/// @return
int vc_gray_erode(IVC *src, IVC *dst, int kernel)
{
    unsigned char *datasrc = (unsigned char *)src->data;
    unsigned char *datadst = (unsigned char *)dst->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;
    int x, y, kx, ky, min, max;
    long int pos, posk;
    int offset = (kernel - 1) / 2; // Calculo do valor do offset

    if (dst->channels != 1)
    {
        printf("Imagen secundária tem mais do que 1 canal");
        return 0;
    }
    if (src->height <= 0 || src->width <= 0 || src->height <= 0 || src->width <= 0 || src == NULL || dst == NULL)
    {
        printf("(vc_gray_erode) Tamanhos inválidos\n");
        return 0;
    }
    if (src->channels != 1 || dst->channels != 1)
    {
        printf("(vc_gray_erode) Imagem tem de ter 1 canal\n");
        return 0;
    }

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            pos = y * bytesperline + x * channels;

            max = 0;
            min = 255;

            for (ky = -offset; ky <= offset; ky++)
            {
                for (kx = -offset; kx <= offset; kx++)
                {
                    if ((y + ky >= 0) && (y + ky < height) && (x + kx >= 0) && (x + kx < width))
                    {
                        posk = (y + ky) * bytesperline + (x + kx) * channels;

                        // if(datasrc[posk]>max) max=datasrc[posk];
                        if (datasrc[posk] < min)
                            min = datasrc[posk];
                    }
                }
            }

            datadst[pos] = min;
        }
    }
    return 1;
}
/// @brief Funcão que faz a dilatação de uma imagem em escala de cinzentos
/// @param src
/// @param dst
/// @param kernel
/// @return
int vc_gray_dilate(IVC *src, IVC *dst, int kernel)
{
    unsigned char *datasrc = (unsigned char *)src->data;
    unsigned char *datadst = (unsigned char *)dst->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;
    int x, y, kx, ky, min, max;
    long int pos, posk;
    int offset = (kernel - 1) / 2; // Calculo do valor do offset

    if (dst->channels != 1)
    {
        printf("(vc_gray_dilate) Imagen secundária tem mais do que 1 canal");
        return 0;
    }
    if (src->height <= 0 || src->width <= 0 || src->height <= 0 || src->width <= 0 || src == NULL || dst == NULL)
    {
        printf("(vc_gray_dilate) Tamanhos inválidos\n");
        return 0;
    }
    if (src->channels != 1 || dst->channels != 1)
    {
        printf("(vc_gray_dilate) Imagem tem de ter 1 canal\n");
        return 0;
    }

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            pos = y * bytesperline + x * channels;

            int flag = 0;
            max = 0;
            min = 255;

            for (ky = -offset; ky <= offset; ky++)
            {
                for (kx = -offset; kx <= offset; kx++)
                {
                    if ((y + ky >= 0) && (y + ky < height) && (x + kx >= 0) && (x + kx < width))
                    {
                        posk = (y + ky) * bytesperline + (x + kx) * channels;

                        if (datasrc[posk] > max)
                            max = datasrc[posk];
                    }
                }
            }

            datadst[pos] = max;
        }
    }
    return 1;
}

#pragma endregion

#pragma region Etiquetagem
// Etiquetagem de blobs
// src		: Imagem bin�ria de entrada
// dst		: Imagem grayscale (ir� conter as etiquetas)
// nlabels	: Endere�o de mem�ria de uma vari�vel, onde ser� armazenado o n�mero de etiquetas encontradas.
// OVC*		: Retorna um array de estruturas de blobs (objectos), com respectivas etiquetas. � necess�rio libertar posteriormente esta mem�ria.
OVC *vc_binary_blob_labelling(IVC *src, IVC *dst, int *nlabels)
{
    unsigned char *datasrc = (unsigned char *)src->data;
    unsigned char *datadst = (unsigned char *)dst->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->bytesperline;
    int channels = src->channels;
    int x, y, a, b;
    long int i, size;
    long int posX, posA, posB, posC, posD;
    int labeltable[256] = {0};
    int labelarea[256] = {0};
    int label = 1; // Etiqueta inicial.
    int num, tmplabel;
    OVC *blobs; // Apontador para array de blobs (objectos) que ser� retornado desta fun��o.

    // Verifica��o de erros
    if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
    {
        printf("(vc_binary_blob_labelling) Tamanhos inválidos.\n");
        return 0;
    }
    if ((src->width != dst->width) || (src->height != dst->height))
    {
        printf("(vc_binary_blob_labelling) As imagens origem e destino têm tamanhos diferentes.\n");
        return 0; // Estava NULL
    }

    if ((src->channels != dst->channels))
    {
        printf("(vc_binary_blob_labelling) As imagens origem e destino têm número de canais diferentes.\n");
        return 0; // Estava NULL
    }

//    if (channels != 1)
//    { // apenas para imagens binárias; valor oficial é um!!
//        printf("(vc_binary_blob_labelling) Canais diferentes de 1\n");
//        return NULL;
//    }
    // Copia dados da imagem bin�ria para imagem grayscale
    memcpy(datadst, datasrc, bytesperline * height);

    // Todos os pix�is de plano de fundo devem obrigat�riamente ter valor 0
    // Todos os pix�is de primeiro plano devem obrigat�riamente ter valor 255
    // Ser�o atribu�das etiquetas no intervalo [1,254]
    // Este algoritmo est� assim limitado a 254 labels
    for (i = 0, size = bytesperline * height; i < size; i++)
    {
        if (datadst[i] != 0)
            datadst[i] = 255;
    }

    // Limpa os rebordos da imagem bin�ria
    for (y = 0; y < height; y++)
    {
        datadst[y * bytesperline + 0 * channels] = 0;
        datadst[y * bytesperline + (width - 1) * channels] = 0;
    }
    for (x = 0; x < width; x++)
    {
        datadst[0 * bytesperline + x * channels] = 0;
        datadst[(height - 1) * bytesperline + x * channels] = 0;
    }

    // Efectua a etiquetagem
    for (y = 1; y < height - 1; y++)
    {
        for (x = 1; x < width - 1; x++)
        {
            // Kernel:
            // A B C
            // D X

            posA = (y - 1) * bytesperline + (x - 1) * channels; // A
            posB = (y - 1) * bytesperline + x * channels;       // B
            posC = (y - 1) * bytesperline + (x + 1) * channels; // C
            posD = y * bytesperline + (x - 1) * channels;       // D
            posX = y * bytesperline + x * channels;             // X

            // Se o pixel foi marcado
            if (datadst[posX] != 0)
            {
                if ((datadst[posA] == 0) && (datadst[posB] == 0) && (datadst[posC] == 0) && (datadst[posD] == 0))
                {
                    datadst[posX] = label;
                    labeltable[label] = label;
                    label++;
                }
                else
                {
                    num = 255;

                    // Se A est� marcado
                    if (datadst[posA] != 0)
                        num = labeltable[datadst[posA]];
                    // Se B est� marcado, e � menor que a etiqueta "num"
                    if ((datadst[posB] != 0) && (labeltable[datadst[posB]] < num))
                        num = labeltable[datadst[posB]];
                    // Se C est� marcado, e � menor que a etiqueta "num"
                    if ((datadst[posC] != 0) && (labeltable[datadst[posC]] < num))
                        num = labeltable[datadst[posC]];
                    // Se D est� marcado, e � menor que a etiqueta "num"
                    if ((datadst[posD] != 0) && (labeltable[datadst[posD]] < num))
                        num = labeltable[datadst[posD]];

                    // Atribui a etiqueta ao pixel
                    datadst[posX] = num;
                    labeltable[num] = num;

                    // Actualiza a tabela de etiquetas
                    if (datadst[posA] != 0)
                    {
                        if (labeltable[datadst[posA]] != num)
                        {
                            for (tmplabel = labeltable[datadst[posA]], a = 1; a < label; a++)
                            {
                                if (labeltable[a] == tmplabel)
                                {
                                    labeltable[a] = num;
                                }
                            }
                        }
                    }
                    if (datadst[posB] != 0)
                    {
                        if (labeltable[datadst[posB]] != num)
                        {
                            for (tmplabel = labeltable[datadst[posB]], a = 1; a < label; a++)
                            {
                                if (labeltable[a] == tmplabel)
                                {
                                    labeltable[a] = num;
                                }
                            }
                        }
                    }
                    if (datadst[posC] != 0)
                    {
                        if (labeltable[datadst[posC]] != num)
                        {
                            for (tmplabel = labeltable[datadst[posC]], a = 1; a < label; a++)
                            {
                                if (labeltable[a] == tmplabel)
                                {
                                    labeltable[a] = num;
                                }
                            }
                        }
                    }
                    if (datadst[posD] != 0)
                    {
                        if (labeltable[datadst[posD]] != num)
                        {
                            for (tmplabel = labeltable[datadst[posD]], a = 1; a < label; a++)
                            {
                                if (labeltable[a] == tmplabel)
                                {
                                    labeltable[a] = num;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Volta a etiquetar a imagem
    for (y = 1; y < height - 1; y++)
    {
        for (x = 1; x < width - 1; x++)
        {
            posX = y * bytesperline + x * channels; // X

            if (datadst[posX] != 0)
            {
                datadst[posX] = labeltable[datadst[posX]];
            }
        }
    }

    // printf("\nMax Label = %d\n", label);

    // Contagem do n�mero de blobs
    // Passo 1: Eliminar, da tabela, etiquetas repetidas
    for (a = 1; a < label - 1; a++)
    {
        for (b = a + 1; b < label; b++)
        {
            if (labeltable[a] == labeltable[b])
                labeltable[b] = 0;
        }
    }
    // Passo 2: Conta etiquetas e organiza a tabela de etiquetas, para que n�o hajam valores vazios (zero) entre etiquetas
    *nlabels = 0;
    for (a = 1; a < label; a++)
    {
        if (labeltable[a] != 0)
        {
            labeltable[*nlabels] = labeltable[a]; // Organiza tabela de etiquetas
            (*nlabels)++;                         // Conta etiquetas
        }
    }

    // Se n�o h� blobs
    if (*nlabels == 0)
        return NULL;

    // Cria lista de blobs (objectos) e preenche a etiqueta
    blobs = (OVC *)calloc((*nlabels), sizeof(OVC));
    if (blobs != NULL)
    {
        for (a = 0; a < (*nlabels); a++)
            blobs[a].label = labeltable[a];
    }
    else
    {
        printf("(vc_binary_blob_labelling) Não foi possível alocar memória para a lista de blobs.\n");
        return NULL;
    }

    return blobs;
}

OVC *vc_binary_blob_labellingTeste(IVC *src, IVC *dst, int *nlabels)
{
    unsigned char *datasrc = (unsigned char *)src->data;
    unsigned char *datadst = (unsigned char *)dst->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->bytesperline;
    int channels = src->channels;
    int x, y, a, b;
    long int i, size;
    long int posX, posA, posB, posC, posD;
    int labeltable[256] = {0};
    int labelarea[256] = {0};
    int label = 1; // Etiqueta inicial.
    int num, tmplabel;
    OVC *blobs; // Apontador para array de blobs (objectos) que será retornado desta função.

    // Verificação de erros
    if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
    {
        printf("(vc_binary_blob_labelling) Tamanhos inválidos.\n");
        return 0;
    }
    if ((src->width != dst->width) || (src->height != dst->height))
    {
        printf("(vc_binary_blob_labelling) As imagens origem e destino têm tamanhos diferentes.\n");
        return 0; // Estava NULL
    }

    if ((src->channels != dst->channels))
    {
        printf("(vc_binary_blob_labelling) As imagens origem e destino têm número de canais diferentes.\n");
        return 0; // Estava NULL
    }

    // Copia dados da imagem binária para imagem grayscale
    memcpy(datadst, datasrc, bytesperline * height);

    // Todos os pixéis de plano de fundo devem obrigatoriamente ter valor 0
    // Todos os pixéis de primeiro plano devem obrigatoriamente ter valor 255
    // Serão atribuídas etiquetas no intervalo [1,254]
    // Este algoritmo está assim limitado a 254 labels
    for (i = 0, size = bytesperline * height; i < size; i++)
    {
        if (datadst[i] != 0)
            datadst[i] = 255;
    }

    // Limpa os rebordos da imagem binária
    for (y = 0; y < height; y++)
    {
        datadst[y * bytesperline + 0 * channels] = 0;
        datadst[y * bytesperline + (width - 1) * channels] = 0;
    }
    for (x = 0; x < width; x++)
    {
        datadst[0 * bytesperline + x * channels] = 0;
        datadst[(height - 1) * bytesperline + x * channels] = 0;
    }

    // Calcular os limites para os 40% centrais da imagem
    int x_start = width * 0.3; // 30% da largura da imagem
    int x_end = width * 0.7;   // 70% da largura da imagem


    // Efectua a etiquetagem apenas na região central
    for (y = 1; y < height - 1; y++)
    {
        for (x = x_start; x < x_end; x++)
        {
            // Kernel:
            // A B C
            // D X

            posA = (y - 1) * bytesperline + (x - 1) * channels; // A
            posB = (y - 1) * bytesperline + x * channels;       // B
            posC = (y - 1) * bytesperline + (x + 1) * channels; // C
            posD = y * bytesperline + (x - 1) * channels;       // D
            posX = y * bytesperline + x * channels;             // X

            // Se o pixel foi marcado
            if (datadst[posX] != 0)
            {
                if ((datadst[posA] == 0) && (datadst[posB] == 0) && (datadst[posC] == 0) && (datadst[posD] == 0))
                {
                    datadst[posX] = label;
                    labeltable[label] = label;
                    label++;
                }
                else
                {
                    num = 255;

                    // Se A está marcado
                    if (datadst[posA] != 0)
                        num = labeltable[datadst[posA]];
                    // Se B está marcado, e é menor que a etiqueta "num"
                    if ((datadst[posB] != 0) && (labeltable[datadst[posB]] < num))
                        num = labeltable[datadst[posB]];
                    // Se C está marcado, e é menor que a etiqueta "num"
                    if ((datadst[posC] != 0) && (labeltable[datadst[posC]] < num))
                        num = labeltable[datadst[posC]];
                    // Se D está marcado, e é menor que a etiqueta "num"
                    if ((datadst[posD] != 0) && (labeltable[datadst[posD]] < num))
                        num = labeltable[datadst[posD]];

                    // Atribui a etiqueta ao pixel
                    datadst[posX] = num;
                    labeltable[num] = num;

                    // Actualiza a tabela de etiquetas
                    if (datadst[posA] != 0)
                    {
                        if (labeltable[datadst[posA]] != num)
                        {
                            for (tmplabel = labeltable[datadst[posA]], a = 1; a < label; a++)
                            {
                                if (labeltable[a] == tmplabel)
                                {
                                    labeltable[a] = num;
                                }
                            }
                        }
                    }
                    if (datadst[posB] != 0)
                    {
                        if (labeltable[datadst[posB]] != num)
                        {
                            for (tmplabel = labeltable[datadst[posB]], a = 1; a < label; a++)
                            {
                                if (labeltable[a] == tmplabel)
                                {
                                    labeltable[a] = num;
                                }
                            }
                        }
                    }
                    if (datadst[posC] != 0)
                    {
                        if (labeltable[datadst[posC]] != num)
                        {
                            for (tmplabel = labeltable[datadst[posC]], a = 1; a < label; a++)
                            {
                                if (labeltable[a] == tmplabel)
                                {
                                    labeltable[a] = num;
                                }
                            }
                        }
                    }
                    if (datadst[posD] != 0)
                    {
                        if (labeltable[datadst[posD]] != num)
                        {
                            for (tmplabel = labeltable[datadst[posD]], a = 1; a < label; a++)
                            {
                                if (labeltable[a] == tmplabel)
                                {
                                    labeltable[a] = num;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Volta a etiquetar a imagem
    for (y = 1; y < height - 1; y++)
    {
        for (x = x_start; x < x_end; x++)
        {
            posX = y * bytesperline + x * channels; // X

            if (datadst[posX] != 0)
            {
                datadst[posX] = labeltable[datadst[posX]];
            }
        }
    }

    // printf("\nMax Label = %d\n", label);

    // Contagem do número de blobs
    // Passo 1: Eliminar, da tabela, etiquetas repetidas
    for (a = 1; a < label - 1; a++)
    {
        for (b = a + 1; b < label; b++)
        {
            if (labeltable[a] == labeltable[b])
                labeltable[b] = 0;
        }
    }
    // Passo 2: Conta etiquetas e organiza a tabela de etiquetas, para que não hajam valores vazios (zero) entre etiquetas
    *nlabels = 0;
    for (a = 1; a < label; a++)
    {
        if (labeltable[a] != 0)
        {
            labeltable[*nlabels] = labeltable[a]; // Organiza tabela de etiquetas
            (*nlabels)++;                         // Conta etiquetas
        }
    }

    // Se não há blobs
    if (*nlabels == 0)
        return NULL;

    // Cria lista de blobs (objectos) e preenche a etiqueta
    blobs = (OVC *)calloc((*nlabels), sizeof(OVC));
    if (blobs != NULL)
    {
        for (a = 0; a < (*nlabels); a++)
            blobs[a].label = labeltable[a];
    }
    else
    {
        printf("(vc_binary_blob_labelling) Não foi possível alocar memória para a lista de blobs.\n");
        return NULL;
    }

    return blobs;
}

/// @brief Função fornecida pelo Prof. permite encontrar a informação dos blobs
/// area, perimetro.
/// @param src
/// @param blobs
/// @param nblobs
/// @return
int vc_binary_blob_info(IVC *src, OVC *blobs, int nblobs)
{
    unsigned char *data = (unsigned char *)src->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->bytesperline;
    int channels = src->channels;
    int x, y, i;
    long int pos;
    int xmin, ymin, xmax, ymax;
    long int sumx, sumy;


    // Verifica��o de erros
    if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
    {
        printf("(vc_binary_blob_info) Erro: A imagem não tem dados.\n");
        return 0;
    }
    if (channels != 1)
    {
        printf("(vc_binary_blob_info) Erro: A imagem não é binária.\n");
        return 0;
    }

    // Conta �rea de cada blob
    for (i = 0; i < nblobs; i++)
    {
        xmin = width - 1;
        ymin = height - 1;
        xmax = 0;
        ymax = 0;

        sumx = 0;
        sumy = 0;

        blobs[i].area = 0;

        for (y = 1; y < height - 1; y++)
        {
            for (x = 1; x < width - 1; x++)
            {
                pos = y * bytesperline + x * channels;

                if (data[pos] == blobs[i].label)
                {
                    // �rea
                    blobs[i].area++;

                    // Centro de Gravidade
                    sumx += x;
                    sumy += y;

                    // Bounding Box
                    if (xmin > x)
                        xmin = x;
                    if (ymin > y)
                        ymin = y;
                    if (xmax < x)
                        xmax = x;
                    if (ymax < y)
                        ymax = y;

                    // Per�metro
                    // Se pelo menos um dos quatro vizinhos n�o pertence ao mesmo label, ent�o � um pixel de contorno
                    if ((data[pos - 1] != blobs[i].label) || (data[pos + 1] != blobs[i].label) || (data[pos - bytesperline] != blobs[i].label) || (data[pos + bytesperline] != blobs[i].label))
                    {
                        blobs[i].perimeter++;
                    }
                }
            }
        }

        // Bounding Box
        blobs[i].x = xmin;
        blobs[i].y = ymin;
        blobs[i].width = (xmax - xmin) + 1;
        blobs[i].height = (ymax - ymin) + 1;
        

        // Centro de Gravidade
        // blobs[i].xc = (xmax - xmin) / 2;
        // blobs[i].yc = (ymax - ymin) / 2;
        blobs[i].xc = sumx / MAX(blobs[i].area, 1);
        blobs[i].yc = sumy / MAX(blobs[i].area, 1);
    }

  
    return 1;
}

int vc_binary_blob_infoTeste(IVC *src, OVC *blobs, int nblobs)
{
    unsigned char *data = (unsigned char *)src->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->bytesperline;
    int channels = src->channels;
    int x, y, i;
    long int pos;
    int xmin, ymin, xmax, ymax;
    long int sumx, sumy;

    // Verificação de erros
    if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
    {
        printf("(vc_binary_blob_info) Erro: A imagem não tem dados.\n");
        return 0;
    }
    if (channels != 1)
    {
        printf("(vc_binary_blob_info) Erro: A imagem não é binária.\n");
        return 0;
    }

    // Calcular os limites para os 40% centrais da largura da imagem
    int x_start = width * 0.3; // 30% da largura da imagem
    int x_end = width * 0.7;   // 70% da largura da imagem

    // Conta área de cada blob
    for (i = 0; i < nblobs; i++)
    {
        xmin = width - 1;
        ymin = height - 1;
        xmax = 0;
        ymax = 0;

        sumx = 0;
        sumy = 0;

        blobs[i].area = 0;
        blobs[i].perimeter = 0;

        for (y = 1; y < height - 1; y++)
        {
            for (x = x_start; x < x_end; x++)
            {
                pos = y * bytesperline + x * channels;

                if (data[pos] == blobs[i].label)
                {
                    // Área
                    blobs[i].area++;

                    // Centro de Gravidade
                    sumx += x;
                    sumy += y;

                    // Bounding Box
                    if (xmin > x)
                        xmin = x;
                    if (ymin > y)
                        ymin = y;
                    if (xmax < x)
                        xmax = x;
                    if (ymax < y)
                        ymax = y;

                    // Perímetro
                    // Se pelo menos um dos quatro vizinhos não pertence ao mesmo label, então é um pixel de contorno
                    if ((data[pos - 1] != blobs[i].label) || (data[pos + 1] != blobs[i].label) ||
                        (data[pos - bytesperline] != blobs[i].label) || (data[pos + bytesperline] != blobs[i].label))
                    {
                        blobs[i].perimeter++;
                    }
                }
            }
        }

        // Bounding Box
        blobs[i].x = xmin;
        blobs[i].y = ymin;
        blobs[i].width = (xmax - xmin) + 1;
        blobs[i].height = (ymax - ymin) + 1;

        // Centro de Gravidade
        blobs[i].xc = sumx / MAX(blobs[i].area, 1);
        blobs[i].yc = sumy / MAX(blobs[i].area, 1);
    }

    return 1;
}


/// @brief Transforma a imagem produzida pela etiquetagem em grayscale
/// Mostra os blobs com  diferentes tons de cinza
///                     Função foi substituida pela vc_bin_labels (proxima função)
/// @param src
/// @param dst
/// @param nblobs
/// @param blobs
/// @return
int vc_cores_labels(IVC *src, IVC *dst, int nblobs, OVC *blobs)
{
    unsigned char *datasrc = (unsigned char *)src->data;
    unsigned char *datadst = (unsigned char *)dst->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;
    int x, y;
    long int pos;
    int i;
    int lables[nblobs];

    if (src->height <= 0 || src->width <= 0 || src->height <= 0 || src->width <= 0 || src == NULL || dst == NULL)
    {
        printf("(vc_bin_labels) Tamanhos inválidos\n");
        return 0;
    }

    // if (src->channels != 1 || dst->channels != 3) return 0; // Verifica se as imagens têm os canais corretos

    printf("Numero de blobs = %d\n", nblobs);
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            pos = y * bytesperline + x * channels;
            for (int i = 0; i <= nblobs; i++)
            {

                if (datasrc[pos] == blobs[i].label)
                {
                    datadst[pos] = (datasrc[pos] * 255) / nblobs;
                    lables[i] = datadst[pos];
                }
            }
        }
    }
    //     // Jorge
    // if (pos >= 0 && pos < width * height * channels)
    // {
    //     datadst[pos] = (datasrc[pos] * 255) / nblobs;
    // }
    // }
    // }

    printf("Numero de blobs após = %d\n", nblobs);
    for (int i = 0; i < nblobs; i++)
    {
        printf("Label %d = %d\n", i, lables[i]);
    }

    return 1;
}

/// @brief Passa uma imagem binaria para grayscale
/// o grayscale é a divisão do valor do pixel pelo número de blobs, assim fica 'automatico' o número de blobs
/// o parametro detalhes é para mostrar ou não os detalhes do valor de cada blob
/// Esses detalhes são mostrados em forma de printf na consola
/// @param src
/// @param dst
/// @param nblobs
/// @param blobs
/// @return
int vc_bin_labels(IVC *src, IVC *dst, int nblobs, OVC *blobs, int detalhes)
{
    unsigned char *datasrc = (unsigned char *)src->data;
    unsigned char *datadst = (unsigned char *)dst->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;
    int x, y;
    long int pos;
    int i;
    int lables[nblobs];

    if (src->height <= 0 || src->width <= 0 || src->height <= 0 || src->width <= 0 || src == NULL || dst == NULL)
    {
        printf("(vc_bin_labels) Tamanhos inválidos\n");
        return 0;
    }
    // if (src->channels != 1 || dst->channels != 3) return 0; // Verifica se as imagens têm os canais corretos

    printf("Numero de blobs = %d\n", nblobs);
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            pos = y * bytesperline + x * channels;

            if (datasrc[pos] != 0)
            {
                for (int i = 0; i <= nblobs; i++)
                {

                    if (datasrc[pos] == blobs[i].label)
                    {
                        datadst[pos] = (datasrc[pos] * 255) / nblobs;
                        lables[i] = datadst[pos];
                    }
                }
            }
        }
    }

    if (detalhes)
    {
        printf("Numero de blobs %d\n", nblobs);
//        unsigned char *data_src = (unsigned char *)src->data;

        for (int i = 0; i < nblobs; i++)
        {
            printf("Label %d = %d\n", i, lables[i]);
            printf("Ponto mais à esquerda %d\n", blobs[i].xc);
            printf("Area: %d", blobs[i].area);
        }
    }

    return 1;
}

int vc_bin_labelsTeste(IVC *src, IVC *dst, int nblobs, OVC *blobs, int detalhes)
{
    unsigned char *datasrc = (unsigned char *)src->data;
    unsigned char *datadst = (unsigned char *)dst->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;
    int x, y;
    long int pos;
    int i;
    int labels[nblobs];

    if (src->height <= 0 || src->width <= 0 || src->data == NULL || dst == NULL)
    {
        printf("(vc_bin_labels) Tamanhos inválidos\n");
        return 0;
    }


    // Calcular os limites para os 40% centrais da largura da imagem
    int x_start = width * 0.3; // 30% da largura da imagem
    int x_end = width * 0.7;   // 70% da largura da imagem

    for (y = 0; y < height; y++)
    {
        for (x = x_start; x < x_end; x++)
        {
            pos = y * bytesperline + x * channels;

            if (datasrc[pos] != 0)
            {
                for (int i = 0; i < nblobs; i++)
                {
                    if (datasrc[pos] == blobs[i].label)
                    {
                        datadst[pos] = (datasrc[pos] * 255) / nblobs;
                        labels[i] = datadst[pos];
                    }
                }
            }
        }
    }

    if (detalhes)
    {
        printf("Numero de blobs após = %d\n", nblobs);

        for (int i = 0; i < nblobs; i++)
        {
            printf("Label %d = %d\n", i, labels[i]);
            printf("Ponto mais à esquerda %d\n", blobs[i].xc);
        }
    }

    return 1;
}


#pragma region Desenhar Bounding Box e centro de massa

/// @brief Percore os blobs, ve a altura max e min e a largura max e min
/// depois disso, faz loop pelos eixo x e y, se for igual a um dos valores max ou min desenha a bounding box
/// Deixei comentado uma outra abordagem, que é percorrer a altura e largura do blob e desenhar a bounding box
/// Dá para adicionar margem nos eixos x e y.
/// @param src
/// @param blobs
/// @param nblobs
/// @param margem
/// @return
int vc_draw_bounding_box(IVC *src, OVC *blobs, int nblobs, int margemX, int margemY, int frame, int *listaBlobs, IVC *teste, int *contador, int *contaAnalise)
{
    unsigned char *data_src = (unsigned char *)src->data;
    int width = src->width;
    int height = src->height;
    int channels = src->channels;
    int bytesperline = width * channels;
    int x, y;
    long int pos;
    int i;
    int xmin, ymin, xmax, ymax;
    
    unsigned char *data_teste = (unsigned char *)teste->data;

    if (width <= 0 || height <= 0 || src->data == NULL)
    {
        printf("(vc_draw_bounding_box) Tamanhos inválidos\n");
        return 0;
    }

    // Desenha a bounding box para cada blob
    for (i = 0; i < nblobs; i++)
    {
        if(frame > 716) break;
        if(blobs[i].width > 150 &&
           blobs[i].area > 11000 && blobs[i].area < 21000 &&
           blobs[i].height > 80 && blobs[i].height <= 115)
        {
            xmin = blobs[i].x - margemX;
            xmax = blobs[i].x + blobs[i].width + margemX;
            ymin = blobs[i].y - margemY;
            ymax = blobs[i].y + blobs[i].height + margemY;

            int preto = 0, castanho = 0, vermelho = 0, laranja = 0, amarelo = 0;
            int verde = 0, azul = 0, violeta = 0, cinza = 0, branco = 0, dourado = 0;
            blobs[i].ultimaCor = -1; // teste
            
            // Contar blobs
            (blobs[i].yc >= 39 && blobs[i].yc <= 41) ? (*contador)++ : 0; // Lembrar EDA!

            
            if(*contador != *contaAnalise){
                
                (*contaAnalise)++;
   

            }






            // Contar blobs fim
            
            

        

            // Desenha a bounding box no eixo dos x
            for (x = xmin; x <= xmax; x++)
            {
                pos = ymin * bytesperline + x * channels;
                data_src[pos] = 255; // Azul
                 data_src[pos + 1] = 0; // Verde
                 data_src[pos + 2] = 0; // Vermelho
                
                pos = ymax * bytesperline + x * channels;
                data_src[pos] = 255;
                 data_src[pos + 1] = 0;
                 data_src[pos + 2] = 0;
   
                
                // Verificação da cor dos pixels ao longo do y_center
                int y_center = blobs[i].y + blobs[i].height / 2;
                pos = y_center * bytesperline + x * channels;

                unsigned char blue = data_src[pos];
                unsigned char green = data_src[pos + 1];
                unsigned char red = data_src[pos + 2];
                
                printf("Pixel at (x=%d, y_center=%d): Red=%d, Green=%d, Blue=%d Posição: %ld\n", x, y_center, red, green, blue, pos);
                
                float h, s, v;
                ValoresRgb_to_hsv(red, green, blue, &h, &s, &v); // Troca de canais!
                
                s = (int)(s * 100);
                v = (int)(v * 100);
                
                if(frame == 196){
                    if(x == 360){
                        printf(  "teste");
                    }
                }
                
                // Condição para o mudar de cor, procurar o amarelo que está entre as cores
                if((h > 30 && h < 40)){
                    blobs[i].ultimaCor = -1;
                }
                
                
                if(blobs[i].ultimaCor == -1){
                    // Verde
                    if((h >= 70 && h <= 130) && (s > 30 && s < 100) && (v > 30 && v < 100)){
                        if(blobs[i].ultimaCor == -1){
                            
                            if(blobs[i].primeiro == 0){
                                blobs[i].primeiro = 5;
                                blobs[i].ultimaCor = 5;
                            }
                            else if(blobs[i].segundo == 0){
                                blobs[i].segundo = 5;
                                blobs[i].ultimaCor = 5;
                            }
                            else if(blobs[i].terceiro == 0){
                                blobs[i].terceiro = 5;
                                blobs[i].ultimaCor = 5;
                            }
                        }
                    }
                    // azul
                    if ((h >= 180 && h <= 280) && (s > 30 && s < 100) && (v > 30 && v < 100)){
                        if(blobs[i].ultimaCor == -1){
                            
                            if(blobs[i].primeiro == 0){
                                blobs[i].primeiro = 6;
                                blobs[i].ultimaCor = 6;
                            }
                            else if(blobs[i].segundo == 0){
                                blobs[i].segundo = 6;
                                blobs[i].ultimaCor = 6;
                            }
                            else if(blobs[i].terceiro == 0){
                                blobs[i].terceiro = 6;
                                blobs[i].ultimaCor = 6;
                            }
                        }
                    }
                    // Vermelho
                    if ((h > 0 && h < 30) || (h >= 340 && h <= 360)) {                        if(blobs[i].ultimaCor == -1){
                            
                            if(blobs[i].primeiro == 0){
                                blobs[i].primeiro = 2;
                                blobs[i].ultimaCor = 2;
                            }
                            else if(blobs[i].segundo == 0){
                                blobs[i].segundo = 2;
                                blobs[i].ultimaCor = 2;
                            }
                            else if(blobs[i].terceiro == 0){
                                blobs[i].terceiro = 2;
                                blobs[i].ultimaCor = 2;
                            }
                        }
                    }
                }
                
                // #TODO -> ajustar as cores. Está a considerar castanho como vermelho...

//                // deixei o indice das cores com 1 numero a cima para fugir ao zero
//                  if ((h >= 0 && h <= 30) || (h >= 340 && h <= 360)) { // testar
//                      vermelho = 3;
//                  } else if (h >= 31 && h <= 50) { // testar
//                      laranja = 4;
////                  } else if ((h >= 26 && h <= 35) &&) {
////                      amarelo = 5;
//                  } else if ((h >= 70 && h <= 130) && (s > 30 && s < 100) && (v > 30 && v < 100) ) {
//                      verde = 6;
//                  } else if ((h >= 180 && h <= 280) && (s > 30 && s < 100) && (v > 30 && v < 100)) {
//                      azul = 7;
//                  } else if (h >= 126 && h <= 160) {
//                      violeta = 8;
//                  } else if ((h >= 31 && h <= 40) && v <= 50) {
//                      castanho = 2;
//                  }else if ((h > 41 && h < 60) && (s > 50) && (v > 50)){
//                      dourado = 1;
//                  }
//                  
//                
//                if(verde != 0){
//                    printf("Encontrou verde!\n");
//                    blobs[i].valor = 6;
//                }
//                if(azul != 0){
//                    printf("Encontrou azul!\n");
//                    blobs[i].valor = 6;
//                }
//                if(vermelho != 0){
//                    printf("Encontrou vermelho!\n");
//                    blobs[i].valor = 6;
//                }
//                if(dourado != 0){
//                    printf("Encontrou dourado!\n");
//                    blobs[i].valor = 6;
//                }
//                if(laranja != 0){
//                    printf("Encontrou laranja!\n");
//                    blobs[i].valor = 6;
//                }
//                
//                
//                
//                if(s > 100 || v > 100 ){
//                    printf("\tErro\n");
//                }
                


            }
            // Desenha a bounding box no eixo dos y
            for (y = ymin; y <= ymax; y++)
            {
                

                pos = y * bytesperline + xmin * channels;
                data_src[pos] = 255;
                 data_src[pos + 1] = 0;
                 data_src[pos + 2] = 0;
                
                pos = y * bytesperline + xmax * channels;
                data_src[pos] = 255;
                 data_src[pos + 1] = 0;
                 data_src[pos + 2] = 0;
            }
            
                 for (int t = - 5 ; t <= 5; t++)
                 {
                     for (int z = - 5; z <= 5; z++)
                     {
                         if(t==0||z==0)
                         {
                             pos = (t+blobs[i].yc) * bytesperline + (z+blobs[i].xc) * channels;
                             
                             data_src[pos] = 255;
                             data_src[pos + 1] = 0;
                             data_src[pos + 2] = 0;
                         }
                     }
                 }
            
            
        }
    }


    return 1;
}

/// Converter valores RGB para HSV, para ser 
void ValoresRgb_to_hsv(unsigned char red, unsigned char green, unsigned char blue, float *h, float *s, float *v) {
    float r = red / 255.0;
    float g = green / 255.0;
    float b = blue / 255.0;

    float max = fmaxf(r, fmaxf(g, b));
    float min = fminf(r, fminf(g, b));
    float delta = max - min;

    *v = max;

    if (max == 0) {
        *s = 0;
        *h = 0; // undefined
        return;
    }

    *s = delta / max;

    if (delta == 0) {
        *h = 0; // undefined
        return;
    }

    if (max == r) {
        *h = 60.0 * fmodf(((g - b) / delta), 6);
    } else if (max == g) {
        *h = 60.0 * (((b - r) / delta) + 2);
    } else if (max == b) {
        *h = 60.0 * (((r - g) / delta) + 4);
    }

    if (*h < 0) {
        *h += 360.0;
    }
}




int vc_draw_bounding_boxTeste(IVC *src, OVC *blobs, int nblobs, int margemX, int margemY)
{
    unsigned char *data_src = (unsigned char *)src->data;
    int width = src->width;
    int height = src->height;
    int channels = src->channels;
    int bytesperline = width * channels;
    int x, y;
    long int pos;
    int i;
    int xmin, ymin, xmax, ymax;

    if (width <= 0 || height <= 0 || src->data == NULL)
    {
        printf("(vc_draw_bounding_box) Tamanhos inválidos\n");
        return 0;
    }

    // Desenha a bounding box para cada blob
    for (i = 0; i < nblobs; i++)
    {
        // filtrar para conseguir apenas as resistencias
        if(blobs[i].width > 100 &&
           blobs[i].area > 11000 && blobs[i].area < 18000 &&
           blobs[i].height > 75 && blobs[i].height <= 102)
        {
            
            
            xmin = blobs[i].x - margemX;
            xmax = blobs[i].x + blobs[i].width + margemX;
            ymin = blobs[i].y - margemY;
            ymax = blobs[i].y + blobs[i].height + margemY;
            
            
            // Desenha a bounding box no eixo dos x
            for (x = xmin; x <= xmax; x++)
            {
                pos = ymin * bytesperline + x * channels;
                data_src[pos] = 255; // Vermelho
                // data_src[pos + 1] = 0; // Verde
                // data_src[pos + 2] = 0; // Azul
                
                pos = ymax * bytesperline + x * channels;
                data_src[pos] = 255;
                // data_src[pos + 1] = 0;
                // data_src[pos + 2] = 0;
            }
            // Desenha a bounding box no eixo dos y
            for (y = ymin; y <= ymax; y++)
            {
                pos = y * bytesperline + xmin * channels;
                data_src[pos] = 255;
                // data_src[pos + 1] = 0;
                // data_src[pos + 2] = 0;
                
                pos = y * bytesperline + xmax * channels;
                data_src[pos] = 255;
                // data_src[pos + 1] = 0;
                // data_src[pos + 2] = 0;
            }
        }
    }

    // // Jorge, outra abordagem, talvez mais simples.
    // loop para cada blob; loop no pela altura e largura do blob; se for igual a 0 ou a altura ou a largura, desenha a bounding box
    // for (int i = 0; i < nblobs; i++)
    // {
    //     for (int y = 0; y <= blobs[i].height; y++)
    //     {
    //         for (int x = 0; x <= blobs[i].width; x++)
    //         {
    //             if(y==0||x==0||x==blobs[i].width||y == blobs[i].height)
    //             {
    //                 pos = (y+blobs[i].y) * bytesperline + (x+blobs[i].x) * channels;

    //                 data_src[pos] = 255;
    //             }

    //         }
    //     }

    //     for (int t = - 5 ; t <= 5; t++)
    //     {
    //         for (int z = - 5; z <= 5; z++)
    //         {
    //             if(t==0||z==0)
    //             {
    //                 pos = (t+blobs[i].yc) * bytesperline + (z+blobs[i].xc) * channels;

    //                 data_src[pos]=0;
    //             }
    //         }

    //     }

    // }

    return 1;
}


int vc_draw_bounding_box_RGB(IVC *src, OVC *blobs, int nblobs, int margemX, int margemY, int red, int green, int blue)
{
    unsigned char *data_src = (unsigned char *)src->data;
    int width = src->width;
    int height = src->height;
    int channels = src->channels;
    int bytesperline = width * channels;
    int x, y;
    long int pos;
    int i;
    int xmin, ymin, xmax, ymax;

    if (width <= 0 || height <= 0 || src->data == NULL)
    {
        printf("(vc_draw_bounding_box) Tamanhos inválidos\n");
        return 0;
    }

    // Desenha a bounding box para cada blob
    for (i = 0; i < nblobs; i++)
    {
        xmin = blobs[i].x - margemX;
        xmax = blobs[i].x + blobs[i].width + margemX;
        ymin = blobs[i].y - margemY;
        ymax = blobs[i].y + blobs[i].height + margemY;

        // Desenha a bounding box no eixo dos x
        for (x = xmin; x <= xmax; x++)
        {
            pos = ymin * bytesperline + x * channels;
            data_src[pos] = red;       // Vermelho
            data_src[pos + 1] = green; // Verde
            data_src[pos + 2] = blue;  // Azul

            pos = ymax * bytesperline + x * channels;
            data_src[pos] = red;       // Vermelho
            data_src[pos + 1] = green; // Verde
            data_src[pos + 2] = blue;  // Azul
        }
        // Desenha a bounding box no eixo dos y
        for (y = ymin; y <= ymax; y++)
        {
            pos = y * bytesperline + xmin * channels;
            data_src[pos] = red;       // Vermelho
            data_src[pos + 1] = green; // Verde
            data_src[pos + 2] = blue;  // Azul

            pos = y * bytesperline + xmax * channels;
            data_src[pos] = red;       // Vermelho
            data_src[pos + 1] = green; // Verde
            data_src[pos + 2] = blue;  // Azul
        }
    }

    return 1;
}

/// @brief Percore os blobs, utiliza o centro de massa para desenhar uma cruz
/// Recebe o tamaho do alvo e tambem a cor da cruz, 0 para preto e 1 para branco
/// @param src
/// @param blobs
/// @param nblobs
/// @param tamanho_alvo
/// @param cor
/// @return
int vc_draw_center_of_mass(IVC *src, OVC *blobs, int nblobs, int tamanho_alvo, int cor)
{
    unsigned char *data_src = (unsigned char *)src->data;
    int width = src->width;
    int height = src->height;
    int channels = src->channels;
    int bytesperline = width * channels;
    int x, y;
    long int pos;
    int i;

    if (width <= 0 || height <= 0 || src->data == NULL)
    {
        printf("(vc_draw_center_of_mass) Tamanhos inválidos\n");
        return 0;
    }

    // Desenha o centro de massa
    for (i = 0; i < nblobs; i++)
    {
        for (y = -tamanho_alvo; y <= tamanho_alvo; y++)
        {
            for (x = -tamanho_alvo; x <= tamanho_alvo; x++)
            {
                // desenha a cruz
                if (y == 0 || x == 0)
                {
                    pos = (y + blobs[i].yc) * bytesperline + (x + blobs[i].xc) * channels;

                    if (cor == 0)
                    {
                        data_src[pos] = 0;
                    }
                    else
                    {
                        data_src[pos] = 255;
                    }
                }
            }
        }
    }

    return 1;
}

int vc_draw_center_of_mass_RGB(IVC *src, OVC *blobs, int nblobs, int tamanho_alvo, int cor, int red, int green, int blue)
{
    unsigned char *data_src = (unsigned char *)src->data;
    int width = src->width;
    int height = src->height;
    int channels = src->channels;
    int bytesperline = width * channels;
    int x, y;
    long int pos;
    int i;

    if (width <= 0 || height <= 0 || src->data == NULL)
    {
        printf("(vc_draw_center_of_mass) Tamanhos inválidos\n");
        return 0;
    }

    // Desenha o centro de massa
    for (i = 0; i < nblobs; i++)
    {
        for (y = -tamanho_alvo; y <= tamanho_alvo; y++)
        {
            for (x = -tamanho_alvo; x <= tamanho_alvo; x++)
            {
                // desenha a cruz
                if (y == 0 || x == 0)
                {
                    pos = (y + blobs[i].yc) * bytesperline + (x + blobs[i].xc) * channels;

                    data_src[pos] = red;
                    data_src[pos + 1] = green;
                    data_src[pos + 2] = blue;
                }
            }
        }
    }
    // printf("centro de massa RGB\n");
    return 1;
}

#pragma endregion

#pragma region 3 canais para apenas 1
// converte uma imagem de 3 canais para 1 canal
// atenção, as imagens já têm (deve) de estar em 1 e zeros
int vc_3chanels_to_1(IVC *src, IVC *dst)
{
    unsigned char *data_src = (unsigned char *)src->data;
    unsigned char *data_dst = (unsigned char *)dst->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->width * src->channels;
    int channels = src->channels;
    int x, y;
    long int pos;

    if ((src->width) <= 0 || (src->height <= 0) || (src->data == NULL))
    {
        printf("(vc_3chanels_to_1) Tamanhos inválidos\n");
        return 0;
    }
    if (src->channels != 3)
    {
        printf("(vc_3chanels_to_1) Imagem tem de ter 3 canais\n");
        return 0;
    }
    if ((dst->width) <= 0 || (dst->height <= 0) || (dst->data == NULL))
    {
        printf("(vc_3chanels_to_1) Tamanhos inválidos\n");
        return 0;
    }
    if (dst->channels != 1)
    {
        printf("(vc_3chanels_to_1) Imagem tem de ter 1 canal\n");
        return 0;
    }
    if (src->width != dst->width || src->height != dst->height)
    {
        printf("(vc_3chanels_to_1) Imagens têm tamanhos diferentes\n");
        return 0;
    }

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            pos = y * bytesperline + x * channels;
            data_dst[y * width + x] = (unsigned char)(0.299 * data_src[pos] + 0.587 * data_src[pos + 1] + 0.114 * data_src[pos + 2]);
        }
    }

    return 1;
}
#pragma endregion

#pragma endregion

#pragma region Histograma

/// @brief Histograma para imagens com 8 bits.
/// Ter atenção que a imagem de destino tem de ter 1 canal e uma largura de 256 pixeis
/// Pode ter qql altura, porque fazemos uma normalização, que permite que o histograma seja mostrado em qualquer altura
/// @param src
/// @param dst
/// @return
int vc_gray_histogram_show(IVC *src, IVC *dst)
{
    unsigned char *datasrc = (unsigned char *)src->data;
    unsigned char *datadst = (unsigned char *)dst->data;
    int width = src->width;
    int height = src->height;
    int dstHeight = dst->height;
    int dstWidth = dst->width;
    int bytesperline = src->width * src->channels;
    int bytesperlineDst = dst->width * dst->channels;
    int channels = src->channels;
    int x, y, kx, ky, min, max = 0;
    long int pos, posk;
    int size = width * height;

    if (dst->width != 256)
    {
        printf("(vc_gray_histogram_show) Largura da imagem de destino tem de ser 256 pixeis!\n");
        return 0;
    }
    if (src->height <= 0 || src->width <= 0 || src->height <= 0 || src->width <= 0)
    {
        printf("(vc_gray_histogram_show) Tamanhos inválidos\n");
        return 0;
    }
    if (src->channels != 1 || dst->channels != 1)
    {
        printf("(vc_gray_histogram_show) Imagem tem de ter 1 canal\n");
        return 0;
    }

    // contagem dos pixeis
    int histograma[256] = {0};

    // percorre a imagem e conta os pixeis e define o maximo
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            pos = y * bytesperline + x * channels;
            histograma[datasrc[pos]]++;
            if (histograma[datasrc[pos]] > max)
                max = histograma[datasrc[pos]];
        }
    }

    // normaliza o histograma
    for (int i = 0; i < 256; i++)
    {
        histograma[i] = (histograma[i] * dstHeight) / max;
    }

    // Apenas para ver o conteudo do histograma
    // for (int i = 0; i < 256; i++)
    // {
    //     printf("histograma[%d] = %d\n", i, histograma[i]);
    // }

    // preenche o histograma
    for (x = 0; x < dstWidth; x++)
    {
        for (y = 0; y < dstHeight; y++)
        {
            if (y > dstHeight - histograma[x])
            {
                pos = y * bytesperlineDst + x * channels; // O erro que estava a dar era ter o bytesperline da imagem original...
                datadst[pos] = 255;
            }
        }
    }

    return 1;
}

/// @brief  Aqui o objetivo é equalizar o histograma, pensar nisto como "esticar" o histograma para que os valores tenham maior contraste
/// @param src
/// @param dst
/// @return
int vc_gray_histogram_equalization(IVC *src, IVC *dst)
{
    unsigned char *data_src = (unsigned char *)src->data;
    unsigned char *data_dst = (unsigned char *)dst->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->bytesperline;
    int channels = src->channels;
    int x, y, i;
    long int pos;
    int max = 0;
    int hist[256] = {0};         // Histograma
    int cdf[256] = {0};          // Função de distribuição cumulativa
    int equalization[256] = {0}; // Mapeamento de equalização

    // Verificação de erros
    if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
    {
        printf("(vc_gray_histogram_equalization) Tamanhos inválidos.\n");
        return 0;
    }
    if ((dst->width <= 0) || (dst->height <= 0) || (dst->data == NULL))
    {
        printf("(vc_gray_histogram_equalization) Tamanhos inválidos.\n");
        return 0;
    }
    if (src->width != dst->width || src->height != dst->height || src->channels != dst->channels)
    {
        printf("(vc_gray_histogram_equalization) As imagens origem e destino têm tamanhos diferentes.\n");
        return 0;
    }
    if (channels != 1)
        return 0;

    // Contagem do histograma
    for (i = 0; i < width * height; i++)
    {
        hist[data_src[i]]++;
    }

    // Cálculo da função de distribuição cumulativa (CDF)
    cdf[0] = hist[0];
    for (i = 1; i < 256; i++)
    {
        cdf[i] = cdf[i - 1] + hist[i];
    }

    // Encontra o valor máximo do CDF
    max = cdf[255]; // valor maximo do acumular

    // Cálculo do mapeamento de equalização
    for (i = 0; i < 256; i++)
    {
        equalization[i] = (int)(((float)cdf[i] - cdf[0]) / ((float)max - cdf[0]) * (src->levels - 1));
    }

    // Aplica a equalização aos pixels da imagem de entrada
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            pos = y * bytesperline + x * channels;
            data_dst[pos] = equalization[data_src[pos]];
        }
    }

    return 1;
}

int vc_hsv_to_rgb(IVC *srcdst)
{
    unsigned char *data = (unsigned char *)srcdst->data;
    int width = srcdst->width;
    int height = srcdst->height;
    int bytesperline = srcdst->bytesperline;
    int channels = srcdst->channels;
    float hue, saturation, value, c, x, m;
    int i, size;

    // Verificação de erros
    if ((width <= 0) || (height <= 0) || (data == NULL))
        return 0;
    if (channels != 3)
        return 0;

    size = width * height * channels;

    for (i = 0; i < size; i = i + channels)
    {
        hue = (float)data[i] / 255.0f * 360.0f;
        saturation = (float)data[i + 1] / 255.0f;
        value = (float)data[i + 2] / 255.0f;

        c = value * saturation;
        x = c * (1 - fabs(fmod(hue / 60.0f, 2) - 1));
        m = value - c;

        float r, g, b;
        if (hue >= 0 && hue < 60)
        {
            r = c;
            g = x;
            b = 0;
        }
        else if (hue >= 60 && hue < 120)
        {
            r = x;
            g = c;
            b = 0;
        }
        else if (hue >= 120 && hue < 180)
        {
            r = 0;
            g = c;
            b = x;
        }
        else if (hue >= 180 && hue < 240)
        {
            r = 0;
            g = x;
            b = c;
        }
        else if (hue >= 240 && hue < 300)
        {
            r = x;
            g = 0;
            b = c;
        }
        else
        {
            r = c;
            g = 0;
            b = x;
        }

        data[i] = (unsigned char)((r + m) * 255.0f);
        data[i + 1] = (unsigned char)((g + m) * 255.0f);
        data[i + 2] = (unsigned char)((b + m) * 255.0f);
    }

    return 1;
}

#pragma endregion

#pragma region Contornos

/// @brief
/// @param src
/// @param dst
/// @param th
/// @return
int vc_gray_edge_prewitt(IVC *src, IVC *dst, float th)
{
    unsigned char *data_src = (unsigned char *)src->data;
    unsigned char *data_dst = (unsigned char *)dst->data;
    int width = src->width;
    int height = src->height;
    int bytesperline = src->bytesperline;
    int channels = src->channels;
    int x, y, i;
    long int pos, size = width * height;
    int maskx[3][3] = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
    int masky[3][3] = {{-1, -1, -1}, {0, 0, 0}, {1, 1, 1}};

    if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
    {
        printf("(vc_gray_edge_prewitt) Tamanhos inválidos\n");
        return 0;
    }

    // Faz loop pela imagem original
    // Loop through the image
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            long int pos = y * bytesperline + x * channels;

            int sumx = 0;
            int sumy = 0;
            for (int kx = -1; kx <= 1; kx++) // loop pela mascara / kernel
            {
                for (int ky = -1; ky <= 1; ky++)
                {
                    long int pos_mask = (y + ky) * bytesperline + (x + kx) * channels;
                    sumx += data_src[pos_mask] * maskx[ky + 1][kx + 1];
                    sumy += data_src[pos_mask] * masky[ky + 1][kx + 1];
                }
            }

            // Calcular a magnitude
            data_dst[pos] = (unsigned char)((sqrt(sumx * sumx + sumy * sumy)) / 3);

            // Thresholding
            if (data_dst[pos] > th)
            {
                data_dst[pos] = 255;
            }
            else
            {
                data_dst[pos] = 0;
            }
        }
    }

    return 1;
}

#pragma endregion
