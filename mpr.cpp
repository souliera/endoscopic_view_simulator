#include <iostream>

#include "CImg.h"

using namespace std;
using namespace cimg_library;

template<typename T>
CImg<T> get_planeX(const CImg<T> origin, const unsigned int x, const unsigned int y, const unsigned int z, const int min, const int max) {
    unsigned int hor = origin.height();
    unsigned int dor = origin.depth();

    CImg<> imgTmp(hor, dor, 1, 3);

    for(int i = 0; i < hor; i++) {
        for(int j = 0; j < dor; j++) {
            imgTmp(i, j, 0, 0) = origin(x, i, j);
            imgTmp(i, j, 0, 1) = origin(x, i, j);
            imgTmp(i, j, 0, 2) = origin(x, i, j);
        }
    }

    return imgTmp;
}

template<typename T>
CImg<T> get_planeY(const CImg<T> origin, const unsigned int x, const unsigned int y, const unsigned int z, const int min, const int max) {
    unsigned int wor = origin.width();
    unsigned int dor = origin.depth();

    CImg<> imgTmp(wor, dor, 1, 3);

    for(int i = 0; i < wor; i++) {
        for(int j = 0; j < dor; j++) {
            imgTmp(i, j, 0, 0) = origin(i, y, j);
            imgTmp(i, j, 0, 1) = origin(i, y, j);
            imgTmp(i, j, 0, 2) = origin(i, y, j);
        }
    }

    return imgTmp;
}

template<typename T>
CImg<T> get_planeZ(const CImg<T> origin, const unsigned int x, const unsigned int y, const unsigned int z, const int min, const int max) {
    unsigned int wor = origin.width();
    unsigned int hor = origin.height();

    CImg<> imgTmp(wor, hor, 1, 3);

    for(int i = 0; i < wor; i++) {
        for(int j = 0; j < hor; j++) {
            imgTmp(i, j, 0, 0) = origin(i, j, z);
            imgTmp(i, j, 0, 1) = origin(i, j, z);
            imgTmp(i, j, 0, 2) = origin(i, j, z);
        }
    }

    return imgTmp;
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cout << "Usage: " << argv[0] << " <image.hdr>" << endl;
        exit(EXIT_FAILURE);
    }

    // ****************************
    // *** VARIABLE DECLARATION ***
    // ****************************

    CImg<> imgOrigin;
    CImg<> img[3];
    CImg<> grid;
    CImgDisplay disp;

    float voxelSize[3];

    float min;
    float max;

    unsigned int width, height, depth;
    unsigned int dispWidth, dispHeight;

    unsigned int displayedColumn;
    unsigned int displayedRow;
    unsigned int displayedSlice;

    bool redraw[4];

    // *******************************
    // *** VARIABLE INITAILIZATION ***
    // *******************************

    imgOrigin.load_analyze(argv[1], voxelSize);

    min = imgOrigin.min();
    max = imgOrigin.max();

    const float red[] = {max, min, min}, green[] = {min, max, min}, blue[] = {min, min, max};

    width = imgOrigin.width();
    height = imgOrigin.height();
    depth = imgOrigin.depth();

    img[0].resize(height, depth, 1, 3);
    img[1].resize(width, depth, 1, 1);
    img[2].resize(width, height, 1, 1);

    dispWidth = width + height;
    dispHeight = depth + height;

    disp.resize(dispWidth, dispHeight);

    displayedColumn = width / 2;
    displayedRow = height / 2;
    displayedSlice = depth / 2;
    redraw[0] = redraw[1] = redraw[2] = redraw[3] = true;

    // imgX = get_planeX(imgOrigin, displayedColumn, displayedRow, displayedSlice, min, max);
    // imgY = get_planeY(imgOrigin, displayedColumn, displayedRow, displayedSlice, min, max);
    // imgZ = get_planeZ(imgOrigin, displayedColumn, displayedRow, displayedSlice, min, max);

    // ***************
    // *** PROGRAM ***
    // ***************

    while(!disp.is_closed() && !disp.is_keyESC()) {
        if(disp.wheel()) {
            if(disp.mouse_x() < width && disp.mouse_y() < depth) {
                if(displayedRow != height-1 && disp.wheel() > 0) {
                    displayedRow++;
                    redraw[2] = true;
                    redraw[0] = true;
                }
                if(displayedRow != 0 && disp.wheel() < 0) {
                    displayedRow--;
                    redraw[2] = true;
                    redraw[0] = true;
                }
            } else if(disp.mouse_x() > width && disp.mouse_y() < depth) {
                if(displayedColumn != width-1 && disp.wheel() > 0) {
                    displayedColumn++;
                    redraw[1] = true;
                    redraw[0] = true;
                }
                if(displayedColumn != 0 && disp.wheel() < 0) {
                    displayedColumn--;
                    redraw[1] = true;
                    redraw[0] = true;
                }
            } else if(disp.mouse_x() < width && disp.mouse_y() > depth) {
               if(displayedSlice != depth-1 && disp.wheel() > 0) {
                   displayedSlice++;
                   redraw[3] = true;
                   redraw[0] = true;
               }
               if(displayedSlice != 0 && disp.wheel() < 0) {
                   displayedSlice--;
                   redraw[3] = true;
                   redraw[0] = true;
               }
           }

           //reinitialise l'event wheel (set to 0)
           disp.set_wheel();
        }

        if(disp.button()&1) {
            if(disp.mouse_x() < width && disp.mouse_y() < depth) {
                displayedColumn = disp.mouse_x();
                displayedSlice = disp.mouse_y();
                redraw[1] = true;
                redraw[3] = true;
                redraw[0] = true;
            } else if(disp.mouse_x() > width && disp.mouse_y() < depth) {
                displayedRow = disp.mouse_x() - width;
                displayedSlice = disp.mouse_y();
                redraw[2] = true;
                redraw[3] = true;
                redraw[0] = true;
            } else if(disp.mouse_x() < width && disp.mouse_y() > depth) {
                displayedColumn = disp.mouse_x();
                displayedRow = disp.mouse_y() - depth;
                redraw[1] = true;
                redraw[2] = true;
                redraw[0] = true;
            }
        }

        // if(disp.is_resized()) {
        //
        // }

        if(redraw[0]) {
            if(redraw[1]) {
                img[0] = get_planeX(imgOrigin, displayedColumn, displayedRow, displayedSlice, min, max);
                redraw[1] = false;
            }
            if(redraw[2]) {
                img[1] = get_planeY(imgOrigin, displayedColumn, displayedRow, displayedSlice, min, max);
                redraw[2] = false;
            }
            if(redraw[3]) {
                img[2] = get_planeZ(imgOrigin, displayedColumn, displayedRow, displayedSlice, min, max);
                redraw[3] = false;
            }

            grid.resize(0, 0, 1, 3);
            grid = grid.append(img[1], 'x');
            grid = grid.append(img[0], 'x');
            grid = grid.append(img[2], 'y');

            grid.draw_line(displayedColumn, 0, displayedColumn, dispHeight, red, 1);
            grid.draw_line(displayedRow+width, 0, displayedRow+width, depth, green, 1);
            grid.draw_line(0, displayedRow+depth, width, displayedRow+depth, green, 1);
            grid.draw_line(0, displayedSlice, dispWidth, displayedSlice, blue, 1);

            grid.display(disp);

            redraw[0] = false;
        }
    }

    // **************************
    // *** VARIABLE RELEASING ***
    // **************************

    exit(EXIT_SUCCESS);
}