#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cmath>

namespace bmpr
{
#pragma pack(push, 1)
    struct Header
    {
        std::uint16_t signature;
        std::uint32_t file_size;
        std::uint32_t reserved;
        std::uint32_t data_offset;
        std::uint32_t info_header_size;
        std::int32_t width;
        std::int32_t height;
        std::uint16_t planes;
        std::uint16_t bit_depth;
        std::uint32_t compression;
        std::uint32_t img_size;
        std::int32_t xpix_per_m;
        std::int32_t ypix_per_m;
        std::uint32_t colors_used;
        std::uint32_t colors_important;
    };
#pragma pack(pop)

    struct Vector2
    {
        std::int32_t x, y;
        Vector2() : x(0), y(0) {}
        Vector2(int32_t xy) : x(xy), y(xy) {}
        Vector2(int32_t x, int32_t y) : x(x), y(y) {}
    };

    struct Color
    {
        uint8_t r, g, b;
        Color() : r(0), g(0), b(0) {}
        Color(uint8_t rgb) : r(rgb), g(rgb), b(rgb) {}
        Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
        static Color Random()
        {
            return {static_cast<uint8_t>(rand() % 255), static_cast<uint8_t>(rand() % 255), static_cast<uint8_t>(rand() % 255)};
        }

        // Color defines
        static const Color BLACK;
        static const Color WHITE;
        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;
        static const Color YELLOW;
        static const Color CYAN;
        static const Color MAGENTA;
        static const Color ORANGE;
        static const Color PINK;
        static const Color PURPLE;
        static const Color BROWN;
        static const Color GRAY;
        static const Color LIGHT_GRAY;
        static const Color DARK_GRAY;
        static const Color PASTEL_YELLOW;
        static const Color PASTEL_GREEN;
        static const Color PASTEL_BLUE;
    };

    // Color defines
    const Color Color::BLACK = {0};
    const Color Color::WHITE = {255};
    const Color Color::RED = {255, 0, 0};
    const Color Color::GREEN = {0, 255, 0};
    const Color Color::BLUE = {0, 0, 255};
    const Color Color::YELLOW = {255, 255, 0};
    const Color Color::CYAN = {0, 255, 255};
    const Color Color::MAGENTA = {255, 0, 255};
    const Color Color::ORANGE = {255, 165, 0};
    const Color Color::PINK = {255, 192, 203};
    const Color Color::PURPLE = {128, 0, 128};
    const Color Color::BROWN = {139, 69, 19};
    const Color Color::GRAY = {128, 128, 128};
    const Color Color::LIGHT_GRAY = {192, 192, 192};
    const Color Color::DARK_GRAY = {64, 64, 64};
    const Color Color::PASTEL_YELLOW = {255, 255, 153};
    const Color Color::PASTEL_GREEN = {153, 255, 153};
    const Color Color::PASTEL_BLUE = {153, 204, 255};

    class Image
    {
    public:
        // Initialize an image with a width and height in pixels
        Image(std::size_t width, std::size_t height);
        // Set the color of a specific pixel
        void Set(std::int32_t x, std::int32_t y, const Color &color);
        // Set the color of a specific pixel only if it's valid
        void SetSafe(std::int32_t x, std::int32_t y, const Color &color);
        // Sets the whole image to the specific color
        void Clear(const Color &color);
        // Draws a line from x1;y1 to x2;y2
        void DrawLine(std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2, Color color);
        // Draws a line from x1;y1 to x2;y2 with a certain thickness
        void DrawLine(std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2, int thickness, Color color);
        // Draws a quadratic bezier curve
        void DrawQuadraticBezierCurve(const Vector2 &start, const Vector2 &control, const Vector2 &end, float step_size, const Color &color);
        void DrawQuadraticBezierCurve(const Vector2 &start, const Vector2 &control, const Vector2 &end, int num_points, const Color &color);
        // Draws a filled circle at x;y as its center
        void DrawCircle(std::int32_t x, std::int32_t y, std::int32_t r, Color color);
        // Draws the circumference of a circle at x;y as its center
        void DrawCircleLine(std::int32_t x, std::int32_t y, std::int32_t r, Color color);
        // Draws a filled rectangle that contains the circle, with the circle not being drawn
        void DrawCircleInverted(std::int32_t x, std::int32_t y, std::int32_t r, Color color);
        // Draws a filled rectangle with the top-left most point at x;y
        void DrawRectangle(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, Color color);
        // Draws the perimeter of a rectangle witht the top-left most point at x;y
        void DrawRectangleLine(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, Color color);
        // Returns image width in pixels
        std::int32_t Width() const noexcept;
        // Returns image height in pixels
        std::int32_t Height() const noexcept;
        // Saves the image to file. NOTE: Include the .bmp extension
        bool Save(const std::string &path);
        // Rotates the image 180 degrees
        void Rotate180();
        // Flips the image horizontally
        void FlipHorizontally();
        // Flips the image vertically
        void FlipVertically();
        // Inverts the colors of the image
        void Invert();

        //
        // DEBUG FUNCTIONS
        //
        // These will get removed or be implemented in the future.
        // Rotates the image ???-wise by an angle in degrees
        void DEBUGRotate(float angle);
        // Bilinear color interpolation
        Color DEBUGInterpolateColor(float x, float y);

    private:
        std::vector<Color> m_data;
        std::int32_t m_width = 0, m_height = 0;
    };
}

// Implementations
namespace bmpr
{
    Image::Image(std::size_t width, std::size_t height) : m_data(width * height), m_width{static_cast<std::int32_t>(width)}, m_height{static_cast<std::int32_t>(height)} {}

    void Image::Set(std::int32_t x, std::int32_t y, const Color &color)
    {
        m_data[static_cast<std::size_t>(y) * m_width + x] = color;
    }

    void Image::SetSafe(std::int32_t x, std::int32_t y, const Color &color)
    {
        if (x >= 0 && x < m_width && y >= 0 && y < m_height)
        {
            std::size_t index = static_cast<std::size_t>(y) * m_width + x;
            m_data[index] = color;
        }
    }

    void Image::DrawLine(std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2, Color color)
    {
        int delta_x = abs(x2 - x1);
        int delta_y = abs(y2 - y1);
        int sign_x = (x1 < x2) ? 1 : -1;
        int sign_y = (y1 < y2) ? 1 : -1;

        int error = delta_x - delta_y;

        int x = x1;
        int y = y1;

        while (x != x2 || y != y2)
        {
            // Set the pixel color
            SetSafe(x, y, color);

            int error2 = error * 2;

            if (error2 > -delta_y)
            {
                error -= delta_y;
                x += sign_x;
            }

            if (error2 < delta_x)
            {
                error += delta_x;
                y += sign_y;
            }
        }
    }

    void Image::DrawLine(std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2, int thickness, Color color)
    {
        // Check if the line thickness is less than 1
        if (thickness < 1)
            thickness = 1;

        int delta_x = abs(x2 - x1);
        int delta_y = abs(y2 - y1);
        int sign_x = (x1 < x2) ? 1 : -1;
        int sign_y = (y1 < y2) ? 1 : -1;

        int error = delta_x - delta_y;

        int x = x1;
        int y = y1;

        while (x != x2 || y != y2)
        {
            // Set the pixel color for the current thickness
            for (int i = 0; i < thickness; ++i)
            {
                for (int j = 0; j < thickness; ++j)
                {
                    SetSafe(x + i - thickness / 2, y + j - thickness / 2, color);
                }
            }

            int error2 = error * 2;

            if (error2 > -delta_y)
            {
                error -= delta_y;
                x += sign_x;
            }

            if (error2 < delta_x)
            {
                error += delta_x;
                y += sign_y;
            }
        }
    }

    void Image::DrawQuadraticBezierCurve(const Vector2 &start, const Vector2 &control, const Vector2 &end, int num_points, const Color &color)
    {
        for (int i = 0; i <= num_points; ++i)
        {
            float t = static_cast<float>(i) / static_cast<float>(num_points);

            float x = pow(1 - t, 2) * start.x + 2 * t * (1 - t) * control.x + pow(t, 2) * end.x;
            float y = pow(1 - t, 2) * start.y + 2 * t * (1 - t) * control.y + pow(t, 2) * end.y;

            // Set the pixel color for the calculated point
            SetSafe(static_cast<int>(x), static_cast<int>(y), color);
        }
    }
    void Image::DrawQuadraticBezierCurve(const Vector2 &start, const Vector2 &control, const Vector2 &end, float step_size, const Color &color)
    {
        float t = 0.0;

        while (t <= 1.0)
        {
            float x = pow(1 - t, 2) * start.x + 2 * t * (1 - t) * control.x + pow(t, 2) * end.x;
            float y = pow(1 - t, 2) * start.y + 2 * t * (1 - t) * control.y + pow(t, 2) * end.y;

            // Set the pixel color for the calculated point
            SetSafe(static_cast<int>(x), static_cast<int>(y), color);

            t += step_size;
        }
    }

    // Probably not the best solution, but it's only addition
    void Image::DrawCircle(std::int32_t x, std::int32_t y, std::int32_t r, Color color)
    {
        for (std::int32_t y1 = -r; y1 <= r; y1++)
            for (std::int32_t x1 = -r; x1 <= r; x1++)
                if (x1 * x1 + y1 * y1 < r * r + r)
                    SetSafe(x + x1, y + y1, color);
    }

    void Image::DrawCircleLine(std::int32_t x, std::int32_t y, std::int32_t r, Color color)
    {
        int center_x = 0;
        int center_y = r;
        int d = 3 - 2 * r;

        while (center_x <= center_y)
        {
            // Octants
            SetSafe(x + center_x, y + center_y, color);
            SetSafe(x - center_x, y + center_y, color);
            SetSafe(x + center_x, y - center_y, color);
            SetSafe(x - center_x, y - center_y, color);
            SetSafe(x + center_y, y + center_x, color);
            SetSafe(x - center_y, y + center_x, color);
            SetSafe(x + center_y, y - center_x, color);
            SetSafe(x - center_y, y - center_x, color);

            if (d < 0)
                d += 4 * center_x++ + 6;
            else
                d += 4 * (center_x++ - center_y--) + 10;
        }
    }

    void Image::DrawCircleInverted(std::int32_t x, std::int32_t y, std::int32_t r, Color color)
    {
        for (std::int32_t y1 = -r; y1 <= r; y1++)
            for (std::int32_t x1 = -r; x1 <= r; x1++)
                if (x1 * x1 + y1 * y1 >= r * r + r && x1 / x1 + y1 / y1 <= r * r - r)
                    SetSafe(x + x1, y + y1, color);
    }

    void Image::DrawRectangle(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, Color color)
    {
        for (std::int32_t width = 0; width < w; width++)
            for (std::int32_t height = 0; height < h; height++)
                SetSafe(x + width, y + height, color);
    }

    void Image::DrawRectangleLine(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, Color color)
    {
        for (std::int32_t width = 0; width < w; width++)
        {
            SetSafe(x + width, y, color);
            SetSafe(x + width, y + h, color);
        }
        for (std::int32_t height = 0; height < h; height++)
        {
            SetSafe(x, y + height, color);
            SetSafe(x + w, y + height, color);
        }
        SetSafe(x + w, y + h, color);
    }

    void Image::Clear(const Color &color)
    {
        std::fill(m_data.begin(), m_data.end(), color);
    }

    std::int32_t Image::Width() const noexcept { return m_width; }

    std::int32_t Image::Height() const noexcept { return m_height; }

    bool Image::Save(const std::string &path)
    {
        const std::int32_t row_size = m_width * 3 + m_width % 4;
        const std::uint32_t bmp_size = row_size * m_height;

        const Header header = {0x4d42,                                                // Signature ('BM')
                               static_cast<std::uint32_t>(bmp_size + sizeof(Header)), // File size in bytes
                               0,                                                     // reserved (unused)
                               sizeof(Header),                                        // Offset from beginning of file to the beginning of the bitmap data
                               40,                                                    // Size of InfoHeader
                               m_width,                                               // Horizontal width of bitmap in pixels
                               m_height,                                              // Vertical height of bitmap in pixels
                               1,                                                     // Number of Planes
                               24,                                                    // Bit-depth
                               0,                                                     // Compression (0 = none)
                               bmp_size,                                              // Size of Image
                               0,                                                     // Horizontal resolution: Pixels/meter
                               0,                                                     // Vertical resolution: Pixels/meter
                               0,                                                     // Colors used
                               0};                                                    // Number of important colors (0 = all)

        if (std::ofstream ofs{path, std::ios_base::binary})
        {
            ofs.write(reinterpret_cast<const char *>(&header), sizeof(header));

            std::vector<std::uint8_t> line(row_size);

            for (std::int32_t y = m_height - 1; -1 < y; --y)
            {
                size_t pos = 0;

                for (std::int32_t x = 0; x < m_width; ++x)
                {
                    const Color &col = m_data[static_cast<std::size_t>(y) * m_width + x];
                    line[pos++] = col.b;
                    line[pos++] = col.g;
                    line[pos++] = col.r;
                }

                ofs.write(reinterpret_cast<const char *>(line.data()), line.size());
            }

            return true;
        }
        else
            return false;
    }

    void Image::Rotate180()
    {
        std::reverse(m_data.begin(), m_data.end());
    }

    void Image::FlipHorizontally()
    {
        for (int y = 0; y < m_height; ++y)
        {
            for (int x = 0; x < m_width / 2; ++x)
            {
                int index_1D_left = y * m_width + x;
                int index_1D_right = y * m_width + (m_width - 1 - x);
                std::swap(m_data[index_1D_left], m_data[index_1D_right]);
            }
        }
    }

    void Image::FlipVertically()
    {
        for (int y = 0; y < m_height / 2; ++y)
        {
            for (int x = 0; x < m_width; ++x)
            {
                int index_1D_top = y * m_width + x;
                int index_1D_bottom = (m_height - 1 - y) * m_width + x;
                std::swap(m_data[index_1D_top], m_data[index_1D_bottom]);
            }
        }
    }

    void Image::Invert()
    {
        for (std::size_t i = 0; i < m_width * m_height; i++)
        {
            Color &pixel = m_data[i];
            // Invert each color component
            pixel.r = 255 - pixel.r;
            pixel.g = 255 - pixel.g;
            pixel.b = 255 - pixel.b;
        }
    }

    void Image::DEBUGRotate(float angle)
    {
        const float center_x = static_cast<float>(m_width) / 2.0f;
        const float center_y = static_cast<float>(m_height) / 2.0f;

        for (int y = 0; y < m_height; ++y)
        {
            for (int x = 0; x < m_width; ++x)
            {
                // Translate the coordinates so that the center of the image is the origin
                float translated_x = static_cast<float>(x) - center_x;
                float translated_y = static_cast<float>(y) - center_y;

                // Rotate the coordinates around the origin
                float rotated_x = translated_x * cos(angle) - translated_y * sin(angle);
                float rotated_y = translated_x * sin(angle) + translated_y * cos(angle);

                // Translate the coordinates back to the original coordinate system
                rotated_x += center_x;
                rotated_y += center_y;

                // Interpolate the color using the rotated coordinates (you may implement this)
                Color interpolatedColor = DEBUGInterpolateColor(rotated_x, rotated_y);

                // Set the color for the current pixel
                SetSafe(x, y, interpolatedColor);
            }
        }
    }

    Color Image::DEBUGInterpolateColor(float x, float y)
    {
        // Calculate the integer coordinates of the surrounding pixels
        int x1 = static_cast<int>(floor(x));
        int x2 = x1 + 1;
        int y1 = static_cast<int>(floor(y));
        int y2 = y1 + 1;

        if (x1 < 0 || x2 >= m_width || y1 < 0 || y2 >= m_height)
        {
            // Handle out-of-bounds condition (e.g., return a default color)
            return Color(0, 0, 0);
        }

        // Get the colors of the surrounding pixels
        // x * row_len + y
        const Color &topLeft = m_data.at(x1 * m_width + y1);     // img.GetPixel(x1, y1);
        const Color &topRight = m_data.at(x2 * m_width + y1);    // img.GetPixel(x2, y1);
        const Color &bottomLeft = m_data.at(x1 * m_width + y2);  // img.GetPixel(x1, y2);
        const Color &bottomRight = m_data.at(x2 * m_width + y2); // img.GetPixel(x2, y2);

        // Calculate the weights for the interpolation
        float weightTopLeft = (x2 - x) * (y2 - y);
        float weightTopRight = (x - x1) * (y2 - y);
        float weightBottomLeft = (x2 - x) * (y - y1);
        float weightBottomRight = (x - x1) * (y - y1);

        // Interpolate the color components
        uint8_t r = static_cast<uint8_t>(topLeft.r * weightTopLeft + topRight.r * weightTopRight +
                                         bottomLeft.r * weightBottomLeft + bottomRight.r * weightBottomRight);
        uint8_t g = static_cast<uint8_t>(topLeft.g * weightTopLeft + topRight.g * weightTopRight +
                                         bottomLeft.g * weightBottomLeft + bottomRight.g * weightBottomRight);
        uint8_t b = static_cast<uint8_t>(topLeft.b * weightTopLeft + topRight.b * weightTopRight +
                                         bottomLeft.b * weightBottomLeft + bottomRight.b * weightBottomRight);

        return Color(r, g, b);
    }
}