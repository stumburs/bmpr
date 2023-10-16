# bmpr

This is a simple header only BMP image library for C++, allowing you to create, and save BMP images programmatically with various functions.

## Table of Contents

-   [bmpr](#bmpr)
    -   [Table of Contents](#table-of-contents)
    -   [Introduction](#introduction)
    -   [Usage](#usage)
    -   [License](#license)

## Introduction

This library provides a straightforward way to create BMP images and perform operations such as setting pixels, drawing lines, circles, rectangles, and more.

## Usage

To use this library, simple include the header. Create an `Image` object, set pixels, draw shapes, and save the image to a file.

Example usage:

```cpp
#include "bmpr.hpp"

int main()
{
    // Create an image with dimensions 800x600 pixels
    bmpr::Image img(800, 600);

    // Set the background to a specific color
    img.Clear({128, 128, 128});

    // Set a pixel at (100, 200) to red
    img.Set(100, 200, bmpr::RED);

    // Draw a line from (100, 200) to (500, 400) with a thickness of 2 and a random color
    img.DrawLine(100, 200, 500, 400, 2, bmpr::Color::Random());

    // Save the image to a BMP file
    img.Save("output.bmp");
}
```

## License

This project is licensed under the [MIT License](LICENSE).
